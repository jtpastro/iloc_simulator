#include <string>
#include <map>
#include <vector>

/* This is the list of valid opcode names.
 * It should be kept synchronized with the opcode_specs table. */
typedef enum opcode_name {NOP=0, ADD, SUB, MULT, DIV, ADDI, SUBI, MULTI,
			      DIVI, LSHIFT, LSHIFTI, RSHIFT, RSHIFTI,
			      LOADI, LOAD, LOADAI, LOADAO, CLOAD, CLOADAI,
			      CLOADAO, STORE, STOREAI, STOREAO, CSTORE, 
			      CSTOREAI, CSTOREAO, JUMPI, CBR, CBR_LT, CBR_LE,
			      CBR_GT, CBR_GE, CBR_NE, CBR_EQ, COMP, CMPLT, CMPLE,
			      CMPEQ, CMPNE, CMPGE, CMPGT, I2I, C2C, C2I, I2C,
			      OUTPUT, COUTPUT, INVALID_OP} Opcode_Name;

typedef struct opcode {
    Opcode_Name name;
    std::string str;
    int regs, consts, labels;
    int latency;
} Opcode;

class Operation{
        static std::map<std::string, Opcode_Name> map;
    public:
        static std::map<std::string, Opcode_Name> init();
        static bool is_opcode(std::string);
        static Opcode_Name str_to_opcode(std::string);
        std::string get_string();
        unsigned int num_regs();
        unsigned int num_lbls();        
        unsigned int num_consts();        
        unsigned int get_latency();        
        unsigned int is_source();        
        void set_latency(unsigned int);
        void concatenate(Operation);
        bool verify_operation();
        Opcode_Name opcode;
        std::vector<int> consts;
        std::vector<int> regs;
        std::vector<std::string> labels;
};
