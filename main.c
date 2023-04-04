/********************************************************************************
* main.c: Demonstration of an 8-bit CPU in progress, based on AVR architecture.
********************************************************************************/
#include "control_unit.h"

/********************************************************************************
* main: Controls the program flow of an 8-bit processor by keyboard input.
********************************************************************************/
int main(void)
{
	control_unit_reset();
	
	while (1)
	{
		control_unit_run_next_state();
	}
	return 0;
}