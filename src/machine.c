/*  machine.c
 *  Source code for low level operations used in a simulator for
 *  the ILOC subset defined in
 *  "Engineering a Compiler" by Cooper and Torczon
 *  written by Todd Waterman
 *  11/30/00 */

#include <stdlib.h>
#include <stdio.h>
#include "machine.h"

/* Initialize arrays used in the machine during the simulation */
void initialize_machine(int reg_size, int mem_size)
{
    int i;
    
    if (reg_size == 0)
	NUM_REGISTERS = DEFAULT_NUM_REGISTERS;
    else 
	NUM_REGISTERS = reg_size;

    if (mem_size == 0)
	MEMORY_SIZE = DEFAULT_MEMORY_SIZE;
    else
	MEMORY_SIZE = mem_size;

    register_bank = (Reg*)malloc(2*NUM_REGISTERS*sizeof(int));

    for(i=0;i<NUM_REGISTERS;i++){
	    register_bank[i].data = 0;
	    register_bank[i].accessed= 0;
    }


    memory_bank = (Mem*)malloc(MEMORY_SIZE*(sizeof(char)+sizeof(int)));

    for(i=0;i<MEMORY_SIZE;i++){
	    memory_bank[i].data = 0;
	    memory_bank[i].accessed = 0;
    }

}

/* These functions allow word (integer) access to memory, a word of memory
   is assumed to be 4 bytes */
int get_word(int location)
{
    int value = ((int)(unsigned char)get_memory(location) << 24 | 
		 (int)(unsigned char)get_memory(location+1) << 16 |
		 (int)(unsigned char)get_memory(location+2) << 8 | 
		 (int)(unsigned char)get_memory(location+3));
    return(value);
}

void set_word(int location, int value)
{
    set_memory(location,(char)(value >> 24));
    set_memory(location+1,(char)((value << 8) >> 24));
    set_memory(location+2,(char)((value << 16) >> 24));
    set_memory(location+3,(char)((value << 24) >> 24));
}


/* The following functions manage access to the register and memory banks,
   this prevents the simulator from crashing if the input program uses an 
   invalid memory address or register name */

int get_register(int reg)
{
    if (reg >= 0 && reg < NUM_REGISTERS)
	return register_bank[reg].data;
    
    fprintf(stderr,"Simulator Error: Invalid register number r%d used.\n", reg);
    exit(1);
}
	
void set_register(int reg, int value)
{
    if (reg >= 0 && reg < NUM_REGISTERS)
	register_bank[reg].data = value;
    else
    {
	fprintf(stderr,"Simulator Error: Invalid register number r%d used.\n", reg);
	exit(1);
    }
}


char get_memory(int location)
{
    if (location >= 0 && location < MEMORY_SIZE)
	return memory_bank[location].data;
    
    fprintf(stderr,"Simulator Error: Invalid memory address %d accessed.\n", location);
    exit(1);
}

void set_memory(int location,char value)
{
    if (location >= 0 && location < MEMORY_SIZE)
	memory_bank[location].data = value;
    else
    {
	fprintf(stderr,"Simulator Error: Invalid memory address %d accessed.\n", location);
	exit(1);
    }
}


