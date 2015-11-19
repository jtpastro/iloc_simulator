#include <sstream>  //std::stringstream
#include <iomanip>
#include "SimulationError.hpp" //SimulationError
#include "Machine.hpp"

template <class T> const T& max (const T& a, const T& b) {
  return (a<b)?b:a;
}

uint num_digits(uint number){
    uint digits = 0; do { number /= 10; digits++; } while (number != 0); return digits;
}

Machine::Machine(Program prog, uint mem_size, uint frame_start)
{
    program = prog;
    PC=0;
    frame_start = (frame_start/4)*4;
    bss = program.get_size()*4;
    fp = frame_start;
    memory.resize(mem_size*4);
}

int Machine::get_register(std::string reg)
{
    if(reg=="rbss") return bss;
    if(reg=="rarp") return fp;
    if(reg=="pc")   return PC;
    return registers[reg];
}

void Machine::set_register(std::string reg, int value)
{
    if(reg=="rbss" || reg=="rarp" || reg=="pc"){
        std::stringstream ss;
        ss << "Simulation error executing " << program.get_operation(PC).toString() << " at position " << PC*4 << ": register " << reg << " cannot be modified.";
        throw SimulationError(ss.str());
    }
    registers[reg] = value;
}

void Machine::check_access(uint location){
    if(location < bss || location >= memory.size()){
            std::stringstream ss;
            ss << "Simulation error executing " << program.get_operation(PC).toString() << " at position " << PC*4 << ": Illegal memory access.";
            throw SimulationError(ss.str());
    }
}

char Machine::get_memory(uint location)
{
    check_access(location);
    return memory[location - bss];
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
    return program.get_label(lbl);
}

void Machine::set_memory(uint location,char value)
{
    check_access(location);
    memory[location - bss] = value;
}

void Machine::set_word(uint location,int value)
{
    set_memory(location, value >> 24);
	set_memory(location+1, (value << 8) >> 24);
	set_memory(location+2, (value << 16) >> 24);
	set_memory(location+3, (value << 24) >> 24);
}

std::string Machine::reg_state(){	
    std::stringstream ss;
    ss << "\nRegisters state:\n" << "|pc| " << PC << '\n'<< "|rbss| " << bss << '\n' << "|rarp| " << fp << '\n';
    if(!registers.empty())
        for (std::map<std::string,int>::iterator it=registers.begin(); it!=registers.end(); ++it)
            ss << "|" << it->first << "| " << it->second << '\n';
    return ss.str();
}

std::string Machine::mem_state(){	
    std::stringstream ss;
    ss << "\nMemory state:\n";
    if(memory.empty())
        ss << "No memory address accessed.\n";
    else{
        uint n_digits = num_digits(memory.size());
        for (int i=bss; i<memory.size(); i+=4)
            ss << "|" << std::setfill('0') << std::setw(n_digits) << i << ".." << std::setfill('0') << std::setw(n_digits) << i+3 << "| " << get_word(i) << '\n';
    }
    return ss.str();
}

std::string Machine::prog_state(){
    uint n_digits = num_digits(memory.empty() ? bss : (memory.size()/4)*4+3);	
    std::stringstream ss;
    ss << "\nProgram code:\n";
    int i=0;
    for(; i<program.get_size(); i++)
        ss << "|" << std::setfill('0') << std::setw(n_digits) << i*4 << ".." << std::setfill('0') << std::setw(n_digits) << i*4+3 << "| "  << program.get_line(i) << '\n' ;
    for(; i<bss/4; i++)
        ss << "|" << std::setfill('0') << std::setw(n_digits) << i*4 << ".." << std::setfill('0') << std::setw(n_digits) << i*4+3 << "| nop\n";
    return ss.str();
}

std::string Machine::prog_state(uint pos){
    return (pos < program.get_size()) ? program.get_line(pos):"nop";
}

std::string Machine::exec_state(){
    std::stringstream ss;
    ss << "\nExecution state:\n";
    ss << op_count << " instructions executed in " << cycles <<  " cycles.\n";
    return ss.str();
}

void Machine::run(){
    while(execute_operation());
}

void Machine::onereg(Operation op, int value){ 
    set_register(op.get_regs().back(), value); 
    PC++;
}

