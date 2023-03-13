/********************************************************************************
* main.c: Demonstration of a 32-bit CPU in progress, based on the AVR architecture.
********************************************************************************/
#include "cpu_controller.h"

/********************************************************************************
* main: Controls the program flow of the 32-bit processor via keyboard input from the user.
********************************************************************************/
int main(void)
{
   cpu_controller_run_by_input();
   return 0;
}
