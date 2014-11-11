/*  sim.c
 *  Source code for a simulator of the ILOC subset defined in
 *  "Engineering a Compiler" by Cooper and Torczon
 *  written by Todd Waterman
 *  11/30/00 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <argp.h>
#include "operation.h"
#include "machine.h"
#include "sim.h"

static struct argp_option options[] = {
    {"costs",		'c', "COST_FILE", 0, "Parametrize instruction cost"},
    {"output",		'o', "OUT_FORMAT", 0, "Output format"},
    {"reg",		'r', "NUM_REG", 0, "Number of registers of the machine"},
    {"mem",		'm', "MEM_SIZE", 0, "Memory size in words"},
    {"stack_size",	's', "STACK_SIZE", 0, "Maximum stack size"},
    {"stack_start",	'p', "STACK_START", 0, "Base memory address for stack"},
    { 0 }
};

struct arguments {
    int stack_size;
    int stack_start;
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
        case 'p': arguments->stack_start = atoi(arg); break;
        case 's': arguments->stack_size = atoi(arg); break;
        case ARGP_KEY_END:
                  if (state->arg_num < 0) {
                      /* Not enough arguments. */
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

/* Program documentation. */
static char doc[] =
"ILOCsim -- a pretty simple iloc simulator";

//static char args_doc[] = "ARG1";

static struct argp argp = { options, parse_options, 0/*args_doc*/, doc };

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

int main(int argc, char** argv) {
    int mem_size = 0;
    int reg_size = 0;
    int machine_initialized = 0;
    Operation* code;

    struct arguments arguments;
    bzero (&arguments, sizeof (struct arguments));
    
    if (argp_parse (&argp, argc, argv, 0, 0, &arguments) == ARGP_KEY_ERROR){
        fprintf(stderr, "%s, error during the parsing of parameters\n", argv[0]);
    }
    reg_size= arguments.num_reg;
    mem_size= arguments.mem_size;

    if(arguments.costs_file[0]!='\0')
        read_ints(arguments.costs_file);

    if (!machine_initialized)
        initialize_machine(reg_size,mem_size);

    code = parse();

    if (!code) {
        fprintf(stderr,"Error reading input file, simulator not run.\n");
        return 1;
    }

    simulate(code);

    reg_state();
    return 0;
};

/* Simulate the code and output results to standard out */
void simulate(Operation* code) {
    Change* list_of_effects = NULL; 
    Change* last_effect = NULL;
    Change* new_effects;
    int operation_count = 0;
    int cycle_count = 0;

    int error_code_machine_constraints = 0;

    while(code) {
        new_effects = execute_operation(code);
        operation_count++;
        if (!list_of_effects) {
            list_of_effects = new_effects;
            last_effect = new_effects;
        }
        else
            last_effect->next = new_effects;

        /* Move last effect to end */
        if (last_effect)
            while(last_effect->next)
                last_effect = last_effect->next;

        /* Go to next operation */
        code = code->next;

        list_of_effects = execute_changes(list_of_effects,&last_effect,&code);
        cycle_count++;

    }

    while(list_of_effects) {
        list_of_effects = execute_changes(list_of_effects,&last_effect,&code);
        cycle_count++;
    }
}

