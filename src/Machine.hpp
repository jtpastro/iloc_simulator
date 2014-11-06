#include <vector>
#include "Program.hpp"

#define DEFAULT_MEMORY_SIZE 32768
#define DEFAULT_NUM_REGISTERS 256
typedef unsigned int uint;
typedef unsigned char uchar;

class Reg{
    public:
        int data;
        bool accessed;
};

class Mem {
    public:
        char data;
        bool accessed;
};

class State {
    public:
        uint PC;
        std::vector<Mem> memory_bank;
        std::vector<Reg> register_bank;
};

class Machine {
        uint PC;
        Program program;
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
        void execute_operation();
    public:
        bool quiet=false;
        Machine(uint, uint, Program);
        Machine(State, Program);
        State execute_next();
        void run();
        void reg_state();
        void mem_state();
        void prog_state();
};

