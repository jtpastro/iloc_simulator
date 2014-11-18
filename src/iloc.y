%{
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include "Machine.hpp"

  #define MAX_ERROR_MESSAGE_LENGTH 100

  extern char yytext[];

  extern int line_counter;

%}

%union {
    int ival;
    Program *program;
    Operation *operation;
    Opcode opcode;
    std::string str;
}

%token OPEN_BRACKET
%token CLOSE_BRACKET
%token SEMICOLON
%token COMMA
%token ARROW
%token OPCODE
%token DOUTPUT
%token REGISTER
%token NUMBER
%token <str> LABEL
%token <str> TARGET

%type <program> operation_list
%type <operation> operation
%type <operation> operand_list
%type <opcode> the_opcode
%type <ival> reg
%type <ival> const
%type <str> lbl
%type <str> label_def

%start iloc_program

%% /* Beginning of rules */

iloc_program     : operation_list
                 {
		     first_operation = $1;
		 }
                 ;

operation_list : operation {
                    $$ = new Program();
                    $$->add_operation($1);
                }
                | label_def operation {
                    $$ = new Program();
		            $$->add_operation($1, $2);
                }
                | operation operation_list
                {
                    $$ = $1;
                    $$->add_operation($$);
                }
                | label_def operation operation_list
                {
                    $$ = $2;
                    program.add_operation($1, $$);
                };
operation       : the_opcode operand_list ARROW operand_list
                {
                    $2->opcode = $1;
		            $2->merge($4);
                    $2->verify_operation();
                    $$=$2;
		            free($4);
		 }
                 | the_opcode operand_list {
                    $2->opcode = $1;
                    $2->verify_operation();
                    $$=$2;
		         }
                 | the_opcode ARROW operand_list { 
                    $3->opcode = $1;
                    $3->verify_operation();
                    $$=$3;
		         }
                 | the_opcode {
                    $$ = new Operation();
                    $$->opcode = $1;
		         }
                 ;

the_opcode       : OPCODE
                 {
		            $$ = current_opcode;
		         }
                 ;

operand_list     : reg
                 {
		     $$ = new_operands();
		     $$->num_regs = 1;
		     $$->regs = $1;
		 }
                 | reg COMMA operand_list
                 {
		     $$ = $3;
		     $$->num_regs += 1;
		     $1->next = $$->regs;
		     $$->regs = $1;
		 }
                 | const
                 {
		     $$ = new_operands();
		     $$->num_consts = 1;
		     $$->consts = $1;
		 }
                 | const COMMA operand_list
                 {
		     $$ = $3;
		     $$->num_consts += 1;
		     $1->next = $$->consts;
		     $$->consts = $1;
		 }
                 | lbl
                 {
		            $$ = $1
		         }
                 | lbl COMMA operand_list
                 {
		            $$ = $3;
		            $$->num_labels += 1;
		     $1->next = $$->labels;
		     $$->labels = $1;
		 }
                 ;

reg              : REGISTER
                 {
		     $$ = malloc(sizeof(Operand));
		     //$$->value = (int) strtol(yylval.ival, (char**) NULL, 10);
		     $$->value = yylval.ival;
		     
		     $$->next = NULL;
		 }
                 ;

const            : NUMBER
                 {
		            $$ = yylval.ival;
		 }
		 ;

lbl              : LABEL
                 {
                     char *myLabel = $1;
		     $$ = malloc(sizeof(Operand));
		     $$->value = insert_label(myLabel);
		     $$->next = NULL;
		 }
                 ;

label_def        : TARGET
                 {
             char *myLabel = $1; 
		     $$ = insert_label(strdup(myLabel));
		 }
                 ;

%%
    
