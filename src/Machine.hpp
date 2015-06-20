#include <map> //std::map(memory registers)
#include <string> //std::string
#include "Program.hpp"

class State {
    public:
        uint PC;
        std::map<uint,char> memory;
        std::map<std::string,int> registers;
};

class Machine {
        uint cycles=0;
        uint op_count=0;
        State state;
        Program program;
        int get_word(uint);
        char get_memory(uint);
        int get_register(std::string);
        void set_word(uint,int);
        void set_register(std::string,int);
        void set_memory(uint,char);
        void onereg(Operation, int);
        uint get_branch_destination(std::string);
    public:
        Machine(Program);
        State get_state();
        void set_state(State);
        void execute_operation();
        void run();
        std::string reg_state();
        std::string mem_state();
        std::string prog_state();
        std::string exec_state();
};

