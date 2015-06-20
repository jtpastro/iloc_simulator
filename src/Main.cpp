#include <stdlib.h>
#include <argp.h>
#include <iostream> //cout
#include <iostream> //cout
#include <fstream> //ifstream
#include <string> //std::string;
#include <sstream> //stringstream
#include <exception> //std::out_of_range

#include "Machine.hpp"
#include "parser.hpp"

int yyparse();
extern FILE *yyin;
extern Program program;

static struct argp_option options[] = {
    {"costs",		'c', "COST_FILE", 0, "Parametrize instruction cost"},
    { 0 }
};

struct arguments {
    int frame_size;
    int frame_start;
    int num_reg;
    int mem_size;
    int output_format;
    
    char *costs_file;

};

static error_t parse_options (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = (struct arguments*)(state->input);
    switch (key){
        case 'c': arguments->costs_file = arg; break;
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

//static char args_doc[] = "ARG1";

static struct argp argp = { options, parse_options, 0, doc };

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
                std::cerr << "Error loading file " << filename << " at line " << line_count << ": " << line <<  std::endl;
                exit(EXIT_FAILURE);
            }
        }
        myfile.close();
    } else {
        std::cerr << "Unable to open file: " << filename << "." << std::endl;
        exit(EXIT_FAILURE);
    }
}



int main(int argc, char** argv) {
    int mem_size = 0;
    int reg_size = 0;
    int machine_initialized = 0;

    struct arguments arguments;
    
    arguments.frame_size=0;
    arguments.frame_start=0;
    arguments.num_reg=0;
    arguments.mem_size=0;
    arguments.output_format=0;
    arguments.costs_file="";

    if (argp_parse (&argp, argc, argv, 0, 0, &arguments) == ARGP_KEY_ERROR){
        std::cerr << argv[0] << " error during the parsing of parameters." << std::endl;
        exit(EXIT_FAILURE);
    }
    reg_size= arguments.num_reg;
    mem_size= arguments.mem_size;

    if(arguments.costs_file[0]!='\0')
        read_ints(arguments.costs_file);

    yyparse();

    Machine mach(program);
    mach.run();

    return 0;
};

