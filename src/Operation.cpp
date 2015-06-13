#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include "Operation.hpp"

       /*               name     regs  consts labels latency 
                        ------   ------ ------ ------ ------- */
std::map<Opcode_Name, Opcode> op_specs = {
          { NOP,      { "nop",       0,    0,     0,     1 }},
          { ADD,      { "add",       3,    0,     0,     1 }},
          { SUB,      { "sub",       3,    0,     0,     1 }},
          { MULT,     { "mult",      3,    0,     0,     3 }},
          { DIV,      { "div",       3,    0,     0,     3 }},
          { OR,       { "or",        3,    0,     0,     1 }},
          { XOR,      { "xor",       3,    0,     0,     1 }},
          { AND,      { "and",       3,    0,     0,     1 }},
          { LSHIFT,   { "lshift",    3,    0,     0,     1 }},
          { RSHIFT,   { "rshift",    3,    0,     0,     1 }},
          { ADDI,     { "addI",      2,    1,     0,     1 }},
          { SUBI,     { "subI",      2,    1,     0,     1 }},
          { RSUBI,    { "rsubI",     2,    1,     0,     1 }},
          { MULTI,    { "multI",     2,    1,     0,     1 }},
          { DIVI,     { "divI",      2,    1,     0,     1 }},
          { RDIVI,    { "rdivI",     2,    1,     0,     1 }},
          { OR,       { "orI",       2,    1,     0,     1 }},
          { XOR,      { "xorI",      2,    1,     0,     1 }},
          { AND,      { "andI",      2,    1,     0,     1 }},
          { LSHIFTI,  { "lshiftI",   2,    1,     0,     1 }},
          { RSHIFTI,  { "rshiftI",   2,    1,     0,     1 }},
          { LOADI,    { "loadI",     1,    1,     0,     1 }},
          { LOAD,     { "load",      2,    0,     0,     5 }},
          { LOADAI,   { "loadAI",    2,    1,     0,     5 }},
          { LOADAO,   { "loadAO",    3,    0,     0,     5 }},
          { CLOAD,    { "cload",     2,    0,     0,     5 }},
          { CLOADAI,  { "cloadAI",   2,    1,     0,     5 }},
          { CLOADAO,  { "cloadAO",   3,    0,     0,     5 }},
          { STORE,    { "store",     2,    0,     0,     5 }},
          { STOREAI,  { "storeAI",   2,    1,     0,     5 }},
          { STOREAO,  { "storeAO",   3,    0,     0,     5 }},
          { CSTORE,   { "cstore",    2,    0,     0,     5 }},
          { CSTOREAI, { "cstoreAI",  2,    1,     0,     5 }},
          { CSTOREAO, { "cstoreAO",  3,    0,     0,     5 }},
          { JUMPI,    { "jumpI",     0,    0,     1,     1 }},
          { JUMP,     { "jump",      1,    0,     0,     1 }},
          { CBR,      { "cbr",       1,    0,     2,     1 }},
          { CMPLT,    { "cmp_LT",    3,    0,     0,     1 }},
          { CMPLE,    { "cmp_LE",    3,    0,     0,     1 }},
          { CMPEQ,    { "cmp_EQ",    3,    0,     0,     1 }},
          { CMPNE,    { "cmp_NE",    3,    0,     0,     1 }},
          { CMPGE,    { "cmp_GE",    3,    0,     0,     1 }},
          { CMPGT,    { "cmp_GT",    3,    0,     0,     1 }},
          { I2I,      { "i2i",       2,    0,     0,     1 }},
          { C2C,      { "c2c",       2,    0,     0,     1 }},
          { I2C,      { "i2c",       2,    0,     0,     1 }},
          { C2I,      { "c2i",       2,    0,     0,     1 }}};

