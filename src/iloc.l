WHITESPACE [ \t]
NUM [0-9]+

%{
  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>

  #include "Operation.hpp"
  #include "parser.hpp"
  
  extern "C" int yylex();
  
  int line_counter = 1;

%}

%option yylineno

%%

\[            {return OPEN_BRACKET;}
\]            {return CLOSE_BRACKET;}
\;            {return SEMICOLON;}
\,            {return COMMA;}
[\=\-]\>      {return ARROW;}

r{NUM}        {yylval.ival = atoi(&yytext[1])+2; return REGISTER;}
rarp          {yylval.ival = 0; return REGISTER;}
rbss          {yylval.ival = 1; return REGISTER;}
{NUM}         {yylval.ival = atoi(yytext); return NUMBER;}
\-{NUM}       {yylval.ival = atoi(yytext); return NUMBER;}

[a-zA-Z0-9\_]+: {   
                    char *lbl = yytext;
                    lbl[strlen(lbl)-1]='\0';
                    yylval.str = lbl;
                    return TARGET;
                }
[a-zA-Z0-9\_]+  {
                    Opcode_Name current_opcode = Operation::str_to_opcode(strdup(yytext));
                    if(current_opcode != INVALID_OP){
                        yylval.opcode = current_opcode;
                        return OPCODE;
                    } else {
                        yylval.str = yytext;
                        return LABEL;
                    }
                }
\/\/[^\n]*  { /* Comment */}

[\n]          {line_counter++;}

{WHITESPACE}  {;}

%%

int yywrap()
{
  return 1;
}

