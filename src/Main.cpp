#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Machine.hpp"
#include "parser.hpp"

int yyparse();
extern FILE *yyin;
/*static struct argp_option options[] = {
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
Program parse(){
    Program prog;
    Operation op, op2, op3, op4, op5, op6, op7, op8, op9;
    op9.opcode = OUTPUT;
    op9.consts.push_back(1024);
    prog.add_operation("L3", op9);
    op8.opcode = STORE;
    op8.regs.push_back(5);
    op8.regs.push_back(6);
    prog.add_operation(op8);
    op7.opcode = LOADI;
    op7.consts.push_back(3);
    op7.regs.push_back(5);
    prog.add_operation("L1", op7);
    op6.opcode = JUMPI;
    op6.labels.push_back("L3");
    prog.add_operation(op6);
    op5.opcode = STORE;
    op5.regs.push_back(5);
    op5.regs.push_back(6);
    prog.add_operation(op5);
    op4.opcode = LOADI;
    op4.consts.push_back(1024);
    op4.regs.push_back(6);
    prog.add_operation(op4);
    op3.opcode = ADD;
    op3.regs.push_back(3);
    op3.regs.push_back(3);
    op3.regs.push_back(5);
    prog.add_operation(op3);
    op2.opcode = MULT;
    op2.regs.push_back(3);
    op2.regs.push_back(3);
    op2.regs.push_back(5);
    prog.add_operation(op2);
    op.opcode = LOADI;
    op.consts.push_back(3);
    op.regs.push_back(3);
    prog.add_operation("L2", op);
    return prog;
}

int main(int argc, char** argv) {
    int mem_size = 0;
    int reg_size = 0;
    int machine_initialized = 0;
    Program code;
/*
    struct arguments arguments;
    
    arguments.frame_size=0;
    arguments.frame_start=0;
    arguments.num_reg=0;
    arguments.mem_size=0;
    arguments.output_format=0;
    arguments.costs_file="";

    if (argp_parse (&argp, argc, argv, 0, 0, &arguments) == ARGP_KEY_ERROR){
        fprintf(stderr, "%s, error during the parsing of parameters\n", argv[0]);
    }
    reg_size= arguments.num_reg;
    mem_size= arguments.mem_size;

    if(arguments.costs_file[0]!='\0')
        read_ints(arguments.costs_file);

    if (!machine_initialized)
        initialize_machine(reg_size,mem_size);
*/
    yyparse();

    Machine mach(reg_size, mem_size, code);
    mach.run();

    return 0;
};

