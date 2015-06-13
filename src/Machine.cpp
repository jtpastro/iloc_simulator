#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <exception>
#include "Machine.hpp"

Machine::Machine(Program prog)
{
    program = prog;
    state.PC=0;
    quiet=false;
}

int Machine::get_register(std::string reg)
{
    std::map<std::string,int>::iterator it = state.registers.find(reg);
    if(it!=state.registers.end())
        return it->second;
    else {
        if(!quiet)
            fprintf(stderr,"WARNING: register %s not initialized.\n", reg);
        state.registers[reg] = 0;
        return 0;
    }
}

void Machine::set_register(std::string reg, int value)
{
    state.registers[reg] = value;
}

char Machine::get_memory(uint location)
{
    std::map<uint,char>::iterator it = state.memory.find(location);
    if(it!=state.memory.end())
        return it->second;
    else {
        if(!quiet)
            fprintf(stderr,"WARNING: memory address %d was not initialized.\n", location);
        state.memory[location] = 0;
        return 0;
    }
}

int Machine::get_word(uint location)
{
    int value = ((int)(unsigned char)get_memory(location) << 24 | 
		         (int)(unsigned char)get_memory(location+1) << 16 |
		         (int)(unsigned char)get_memory(location+2) << 8 | 
        		 (int)(unsigned char)get_memory(location+3));
    return(value);
}

uint Machine::get_branch_destination(std::string lbl){
    try {
        return program.get_label(lbl);
    }
    catch (const std::out_of_range& oor) {
        fprintf(stderr,"Semantic Error: Label undeclared: %s.\n", lbl);
        exit(EXIT_FAILURE);
    }
}

void Machine::set_memory(uint location,char value)
{
    state.memory[location] = value;
}

State Machine::get_state(){
    return state;
}

void Machine::set_state(State stat){
    state = stat;
}

void Machine::reg_state(){	
    printf("|%-12s%s|\n","Register","Content");
    for (std::map<std::string,int>::iterator it=state.registers.begin(); it!=state.registers.end(); ++it)
        printf("|%-12s%d|", it->first, it->second);
}

void Machine::mem_state(){	
    printf("|%-12s%s|\n","Address","Content");
    for (std::map<uint,char>::iterator it=state.memory.begin(); it!=state.memory.end(); ++it)
        printf("|%010d: %d|", it->first, it->second);
}

void Machine::prog_state(){	
    printf("Program code:\n");
    for(int i=0; i<program.get_size(); i++)
        printf("%s\n", program.get_line(i));
}

void Machine::run(){
    try {
        while(true){ execute_operation(); }
    } catch (const std::out_of_range& oor) {}
}

void Machine::onereg(Operation op, int value){ 
    set_register(op.regs.back(), value); 
    state.PC++;
}

