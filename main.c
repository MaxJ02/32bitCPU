#include "control_unit.h"

int main(void)
{
	control_unit_reset();
	
	while (1)
	{
		control_unit_run_next_state();
	}
	return 0;
}