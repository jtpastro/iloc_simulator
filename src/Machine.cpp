#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <exception>
#include "Machine.hpp"

Machine::Machine(uint reg_size, uint mem_size, Program prog)
{
    program = prog;
    PC = 0;
    if (reg_size > 0)
        NUM_REGISTERS = reg_size;
    if (mem_size > 0)
        MEMORY_SIZE = mem_size;
    register_bank.resize(NUM_REGISTERS);
    memory_bank.resize(MEMORY_SIZE);
}

Machine::Machine(State state, Program prog)
{
    Program program = prog;
    PC = state.PC;
    register_bank = state.register_bank;
    memory_bank = state.memory_bank;
    NUM_REGISTERS = register_bank.size();
    MEMORY_SIZE = memory_bank.size();
}

int Machine::get_register(uint reg)
{
    if (reg < NUM_REGISTERS)
        return register_bank[reg].data;
    fprintf(stderr,"Simulator Error: Invalid register number r%d used.\n", reg);
    exit(EXIT_FAILURE);
}

void Machine::set_register(uint reg, int value)
{
    if (reg < NUM_REGISTERS){
        register_bank[reg].data = value;
        register_bank[reg].accessed = true;
    } else {
        fprintf(stderr,"Simulator Error: Invalid register number r%d used.\n", reg);
        exit(EXIT_FAILURE);
    }
}

char Machine::get_memory(uint location)
{
    if (location < MEMORY_SIZE)
        return memory_bank[location].data; 
    fprintf(stderr,"Simulator Error: Invalid memory address %d accessed.\n", location);
    exit(EXIT_FAILURE);
}

int Machine::get_word(uint location)
{
    int value = ((int)(unsigned char)get_memory(location) << 24 | 
		 (int)(unsigned char)get_memory(location+1) << 16 |
		 (int)(unsigned char)get_memory(location+2) << 8 | 
		 (int)(unsigned char)get_memory(location+3));
    return(value);
}

void Machine::set_memory(uint location,char value)
{
    if (location < MEMORY_SIZE){
        memory_bank[location].data = value;
        memory_bank[location].accessed = true;
    } else {
        fprintf(stderr,"Simulator Error: Invalid memory address %d accessed.\n", location);
        exit(EXIT_FAILURE);
    }
}

void Machine::reg_state(){	
    int i;
    for(i=0;i<NUM_REGISTERS;i++){
        printf("Accessed: %d, Content: %d\n", register_bank[i].accessed, register_bank[i].data);
    }
}

void Machine::mem_state(){	
    int i;
    for(i=0;i<MEMORY_SIZE;i++){
        printf("Accessed: %d, Content: %d\n", memory_bank[i].accessed, memory_bank[i].data);
    }
}

void Machine::run(){
    try {
        while(true){ execute_operation(); }
    } catch(std::exception& e) {}
}

State Machine::execute_next(){
    execute_operation();
    State state;
    //state.PC = PC;
    //state.register_bank = register_bank;
    return state;
}

void Machine::onereg(Operation op, int value){ 
    set_register(op.regs.back(), value); 
    PC++;
}

void Machine::execute_operation(){
    Operation op = program.get_operation(PC); 
    int i;
    int result;
    uint location;
    switch(op.opcode) {
        case NOP:
            PC++;
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
        case ADDI:
            result = get_register(op.regs[0]) + op.consts[0];
            onereg(op, result);
            break;
        case SUBI:
            result = get_register(op.regs[0]) - op.consts[0];
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
            PC++;
            break;
        case STOREAI:
            for(i=0;i<4;i++) {
                result = (get_register(op.regs[0]) << (8*i)) >> 24;
                location = get_register(op.regs[1]) + op.consts[0] + i;
                set_memory(location, result);
            }
            PC++;
            break;
        case STOREAO:
            for(i=0;i<4;i++) {
                result = (get_register(op.regs[0]) << (8*i)) >> 24;
                location = get_register(op.regs[1]) + get_register(op.regs[2]) + i;
                set_memory(location, result);
            }
            PC++;
            break; 
        case CSTORE:
            result = (get_register(op.regs[0]) << 24) >> 24;
            location = get_register(op.regs[1]);
            set_memory(location, result);
            PC++;
            break;
        case CSTOREAI:
            result = (get_register(op.regs[0]) << 24) >> 24;
            location = get_register(op.regs[1]) + op.consts[0];
            set_memory(location, result);
            PC++;
            break;
        case CSTOREAO:
            result = (get_register(op.regs[0]) << 24) >> 24;
            location = get_register(op.regs[1]) + 
                get_register(op.regs[2]);
            set_memory(location, result);
            PC++;
            break;
        case JUMPI:
            PC = (program.get_label(op.labels[0]));
            
            break;
        case CBR:
            if (get_register(op.regs[0]))
                PC = (program.get_label(op.labels[0]));
            else
                PC = (program.get_label(op.labels[1]));
            break;
        case CBR_LT:
            if (get_register(op.regs[0])<0)
                PC = (program.get_label(op.labels[0]));
            else
                PC = (program.get_label(op.labels[1]));
            break;
        case CBR_LE:
            if (get_register(op.regs[0])<=0)
                PC = (program.get_label(op.labels[0]));
            else
                PC = (program.get_label(op.labels[1]));
            break;
        case CBR_EQ:
            if (get_register(op.regs[0])==0)
                PC = (program.get_label(op.labels[0]));
            else
                PC = (program.get_label(op.labels[1]));
            break;
        case CBR_GE:
            if (get_register(op.regs[0])>=0)
                PC = (program.get_label(op.labels[0]));
            else
                PC = (program.get_label(op.labels[1]));
            break;
        case CBR_GT:
            if (get_register(op.regs[0])>0)
                PC = (program.get_label(op.labels[0]));
            else
                PC = (program.get_label(op.labels[1]));
            break;
        case CBR_NE:
            if (get_register(op.regs[0])!=0)
                PC = (program.get_label(op.labels[0]));
            else
                PC = (program.get_label(op.labels[1]));
            break;
        case COMP:
            if (get_register(op.regs[0]) > 
                    get_register(op.regs[1]))
                result = 1;
            else if (get_register(op.regs[0]) == 
                    get_register(op.regs[1]))
                result = 0;
            else
                result = -1;
            onereg(op,result);
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
        case OUTPUT:
            if (!quiet)
                printf("%d\n", get_word(op.consts[0]));
            PC++;
            break;
        case COUTPUT:
            if (!quiet)
                printf("%d\n", get_memory(op.consts[0]));
            PC++;
            break;
        default:
            fprintf(stderr,"Simulator Error: Invalid opcode encountered in execute_operation.");
            exit(EXIT_FAILURE);
            break;
    }
}
