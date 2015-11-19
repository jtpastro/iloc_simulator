WHITESPACE [ \t]
NUM [0-9]+
STRING [a-zA-Z0-9\_]+

%{

  #include "Operation.hpp"
  #include "parser.hpp"
  #include "SimulationError.hpp"
  
  #define YY_DECL extern "C" int yylex()

  void yyerror (std::string msg);
  int line_counter = 0;

%}

%option yylineno
%option noyywrap

%%

\;              {return SEMICOLON;}
\,              {return COMMA;}
[\=\-]\>        {return ARROW;}
{STRING}        {   
                    Opcode_Name current_opcode = Operation::string_to_opcode(yytext);                    
                    if(current_opcode != INVALID_OP){
                        yylval.opcode = current_opcode;
                        return OPCODE;
                    } else {
                        REJECT;
                    }
                }
r{STRING}       {yylval.str = yytext;return REGISTER;}
fp              {yylval.str = yytext;return REGISTER;}
sp              {yylval.str = yytext;return REGISTER;}
bss             {yylval.str = yytext;return REGISTER;}
{NUM}           {yylval.ival = atoi(yytext); return NUMBER;}
\-{NUM}         {yylval.ival = atoi(yytext); return NUMBER;}
{STRING}:       {   
                    char *lbl = yytext;
                    lbl[strlen(lbl)-1]='\0';
                    yylval.str = lbl;
                    return TARGET;
                }
{STRING}        {yylval.str = yytext; return LABEL;}
\/\/            { /* Comment */}
[\n]            {line_counter++;}
{WHITESPACE}    {;}
.|\n            {line_counter++; yyerror("no match for '" + std::string(yytext) + "'"); }
%%

