#ifdef CONTROL_UNIT_H_
#define CONTROL_UNIT_H_

#include "cpu.h"
#include "program_memory.h"
#include "data_memory.h"
#include "stack.h"
#include "alu.h"


void control_unit_reset(void);

void control_unit_run_next_state(void);

void control_unit_run_next_instruction_cycle(void);

void control_unit_print(void);

#endif