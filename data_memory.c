#include "data_memory.h"

static uint8_t data[DATA_MEMORY_DATA_WIDTH];
//Clears data
void data_memory_reset(void)
{
	for (uint32_t i = 0; i < DATA_MEMORY_DATA_WIDTH; ++i)
	{
		data[i] = 0x00;
	}
	return;
}
//Writes a byte to specific address and returns 0 after successful write.
//Address: address to write
//Data: value to write to given address
int data_memory_write(const uint16_t address,
	const uint32_t value)
{
	if (address < DATA_MEMORY_ADDRESS_WIDTH)
	{
		data[address] = value;
		return 0;
	}
	else
	{
		return 1;
	}
}
//Returns byte to read, if invalid -> no read and return 0
//Address: Address to read
uint32_t data_memory_read(const uint16_t address)
{
	if (address < DATA_MEMORY_ADDRESS_WIDTH)
	{
		return data[address];
	}
	else
	{
		return 0;
	}
}