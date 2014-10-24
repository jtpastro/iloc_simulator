#include <iostream>
#include "Program.hpp"

using namespace std;

typedef enum t { NOP_OK=0, NOP_NOT_OK, GET_OPCODE, IS_OPCODE, OPCODE_TO_STR,
                PROGRAM_OP_ORDER, PROGRAM_LBL_ORDER, PROGRAM_LABEL_FAIL, PROGRAM_LABEL_OK, NO_MORE_TESTS} TEST_CASE; 

bool test(int t_c ){
    Operation op, op2, op3;
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
