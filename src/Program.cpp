#include <sstream>
#include "Program.hpp"
#include "SimulationError.hpp" //SimulationError

void Program::add_operation(Operation op){
    op_list.push_back(op);
    for(std::string lbl : op.get_labels())
        if(symbol_table.count(lbl)==0)
            unused_labels[lbl] = op_list.size();
}
 
void Program::add_operation(std::string lbl, Operation op){
    if(symbol_table.count(lbl)!=0)
        throw SimulationError("Erro semântico: Declaração de rótulo destino repetida.");
    unused_labels.erase(lbl);
    symbol_table[lbl] = op_list.size();
    line_table[op_list.size()] = lbl;
    op_list.push_back(op);
}

Operation Program::get_operation(size_t pc){
    try {
        return op_list.at(pc);
    }
    catch (const std::out_of_range& oor) {
        throw SimulationError("Erro de simulação: final da memória de programa alcançado.");
    }

}

size_t Program::get_label(std::string lbl){
    try {
        return symbol_table.at(lbl);
    }
    catch (const std::out_of_range& oor) {
        std::stringstream ss;
        ss << "Erro de simulação: rótulo não declarado: " << lbl << "."; 
        throw SimulationError(ss.str());
    }
}

size_t Program::get_size(){
    return op_list.size();
}

std::string Program::get_line(size_t pc){
    std::map<size_t,std::string>::iterator it = line_table.find(pc);
    std::stringstream ss;
    if(it != line_table.end())
        ss << it->second << ": ";
    ss << get_operation(pc).toString(); 
    return ss.str(); 
}

std::string Program::get_unused_labels() {
    std::stringstream ss;
    for(auto& it : unused_labels)
        ss << "Erro semântico na linha " << it.second << ": rótulo " << it.first << " não declarado.\n";    
    return ss.str();
}
