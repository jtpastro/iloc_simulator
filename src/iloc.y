%{
    #include <sstream> //stringstream
    #include "Program.hpp"
    #include "SimulationError.hpp"

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
        ss << "At line " << line_counter << ": " << msg << "." << std::endl;
        throw SimulationError(ss.str().c_str());
   }
%}

%union {
    int ival;
    Operation *operation;
    Opcode_Name opcode;
    char *str;
}

%token SEMICOLON
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
                    const std::map<std::string,uint>& lbls = program.get_unused_labels();
                    if(!lbls.empty()){
                        std::stringstream ss;
                        for(auto& it : lbls)
                            ss << "At line " << it.second << ": semantic error, label " << it.first << " undeclared.\n";
                        throw SimulationError(ss.str().c_str());
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
                | the_opcode operand_list {
                    $$ = $2;
                    $$->opcode = $1;
                    if(!$$->verify_operation())
                        yyerror("syntax error, malformed instruction " + Operation::opcode_to_string($$->opcode));
                }
                | the_opcode ARROW operand_list { 
                    $$ = $3;
                    $$->opcode = $1;
                    if(!$$->verify_operation())
                        yyerror("syntax error, malformed instruction " + Operation::opcode_to_string($$->opcode));
                }
                | the_opcode {
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

