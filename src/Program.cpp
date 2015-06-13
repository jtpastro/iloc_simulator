#include <stdlib.h>
#include <stdio.h>
#include "Program.hpp"

void Program::add_operation(Operation op){
    op_list.push_back(op);
}
 
void Program::add_operation(std::string lbl, Operation op){
    if(symbol_table.count(lbl)==0){
        symbol_table[lbl] = op_list.size();
        line_table[op_list.size()] = lbl;
    } else {
        fprintf(stderr,"Semantic Error: Repeated label declaration.\n");
        exit(EXIT_FAILURE);
    }
    add_operation(op);
}

Operation Program::get_operation(uint pc){
    return op_list.at(pc);
}

uint Program::get_label(std::string lbl){
    return symbol_table.at(lbl);
}

uint Program::get_size(){
    return op_list.size();
}

std::string Program::get_line(uint pc){
  std::map<uint,std::string>::iterator it = line_table.find(pc);
  return (it != line_table.end() ? it->second + ": " : "") + op_list.at(pc).toString();  
}