/* Execute a single operation */
Change* execute_operation(Operation* op) {
    Change* effects;
    Change* effect_ptr;
    int i;

    /* This is a big, ugly switch statement that deals with every operation */
    switch(op->opcode) {
        case NOP:
            return NULL;
            break;

        case ADD:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value) + 
                get_register(op->srcs->next->value);
            return effects;
            break;

        case SUB:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value) - 
                get_register(op->srcs->next->value);
            return effects;
            break;

        case MULT:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value) * 
                get_register(op->srcs->next->value);
            return effects;
            break;

        case DIV:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value) / 
                get_register(op->srcs->next->value);
            return effects;
            break;	  

        case ADDI:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value) + op->consts->value;
            return effects;
            break;

        case SUBI:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value) - op->consts->value;
            return effects;
            break;

        case MULTI:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value) * op->consts->value;
            return effects;
            break;

        case DIVI:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value) / op->consts->value;
            return effects;
            break;

        case LSHIFT:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value) << 
                get_register(op->srcs->next->value);
            return effects;
            break;

        case LSHIFTI:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value) << op->consts->value;
            return effects;
            break;

        case RSHIFT:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value) >> 
                get_register(op->srcs->next->value);
            return effects;
            break;

        case RSHIFTI:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value) >> op->consts->value;
            return effects;
            break;

        case LOADI:
            effects = onereg(op);
            effects->value = op->consts->value;
            return effects;
            break;

        case LOAD:
            effects = onereg(op);
            effects->value = get_word(get_register(op->srcs->value));
            return effects;
            break;

        case LOADAI:
            effects = onereg(op);
            effects->value = get_word(get_register(op->srcs->value) +
                    op->consts->value);
            return effects;
            break;

        case LOADAO:
            effects = onereg(op);
            effects->value = get_word(get_register(op->srcs->value) +
                    get_register(op->srcs->next->value));
            return effects;
            break;

        case CLOAD:
            effects = onereg(op);
            effects->value = get_memory(get_register(op->srcs->value));
            return effects;
            break;

        case CLOADAI:
            effects = onereg(op);
            effects->value = get_memory(get_register(op->srcs->value) +
                    op->consts->value);
            return effects;
            break;

        case CLOADAO:
            effects = onereg(op);
            effects->value = get_memory(get_register(op->srcs->value) +
                    get_register(op->srcs->next->value));
            return effects;
            break;

        case STORE:
            effect_ptr = NULL;
            for(i=0;i<4;i++) {
                effects = storeop(op);
                effects->value = (get_register(op->srcs->value) << (8*i)) >> 24;
                effects->location = get_register(op->defs->value) + i;
                effects->next = effect_ptr;
                effect_ptr = effects;
            }
            return effects;
            break;

        case STOREAI:
            effect_ptr = NULL;
            for(i=0;i<4;i++) {
                effects = storeop(op);
                effects->value = (get_register(op->srcs->value) << (8*i)) >> 24;
                effects->location = get_register(op->defs->value) + 
                    op->consts->value + i;
                effects->next = effect_ptr;
                effect_ptr = effects;
            }
            return effects;
            break;

        case STOREAO:
            effect_ptr = NULL;
            for(i=0;i<4;i++) {
                effects = storeop(op);
                effects->value = (get_register(op->srcs->value) << (8*i)) >> 24;
                effects->location = get_register(op->defs->value) + 
                    get_register(op->defs->next->value) + i;
                effects->next = effect_ptr;
                effect_ptr = effects;
            }
            return effects;
            break;

        case CSTORE:
            effects = storeop(op);
            effects->value = (get_register(op->srcs->value) << 24) >> 24;
            effects->location = get_register(op->defs->value);
            effects->next = NULL;
            return effects;
            break;

        case CSTOREAI:
            effects = storeop(op);
            effects->value = (get_register(op->srcs->value) << 24) >> 24;
            effects->location = get_register(op->defs->value) + op->consts->value;
            effects->next = NULL;
            return effects;
            break;

        case CSTOREAO:
            effects = storeop(op);
            effects->value = (get_register(op->srcs->value) << 24) >> 24;
            effects->location = get_register(op->defs->value) + 
                get_register(op->defs->next->value);
            effects->next = NULL;
            return effects;
            break;

        case JUMPI:
            effects = (Change*)malloc(sizeof(Change));
            effects->type = BRANCH;
            effects->target = (get_label(op->labels->value))->target;
            effects->cycles_away = opcode_specs[JUMPI].latency;
            effects->next = NULL;
            return effects;
            break;
        case CBR:
            effects = (Change*)malloc(sizeof(Change));
            effects->type = BRANCH;
            if (get_register(op->srcs->value))
                effects->target = (get_label(op->labels->value))->target;
            else
                effects->target = (get_label(op->labels->next->value))->target;
            effects->cycles_away = opcode_specs[JUMPI].latency;
            effects->next = NULL;
            return effects;
            break;
        case CBR_LT:
            effects = (Change*)malloc(sizeof(Change));
            effects->type = BRANCH;
            if (get_register(op->srcs->value)<0)
                effects->target = (get_label(op->labels->value))->target;
            else
                effects->target = (get_label(op->labels->next->value))->target;
            effects->cycles_away = opcode_specs[JUMPI].latency;
            effects->next = NULL;
            return effects;
            break;
        case CBR_LE:
            effects = (Change*)malloc(sizeof(Change));
            effects->type = BRANCH;
            if (get_register(op->srcs->value)<=0)
                effects->target = (get_label(op->labels->value))->target;
            else
                effects->target = (get_label(op->labels->next->value))->target;
            effects->cycles_away = opcode_specs[JUMPI].latency;
            effects->next = NULL;
            return effects;
            break;
        case CBR_EQ:
            effects = (Change*)malloc(sizeof(Change));
            effects->type = BRANCH;
            if (get_register(op->srcs->value)==0)
                effects->target = (get_label(op->labels->value))->target;
            else
                effects->target = (get_label(op->labels->next->value))->target;
            effects->cycles_away = opcode_specs[JUMPI].latency;
            effects->next = NULL;
            return effects;
            break;
        case CBR_GE:
            effects = (Change*)malloc(sizeof(Change));
            effects->type = BRANCH;
            if (get_register(op->srcs->value)>=0)
                effects->target = (get_label(op->labels->value))->target;
            else
                effects->target = (get_label(op->labels->next->value))->target;
            effects->cycles_away = opcode_specs[JUMPI].latency;
            effects->next = NULL;
            return effects;
            break;
        case CBR_GT:
            effects = (Change*)malloc(sizeof(Change));
            effects->type = BRANCH;
            if (get_register(op->srcs->value)>0)
                effects->target = (get_label(op->labels->value))->target;
            else
                effects->target = (get_label(op->labels->next->value))->target;
            effects->cycles_away = opcode_specs[JUMPI].latency;
            effects->next = NULL;
            return effects;
            break;
        case CBR_NE:
            effects = (Change*)malloc(sizeof(Change));
            effects->type = BRANCH;
            if (get_register(op->srcs->value)!=0)
                effects->target = (get_label(op->labels->value))->target;
            else
                effects->target = (get_label(op->labels->next->value))->target;
            effects->cycles_away = opcode_specs[JUMPI].latency;
            effects->next = NULL;
            return effects;
            break;
	case COMP:
            effects = onereg(op);
            if (get_register(op->srcs->value) > 
                    get_register(op->srcs->next->value))
                effects->value = 1;
            else if (get_register(op->srcs->value) == 
                    get_register(op->srcs->next->value))
                effects->value = 0;
	    else
                effects->value = -1;
            return effects;
            break;
        case CMPLT:
            effects = onereg(op);
            if (get_register(op->srcs->value) < 
                    get_register(op->srcs->next->value))
                effects->value = 1;
            else
                effects->value = 0;
            return effects;
            break;

        case CMPLE:
            effects = onereg(op);
            if (get_register(op->srcs->value) <= 
                    get_register(op->srcs->next->value))
                effects->value = 1;
            else
                effects->value = 0;
            return effects;
            break;

        case CMPEQ:
            effects = onereg(op);
            if (get_register(op->srcs->value) == 
                    get_register(op->srcs->next->value))
                effects->value = 1;
            else
                effects->value = 0;
            return effects;
            break;

        case CMPNE:
            effects = onereg(op);
            if (get_register(op->srcs->value) != 
                    get_register(op->srcs->next->value))
                effects->value = 1;
            else
                effects->value = 0;
            return effects;
            break;

        case CMPGE:
            effects = onereg(op);
            if (get_register(op->srcs->value) >= 
                    get_register(op->srcs->next->value))
                effects->value = 1;
            else
                effects->value = 0;
            return effects;
            break;

        case CMPGT:
            effects = onereg(op);
            if (get_register(op->srcs->value) > 
                    get_register(op->srcs->next->value))
                effects->value = 1;
            else
                effects->value = 0;
            return effects;
            break;

        case I2I:
            effects = onereg(op);
            effects->value = get_register(op->srcs->value);
            return effects;
            break;

        case C2C:
        case C2I:
        case I2C:
            effects = onereg(op);
            effects->value = (get_register(op->srcs->value) << 24) >> 24;
            return effects;
            break;

        case OUTPUT:
            effects = (Change*)malloc(sizeof(Change));
            effects->type = DISPLAY;
            effects->cycles_away = opcode_specs[OUTPUT].latency;
            effects->next = NULL;
            effects->value = get_word(op->consts->value);
            return effects;
            break;

        case COUTPUT:
            effects = (Change*)malloc(sizeof(Change));
            effects->type = DISPLAY;
            effects->cycles_away = opcode_specs[OUTPUT].latency;
            effects->next = NULL;
            effects->value = get_memory(op->consts->value);
            return effects;
            break;

        default:
            fprintf(stderr,"Simulator Error: Invalid opcode encountered in execute_operation.");
            return NULL;
            break;
    }
}

