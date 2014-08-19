#ifndef lint
static char const 
yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.28 2000/01/17 02:04:06 bde Exp $";
#endif
#include <stdlib.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex()
#define YYEMPTY -1
#define yyclearin (yychar=(YYEMPTY))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING() (yyerrflag!=0)
static int yygrowstack();
#define YYPREFIX "yy"
#line 8 "iloc.y"
  #include <stdio.h>
  #include <string.h>
  #include "instruction.h"

  #define MAX_ERROR_MESSAGE_LENGTH 100

  Operands* new_operands(void);
  Operand* append_operands(Operand*,Operand*);
  int verify_args(Opcode*,int,int,int,int);

  extern char yytext[];

  extern int line_counter;
  extern Opcode* current_opcode;

  /* This function must be defined */
  void yyerror(char*);

  /* If an error is encountered during parsing this is changed to 1 */
  int error_found = 0;

  /* Pointer to the first instruction */
  Instruction* first_instruction;

#line 34 "iloc.y"
typedef union {
    int ival;
    Instruction* inst_ptr;
    Operation* op_ptr;
    Operands* operands_ptr;
    Operand* operand_ptr;
    Opcode* opcode_ptr;
} YYSTYPE;
#line 51 "y.tab.c"
#define YYERRCODE 256
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
const short yylhs[] = {                                        -1,
    0,    1,    1,    1,    1,    2,    2,    3,    3,    4,
    4,    4,    4,    5,    6,    6,    6,    6,    6,    6,
    7,    8,    9,   10,
};
const short yylen[] = {                                         2,
    1,    1,    2,    2,    3,    1,    3,    1,    3,    4,
    2,    3,    1,    1,    1,    3,    1,    3,    1,    3,
    1,    1,    1,    1,
};
const short yydefred[] = {                                      0,
    0,   14,   24,    0,    1,    0,    6,    0,    0,    0,
    0,    4,    0,   21,   22,   23,    0,    0,    0,    0,
    0,    7,    0,   12,    0,    0,    0,    0,    5,    9,
   10,   16,   18,   20,
};
const short yydgoto[] = {                                       4,
    5,    6,   10,    7,    8,   17,   18,   19,   20,    9,
};
const short yysindex[] = {                                   -255,
 -259,    0,    0,    0,    0, -255,    0, -236, -252, -247,
 -253,    0, -233,    0,    0,    0, -248, -246, -240, -237,
 -255,    0, -259,    0, -233, -233, -233, -233,    0,    0,
    0,    0,    0,    0,
};
const short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,   26,    0,   21,    0,    0,
 -224,    0,    0,    0,    0,    0,   27,    1,    8,   15,
   35,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,
};
const short yygindex[] = {                                      0,
    3,   28,   13,   -1,    0,   -9,    0,    0,    0,    0,
};
#define YYTABLESIZE 294
const short yytable[] = {                                      11,
   15,    1,    2,   24,    1,   23,    2,   17,   12,    2,
   22,    3,   25,   26,   19,   31,   32,   33,   34,   27,
   13,   11,   28,   29,   13,    2,   11,   14,   15,   16,
   14,   15,   16,    8,    3,   30,   21,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   15,   15,   15,
    0,   15,   15,    0,   17,   17,   17,   15,   17,   17,
    0,   19,   19,   19,   17,   19,   19,   13,   13,   13,
    0,   19,   13,   11,   11,   11,    0,   13,   11,    0,
    0,    0,    0,   11,
};
const short yycheck[] = {                                       1,
    0,  257,  262,   13,  257,  259,  262,    0,    6,  262,
  258,  267,  261,  260,    0,   25,   26,   27,   28,  260,
    0,   23,  260,   21,  261,    0,    0,  264,  265,  266,
  264,  265,  266,  258,    0,   23,    9,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  257,  258,  259,
   -1,  261,  262,   -1,  257,  258,  259,  267,  261,  262,
   -1,  257,  258,  259,  267,  261,  262,  257,  258,  259,
   -1,  267,  262,  257,  258,  259,   -1,  267,  262,   -1,
   -1,   -1,   -1,  267,
};
#define YYFINAL 4
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 267
#if YYDEBUG
const char * const yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"OPEN_BRACKET","CLOSE_BRACKET",
"SEMICOLON","COMMA","ARROW","OPCODE","OUTPUT","REGISTER","NUMBER","LABEL",
"TARGET",
};
const char * const yyrule[] = {
"$accept : iloc_program",
"iloc_program : instruction_list",
"instruction_list : instruction",
"instruction_list : label_def instruction",
"instruction_list : instruction instruction_list",
"instruction_list : label_def instruction instruction_list",
"instruction : operation",
"instruction : OPEN_BRACKET operation_list CLOSE_BRACKET",
"operation_list : operation",
"operation_list : operation SEMICOLON operation_list",
"operation : the_opcode operand_list ARROW operand_list",
"operation : the_opcode operand_list",
"operation : the_opcode ARROW operand_list",
"operation : the_opcode",
"the_opcode : OPCODE",
"operand_list : reg",
"operand_list : reg COMMA operand_list",
"operand_list : const",
"operand_list : const COMMA operand_list",
"operand_list : lbl",
"operand_list : lbl COMMA operand_list",
"reg : REGISTER",
"const : NUMBER",
"lbl : LABEL",
"label_def : TARGET",
};
#endif
#if YYDEBUG
#include <stdio.h>
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short *yyss;
short *yysslim;
YYSTYPE *yyvs;
int yystacksize;
#line 255 "iloc.y"
 /* Support Code */

