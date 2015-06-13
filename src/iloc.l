WHITESPACE [ \t]
NUM [0-9]+
STRING [a-zA-Z0-9\_]+

%{

  #include "Operation.hpp"
  #include "parser.hpp"
  
  extern "C" int yylex();
  
  int line_counter = 1;

%}

%option yylineno

%%

\;            {return SEMICOLON;}
\,            {return COMMA;}
[\=\-]\>      {return ARROW;}

[rR]{STRING}  {yylval.str = yytext; return REGISTER;}
{NUM}         {yylval.ival = atoi(yytext); return NUMBER;}
\-{NUM}       {yylval.ival = atoi(yytext); return NUMBER;}

{STRING}: {   
                    char *lbl = yytext;
                    lbl[strlen(lbl)-1]='\0';
                    yylval.str = lbl;
                    return TARGET;
              }
{STRING}  {
                    Opcode_Name current_opcode = Operation::string_to_opcode(yytext);                    
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

