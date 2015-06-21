#include <stdlib.h>
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

int yyparse();
extern Program program;

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
        case ARGP_KEY_END:
                  if (state->arg_num < 0) {
                      // Not enough arguments.
                      argp_usage (state);
                  }
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
"ILOCsim -- a pretty simple iloc simulator";

static struct argp argp = { options, parse_options, doc, doc };

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

int main(int argc, char** argv) {
    try{
        struct arguments arguments = {"",false,false,false,false,false,DEFAULT_FRAME_SIZE,0};    
        if (argp_parse (&argp, argc, argv, 0, 0, &arguments) == ARGP_KEY_ERROR){
            std::stringstream ss;
            ss << argv[0] << " error during the parsing of parameters." << std::endl;
            throw SimulationError(ss.str().c_str());
        }
        if(!arguments.costs_file.empty())
            read_ints(arguments.costs_file);

        yyparse();

        Machine mach(program, arguments.bss, arguments.fp);
        if(!arguments.debug)
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
        std::cerr << simerr.what();
    }
    return 0;
};