/* Create a new initialized Operands structure */
Operands* new_operands()
{
    Operands* operands_ptr = malloc(sizeof(Operands));
    operands_ptr->num_regs = 0;
    operands_ptr->regs = NULL;
    operands_ptr->num_consts = 0;
    operands_ptr->consts = NULL;
    operands_ptr->num_labels = 0;
    operands_ptr->labels = NULL;
    
    return(operands_ptr);
}

/* Append the second list of operands to the end of the first */
Operand* append_operands(Operand* list1, Operand* list2)
{
    Operand* start = list1;

    if (!list1)
	return list2;
    
    while(list1->next)
	list1 = list1->next;

    list1->next = list2;

    return(start);
}

/* Make sure that the operation was called with the correct number and type
   of arguments */
int verify_args(Opcode* operation,int srcs, int consts, int labels, int defs)
{
    char* error_message;

    if (operation->srcs != srcs)
    {
	error_message = malloc(MAX_ERROR_MESSAGE_LENGTH*sizeof(char));
	sprintf(error_message,"%s used with incorrect number of source registers",
		operation->string);
	yyerror(error_message);
	free(error_message);
	return 0;
    }
    
    if (operation->consts != consts)
    {
	error_message = malloc(MAX_ERROR_MESSAGE_LENGTH*sizeof(char));
	sprintf(error_message,"%s used with incorrect number of constants",
		operation->string);
	yyerror(error_message);
	free(error_message);
	return 0;
    }

    if (operation->labels != labels)
    {
	error_message = malloc(MAX_ERROR_MESSAGE_LENGTH*sizeof(char));
	sprintf(error_message,"%s used with incorrect number of labels",
		operation->string);
	yyerror(error_message);
	free(error_message);
	return 0;
    }

    if (operation->defs != defs)
    {
	error_message = malloc(MAX_ERROR_MESSAGE_LENGTH*sizeof(char));
	sprintf(error_message,"%s used with incorrect number of defined registers",
		operation->string);
	yyerror(error_message);
	free(error_message);
	return 0;
    }

    return 1;
}
    
	
void yyerror(char* s)
{
  (void) fprintf(stderr, "%s at line %d\n", s, line_counter);
  error_found = 1;
}
#line 323 "y.tab.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack()
{
    int newsize, i;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    i = yyssp - yyss;
    newss = yyss ? (short *)realloc(yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    yyss = newss;
    yyssp = newss + i;
    newvs = yyvs ? (YYSTYPE *)realloc(yyvs, newsize * sizeof *newvs) :
      (YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab

#ifndef YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG void
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* YYPARSE_PARAM */
#ifndef YYPARSE_PARAM_TYPE
#define YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define YYPARSE_PARAM_ARG YYPARSE_PARAM_TYPE YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL YYPARSE_PARAM_TYPE YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! YYPARSE_PARAM */

int
yyparse (YYPARSE_PARAM_ARG)
    YYPARSE_PARAM_DECL
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register const char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate])) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#if defined(lint) || defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 71 "iloc.y"
{
		     first_instruction = yyvsp[0].inst_ptr;
		 }