bool Machine::execute_operation(){
    Operation op = program.get_operation(PC); 
    int result;
    uint location;
    switch(op.opcode) {
        case NOP:
            PC++;
            break;
        case ADD:
            result = get_register(op.get_regs().at(0)) + 
                get_register(op.get_regs().at(1));
            onereg(op, result);
            break;
        case SUB:
            result = get_register(op.get_regs().at(0)) - 
                get_register(op.get_regs().at(1));
            onereg(op, result);
            break;
        case MULT:
            result = get_register(op.get_regs().at(0)) * 
                get_register(op.get_regs().at(1));
            onereg(op, result);
            break;
        case DIV:
            result = get_register(op.get_regs().at(0)) / 
                get_register(op.get_regs().at(1));
            onereg(op, result);
            break;	
        case AND:
            result = get_register(op.get_regs().at(0)) & 
                get_register(op.get_regs().at(1));
            onereg(op, result);
            break;
        case OR:
            result = get_register(op.get_regs().at(0)) | 
                get_register(op.get_regs().at(1));
            onereg(op, result);
            break;
        case XOR:
            result = get_register(op.get_regs().at(0)) ^ 
                get_register(op.get_regs().at(1));
            onereg(op, result);
            break;  
        case ANDI:
            result = get_register(op.get_regs().at(0)) & op.get_constant();
            onereg(op, result);
            break;
        case ORI:
            result = get_register(op.get_regs().at(0)) | op.get_constant();
            onereg(op, result);
            break;
        case XORI:
            result = get_register(op.get_regs().at(0)) ^ op.get_constant();
            onereg(op, result);
            break;	  
        case ADDI:
            result = get_register(op.get_regs().at(0)) + op.get_constant();
            onereg(op, result);
            break;
        case SUBI:
            result = get_register(op.get_regs().at(0)) - op.get_constant();
            onereg(op, result);
            break;
        case RSUBI:
            result = op.get_constant() - get_register(op.get_regs().at(0));
            onereg(op, result);
            break;
        case MULTI:
            result = get_register(op.get_regs().at(0)) * op.get_constant();
            onereg(op, result);
            break;
        case DIVI:
            result = get_register(op.get_regs().at(0)) / op.get_constant();
            onereg(op, result);
            break;
        case RDIVI:
            result = op.get_constant() / get_register(op.get_regs().at(0));
            onereg(op, result);
            break;
        case LSHIFT:
            result = get_register(op.get_regs().at(0)) << 
                get_register(op.get_regs().at(1));
            onereg(op, result);
            break;
        case LSHIFTI:
            result = get_register(op.get_regs().at(0)) << op.get_constant();
            onereg(op, result);
            break;
        case RSHIFT:
            result = get_register(op.get_regs().at(0)) >> 
                get_register(op.get_regs().at(1));
            onereg(op, result);
            break;
        case RSHIFTI:
            result = get_register(op.get_regs().at(0)) >> op.get_constant();
            onereg(op, result);
            break;
        case LOADI:
            result = op.get_constant();
            onereg(op, result);
            break;
        case LOAD:
            location = get_register(op.get_regs().at(0));
            result = get_word(location);
            onereg(op, result);
            break;
        case LOADAI:
            location = get_register(op.get_regs().at(0)) +
                    op.get_constant();
            result = get_word(location);
            onereg(op, result);
            break;
        case LOADAO:
            location = get_register(op.get_regs().at(0)) +
                    get_register(op.get_regs().at(1));
            result = get_word(location);
            onereg(op, result);
            break;
        case CLOAD:
            location = get_register(op.get_regs().at(0));
            result = get_memory(location);
            onereg(op, result);
            break;
        case CLOADAI:
            location = get_register(op.get_regs().at(0)) +
                    op.get_constant();
            result = get_memory(location);
            onereg(op, result);
            break;
        case CLOADAO:
            location = get_register(op.get_regs().at(0)) +
                    get_register(op.get_regs().at(1));
            result = get_memory(location);
            onereg(op, result);
            break;
        case STORE:
            result = get_register(op.get_regs().at(0));
            location = get_register(op.get_regs().at(1));
            set_word(location, result);
            PC++;
            break;
        case STOREAI:
            result = get_register(op.get_regs().at(0));
            location = get_register(op.get_regs().at(1)) + op.get_constant();
            set_word(location, result);
            PC++;
            break;
        case STOREAO:
            result = get_register(op.get_regs().at(0));
            location = get_register(op.get_regs().at(1)) + get_register(op.get_regs().back());
            set_word(location, result);
            PC++;
            break; 
        case CSTORE:
            result = (get_register(op.get_regs().at(0)) << 24) >> 24;
            location = get_register(op.get_regs().at(1));
            set_memory(location, result);
            PC++;
            break;
        case CSTOREAI:
            result = (get_register(op.get_regs().at(0)) << 24) >> 24;
            location = get_register(op.get_regs().at(1)) + op.get_constant();
            set_memory(location, result);
            PC++;
            break;
        case CSTOREAO:
            result = (get_register(op.get_regs().at(0)) << 24) >> 24;
            location = get_register(op.get_regs().at(1)) + 
                get_register(op.get_regs().back());
            set_memory(location, result);
            PC++;
            break;
        case JUMPI:
            PC = (get_branch_destination(op.get_labels().at(0)));
            break;
        case JUMP:
            PC = get_register(op.get_regs().at(0));
            break;
        case CBR:
            if (get_register(op.get_regs().at(0)))
                PC = (get_branch_destination(op.get_labels().at(0)));
            else
                PC = (get_branch_destination(op.get_labels().at(1)));
            break;
        case CMPLT:
            if (get_register(op.get_regs().at(0)) < 
                    get_register(op.get_regs().at(1)))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPLE:
            if (get_register(op.get_regs().at(0)) <= 
                    get_register(op.get_regs().at(1)))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPEQ:
            if (get_register(op.get_regs().at(0)) == 
                    get_register(op.get_regs().at(1)))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPNE:
            if (get_register(op.get_regs().at(0)) != 
                    get_register(op.get_regs().at(1)))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPGE:
            if (get_register(op.get_regs().at(0)) >= 
                    get_register(op.get_regs().at(1)))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case CMPGT:
            if (get_register(op.get_regs().at(0)) > 
                    get_register(op.get_regs().at(1)))
                result = 1;
            else
                result = 0;
            onereg(op,result);
            break;
        case I2I:
            result = get_register(op.get_regs().at(0));
            onereg(op,result);
            break;
        case C2C:
        case C2I:
        case I2C:
            result = (get_register(op.get_regs().at(0)) << 24) >> 24;
            onereg(op,result);
            break;
        case HALT:
            return false;
        default:
            std::stringstream ss;
            ss << "Simulation error: Invalid opcode encountered in execute_operation.";
            throw SimulationError(ss.str());
    }
    cycles += op.get_latency();
    op_count++;
    return true;
}
