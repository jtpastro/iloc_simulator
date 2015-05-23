#include <stdlib.h>
#include "Operation.hpp"


        /*  name      string      regs   consts labels latency */
        /*  ------    ------      ------ ------ ------ ------- */
Opcode opcode_specs[INVALID_OP] = {
          { NOP,      "nop",       0,    0,     0,     1 },
          { ADD,      "add",       3,    0,     0,     1 },
          { SUB,      "sub",       3,    0,     0,     1 },
          { MULT,     "mult",      3,    0,     0,     3 },
          { DIV,      "div",       3,    0,     0,     3 },
          { LSHIFT,   "lshift",    3,    0,     0,     1 },
          { RSHIFT,   "rshift",    3,    0,     0,     1 },
          { ADDI,     "addI",      2,    1,     0,     1 },
          { SUBI,     "subI",      2,    1,     0,     1 },
          { MULTI,    "multI",     2,    1,     0,     1 },
          { DIVI,     "divI",      2,    1,     0,     1 },
          { LSHIFTI,  "lshiftI",   2,    1,     0,     1 },
          { RSHIFTI,  "rshiftI",   2,    1,     0,     1 },
          { LOADI,    "loadI",     1,    1,     0,     1 },
          { LOAD,     "load",      2,    0,     0,     5 },
          { LOADAI,   "loadAI",    2,    1,     0,     5 },
          { LOADAO,   "loadAO",    3,    0,     0,     5 },
          { CLOAD,    "cload",     2,    0,     0,     5 },
          { CLOADAI,  "cloadAI",   2,    1,     0,     5 },
          { CLOADAO,  "cloadAO",   3,    0,     0,     5 },
          { STORE,    "store",     2,    0,     0,     5 },
          { STOREAI,  "storeAI",   2,    1,     0,     5 },
          { STOREAO,  "storeAO",   3,    0,     0,     5 },
          { CSTORE,   "cstore",    2,    0,     0,     5 },
          { CSTOREAI, "cstoreAI",  2,    1,     0,     5 },
          { CSTOREAO, "cstoreAO",  3,    0,     0,     5 },
          { JUMPI,    "jumpI",     0,    0,     1,     1 },
          { CBR,      "cbr",       1,    0,     2,     1 },
          { CBR_LT,   "cbr_LT",    1,    0,     2,     1 },
          { CBR_LE,   "cbr_LE",    1,    0,     2,     1 },
          { CBR_GT,   "cbr_GT",    1,    0,     2,     1 },
          { CBR_GE,   "cbr_GE",    1,    0,     2,     1 },
          { CBR_NE,   "cbr_NE",    1,    0,     2,     1 },
          { CBR_EQ,   "cbr_EQ",    1,    0,     2,     1 },
          { COMP,     "comp",      3,    0,     0,     1 },
          { CMPLT,    "cmp_LT",    3,    0,     0,     1 },
          { CMPLE,    "cmp_LE",    3,    0,     0,     1 },
          { CMPEQ,    "cmp_EQ",    3,    0,     0,     1 },
          { CMPNE,    "cmp_NE",    3,    0,     0,     1 },
          { CMPGE,    "cmp_GE",    3,    0,     0,     1 },
          { CMPGT,    "cmp_GT",    3,    0,     0,     1 },
          { I2I,      "i2i",       2,    0,     0,     1 },
          { C2C,      "c2c",       2,    0,     0,     1 },
          { I2C,      "i2c",       2,    0,     0,     1 },
          { C2I,      "c2i",       2,    0,     0,     1 },
          { OUTPUT,   "output",    0,    1,     0,     1 },
          { COUTPUT,  "coutput",   0,    1,     0,     1 }};

std::map<std::string,Opcode_Name> Operation::init() {
    int i;
    std::map<std::string,Opcode_Name> m;
    /* Create empty table */
    for(i=0;i<INVALID_OP;i++){
        m[opcode_specs[i].str] = opcode_specs[i].name; 
    }
    return m;
}

bool Operation::is_opcode(std::string op){
    return map.count(op) == 1;
}

Opcode_Name Operation::str_to_opcode(std::string op){
        return is_opcode(op) ? map.at(op) : INVALID_OP;
}

std::string Operation::get_string(){
    return opcode_specs[opcode].str;
}

unsigned int Operation::num_regs(){
    return opcode_specs[opcode].regs;
}

unsigned int Operation::num_lbls(){
    return opcode_specs[opcode].labels;
}

unsigned int Operation::num_consts(){
    return opcode_specs[opcode].consts;
}

unsigned int Operation::get_latency(){
    return opcode_specs[opcode].latency;
}

void Operation::set_latency(unsigned int latency){
    opcode_specs[opcode].latency = latency;
}

bool Operation::verify_operation(){
    return  regs.size()   == num_regs() &&
            consts.size() == num_consts() &&
            labels.size() == num_lbls();
}

void Operation::concatenate(Operation other){
    regs.insert(regs.end(), other.regs.begin(), other.regs.end()); 
    consts.insert(consts.end(), other.consts.begin(), other.consts.end()); 
    labels.insert(labels.end(), other.labels.begin(), other.labels.end()); 
}

std::map<std::string,Opcode_Name> Operation::map = Operation::init();
