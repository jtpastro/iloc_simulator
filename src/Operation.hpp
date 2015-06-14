#include <string>
#include <vector>

/*  */
typedef enum opcode_name {NOP=0, ADD, SUB, MULT, DIV,
                  ADDI, SUBI, RSUBI, MULTI, DIVI, RDIVI,
                  LSHIFT, LSHIFTI, RSHIFT, RSHIFTI,
                  AND, ANDI, OR, ORI, XOR, XORI,
			      LOADI, LOAD, LOADAI, LOADAO, CLOAD, CLOADAI, CLOADAO,
                  STORE, STOREAI, STOREAO, CSTORE, CSTOREAI, CSTOREAO,
                  I2I, C2C, C2I, I2C,
                  JUMPI, JUMP, CBR,
                  CMPLT, CMPLE, CMPEQ, CMPNE, CMPGE, CMPGT, 
			      INVALID_OP} Opcode_Name;

class Opcode {
    public:
        const std::string name;
        int regs, consts, labels, latency;
};


class Operation{
        std::vector<int> consts;
        std::vector<std::string> regs;
        std::vector<std::string> labels;
    public:
        static Opcode_Name string_to_opcode(std::string);
        static std::string opcode_to_string(Opcode_Name);
        unsigned int num_regs();
        unsigned int num_lbls();        
        unsigned int num_consts();        
        unsigned int get_latency();               
        void set_latency(unsigned int);
        void concatenate(Operation);
        bool verify_operation();
        std::string toString();
        Opcode_Name opcode;
        void add_label(std::string);
        void add_register(std::string);
        void add_constant(int);
        std::string get_first_label();
        std::string get_second_label();
        std::string get_first_register();
        std::string get_second_register();
        std::string get_last_register();
        int get_constant();
};
