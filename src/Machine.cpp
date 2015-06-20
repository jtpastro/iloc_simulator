#include <iostream> //cout, cerr
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
            std::cerr <<"WARNING: register " << reg << " not initialized." << std::endl;
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
            std::cerr <<"WARNING: memory address " << location << " was not initialized." << std::endl;
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
        std::cerr <<"Semantic Error: label undeclared: " << lbl << "." << std::endl;
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
    std::cout << std::endl << "Registers state: " << std::endl;
    if(state.registers.empty())
        std::cout << "No registers accessed." << std::endl;
    else
        for (std::map<std::string,int>::iterator it=state.registers.begin(); it!=state.registers.end(); ++it)
            std::cout << it->first << ": " << it->second << std::endl;
}

void Machine::mem_state(){	
    std::cout << std::endl << "Memory state:" << std::endl;
    if(state.memory.empty())
        std::cout << "No memory address accessed." << std::endl;
    else
        for (std::map<uint,char>::iterator it=state.memory.begin(); it!=state.memory.end(); ++it)
            std::cout << it->first << ": " << std::to_string(it->second) << std::endl;
}

void Machine::prog_state(){	
    std::cout << std::endl << "Program code:" << std::endl;
    for(int i=0; i<program.get_size(); i++)
        std::cout << program.get_line(i) << std::endl ;
}

void Machine::exec_state(){
    std::cout << std::endl << "Execution state:" << std::endl;
    std::cout << op_count << " instructions executed in " << cycles <<  " cycles." << std::endl;
}

void Machine::run(){
    while(state.PC < program.get_size())
        execute_operation();
    prog_state();
    reg_state();
    mem_state();
    exec_state();
}

void Machine::onereg(Operation op, int value){ 
    set_register(op.get_last_register(), value); 
    state.PC++;
}

