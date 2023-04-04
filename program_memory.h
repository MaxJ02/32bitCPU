#ifndef PROGRAM_MEMORY_H_
#define PROGRAM_MEMORY_H_

#include "cpu.h"

#define PROGRAM_MEMORY_DATA_WIDTH 24
#define PROGRAM_MEMORY_ADDRESS_WIDTH 256

void program_memory_write(void);

uint32_t program_memory_read(const uint32_t address);

const char* program_memory_subroutine_name(const uint32_t address);

#endif