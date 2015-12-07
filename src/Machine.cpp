#include <sstream>  //std::stringstream
#include <iomanip> //std::setfill
#include "SimulationError.hpp" //SimulationError
#include "Machine.hpp"

template <class T> const T& max (const T& a, const T& b) {
  return (a<b)?b:a;
}

size_t num_digits(size_t number){
    size_t digits = 0; do { number /= 10; digits++; } while (number != 0); return digits;
}

Machine::Machine(Program prog, size_t mem_size, size_t frame_start)
{
    program = prog;
    PC=0;
    frame_start = (frame_start/4)*4;
    bss = program.get_size()*4;
    fp = frame_start;
    memory.resize((1+(mem_size-1)/4)*4);
}

int Machine::mem_size(){
    return memory.size();
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
        ss << "Erro de simulação executando " << program.get_operation(PC).toString() << " na posição de memória " << PC*4 << ": o registrador " << reg << " não pode ser modificado.";
        throw SimulationError(ss.str());
    }
    registers[reg] = value;
}

void Machine::check_access(size_t location){
    if(location < bss || location >= memory.size()){
            std::stringstream ss;
            ss << "Erro de simulação executando " << program.get_operation(PC).toString() << " na posição de memória " << PC*4 << ": Acesso ilegal à memória.";
            throw SimulationError(ss.str());
    }
}

char Machine::get_memory(size_t location)
{
    check_access(location);
    return memory[location - bss];
}

int Machine::get_word(size_t location)
{
    int value = ((int)(unsigned char)get_memory(location) << 24 | 
		         (int)(unsigned char)get_memory(location+1) << 16 |
		         (int)(unsigned char)get_memory(location+2) << 8 | 
        		 (int)(unsigned char)get_memory(location+3));
    return(value);
}

size_t Machine::get_branch_destination(std::string lbl){
    return program.get_label(lbl);
}

void Machine::set_memory(size_t location,char value)
{
    check_access(location);
    memory[location - bss] = value;
}

void Machine::set_word(size_t location,int value)
{
    set_memory(location, value >> 24);
	set_memory(location+1, (value << 8) >> 24);
	set_memory(location+2, (value << 16) >> 24);
	set_memory(location+3, (value << 24) >> 24);
}

std::string Machine::reg_state(){	
    std::stringstream ss;
    ss << "|pc| " << PC << '\n'<< "|rbss| " << bss << '\n' << "|rarp| " << fp << '\n';
    if(!registers.empty())
        for (std::map<std::string,int>::iterator it=registers.begin(); it!=registers.end(); ++it)
            ss << "|" << it->first << "| " << it->second << '\n';
    return ss.str();
}

std::string Machine::print_address(size_t addr){
    std::stringstream ss;
    size_t n_digits = num_digits(memory.size());
    ss << "|" << std::setfill('0') << std::setw(n_digits) << addr << ".." << std::setfill('0') << std::setw(n_digits) << addr+3 << "| ";
    return ss.str();
}

std::string Machine::mem_state(){	
    std::stringstream ss;
    for (int i=bss; i<memory.size(); i+=4)
        ss << print_address(i) << get_word(i) << '\n';
    return ss.str();
}

std::string Machine::mem_state(size_t pos){	
    std::stringstream ss;
    if(pos < memory.size())
        ss << print_address(pos) << get_word(pos) << '\n';
    else
        ss << "Acesso ilegal à memória.\n";
    return ss.str();
}

std::string Machine::prog_state(){
    std::stringstream ss;
    int i=0;
    for(; i<program.get_size(); i++)
        ss << print_address(i*4) << program.get_line(i) << '\n' ;
    return ss.str();
}

std::string Machine::prog_state(size_t pos){
    std::stringstream ss;
    if(pos < program.get_size())
        ss << print_address(pos*4) << program.get_line(pos);
    else
        ss << "Posição fora da área de programa.";
    return ss.str();
}

