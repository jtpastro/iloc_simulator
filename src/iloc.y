/**
    O analisador sintático da linguagem ILOC foi desenvolvido em BISON/C++.
    A árvore sintática gerada é convertido em um objeto da classe Program que contém todas as instruções ILOC do programa de entrada.
    Um programa iloc é definido como uma lista de instruções.
    Por sua vez, uma lista de instruções pode ser uma única instrução ou uma instrução seguida de uma lista de instrução, ambos os casos podem ser precedidos por uma definição de rótulo. 
    Uma instrução é sempre iniciada por um código de operação.
    Instruções como NOP e HALT são representadas apenas pelo código de operação, pois não possuem operandos.
    As instruções de desvio incondicional são representados por código de operação e uma seta seguida da lista de operandos.
    Todas as outras instruções são representadas pelo código de operação seguido de duas listas de operandos separadas por uma seta.
    Listas de operandos são definadas recursivamente por um operando ou uma lista de operandos seguida de um operando.
    Um operando pode ser um valor numérico, um rótulo ou um registrador.
*/

%{
    #include <sstream> //stringstream
    #include "Program.hpp"//Program
    #include "SimulationError.hpp"//SimulatorError

    #define MAX_ERROR_MESSAGE_LENGTH 100
    #define YYERROR_VERBOSE 1

    Program program;
    void yyerror (std::string msg);

    extern "C" {
        extern int line_counter;
        extern FILE *yyin;
        extern int yylineno;
        int yylex();
    }

    void yyerror(std::string msg) {
        std::stringstream ss;
        ss << "Erro na linha " << line_counter << ": " << msg << ".";
        throw SimulationError(ss.str());
   }
%}

%union {
    int ival;
    Operation *operation;
    Opcode_Name opcode;
    char *str;
}

%token COMMA
%token ARROW
%token <str> REGISTER
%token <ival> NUMBER
%token <str> LABEL
%token <str> TARGET
%token <opcode> OPCODE

%type <operation> operation
%type <operation> operand_list
%type <opcode> the_opcode
%type <str> reg
%type <ival> const
%type <str> lbl
%type <str> label_def

%start iloc_program

%% /* Beginning of rules */

iloc_program    : operation_list {
                    std::string lbls = program.get_unused_labels();
                    if(!lbls.empty()){
                        throw SimulationError(lbls);
                    }
                };

operation_list  : operation {
                    program.add_operation(*$1);
                    delete $1;
                }
                | label_def operation {
                    program.add_operation($1,*$2);
                    delete $2;
                }
                | operation {
                    program.add_operation(*$1);
                    delete $1;
                } operation_list
                | label_def operation {
                    program.add_operation($1, *$2);
                    delete $2;
                } operation_list;

operation       : the_opcode operand_list ARROW operand_list {
                    $$ = $2;
                    $$->concatenate(*$4);
                    $$->opcode = $1;
                    delete $4;
                    if(!$$->verify_operation())
                        yyerror("syntax error, malformed instruction " + Operation::opcode_to_string($$->opcode));
                }
                | the_opcode ARROW operand_list { //JUMP or JUMPI
                    $$ = $3;
                    $$->opcode = $1;
                    if(!$$->verify_operation())
                        yyerror("syntax error, malformed instruction " + Operation::opcode_to_string($$->opcode));
                }
                | the_opcode { //NOP or HALT
                    $$ = new Operation();
                    $$->opcode = $1;
                    if(!$$->verify_operation())
                         yyerror("syntax error, malformed instruction " + Operation::opcode_to_string($$->opcode));
		        };

the_opcode       : OPCODE {
		            $$ = $1;
		         };

operand_list     : reg {
                    $$ = new Operation();
		            $$->add_register($1);
		         }
                 | operand_list COMMA reg {
                    $$ = $1;
                    $$->add_register($3);
		         } 
                 | const {
                    $$ = new Operation();
                    $$->add_constant($1);
		         }
                 | operand_list COMMA const {
		            $$ = $1;
                    $$->add_constant($3);
		         }  
                 | lbl {
		            $$ = new Operation();
                    $$->add_label($1);
		         }
                 | operand_list COMMA lbl {
		            $$ = $1;
                    $$->add_label($3);
		         };

reg              : REGISTER { 
	                $$ = $1;
    	         };

const            : NUMBER {
		            $$ = yylval.ival;
		         };

lbl              : LABEL {
		            $$ = $1;
                 };

label_def        : TARGET {
		            $$ = $1;
		         };

%%