void Machine::execute_operation(){
    Operation op = program.get_operation(state.PC); 
    int i;
    int result;
    uint location;
    switch(op.opcode) {
        case NOP:
            state.PC++;
            break;
        case ADD:
            result = get_register(op.regs[0]) + 
                get_register(op.regs[1]);
            onereg(op, result);
            break;
        case SUB:
            result = get_register(op.regs[0]) - 
                get_register(op.regs[1]);
            onereg(op, result);
            break;
        case MULT:
            result = get_register(op.regs[0]) * 
                get_register(op.regs[1]);
            onereg(op, result);
            break;
        case DIV:
            result = get_register(op.regs[0]) / 
                get_register(op.regs[1]);
            onereg(op, result);
            break;	
        case AND:
            result = get_register(op.regs[0]) & 
                get_register(op.regs[1]);
            onereg(op, result);
            break;
        case OR:
            result = get_register(op.regs[0]) | 
                get_register(op.regs[1]);
            onereg(op, result);
            break;
        case XOR:
            result = get_register(op.regs[0]) ^ 
                get_register(op.regs[1]);
            onereg(op, result);
            break;  
        case ANDI:
            result = get_register(op.regs[0]) & op.consts[0];
            onereg(op, result);
            break;
        case ORI:
            result = get_register(op.regs[0]) | op.consts[0];
            onereg(op, result);
            break;
        case XORI:
            result = get_register(op.regs[0]) ^ op.consts[0];
            onereg(op, result);
            break;	  
        case ADDI:
            result = get_register(op.regs[0]) + op.consts[0];
            onereg(op, result);
            break;
        case SUBI:
            result = get_register(op.regs[0]) - op.consts[0];
            onereg(op, result);
            break;
        case RSUBI:
            result = op.consts[0] - get_register(op.regs[0]);
            onereg(op, result);
            break;
        case MULTI:
            result = get_register(op.regs[0]) * op.consts[0];
            onereg(op, result);
            break;
        case DIVI:
            result = get_register(op.regs[0]) / op.consts[0];
            onereg(op, result);
            break;
        case RDIVI:
            result = op.consts[0] / get_register(op.regs[0]);
            onereg(op, result);
            break;
        case LSHIFT:
            result = get_register(op.regs[0]) << 
                get_register(op.regs[1]);
            onereg(op, result);
            break;
        case LSHIFTI:
            result = get_register(op.regs[0]) << op.consts[0];
            onereg(op, result);
            break;
        case RSHIFT:
            result = get_register(op.regs[0]) >> 
                get_register(op.regs[1]);
            onereg(op, result);
            break;
        case RSHIFTI:
            result = get_register(op.regs[0]) >> op.consts[0];
            onereg(op, result);
            break;
        case LOADI:
            result = op.consts[0];
            onereg(op, result);
            break;
        case LOAD:
            result = get_word(get_register(op.regs[0]));
            onereg(op, result);
            break;
        case LOADAI:
            result = get_word(get_register(op.regs[0]) +
                    op.consts[0]);
            onereg(op, result);
            break;
        case LOADAO:
            result = get_word(get_register(op.regs[0]) +
                    get_register(op.regs[1]));
            onereg(op, result);
            break;
        case CLOAD:
            result = get_memory(get_register(op.regs[0]));
            onereg(op, result);
            break;
        case CLOADAI:
            result = get_memory(get_register(op.regs[0]) +
                    op.consts[0]);
            onereg(op, result);
            break;
        case CLOADAO:
            result = get_memory(get_register(op.regs[0]) +
                    get_register(op.regs[1]));
            onereg(op, result);
            break;
        case STORE:
            for(i=0;i<4;i++) {
                result = (get_register(op.regs[0]) << (8*i)) >> 24;
                location = get_register(op.regs[1]) + i;
                set_memory(location, result);
            }
            state.PC++;
            break;
        case STOREAI:
            for(i=0;i<4;i++) {
                result = (get_register(op.regs[0]) << (8*i)) >> 24;
                location = get_register(op.regs[1]) + op.consts[0] + i;
                set_memory(location, result);
            }
            state.PC++;
            break;
        case STOREAO:
            for(i=0;i<4;i++) {
                result = (get_register(op.regs[0]) << (8*i)) >> 24;
                location = get_register(op.regs[1]) + get_register(op.regs[2]) + i;
                set_memory(location, result);
            }
            state.PC++;
            break; 
        case CSTORE:
            result = (get_register(op.regs[0]) << 24) >> 24;
            location = get_register(op.regs[1]);
            set_memory(location, result);
            state.PC++;
            break;
        case CSTOREAI:
            result = (get_register(op.regs[0]) << 24) >> 24;
            location = get_register(op.regs[1]) + op.consts[0];
            set_memory(location, result);
            state.PC++;
            break;
        case CSTOREAO:
            result = (get_register(op.regs[0]) << 24) >> 24;
            location = get_register(op.regs[1]) + 
                get_register(op.regs[2]);
            set_memory(location, result);
            state.PC++;
            break;
        case JUMPI:
            state.PC = (get_branch_destination(op.labels[0]));
            break;
        case JUMP:
            state.PC = get_register(op.regs[0]);
            break;
        case CBR:
            if (get_register(op.regs[0]))
                state.PC = (get_branch_destination(op.labels[0]));
            else
                state.PC = (get_branch_destination(op.labels[1]));
            break;
        case CMPLT:
            if (get_register(op.regs[0]) < 
                    get_register(op.regs[1]))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPLE:
            if (get_register(op.regs[0]) <= 
                    get_register(op.regs[1]))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPEQ:
            if (get_register(op.regs[0]) == 
                    get_register(op.regs[1]))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPNE:
            if (get_register(op.regs[0]) != 
                    get_register(op.regs[1]))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPGE:
            if (get_register(op.regs[0]) >= 
                    get_register(op.regs[1]))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPGT:
            if (get_register(op.regs[0]) > 
                    get_register(op.regs[1]))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case I2I:
            result = get_register(op.regs[0]);
            onereg(op,result);
            break;
        case C2C:
        case C2I:
        case I2C:
            result = (get_register(op.regs[0]) << 24) >> 24;
            onereg(op,result);
            break;
        default:
            fprintf(stderr,"Simulator Error: Invalid opcode encountered in execute_operation.");
            exit(EXIT_FAILURE);
            break;
    }
}
