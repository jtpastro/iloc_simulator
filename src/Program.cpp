#include "Program.hpp"
#include "SimulationError.hpp" //SimulationError

void Program::add_operation(Operation op){
    op_list.push_back(op);
}
 
void Program::add_operation(std::string lbl, Operation op){
    if(symbol_table.count(lbl)!=0)
        throw SimulationError("Semantic Error: Repeated label declaration.");
    symbol_table[lbl] = op_list.size();
    line_table[op_list.size()] = lbl;
    add_operation(op);
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

bool Program::check_labels(){
    for(auto &op: op_list)
        if(op.num_lbls()>0 && (symbol_table.count(op.get_first_label())==0 || (op.num_lbls()>1 && symbol_table.count(op.get_second_label())==0)))
            return false;
    return true;
}