break;
case 2:
#line 77 "iloc.y"
{
		     yyval.inst_ptr = yyvsp[0].inst_ptr;
		 }
break;
case 3:
#line 81 "iloc.y"
{
		     Label* label_definition = get_label(yyvsp[-1].ival);
		     label_definition->target = yyvsp[0].inst_ptr;
		     yyval.inst_ptr = yyvsp[0].inst_ptr;
		 }
break;
case 4:
#line 87 "iloc.y"
{
		     yyvsp[-1].inst_ptr->next = yyvsp[0].inst_ptr;
		     yyval.inst_ptr = yyvsp[-1].inst_ptr;
		 }
break;
case 5:
#line 92 "iloc.y"
{
		     Label* label_definition = get_label(yyvsp[-2].ival);
		     label_definition->target = yyvsp[-1].inst_ptr;
		     yyvsp[-1].inst_ptr->next = yyvsp[0].inst_ptr;
		     yyval.inst_ptr = yyvsp[-1].inst_ptr;
		 }
break;
case 6:
#line 101 "iloc.y"
{
		     yyval.inst_ptr = malloc(sizeof(Instruction));
		     yyval.inst_ptr->operations = yyvsp[0].op_ptr;
		     yyval.inst_ptr->next = NULL;
		 }
break;
case 7:
#line 107 "iloc.y"
{
		     yyval.inst_ptr = malloc(sizeof(Instruction));
		     yyval.inst_ptr->operations = yyvsp[-1].op_ptr;
		     yyval.inst_ptr->next = NULL;
		 }
break;
case 8:
#line 115 "iloc.y"
{
		     yyval.op_ptr = yyvsp[0].op_ptr;
		 }
break;
case 9:
#line 119 "iloc.y"
{
		     yyvsp[-2].op_ptr->next = yyvsp[0].op_ptr;
		     yyval.op_ptr = yyvsp[-2].op_ptr;
		 }
break;
case 10:
#line 126 "iloc.y"
{
		     verify_args(yyvsp[-3].opcode_ptr,yyvsp[-2].operands_ptr->num_regs,yyvsp[-2].operands_ptr->num_consts+yyvsp[0].operands_ptr->num_consts,
				 yyvsp[-2].operands_ptr->num_labels+yyvsp[0].operands_ptr->num_labels,yyvsp[0].operands_ptr->num_regs);
		     yyval.op_ptr = malloc(sizeof(Operation));
		     yyval.op_ptr->opcode = yyvsp[-3].opcode_ptr->name;
		     yyval.op_ptr->srcs = yyvsp[-2].operands_ptr->regs;
		     yyval.op_ptr->consts = append_operands(yyvsp[-2].operands_ptr->consts,yyvsp[0].operands_ptr->consts);
		     yyval.op_ptr->labels = append_operands(yyvsp[-2].operands_ptr->labels,yyvsp[0].operands_ptr->labels);
		     yyval.op_ptr->defs = yyvsp[0].operands_ptr->regs;
		     yyval.op_ptr->next = NULL;
		     free(yyvsp[-2].operands_ptr);
		     free(yyvsp[0].operands_ptr);
		 }
break;
case 11:
#line 140 "iloc.y"
{
		     verify_args(yyvsp[-1].opcode_ptr,yyvsp[0].operands_ptr->num_regs,yyvsp[0].operands_ptr->num_consts,yyvsp[0].operands_ptr->num_labels,0);
		     yyval.op_ptr = malloc(sizeof(Operation));
		     yyval.op_ptr->opcode = yyvsp[-1].opcode_ptr->name;
		     yyval.op_ptr->srcs = yyvsp[0].operands_ptr->regs;
		     yyval.op_ptr->consts = yyvsp[0].operands_ptr->consts;
		     yyval.op_ptr->labels = yyvsp[0].operands_ptr->labels;
		     yyval.op_ptr->defs = NULL;
		     yyval.op_ptr->next = NULL;
		     free(yyvsp[0].operands_ptr);
		 }
