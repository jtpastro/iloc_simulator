#include <stdlib.h>
#include <stdio.h>
#include "Program.hpp"

void Program::add_operation(Operation op){
    for(std::string it : op.labels){
        if(symbol_table.count(it)==0){
            symbol_table[it] = -1;
            missing_refs++;
        }
    }
    op_list.push_back(op);
}
 
void Program::add_operation(std::string lbl, Operation op){
    if(symbol_table.count(lbl)==0){
        symbol_table[lbl] = op_list.size();
    } else if(symbol_table[lbl] == -1){
        symbol_table[lbl] = op_list.size();
        missing_refs--;
    } else {
        fprintf(stderr,"Simulator Error: Repeated label declaration");
        exit(EXIT_FAILURE);
    }
    add_operation(op);
}

int Program::get_missing_refs(){
    return missing_refs; 
}

Operation Program::get_operation(int pc){
    return op_list.at(pc);
}

int Program::get_label(std::string lbl){
    return symbol_table.at(lbl);
}

bool Program::check_labels(){
    for(auto& it: symbol_table)
        if(it.second == -1)
            return false;
    return true;
}

int Program::get_size(){
    return op_list.size();
}
