/*  sim.h
 *  Header file for a simulator of the ILOC subset defined in
 *  "Engineering a Compiler" by Cooper and Torczon
 *  written by Todd Waterman
 *  11/30/00 */

#ifndef _SIM_H_
#define _SIM_H_

/* Pointer to the first operation of the parsed block */
/* It is declared here so it is not visible to other programs that include
   the header file */
extern vector<Operation> first_operation;
extern int error_found;

int yyparse();

/* Run yyparse and return a pointer to the first operation if no
   errors occur, otherwise return NULL */
vector<Operation> parse();

/* Initialize the tables needed for get_opcode */
void opcode_init(void);

typedef enum effect_type {REGISTER=0,MEMORY,BRANCH,DISPLAY} Effect_Type; 

/* Simulate the code and output results to standard out */
void simulate(Operation* code);

#endif /* _SIM_H_ */

