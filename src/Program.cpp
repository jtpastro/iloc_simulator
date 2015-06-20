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
        throw SimulationError("Semantic Error: Repeated label declaration.");
    unused_labels.erase(lbl);
    symbol_table[lbl] = op_list.size();
    line_table[op_list.size()] = lbl;
    op_list.push_back(op);
}

Operation Program::get_operation(uint pc){
    return op_list.at(pc);
}

uint Program::get_label(std::string lbl){
    try {
        return symbol_table.at(lbl);
    }
    catch (const std::out_of_range& oor) {
        throw SimulationError(("Simulation Error: label undeclared: " + lbl + ".").c_str());
    }
}

uint Program::get_size(){
    return op_list.size();
}

std::string Program::get_line(uint pc){
  std::map<uint,std::string>::iterator it = line_table.find(pc);
  return (it != line_table.end() ? it->second + ": " : "") + op_list.at(pc).toString();  
}

const std::map<std::string,uint>& Program::get_unused_labels() const {
    return unused_labels;
}