/* onereg creates most of a change structure for the common case where 
   a single register is defined. */
Change* onereg(Operation* op) {
    Change* effect = (Change*)malloc(sizeof(Change));
    effect->type = REGISTER;
    effect->location = op->defs->value;
    effect->cycles_away = opcode_specs[op->opcode].latency;
    effect->next = NULL;
    return effect;
}

/* storeop creates most of a change structure for a store operation */
Change* storeop(Operation* op) {
    Change* effect = (Change*)malloc(sizeof(Change));
    effect->type = MEMORY;
    effect->cycles_away = opcode_specs[op->opcode].latency;
    effect->next = NULL;
    return effect;
}


/* Reduces the cycles_away of all changes by one and executes any changes
   that have a cycles_away of 0 */
Change* execute_changes(Change* changes, Change** last_change_ptr, Operation** code_ptr) {
    Change* first_change = changes;
    Change* prev_change = NULL;

    /* Iterate through all changes */
    while(changes) {
        changes->cycles_away -= 1;

        if (changes->cycles_away == 0) {
            /* Perform effect */
            switch(changes->type) {
                case REGISTER:
                    set_register(changes->location,changes->value);
                    break;
                case MEMORY:
                    set_memory(changes->location,changes->value);
                    break;
                case BRANCH:
                    *code_ptr = changes->target;
                    break;
                case DISPLAY:
                    fprintf(stdout,"%d\n",changes->value);
                    break;
            }

            /* Delete change record */
            if (prev_change) {
                prev_change->next = changes->next;
                free(changes);
                changes = prev_change->next;
            }
            else {
                first_change = changes->next;
                free(changes);
                changes = first_change;
            }
        }
        else {
            prev_change = changes;
            changes = changes->next;
        }

    }

    *last_change_ptr = prev_change;
    return (first_change);
}

