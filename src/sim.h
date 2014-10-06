/*  sim.h
 *  Header file for a simulator of the ILOC subset defined in
 *  "Engineering a Compiler" by Cooper and Torczon
 *  written by Todd Waterman
 *  11/30/00 */

#ifndef _SIM_H_
#define _SIM_H_

typedef enum effect_type {REGISTER=0,MEMORY,BRANCH,DISPLAY} Effect_Type; 

/* This keeps track of assignments to registers or memory so 
   parallel operations can be simulated */
typedef struct change {
    Effect_Type type;
    int location;
    int value;
    Operation* target;
    int cycles_away;
    struct change* next;
} Change;

/* Print a usage message */
void print_help();

/* Set stall flags */
void set_stall_mode(int);

/* Simulate the code and output results to standard out */
void simulate(Operation* code);

/* Execute a single operation */
Change* execute_operation(Operation* op);

/* onereg creates most of a change structure for the common case where 
   a single register is defined. */
Change* onereg(Operation* op);

/* storeop creates most of a change structure for a store operation */
Change* storeop(Operation* op);

/* Reduces the cycles_away of all changes by one and executes any changes
   that have a cycles_away of 0 */
Change* execute_changes(Change*,Change**,Operation**);

#endif /* _SIM_H_ */