void Machine::execute_operation(){
    Operation op = program.get_operation(state.PC); 
    uint i;
    int result;
    uint location;
    switch(op.opcode) {
        case NOP:
            state.PC++;
            break;
        case ADD:
            result = get_register(op.get_first_register()) + 
                get_register(op.get_second_register());
            onereg(op, result);
            break;
        case SUB:
            result = get_register(op.get_first_register()) - 
                get_register(op.get_second_register());
            onereg(op, result);
            break;
        case MULT:
            result = get_register(op.get_first_register()) * 
                get_register(op.get_second_register());
            onereg(op, result);
            break;
        case DIV:
            result = get_register(op.get_first_register()) / 
                get_register(op.get_second_register());
            onereg(op, result);
            break;	
        case AND:
            result = get_register(op.get_first_register()) & 
                get_register(op.get_second_register());
            onereg(op, result);
            break;
        case OR:
            result = get_register(op.get_first_register()) | 
                get_register(op.get_second_register());
            onereg(op, result);
            break;
        case XOR:
            result = get_register(op.get_first_register()) ^ 
                get_register(op.get_second_register());
            onereg(op, result);
            break;  
        case ANDI:
            result = get_register(op.get_first_register()) & op.get_constant();
            onereg(op, result);
            break;
        case ORI:
            result = get_register(op.get_first_register()) | op.get_constant();
            onereg(op, result);
            break;
        case XORI:
            result = get_register(op.get_first_register()) ^ op.get_constant();
            onereg(op, result);
            break;	  
        case ADDI:
            result = get_register(op.get_first_register()) + op.get_constant();
            onereg(op, result);
            break;
        case SUBI:
            result = get_register(op.get_first_register()) - op.get_constant();
            onereg(op, result);
            break;
        case RSUBI:
            result = op.get_constant() - get_register(op.get_first_register());
            onereg(op, result);
            break;
        case MULTI:
            result = get_register(op.get_first_register()) * op.get_constant();
            onereg(op, result);
            break;
        case DIVI:
            result = get_register(op.get_first_register()) / op.get_constant();
            onereg(op, result);
            break;
        case RDIVI:
            result = op.get_constant() / get_register(op.get_first_register());
            onereg(op, result);
            break;
        case LSHIFT:
            result = get_register(op.get_first_register()) << 
                get_register(op.get_second_register());
            onereg(op, result);
            break;
        case LSHIFTI:
            result = get_register(op.get_first_register()) << op.get_constant();
            onereg(op, result);
            break;
        case RSHIFT:
            result = get_register(op.get_first_register()) >> 
                get_register(op.get_second_register());
            onereg(op, result);
            break;
        case RSHIFTI:
            result = get_register(op.get_first_register()) >> op.get_constant();
            onereg(op, result);
            break;
        case LOADI:
            result = op.get_constant();
            onereg(op, result);
            break;
        case LOAD:
            result = get_word(get_register(op.get_first_register()));
            onereg(op, result);
            break;
        case LOADAI:
            result = get_word(get_register(op.get_first_register()) +
                    op.get_constant());
            onereg(op, result);
            break;
        case LOADAO:
            result = get_word(get_register(op.get_first_register()) +
                    get_register(op.get_second_register()));
            onereg(op, result);
            break;
        case CLOAD:
            result = get_memory(get_register(op.get_first_register()));
            onereg(op, result);
            break;
        case CLOADAI:
            result = get_memory(get_register(op.get_first_register()) +
                    op.get_constant());
            onereg(op, result);
            break;
        case CLOADAO:
            result = get_memory(get_register(op.get_first_register()) +
                    get_register(op.get_second_register()));
            onereg(op, result);
            break;
        case STORE:
            for(i=0;i<4;i++) {
                result = (get_register(op.get_first_register()) << (8*i)) >> 24;
                location = get_register(op.get_second_register()) + i;
                set_memory(location, result);
            }
            state.PC++;
            break;
        case STOREAI:
            for(i=0;i<4;i++) {
                result = (get_register(op.get_first_register()) << (8*i)) >> 24;
                location = get_register(op.get_second_register()) + op.get_constant() + i;
                set_memory(location, result);
            }
            state.PC++;
            break;
        case STOREAO:
            for(i=0;i<4;i++) {
                result = (get_register(op.get_first_register()) << (8*i)) >> 24;
                location = get_register(op.get_second_register()) + get_register(op.get_last_register()) + i;
                set_memory(location, result);
            }
            state.PC++;
            break; 
        case CSTORE:
            result = (get_register(op.get_first_register()) << 24) >> 24;
            location = get_register(op.get_second_register());
            set_memory(location, result);
            state.PC++;
            break;
        case CSTOREAI:
            result = (get_register(op.get_first_register()) << 24) >> 24;
            location = get_register(op.get_second_register()) + op.get_constant();
            set_memory(location, result);
            state.PC++;
            break;
        case CSTOREAO:
            result = (get_register(op.get_first_register()) << 24) >> 24;
            location = get_register(op.get_second_register()) + 
                get_register(op.get_last_register());
            set_memory(location, result);
            state.PC++;
            break;
        case JUMPI:
            state.PC = (get_branch_destination(op.get_first_label()));
            break;
        case JUMP:
            state.PC = get_register(op.get_first_register());
            break;
        case CBR:
            if (get_register(op.get_first_register()))
                state.PC = (get_branch_destination(op.get_first_label()));
            else
                state.PC = (get_branch_destination(op.get_second_label()));
            break;
        case CMPLT:
            if (get_register(op.get_first_register()) < 
                    get_register(op.get_second_register()))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPLE:
            if (get_register(op.get_first_register()) <= 
                    get_register(op.get_second_register()))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPEQ:
            if (get_register(op.get_first_register()) == 
                    get_register(op.get_second_register()))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPNE:
            if (get_register(op.get_first_register()) != 
                    get_register(op.get_second_register()))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPGE:
            if (get_register(op.get_first_register()) >= 
                    get_register(op.get_second_register()))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPGT:
            if (get_register(op.get_first_register()) > 
                    get_register(op.get_second_register()))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case I2I:
            result = get_register(op.get_first_register());
            onereg(op,result);
            break;
        case C2C:
        case C2I:
        case I2C:
            result = (get_register(op.get_first_register()) << 24) >> 24;
            onereg(op,result);
            break;
        default:
            std::cerr <<"Simulator Error: Invalid opcode encountered in execute_operation." << std::endl;
            exit(EXIT_FAILURE);
            break;
    }
    cycles += op.get_latency();
    op_count++;
}
