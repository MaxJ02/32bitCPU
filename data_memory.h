/********************************************************************************
* data_memory.h: Contains function declarations and macro definitions for
*                implementation of a 2 kB data memory (2000 x 1 byte).
********************************************************************************/
#ifndef DATA_MEMORY_H_
#define DATA_MEMORY_H_

/* Include directives: */
#include "cpu.h"

/* Macro definitions: */
#define DATA_MEMORY_ADDRESS_WIDTH 300   /* 300 unique addresses in data memory. */
#define DATA_MEMORY_DATA_WIDTH    32    /* 32 bits storage capacity per address. */

/********************************************************************************
* data_memory_reset: Clears entire data memory.
********************************************************************************/
void data_memory_reset(void);

/********************************************************************************
* data_memory_write: Writes an 8-bit value to specified address in data memory.
*                    The value 0 is returned after successful write. Otherwise
*                    if invalid address is specified, no write is done and
*                    error code 1 is returned.
* 
*                    - address: Write location in data memory.
*                    - value  : The 8-bit value to write to data memory.
********************************************************************************/
int data_memory_write(const uint16_t address,
                      const uint32_t value);

/********************************************************************************
* data_memory_read: Returns content from specified read location in data memory.
*                   If an invalid address is specified, the value 0 is returned.
* 
*                   - address: Read location in data memory.
********************************************************************************/
uint32_t data_memory_read(const uint16_t address);


/********************************************************************************
* data_memory_set_bit: Sets bit in specified data memory register. The value 0
*                      is returned after successful write. Otherwise if an
*                      invalid address is specified, no write is done and
*                      error code 1 is returned.
*
*                     - address: Write location in data memory.
*                     - bit    : Bit to set in data memory register.
********************************************************************************/
static inline int data_memory_set_bit(const uint16_t address,
                                      const uint32_t bit)
{
	const uint32_t data = data_memory_read(address);
	return data_memory_write(address, data | (1 << bit));
}

/********************************************************************************
* data_memory_clear_bit: Clears bit in specified data memory register. The value
*                        0 is returned after successful write. Otherwise if
*                        an invalid address is specified, no write is done and
*                        error code 1 is returned.
*
*                        - address: Write location in data memory.
*                        - bit    : Bit to clear in data memory register.
********************************************************************************/
static inline int data_memory_clear_bit(const uint16_t address,
                                        const uint32_t bit)
{
	const uint32_t data = data_memory_read(address);
	return data_memory_write(address, data & ~(1 << bit));
}

#endif /* DATA_MEMORY_H_ */