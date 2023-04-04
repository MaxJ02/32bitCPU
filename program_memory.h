/********************************************************************************
* program_memory.h: Contains function declarations and macro definitions for
*                   implementation of a 6 kB program memory, capable of storing
*                   up to 256 24-bit instructions. Since C doesn't support
*                   unsigned 24-bit integers (without using structs or unions),
*                   the program memory is set to 32 bits data width, but only
*                   24 bits are used.
********************************************************************************/
#ifndef PROGRAM_MEMORY_H_
#define PROGRAM_MEMORY_H_

/* Include directives: */
#include "cpu.h"

/* Macro definitions: */
#define PROGRAM_MEMORY_DATA_WIDTH    64  /* 24 bits per instruction. */
#define PROGRAM_MEMORY_ADDRESS_WIDTH 25 /* Capacity for storage of 256 instructions. */

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
******************************************************************************/
uint64_t program_memory_read(const uint16_t address);

#endif /* PROGRAM_MEMORY_H_ */