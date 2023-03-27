#ifndef DATA_MEMORY_H_
#define DATA_MEMORY_H_

//Directives
#include "cpu.h"

//Macro definitions
#define DATA_MEMORY_ADDRESS_WIDTH 512
#define DATA_MEMORY_DATA_WIDTH 32

//Clears data
void data_memory_reset(void);

//Writes a byte to specific address and returns 0 after successful write.
//Address: address to write
//Data: value to write to given address
int data_memory_write(
	const uint16_t address,
	const uint32_t data );

//Returns byte to read, if invalid -> no read and return 0
//Address: Address to read
int data_memory_read(
	const uint16_t address
);
#endif