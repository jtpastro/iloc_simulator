#include "Program.hpp"

class Machine {
        uint PC;
        int bss;
        int fp;
        int sp;
        std::vector<char> memory;
        std::map<std::string,int> registers;
        uint cycles=0;
        uint op_count=0;
        Program program;
        void set_word(uint,int);
        void set_register(std::string,int);
        void set_memory(uint,char);
        void onereg(Operation, int);
        void check_access(uint);
        uint get_branch_destination(std::string);
    public:
        Machine(Program,uint,uint);
        int get_word(uint);
        char get_memory(uint);
        int get_register(std::string);
        bool execute_operation();
        void run();
        std::string reg_state();
        std::string mem_state();
        std::string prog_state();
        std::string prog_state(uint);
        std::string exec_state();
};

