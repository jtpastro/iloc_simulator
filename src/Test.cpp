#include <iostream>
#include "Machine.hpp"

using namespace std;

typedef enum t { NOP_OK=0, NOP_NOT_OK, GET_OPCODE, IS_OPCODE, OPCODE_TO_STR,
                PROGRAM_OP_ORDER, PROGRAM_LBL_ORDER, PROGRAM_LABEL_FAIL, PROGRAM_LABEL_OK, PROGRAM_GET_OP, MACHINE_CREATION_OK, MACHINE_PROGRAM, NO_MORE_TESTS} TEST_CASE; 

bool test(int t_c ){
    Operation op, op2, op3, op4, op5, op6, op7, op8, op9;
    Program prog;
    switch(t_c){
        case NOP_OK: {
            op.opcode = NOP;
            return op.verify_operation();
        } case NOP_NOT_OK: {
            op.opcode = NOP;
            op.srcs.push_back(1);
            return !op.verify_operation();
        } case GET_OPCODE:{
            return Operation::str_to_opcode("c2i") == C2I;
        } case IS_OPCODE:{
            return Operation::is_opcode("store"); 
        } case OPCODE_TO_STR:{
            op.opcode = LOAD;
            return op.get_string() == "load"; 
        } case PROGRAM_OP_ORDER:{
            op.opcode = JUMPI;
            op.labels.push_back("L1");
            op2.opcode = NOP;
            prog.add_operation(op);
            prog.add_operation("L1", op2);
            return prog.get_operation(0).opcode == NOP &&
                   prog.get_operation(1).opcode == JUMPI;
        } case PROGRAM_LBL_ORDER:{
            op.opcode = NOP;
            prog.add_operation("L2", op);
            prog.add_operation(op);
            prog.add_operation(op);
            prog.add_operation("L1", op);
            return prog.get_label("L1") == 0 &&
                    prog.get_label("L2") == 3;
        } case PROGRAM_LABEL_FAIL:{
            op.opcode = JUMPI;
            op.labels.push_back("L1");
            op2.opcode = NOP;
            prog.add_operation(op);
            prog.add_operation(op2);
            return !prog.check_labels();
        } case PROGRAM_LABEL_OK:{
            op.opcode = JUMPI;
            op.labels.push_back("L1");
            op2.opcode = NOP;
            prog.add_operation(op);
            prog.add_operation("L1", op2);
            return prog.check_labels();
        } case PROGRAM_GET_OP:{        
            op9.opcode = OUTPUT;
            op9.consts.push_back(1024);
            prog.add_operation("L3", op9);
            op8.opcode = STORE;
            op8.srcs.push_back(5);
            op8.defs.push_back(6);
            prog.add_operation(op8);
            op7.opcode = LOADI;
            op7.consts.push_back(3);
            op7.defs.push_back(5);
            prog.add_operation("L1", op7);
            op6.opcode = JUMPI;
            op6.labels.push_back("L3");
            prog.add_operation(op6);
            op5.opcode = STORE;
            op5.srcs.push_back(5);
            op5.defs.push_back(6);
            prog.add_operation(op5);
            op4.opcode = LOADI;
            op4.consts.push_back(1024);
            op4.defs.push_back(6);
            prog.add_operation(op4);
            op3.opcode = ADD;
            op3.srcs.push_back(3);
            op3.srcs.push_back(3);
            op3.defs.push_back(5);
            prog.add_operation(op3);
            op2.opcode = MULT;
            op2.srcs.push_back(3);
            op2.srcs.push_back(3);
            op2.defs.push_back(5);
            prog.add_operation(op2);
            op.opcode = LOADI;
            op.consts.push_back(3);
            op.defs.push_back(3);
            prog.add_operation("L2", op);
            return  prog.get_operation(0).opcode == LOADI &&
                    prog.get_operation(1).opcode == MULT &&
                    prog.get_operation(2).opcode == ADD &&
                    prog.get_operation(3).opcode == LOADI &&
                    prog.get_operation(4).opcode == STORE &&
                    prog.get_operation(5).opcode == JUMPI &&
                    prog.get_operation(6).opcode == LOADI &&
                    prog.get_operation(7).opcode == STORE &&
                    prog.get_operation(8).opcode == OUTPUT;
        } case MACHINE_CREATION_OK:{
            op.opcode = JUMPI;
            op.labels.push_back("L1");
            op2.opcode = NOP;
            prog.add_operation(op);
            prog.add_operation("L1", op2);
            Machine m(0,0,prog);
            return true; 
        } case MACHINE_PROGRAM:{
            op9.opcode = OUTPUT;
            op9.consts.push_back(1024);
            prog.add_operation("L3", op9);
            op8.opcode = STORE;
            op8.srcs.push_back(5);
            op8.defs.push_back(6);
            prog.add_operation(op8);
            op7.opcode = LOADI;
            op7.consts.push_back(3);
            op7.defs.push_back(5);
            prog.add_operation("L1", op7);
            op6.opcode = JUMPI;
            op6.labels.push_back("L3");
            prog.add_operation(op6);
            op5.opcode = STORE;
            op5.srcs.push_back(5);
            op5.defs.push_back(6);
            prog.add_operation(op5);
            op4.opcode = LOADI;
            op4.consts.push_back(1024);
            op4.defs.push_back(6);
            prog.add_operation(op4);
            op3.opcode = ADD;
            op3.srcs.push_back(3);
            op3.srcs.push_back(3);
            op3.defs.push_back(5);
            prog.add_operation(op3);
            op2.opcode = MULT;
            op2.srcs.push_back(3);
            op2.srcs.push_back(3);
            op2.defs.push_back(5);
            prog.add_operation(op2);
            op.opcode = LOADI;
            op.consts.push_back(3);
            op.defs.push_back(3);
            prog.add_operation("L2", op);
            Machine m(0,0,prog); 
            m.quiet = true;
            m.run();
            return true;
        } default:
            return false;
    }
}

int main(){
    for(int i=0; i<NO_MORE_TESTS; i++){
        cout << "Test " << i << ": ";
        if (test(i))
            cout << "PASS" << endl;
        else
            cout << "FAIL" << endl;
    }
}