std::map<std::string,Opcode_Name> op_names = {
          { "nop",       NOP      },
          { "add",       ADD      },
          { "sub",       SUB      },
          { "mult",      MULT     },
          { "div",       DIV      },
          { "or",        OR       },
          { "xor",       XOR      },
          { "and",       AND      },
          { "lshift",    LSHIFT   },
          { "rshift",    RSHIFT   },
          { "addI",      ADDI     },
          { "subI",      SUBI     },
          { "rsubI",     RSUBI    },
          { "multI",     MULTI    },
          { "divI",      DIVI     },
          { "rdivI",     RDIVI    },
          { "orI",       OR       },
          { "xorI",      XOR      },
          { "andI",      AND      },
          { "lshiftI",   LSHIFTI  },
          { "rshiftI",   RSHIFTI  },
          { "loadI",     LOADI    },
          { "load",      LOAD     },
          { "loadAI",    LOADAI   },
          { "loadAO",    LOADAO   },
          { "cload",     CLOAD    },
          { "cloadAI",   CLOADAI  },
          { "cloadAO",   CLOADAO  },
          { "store",     STORE    },
          { "storeAI",   STOREAI  },
          { "storeAO",   STOREAO  },
          { "cstore",    CSTORE   },
          { "cstoreAI",  CSTOREAI },
          { "cstoreAO",  CSTOREAO },
          { "jumpI",     JUMPI    },
          { "jump",      JUMP     },
          { "cbr",       CBR      },
          { "cmp_LT",    CMPLT    },
          { "cmp_LE",    CMPLE    },
          { "cmp_EQ",    CMPEQ    },
          { "cmp_NE",    CMPNE    },
          { "cmp_GE",    CMPGE    },
          { "cmp_GT",    CMPGT    },
          { "i2i",       I2I      },
          { "c2c",       C2C      },
          { "i2c",       I2C      },
          { "c2i",       C2I      }};

Opcode_Name Operation::string_to_opcode(std::string op){
    std::map<std::string,Opcode_Name>::iterator it = op_names.find(op);
    return it!=op_names.end() ? it->second : INVALID_OP;
}

std::string Operation::opcode_to_string(Opcode_Name op){
    std::map<Opcode_Name,Opcode>::iterator it = op_specs.find(op);
    return it!=op_specs.end() ? (it->second).name : "NULL";
}

unsigned int Operation::num_regs(){
    return op_specs[opcode].regs;
}

unsigned int Operation::num_lbls(){
    return op_specs[opcode].labels;
}

unsigned int Operation::num_consts(){
    return op_specs[opcode].consts;
}

unsigned int Operation::get_latency(){
    return op_specs[opcode].latency;
}

void Operation::set_latency(unsigned int latency){
    op_specs[opcode].latency = latency;
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

std::string Operation::toString(){
    std::string op = op_specs[opcode].name;   
    switch(opcode) {
        case NOP:
            return op;
        case ADD:
        case SUB:
        case MULT:
        case DIV:
        case LSHIFT:
        case RSHIFT:
        case AND:
        case OR:
        case XOR:
        case LOADAO:
        case CLOADAO:
            return op + " " + regs[0] + ", " + regs[1] + " => " + regs[2];
        case ADDI:
        case SUBI:
        case RSUBI:
        case MULTI:
        case DIVI:
        case RDIVI:
        case LSHIFTI:
        case RSHIFTI:
        case ANDI:
        case ORI:
        case XORI:
        case LOADAI:
        case CLOADAI:
            return op + " " + regs[0] + ", " + std::to_string(consts[0]) + " => " + regs[1];
        case LOADI:
            return op + " " + std::to_string(consts[0]) + " => " + regs[0];
        case LOAD:
        case CLOAD:
        case STORE:
        case CSTORE:
        case I2I:
        case C2C:
        case C2I:
        case I2C:
            return op + " " + regs[0] + " => " + regs[1];
        case STOREAI:
        case CSTOREAI:
           return op + " " + regs[0] + " => " + regs[1] + ", " + std::to_string(consts[0]);
        case STOREAO:
        case CSTOREAO:
           return op + " " + regs[0] + " => " + regs[1] + ", " + regs[2];
        case CMPLT:
        case CMPLE:
        case CMPEQ:
        case CMPNE:
        case CMPGE:
        case CMPGT:
           return op + " " + regs[0] + ", " + regs[1] + " -> " + regs[2];
        case JUMPI:
           return op + " -> " + labels[0];
        case JUMP:
           return op + " -> " + regs[0];
        case CBR:
           return op + " " + regs[0] + " -> " + labels[0] + ", " + labels[1];
        default:
            fprintf(stderr,"Simulation Error: Invalid opcode encountered in execute_operation.");
            exit(EXIT_FAILURE);
    }
}
