#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <iostream> //cout
#include <fstream> //ifstream
#include <string> //std::string;
#include <sstream> //stringstream
#include <exception> //std::out_of_range
#include "Machine.hpp"
#include "SimulationError.hpp" //SimulationError
#include "parser.hpp"

#define DEFAULT_FRAME_SIZE 65535

extern "C"
{
    extern int yylex(void);
    extern FILE*   yyin;
}
extern Program program;
int yyparse();

static struct argp_option options[] = {
    {"costs",		'c', "COST_FILE", 0, "Parametrize instruction cost."},
    {"fp",          'f', "SIZE", 0, "Set frame pointer."},
    {"bss",         'b', "SIZE", 0, "Set bss register."},
    {"memory",		'm',  0, 0, "Print data memory."},
    {"program",     'p',  0, 0, "Print program memory."},
    {"register",    'r',  0, 0, "Print registers content."},
    {"statistics",  's',  0, 0, "Print execution statistics."},
    {"debug",       'd',  0, 0, "Execute in debug mode."},
    { 0 }
};

struct arguments { 
    std::string iloc_file;   
    std::string costs_file;
    bool mem, reg, prog, stat,debug;
    int fp, bss;
};

static error_t parse_options (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = (struct arguments*)(state->input);
    switch (key){
        case 'c': arguments->costs_file = arg; break;
        case 'm': arguments->mem = true; break;
        case 'r': arguments->reg = true; break;
        case 's': arguments->stat = true; break;    
        case 'p': arguments->prog = true; break;
        case 'd': arguments->debug = true; break;
        case 'f': arguments->fp = atoi(arg); break;
        case 'b': arguments->bss = atoi(arg); break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 1)// Too many arguments.
                argp_usage (state);

            arguments->iloc_file = arg;

            break;
        case ARGP_KEY_END:
            if (state->arg_num < 1)           // Not enough arguments.
                  argp_usage (state);
            break;
        default: return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

const char *argp_program_version =
"ilocsim 1.0";
const char *argp_program_bug_address =
"<jtpastro@inf.ufrgs.br>";

// Program documentation.
static char doc[] =
"ILOCsim -- a pretty simple iloc simulator.\n";

static char args_doc[] = "INPUT_FILE";

static struct argp argp = { options, parse_options, args_doc, doc };

void read_ints (std::string filename) {
    std::ifstream myfile (filename);
    if (myfile.is_open()) {
        int latency, line_count=0;
        std::string opcode, line;
        while ( getline (myfile,line) ) {
            line_count++;
            std::stringstream stream(line);
            try {
                stream >> opcode >> latency;
                Operation::set_latency(Operation::string_to_opcode(opcode), latency);
            } catch (const std::out_of_range& oor) {
                std::stringstream ss;
                ss << "Error loading file " << filename << " at line " << line_count << ": " << line <<  std::endl;
                throw SimulationError(ss.str().c_str());
            }
        }
        myfile.close();
    } else {
        std::stringstream ss;
        ss << "Unable to open file: " << filename << "." << std::endl;
        throw SimulationError(ss.str().c_str());
    }
}

void read_iloc(std::string filename) {
	FILE *myfile = fopen(filename.c_str(), "r");
	if (!myfile) {
		std::stringstream ss;
        ss << "Unable to open file: " << filename << "." << std::endl;
        throw SimulationError(ss.str().c_str());
	}
	yyin = myfile;

	do {
		yyparse();
	} while (!feof(yyin));	
    
    fclose(yyin);
}

void list_commands(){
    std::cout << "Commands available:\n";
    std::cout << "\t?/h: list available commands.\n";
    std::cout << "\te: execute until reaching breakpoint or halt.\n";
    std::cout << "\tn: execute next operation.\n";
    std::cout << "\tb: print all breakpoints.\n";    
    std::cout << "\tb <NUM>: add/remove breakpoint at position <NUM>.\n";
    std::cout << "\tm: print all memory address after program area.\n";
    std::cout << "\tm <NUM>: print content at memory position <NUM>.\n";
    std::cout << "\tp: print program.\n";
    std::cout << "\tr: print all registers content.\n";
    std::cout << "\tr <NAME>: print content of register <NAME>.\n";
    std::cout << "\tq: exit program.\n";
}

void print_prog(Machine* mac){
    for(int i=0;i<mac->get_register("bss")/4;i++)
        std::cout << i << ": " << mac->prog_state(i) << '\n';
}

void debug(Machine* mac){
    std::map<uint,bool> breakpoints;
    char command;
    uint num;
    std::string reg, line;
    bool running=true; 
    list_commands();
    do {
        command = '\0';
        std::cout << "> ";
        std::getline (std::cin, line);
        std::stringstream ss;
        ss << line;
        ss >> command;
        switch(command){
            case '?': case 'h': list_commands(); break;
            case 'e': while((running=mac->execute_operation()) && !breakpoints[mac->get_register("pc")]); break;
            case 'n': running=mac->execute_operation(); break;
            case 'b':
                if((ss >> num))
                    if(num < mac->get_register("bss")/4)
                        breakpoints[num] = !breakpoints[num];
                    else
                        std::cout << "Breakpoint outside program limits.\n";
                else
                    for(auto& b : breakpoints)
                        if(b.second)
                            std::cout << b.first << ": " << mac->prog_state(b.first) << '\n';
                break;
            case 'm':
                if((ss >> num)){
                    if(num < mac->get_register("bss"))
                        std::cout << num << ": " << mac->prog_state(num) << '\n';
                    else if(num < mac->get_register("fp"))
                        std::cout << num << ": " << mac->get_word(num) << '\n';
                    else
                        std::cout << "Illegal memory access.\n";
                } else
                    std::cout << mac->prog_state() << mac->mem_state();
                break;
            case 'p': print_prog(mac); break;
            case 'r':
                if((ss >> reg)){
                    if ((reg=="bss") || (reg=="fp") || (reg=="sp") || (reg=="pc") || (reg.front()=='r'))
                        std::cout << reg << ": " << mac->get_register(reg) << '\n';
                    else
                        std::cout << "Invalid register.\n";
                } else
                    std::cout << mac->reg_state();
                break;
            case 'q': running = false; break;
            default: std::cout << "Invalid command.\n"; break;
        }
    } while(running);
}

int main(int argc, char** argv) {
    try{
        struct arguments arguments = {"","",false,false,false,false,false,DEFAULT_FRAME_SIZE,0};    
        if (argp_parse (&argp, argc, argv, 0, 0, &arguments) == ARGP_KEY_ERROR){
            std::stringstream ss;
            ss << argv[0] << " error during the parsing of parameters.\n";
            throw SimulationError(ss.str().c_str());
        }
        if(!arguments.costs_file.empty())
            read_ints(arguments.costs_file);

        read_iloc(arguments.iloc_file);

        Machine mach(program, arguments.bss, arguments.fp);
        if(arguments.debug)
            debug(&mach);
        else
            mach.run();
                

        if(arguments.prog)
            std::cout << mach.prog_state();
        if(arguments.mem)
            std::cout << mach.mem_state();
        if(arguments.reg)
            std::cout << mach.reg_state();
        if(arguments.stat)
            std::cout << mach.exec_state();
    } catch (SimulationError& simerr) {
        std::cerr << simerr.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
};

