#ifndef CONTROL_UNIT_H_
#define CONTROL_UNIT_H_

/* Include directives: */
#include "cpu.h"
#include "program_memory.h"
#include "data_memory.h"
#include "stack.h"
#include "alu.h"

/********************************************************************************
* control_unit_reset: Resets control unit and corresponding program.
********************************************************************************/
void control_unit_reset(void);

/********************************************************************************
* control_unit_run_next_state: Runs next state in the CPU instruction cycle.
********************************************************************************/
void control_unit_run_next_state(void);

#endif /* CONTROL_UNIT_H_ */