/*  sim.c
 *  Source code for a simulator of the ILOC subset defined in
 *  "Engineering a Compiler" by Cooper and Torczon
 *  written by Todd Waterman
 *  11/30/00 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <argp.h>
#include "operation.h"
#include "machine.h"
#include "sim.h"

int main(int argc, char** argv) {
    int mem_size = 0;
    int reg_size = 0;
    int current_argument = 1;
    int machine_initialized = 0;
    Operation* code;

    argp_parse (0, argc, argv, 0, 0, 0);

    if (!machine_initialized)
        initialize_machine(reg_size,mem_size);

    code = parse();

    if (!code) {
        fprintf(stderr,"Error reading input file, simulator not run.\n");
        return 1;
    }

    simulate(code);

    return 0;
};

/* Print a usage message */
void print_help() {
    printf("Usage: sim [options] < filename\n");
    printf("  Options:\n");
    printf("    -h                 display usage message\n");
    printf("    -r NUM             simulator has NUM available registers\n");
    printf("    -m NUM             simulator has NUM bytes of memory\n");
    printf("    -s NUM             simulator stalls for the following conditions:\n");
    printf("                         0:  nothing\n");
    printf("                         1:  branches\n");
    printf("                         2:  branches and memory interlocks\n");
    printf("                         3:  branches and both register and memory interlocks\n");
    printf("    -i NUM ... NUM     starting at the memory location specified by the first\n");
    printf("                         NUM put the remaining NUMs into memory as words.\n");
    printf("                         Must be the last option specified\n");
    printf("    -c NUM ... NUM     same as -i, but puts the NUMs into memory as bytes\n");
    printf("  filename should be a valid ILOC input file\n");

}

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

    fprintf(stdout,"Executed %d operations in %d cycles.\n",
            operation_count,cycle_count);

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

        case BR:
            effects = (Change*)malloc(sizeof(Change));
            effects->type = BRANCH;
            effects->target = (get_label(op->labels->value))->target;
            effects->cycles_away = opcode_specs[BR].latency;
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
            effects->cycles_away = opcode_specs[BR].latency;
            effects->next = NULL;
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

