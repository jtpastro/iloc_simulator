#ifndef YYERRCODE
#define YYERRCODE 256
#endif

#define OPEN_BRACKET 257
#define CLOSE_BRACKET 258
#define SEMICOLON 259
#define COMMA 260
#define ARROW 261
#define OPCODE 262
#define OUTPUT 263
#define REGISTER 264
#define NUMBER 265
#define LABEL 266
#define TARGET 267
typedef union {
    int ival;
    Instruction* inst_ptr;
    Operation* op_ptr;
    Operands* operands_ptr;
    Operand* operand_ptr;
    Opcode* opcode_ptr;
} YYSTYPE;
extern YYSTYPE yylval;