break;
case 12:
#line 152 "iloc.y"
{
		     verify_args(yyvsp[-2].opcode_ptr,0,yyvsp[0].operands_ptr->num_consts,yyvsp[0].operands_ptr->num_labels,yyvsp[0].operands_ptr->num_regs);
		     yyval.op_ptr = malloc(sizeof(Operation));
		     yyval.op_ptr->opcode = yyvsp[-2].opcode_ptr->name;
		     yyval.op_ptr->srcs = NULL;
		     yyval.op_ptr->consts = yyvsp[0].operands_ptr->consts;
		     yyval.op_ptr->labels = yyvsp[0].operands_ptr->labels;
		     yyval.op_ptr->defs = yyvsp[0].operands_ptr->regs;
		     yyval.op_ptr->next = NULL;
		     free(yyvsp[0].operands_ptr);
		 }
break;
case 13:
#line 164 "iloc.y"
{
		     verify_args(yyvsp[0].opcode_ptr,0,0,0,0);
		     yyval.op_ptr = malloc(sizeof(Operation));
		     yyval.op_ptr->opcode = yyvsp[0].opcode_ptr->name;
		     yyval.op_ptr->srcs = NULL;
		     yyval.op_ptr->consts = NULL;
		     yyval.op_ptr->labels = NULL;
		     yyval.op_ptr->defs = NULL;
		     yyval.op_ptr->next = NULL;
		 }
break;
case 14:
#line 177 "iloc.y"
{
		     yyval.opcode_ptr = current_opcode;
		 }
break;
case 15:
#line 183 "iloc.y"
{
		     yyval.operands_ptr = new_operands();
		     yyval.operands_ptr->num_regs = 1;
		     yyval.operands_ptr->regs = yyvsp[0].operand_ptr;
		 }
break;
case 16:
#line 189 "iloc.y"
{
		     yyval.operands_ptr = yyvsp[0].operands_ptr;
		     yyval.operands_ptr->num_regs += 1;
		     yyvsp[-2].operand_ptr->next = yyval.operands_ptr->regs;
		     yyval.operands_ptr->regs = yyvsp[-2].operand_ptr;
		 }
break;
case 17:
#line 196 "iloc.y"
{
		     yyval.operands_ptr = new_operands();
		     yyval.operands_ptr->num_consts = 1;
		     yyval.operands_ptr->consts = yyvsp[0].operand_ptr;
		 }
break;
case 18:
#line 202 "iloc.y"
{
		     yyval.operands_ptr = yyvsp[0].operands_ptr;
		     yyval.operands_ptr->num_consts += 1;
		     yyvsp[-2].operand_ptr->next = yyval.operands_ptr->consts;
		     yyval.operands_ptr->consts = yyvsp[-2].operand_ptr;
		 }
break;
case 19:
#line 209 "iloc.y"
{
		     yyval.operands_ptr = new_operands();
		     yyval.operands_ptr->num_labels = 1;
		     yyval.operands_ptr->labels = yyvsp[0].operand_ptr;
		 }
break;
case 20:
#line 215 "iloc.y"
{
		     yyval.operands_ptr = yyvsp[0].operands_ptr;
		     yyval.operands_ptr->num_labels += 1;
		     yyvsp[-2].operand_ptr->next = yyval.operands_ptr->labels;
		     yyval.operands_ptr->labels = yyvsp[-2].operand_ptr;
		 }
break;
case 21:
#line 224 "iloc.y"
{
		     yyval.operand_ptr = malloc(sizeof(Operand));
		     yyval.operand_ptr->value = (int) strtol(&yytext[1], (char**) NULL, 10);
		     yyval.operand_ptr->next = NULL;
		 }
break;
case 22:
#line 232 "iloc.y"
{
		     yyval.operand_ptr = malloc(sizeof(Operand));
		     yyval.operand_ptr->value = (int) strtol(yytext, (char**) NULL, 10);
		     yyval.operand_ptr->next = NULL;
		 }
break;
case 23:
#line 240 "iloc.y"
{
		     yyval.operand_ptr = malloc(sizeof(Operand));
		     yyval.operand_ptr->value = insert_label(yytext);
		     yyval.operand_ptr->next = NULL;
		 }
break;
case 24:
#line 248 "iloc.y"
{
		     int last_char = strlen(yytext) - 1;
		     yytext[last_char] = '\0';
		     yyval.ival = insert_label(yytext);
		 }
break;
#line 729 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
