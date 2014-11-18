WHITESPACE [ \t]
NUM [0-9]+

%{
  /* iloc.l
   * Lex specification for the ILOC subset defined in
   * "Engineering a Compiler" by Cooper and Torczon
   * written by Todd Waterman
   * 11/30/00 */

  #include <stdlib.h>
  #include <stdio.h>
  #include <string.h>

  #include "operation.h"
  #include "parser.h"

  int line_counter = 1;
  Opcode* current_opcode;

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
                    return TARGET;}

[a-zA-Z0-9\_]+  {
                
		        current_opcode = get_opcode(strdup(yytext));

                if (current_opcode){
                    yylval.opcode = current_opcode
                    return OPCODE;
                }else{
                    yylval.str = yytext;
                    return LABEL;
                }
              }

\/\/[^\n]*  { /* Comment */}

[\n]          {line_counter++;}

{WHITESPACE}  {;}

%%

yywrap()
{
  return 1;
}

