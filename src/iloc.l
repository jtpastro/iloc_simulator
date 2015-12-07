/** \file iloc.l */
/**
    Analisador léxico da linguagem ILOC escrito em Flex/C++.
    A linguagem consiste de instruções ILOC separadas por quebras de linha.
    Cada instrução possui um código de operação (OPCODE) que são palavras reservadas na linguagem.
    Além disso, cada instrução pode possuir operandos dos tipos registrador, numeral e rótulo.
    Os registradores são quaisquer strings alfanuméricas iniciadas por 'r' minúsculo e que não sejam um opcode, 
    e.g.: 'rarp' pode ser usado como registrador, mas não 'rSHIFT', pois é um opcode.
    Os numerais representam números inteiros, positivos e negativos.
    Os rótulos representam marcadores de desvio e podem ser quaisquer strings alfanuméricas que não sejam opcodes, registradores ou numerais.
    Quando usados como operandos, existe a restrição adicional de não terminarem no caractere de dois pontos ':'.
    Além de servir como operandos, os rótulos podem preceder um opcode, indicando um destino para um desvio.
    Nesse caso, os rótulos serão terminados com o caractere de dois pontos.
    Cada instrução possui um formato específico, podendo ser usados os caractere de vírgula ',' ou a sequência representando uma flecha '=>' ou '->'
    como açucar sintático para aumentar sua expressividade.
    A linguagem também suporta comentários de uma linha iniciados pela sequência de duas barras '//'.
    Os elementos da linguagem são separados por espaços em branco. 
*/

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
[rR]{STRING}    {yylval.str = yytext;return REGISTER;}
{NUM}           {yylval.ival = atoi(yytext); return NUMBER;}
\-{NUM}         {yylval.ival = atoi(yytext); return NUMBER;}
{STRING}:       {   
                    char *lbl = yytext;
                    lbl[strlen(lbl)-1]='\0';
                    
                    Opcode_Name current_opcode = Operation::string_to_opcode(lbl);                    
                     if(current_opcode != INVALID_OP){
                        REJECT;
                    } else {
                        yylval.str = lbl;
                        return TARGET;
                    }

                }
{STRING}        {yylval.str = yytext; return LABEL;}
\/\/            { /* Comment */}
[\n]            {line_counter++;}
{WHITESPACE}    {;}
.|\n            {line_counter++; yyerror("Nenhum TOKEN correspondente para '" + std::string(yytext) + "'"); } 
%%

