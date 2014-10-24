#include <vector>
#include "Operation.hpp"

/* The default number of bytes of addressable memory starting from 0 */
#define DEFAULT_MEMORY_SIZE 32768
/* The default number of registers */
#define DEFAULT_NUM_REGISTERS 256
typedef unsigned int uint;
typedef unsigned char uchar;
typedef struct reg_cell {
    int data;
    bool accessed;
} Reg;

typedef struct mem_cell {
    char data;
    bool accessed;
} Mem;

class Machine {
        uint PC;
        std::vector<Mem> memory_bank;
        std::vector<Reg> register_bank;
        uint MEMORY_SIZE = DEFAULT_MEMORY_SIZE;
        uint NUM_REGISTERS = DEFAULT_NUM_REGISTERS;
        int get_word(uint);
        char get_memory(uint);
        int get_register(uint);
        void set_word(uint,int);
        void set_register(uint,int);
        void set_memory(uint,char);
        void onereg(Operation, int);
        void execute_operation(Operation);
    public:
        Machine(uint, uint);
        Machine execute_next(std::vector<Operation>);
        void reg_state();
        void mem_state();
        void prog_state();
};

