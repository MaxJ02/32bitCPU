#ifndef PROGRAM_MEMORY_H_
#define PROGRAM_MEMORY_H_

/* Include directives: */
#include "cpu.h"

/* Macro definitions: */
#define PROGRAM_MEMORY_ADDRESS_WIDTH 25 /* Capacity for storage of 25 instructions. */

/********************************************************************************
* program_memory_write: Writes machine code to the program memory. This function
*                       should be called once when the program starts.
********************************************************************************/
void program_memory_write(void);

/********************************************************************************
* program_memory_read: Returns the instruction at specified address. If an
*                      invalid address is specified (should be impossible as
*                      long as the program memory address width isn't increased)
*                      no operation (0x00) is returned.
*
*                      - address: Address to instruction in program memory.
********************************************************************************/
uint64_t program_memory_read(const uint16_t address);

#endif /* PROGRAM_MEMORY_H_ */

