#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Machine.hpp"
#include "parser.hpp"
#include <argp.h>

int yyparse();
extern FILE *yyin;
extern Program program;

static struct argp_option options[] = {
    {"costs",		'c', "COST_FILE", 0, "Parametrize instruction cost"},
    {"output",		'o', "OUT_FORMAT", 0, "Output format"},
    {"reg",		'r', "NUM_REG", 0, "Number of registers of the machine"},
    {"mem",		'm', "MEM_SIZE", 0, "Memory size in words"},
    {"start_pos",	'p', "LIM_LOW", 0, "Low frame limit"},
    {"frame_size",	'f', "LIM_SIZE", 0, "Frame size"},
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
        case 'o': arguments->output_format = atoi(arg); break;
        case 'r': arguments->num_reg = atoi(arg); break;
        case 'm': arguments->mem_size = atoi(arg); break;
        case 'p': arguments->frame_start = atoi(arg); break;
        case 'f': arguments->frame_size = atoi(arg); break;
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
"ilocsim 0.1";
const char *argp_program_bug_address =
"<jtpastro@inf.ufrgs.br>";

// Program documentation.
static char doc[] =
"ILOCsim -- a pretty simple iloc simulator";

//static char args_doc[] = "ARG1";

static struct argp argp = { options, parse_options, 0, doc };
/*
void read_ints (const char* file_name)
{
  FILE* file = fopen (file_name, "r");
  int l,i = 0;

  fscanf (file, "%d", &l);    
  while (!feof (file))
    {  
      opcode_specs[i++].latency=l;
      fscanf (file, "%d", &l);      
    }
  fclose (file);        
}
*/

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
    //arguments.costs_file="";

    if (argp_parse (&argp, argc, argv, 0, 0, &arguments) == ARGP_KEY_ERROR){
        fprintf(stderr, "%s, error during the parsing of parameters\n", argv[0]);
    }
    reg_size= arguments.num_reg;
    mem_size= arguments.mem_size;

    //if(arguments.costs_file[0]!='\0')
      //  read_ints(arguments.costs_file);    

    yyparse();

    Machine mach(program);
    mach.run();

    return 0;
};

