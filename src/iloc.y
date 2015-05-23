%{
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include "Machine.hpp"

    #define MAX_ERROR_MESSAGE_LENGTH 100

    extern char yytext[];
    extern int line_counter;

    Program program;

    extern "C" {
        extern FILE *yyin;
        extern int yylineno;
        int yylex(void);
        void yyerror (char const *mensagem);
        int yyparse ();
    }
   void yyerror(const char* msg) {
      fprintf(stderr, "%s\n", msg);
   }
%}

%union {
    int ival;
    Operation *operation;
    Opcode_Name opcode;
    char *str;
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
%token <opcode> OPCODE

%type <operation> operation
%type <operation> operand_list
%type <opcode> the_opcode
%type <ival> reg
%type <ival> const
%type <str> lbl
%type <str> label_def

%start iloc_program

%% /* Beginning of rules */

iloc_program    : operation_list {
                };

operation_list  : operation {
                    program.add_operation(*$1);
                }
                | label_def operation {
                    program.add_operation($1,*$2);
                }
                | operation_list operation {
                    program.add_operation(*$2);
                }
                | operation_list label_def operation {
                    program.add_operation($2, *$3);
                };

operation       : the_opcode operand_list ARROW operand_list {
                    $$ = $2;
                    $$->concatenate(*$4);
                    $$->opcode = $1;
                    delete $4;
                }
                | the_opcode operand_list {
                    $$ = $2;
                    $$->opcode = $1;
                }
                | the_opcode ARROW operand_list { 
                    $$ = $3;
                    $$->opcode = $1;
                }
                | the_opcode {
                    $$->opcode = $1;
		        };

the_opcode       : OPCODE {
		            $$ = $1;
		         };

operand_list     : reg {
                    $$ = new Operation();
		            $$->regs.push_back($1);
		         }
                 | operand_list COMMA reg {
                    $$ = $1;
                    $$->regs.push_back($3);
		         }
                 | const {
                    $$ = new Operation();
                    $$->consts.push_back($1);
		         }
                 | operand_list COMMA const {
		            $$ = $1;
                    $$->consts.push_back($3);
		         }
                 | lbl {
		            $$ = new Operation();
                    $$->labels.push_back($1);
		         }
                 | operand_list COMMA lbl {
		            $$ = $1;
                    $$->labels.push_back($3);
		         };

reg              : REGISTER {
                    $$ = yylval.ival;
		         };

const            : NUMBER {
		            $$ = yylval.ival;
		         };

lbl              : LABEL {
                    char *myLabel = $1; 
		            $$ = strdup(myLabel);
                 };

label_def        : TARGET {
                    char *myLabel = $1; 
		            $$ = strdup(myLabel);
		         };

%%