std::string Machine::exec_state(){
    std::stringstream ss;
    ss << op_count << " instruções executadas em " << cycles <<  " ciclos.\n";
    return ss.str();
}

void Machine::run(){
    while(execute_operation());
}

bool Machine::execute_operation(){
    Operation op = program.get_operation(PC); 
    int result;
    size_t location;
    switch(op.opcode) {
        case NOP:
            break;
        case ADD:
            result = get_register(op.get_regs().at(0)) + 
                get_register(op.get_regs().at(1));
            set_register(op.get_regs().back(), result);
            break;
        case SUB:
            result = get_register(op.get_regs().at(0)) - 
                get_register(op.get_regs().at(1));
            set_register(op.get_regs().back(), result);
            break;
        case MULT:
            result = get_register(op.get_regs().at(0)) * 
                get_register(op.get_regs().at(1));
            set_register(op.get_regs().back(), result);
            break;
        case DIV:
            result = get_register(op.get_regs().at(0)) / 
                get_register(op.get_regs().at(1));
            set_register(op.get_regs().back(), result);
            break;	
        case AND:
            result = get_register(op.get_regs().at(0)) & 
                get_register(op.get_regs().at(1));
            set_register(op.get_regs().back(), result);
            break;
        case OR:
            result = get_register(op.get_regs().at(0)) | 
                get_register(op.get_regs().at(1));
            set_register(op.get_regs().back(), result);
            break;
        case XOR:
            result = get_register(op.get_regs().at(0)) ^ 
                get_register(op.get_regs().at(1));
            set_register(op.get_regs().back(), result);
            break;  
        case ANDI:
            result = get_register(op.get_regs().at(0)) & op.get_constant();
            set_register(op.get_regs().back(), result);
            break;
        case ORI:
            result = get_register(op.get_regs().at(0)) | op.get_constant();
            set_register(op.get_regs().back(), result);
            break;
        case XORI:
            result = get_register(op.get_regs().at(0)) ^ op.get_constant();
            set_register(op.get_regs().back(), result);
            break;	  
        case ADDI:
            result = get_register(op.get_regs().at(0)) + op.get_constant();
            set_register(op.get_regs().back(), result);
            break;
        case SUBI:
            result = get_register(op.get_regs().at(0)) - op.get_constant();
            set_register(op.get_regs().back(), result);
            break;
        case RSUBI:
            result = op.get_constant() - get_register(op.get_regs().at(0));
            set_register(op.get_regs().back(), result);
            break;
        case MULTI:
            result = get_register(op.get_regs().at(0)) * op.get_constant();
            set_register(op.get_regs().back(), result);
            break;
        case DIVI:
            result = get_register(op.get_regs().at(0)) / op.get_constant();
            set_register(op.get_regs().back(), result);
            break;
        case RDIVI:
            result = op.get_constant() / get_register(op.get_regs().at(0));
            set_register(op.get_regs().back(), result);
            break;
        case LSHIFT:
            result = get_register(op.get_regs().at(0)) << 
                get_register(op.get_regs().at(1));
            set_register(op.get_regs().back(), result);
            break;
        case LSHIFTI:
            result = get_register(op.get_regs().at(0)) << op.get_constant();
            set_register(op.get_regs().back(), result);
            break;
        case RSHIFT:
            result = get_register(op.get_regs().at(0)) >> 
                get_register(op.get_regs().at(1));
            set_register(op.get_regs().back(), result);
            break;
        case RSHIFTI:
            result = get_register(op.get_regs().at(0)) >> op.get_constant();
            set_register(op.get_regs().back(), result);
            break;
        case LOADI:
            result = op.get_constant();
            set_register(op.get_regs().back(), result);
            break;
        case LOAD:
            location = get_register(op.get_regs().at(0));
            result = get_word(location);
            set_register(op.get_regs().back(), result);
            break;
        case LOADAI:
            location = get_register(op.get_regs().at(0)) +
                    op.get_constant();
            result = get_word(location);
            set_register(op.get_regs().back(), result);
            break;
        case LOADAO:
            location = get_register(op.get_regs().at(0)) +
                    get_register(op.get_regs().at(1));
            result = get_word(location);
            set_register(op.get_regs().back(), result);
            break;
        case CLOAD:
            location = get_register(op.get_regs().at(0));
            result = get_memory(location);
            set_register(op.get_regs().back(), result);
            break;
        case CLOADAI:
            location = get_register(op.get_regs().at(0)) +
                    op.get_constant();
            result = get_memory(location);
            set_register(op.get_regs().back(), result);
            break;
        case CLOADAO:
            location = get_register(op.get_regs().at(0)) +
                    get_register(op.get_regs().at(1));
            result = get_memory(location);
            set_register(op.get_regs().back(), result);
            break;
        case STORE:
            result = get_register(op.get_regs().at(0));
            location = get_register(op.get_regs().at(1));
            set_word(location, result);
            break;
        case STOREAI:
            result = get_register(op.get_regs().at(0));
            location = get_register(op.get_regs().at(1)) + op.get_constant();
            set_word(location, result);
            break;
        case STOREAO:
            result = get_register(op.get_regs().at(0));
            location = get_register(op.get_regs().at(1)) + get_register(op.get_regs().back());
            set_word(location, result);
            break; 
        case CSTORE:
            result = (get_register(op.get_regs().at(0)) << 24) >> 24;
            location = get_register(op.get_regs().at(1));
            set_memory(location, result);
            break;
        case CSTOREAI:
            result = (get_register(op.get_regs().at(0)) << 24) >> 24;
            location = get_register(op.get_regs().at(1)) + op.get_constant();
            set_memory(location, result);
            break;
        case CSTOREAO:
            result = (get_register(op.get_regs().at(0)) << 24) >> 24;
            location = get_register(op.get_regs().at(1)) + 
                get_register(op.get_regs().back());
            set_memory(location, result);
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
            set_register(op.get_regs().back(),result);
            break;
        case CMPLE:
            if (get_register(op.get_regs().at(0)) <= 
                    get_register(op.get_regs().at(1)))
                result = 1;
            else
                result = 0;
            set_register(op.get_regs().back(),result);
            break;
        case CMPEQ:
            if (get_register(op.get_regs().at(0)) == 
                    get_register(op.get_regs().at(1)))
                result = 1;
            else
                result = 0;
            set_register(op.get_regs().back(),result);
            break;
        case CMPNE:
            if (get_register(op.get_regs().at(0)) != 
                    get_register(op.get_regs().at(1)))
                result = 1;
            else
                result = 0;
            set_register(op.get_regs().back(),result);
            break;
        case CMPGE:
            if (get_register(op.get_regs().at(0)) >= 
                    get_register(op.get_regs().at(1)))
                result = 1;
            else
                result = 0;
            set_register(op.get_regs().back(),result);
            break;
        case CMPGT:
            if (get_register(op.get_regs().at(0)) > 
                    get_register(op.get_regs().at(1)))
                result = 1;
            else
                result = 0;
            set_register(op.get_regs().back(),result);
            break;
        case I2I:
            result = get_register(op.get_regs().at(0));
            set_register(op.get_regs().back(),result);
            break;
        case C2C:
        case C2I:
        case I2C:
            result = (get_register(op.get_regs().at(0)) << 24) >> 24;
            set_register(op.get_regs().back(),result);
            break;
        case HALT:
            return false;
        default:
            std::stringstream ss;
            ss << "Erro de simulação: Código de operação inválido.";
            throw SimulationError(ss.str());
    }
    if(op.opcode != CBR && op.opcode != JUMP && op.opcode != JUMPI)
        PC++;
    cycles += op.get_latency();
    op_count++;
    return true;
}
