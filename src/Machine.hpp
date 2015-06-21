#include "Program.hpp"

class State {
    public:
        uint PC;
        std::vector<char> memory;
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
        void check_access(uint);
        uint get_branch_destination(std::string);
        uint sp();
        uint fp();
        uint bss();
    public:
        Machine(Program,uint,uint);
        State get_state();
        void set_state(State);
        bool execute_operation();
        void run();
        std::string reg_state();
        std::string mem_state();
        std::string prog_state();
        std::string exec_state();
};

