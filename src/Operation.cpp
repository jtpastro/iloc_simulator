#include <stdlib.h>
#include "Operation.hpp"


        /*  name      string      srcs   consts labels defs   latency target_is_source */
        /*  ------    ------      ------ ------ ------ ------ ------- ---------------- */
Opcode opcode_specs[INVALID_OP] = {
          { NOP,      "nop",       0,    0,     0,     0,     1,       0 },
          { ADD,      "add",       2,    0,     0,     1,     1,       0 },
          { SUB,      "sub",       2,    0,     0,     1,     1,       0 },
          { MULT,     "mult",      2,    0,     0,     1,     3,       0 },
          { DIV,      "div",       2,    0,     0,     1,     3,       0 },
          { ADDI,     "addI",      1,    1,     0,     1,     1,       0 },
          { SUBI,     "subI",      1,    1,     0,     1,     1,       0 },
          { MULTI,    "multI",     1,    1,     0,     1,     1,       0 },
          { DIVI,     "divI",      1,    1,     0,     1,     1,       0 },
          { LSHIFT,   "lshift",    2,    0,     0,     1,     1,       0 },
          { LSHIFTI,  "lshiftI",   1,    1,     0,     1,     1,       0 },
          { RSHIFT,   "rshift",    2,    0,     0,     1,     1,       0 },
          { RSHIFTI,  "rshiftI",   1,    1,     0,     1,     1,       0 },
          { LOADI,    "loadI",     0,    1,     0,     1,     1,       0 },
          { LOAD,     "load",      1,    0,     0,     1,     5,       0 },
          { LOADAI,   "loadAI",    1,    1,     0,     1,     5,       0 },
          { LOADAO,   "loadAO",    2,    0,     0,     1,     5,       0 },
          { CLOAD,    "cload",     1,    0,     0,     1,     5,       0 },
          { CLOADAI,  "cloadAI",   1,    1,     0,     1,     5,       0 },
          { CLOADAO,  "cloadAO",   2,    0,     0,     1,     5,       0 },
          { STORE,    "store",     1,    0,     0,     1,     5,       1 },
          { STOREAI,  "storeAI",   1,    1,     0,     1,     5,       1 },
          { STOREAO,  "storeAO",   1,    0,     0,     2,     5,       1 },
          { CSTORE,   "cstore",    1,    0,     0,     1,     5,       1 },
          { CSTOREAI, "cstoreAI",  1,    1,     0,     1,     5,       1 },
          { CSTOREAO, "cstoreAO",  1,    0,     0,     2,     5,       1 },
          { JUMPI,    "jumpI",     0,    0,     1,     0,     1,       0 },
          { CBR,      "cbr",       1,    0,     2,     0,     1,       0 },
          { CBR_LT,   "cbr_LT",    1,    0,     2,     0,     1,       0 },
          { CBR_LE,   "cbr_LE",    1,    0,     2,     0,     1,       0 },
          { CBR_GT,   "cbr_GT",    1,    0,     2,     0,     1,       0 },
          { CBR_GE,   "cbr_GE",    1,    0,     2,     0,     1,       0 },
          { CBR_NE,   "cbr_NE",    1,    0,     2,     0,     1,       0 },
          { CBR_EQ,   "cbr_EQ",    1,    0,     2,     0,     1,       0 },
          { COMP,     "comp",      2,    0,     0,     1,     1,       0 },
          { CMPLT,    "cmp_LT",    2,    0,     0,     1,     1,       0 },
          { CMPLE,    "cmp_LE",    2,    0,     0,     1,     1,       0 },
          { CMPEQ,    "cmp_EQ",    2,    0,     0,     1,     1,       0 },
          { CMPNE,    "cmp_NE",    2,    0,     0,     1,     1,       0 },
          { CMPGE,    "cmp_GE",    2,    0,     0,     1,     1,       0 },
          { CMPGT,    "cmp_GT",    2,    0,     0,     1,     1,       0 },
          { I2I,      "i2i",       1,    0,     0,     1,     1,       0 },
          { C2C,      "c2c",       1,    0,     0,     1,     1,       0 },
          { I2C,      "i2c",       1,    0,     0,     1,     1,       0 },
          { C2I,      "c2i",       1,    0,     0,     1,     1,       0 },
          { OUTPUT,   "output",    0,    1,     0,     0,     1,       0 },
          { COUTPUT,  "coutput",   0,    1,     0,     0,     1,       0 }};

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
    return map.at(op);
}

std::string Operation::get_string(){
    return opcode_specs[opcode].str;
}

unsigned int Operation::num_srcs(){
    return opcode_specs[opcode].srcs;
}

unsigned int Operation::num_lbls(){
    return opcode_specs[opcode].labels;
}

unsigned int Operation::num_defs(){
    return opcode_specs[opcode].defs;
}

unsigned int Operation::num_consts(){
    return opcode_specs[opcode].consts;
}

unsigned int Operation::get_latency(){
    return opcode_specs[opcode].latency;
}

unsigned int Operation::is_source(){
    return opcode_specs[opcode].target_is_source;
}

void Operation::set_latency(unsigned int latency){
    opcode_specs[opcode].latency = latency;
}

bool Operation::verify_operation(){
    return  srcs.size()   == num_srcs() &&
            defs.size()   == num_defs() &&
            consts.size() == num_consts() &&
            labels.size() == num_lbls();
}

std::map<std::string,Opcode_Name> Operation::map = Operation::init();
