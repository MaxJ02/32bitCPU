/***********************************************************************************
* cpu.h: Contains miscellaneous definitions and declarations for the implementation
*        of a 32-bit processor
************************************************************************************/

#ifndef CPU_H_
#define CPU_H_

/* Include directives: */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define NOP  0x00 /* No operation. */
#define LDI  0x01 /* Loads constant into CPU register. */
#define MOV  0x02 /* Copies content of a CPU-register to another CPU register. */
#define OUT  0x03 /* Writes to I/O location. */
#define IN   0x04 /* Reads from I/O location. */
#define STS  0x05 /* Stores data to address in data memory. */
#define LDS  0x06 /* Loads data from address in data memory. */
#define CLR  0x07 /* Clears CPU register. */
#define ORI  0x08 /* Performs bitwise OR with constant. */
#define ANDI 0x09 /* Performs bitwise AND with constant. */
#define XORI 0x0A /* Performs bitwise XOR with constant. */
#define OR   0x0B /* Performs bitwise OR with content in a CPU register. */
#define AND  0x0C /* Performs bitwise AND with content in a CPU register. */
#define XOR  0x0D /* Performs bitwise XOR with content in a CPU register. */
#define ADDI 0x0E /* Performs addition with a constant. */
#define SUBI 0x0F /* Performs subtraction with a constant. */

#define ADD  0x10 /* Performs addition with content in a CPU register. */
#define SUB  0x11 /* Performs subtraction with content in a CPU register. */
#define INC  0x12 /* Increments content of a CPU register. */
#define DEC  0x13 /* Decrements content of a CPU register. */
#define CPI  0x14 /* Compares content of a CPU register with a constant. */
#define CP   0x15 /* Compares content between CPU registers. */
#define JMP  0x16 /* Jumps to specified address. */
#define BREQ 0x17 /* Jumps to specified address if result of last calculation is zero. */
#define BRNE 0x18 /* Jumps to specified address if result of last calculation is not equal to zero. */
#define BRGE 0x19 /* Jumps to specified address if result of last calculation is greater or equal to zero. */
#define BRGT 0x1A /* Jumps to specified address if result of last calculation is greater than zero. */
#define BRLE 0x1B /* Jumps to specified address if result of last calculation is lower or equal to zero. */
#define BRLT 0x1C /* Jumps to specified address if result of last calculation is lower than zero. */
#define CALL 0x1D /* Calls subroutine at specified address and stores the return address on the stack. */
#define RET  0x1E /* Returns from subroutine by jumping to the return address popped from the stack. */
#define RETI 0x1F /* Returns from interrupt by restoring program as it was before the interrupt occured. */

#define PUSH 0x20 /* Pushes value stored in a CPU register to the stack. */
#define POP  0x21 /* Pops value from the stack and loads it into a CPU register. */
#define LSL  0x22 /* Shifts content of a CPU register one step to the left. */
#define LSR  0x23 /* Shifts content of a CPU register one step to the right. */
#define SEI  0x24 /* Enables interrupts globally by setting the I-flag of the status register. */
#define CLI  0x25 /* Disables interrupts globally by clearning the I-flag of the status register. */

#define RESET_vect  0x00 /* Reset vector. */
#define PCINT_vect 0x02 /* Pin change interrupt vector 0 (for I/O port A). */

#define DDRA  0x00 /* Data direction register for I/O port A. */
#define PORTA 0x01 /* Data register for I/O port A. */
#define PINA  0x02 /* Pin input register for I/O port A. */

#define PCICR 0x09 /* Pin change interrupt control register for all I/O ports. */
#define PCIFR 0x0A /* Pin change interrupt flag register for all I/O ports. */

#define PCMSKA0 0x10 /* Pin change interrupt mask register for I/O port A. */

#define PCIEA 0 /* Pin change interrupt enable bit for I/O port A. */
#define PCIFA0 0 /* Pin change interrupt flag bit for I/O port A. */

#define PORTA0 0 /* Bit number for pin 0 at I/O port A. */
#define PORTA1 1 /* Bit number for pin 1 at I/O port A. */
#define PORTA2 2 /* Bit number for pin 2 at I/O port A. */
#define PORTA3 3 /* Bit number for pin 3 at I/O port A. */
#define PORTA4 4 /* Bit number for pin 4 at I/O port A. */
#define PORTA5 5 /* Bit number for pin 5 at I/O port A. */
#define PORTA6 6 /* Bit number for pin 6 at I/O port A. */
#define PORTA7 7 /* Bit number for pin 7 at I/O port A. */
#define PORTA8 8 /* Bit number for pin 8 at I/O port A. */
#define PORTA9 9 /* Bit number for pin 9 at I/O port A. */
#define PORTA10 10 /* Bit number for pin 10 at I/O port A. */
#define PORTA11 11 /* Bit number for pin 11 at I/O port A. */
#define PORTA12 12 /* Bit number for pin 12 at I/O port A. */
#define PORTA13 13 /* Bit number for pin 13 at I/O port A. */

#define PORTA14 A0
#define PORTA15 A1
#define PORTA16 A2
#define PORTA17 A3
#define PORTA18 A4
#define PORTA19 A5

#define R0  0x00 /* Address for CPU register R0. */
#define R1  0x01 /* Address for CPU register R1. */
#define R2  0x02 /* Address for CPU register R2. */
#define R3  0x03 /* Address for CPU register R3. */
#define R4  0x04 /* Address for CPU register R4. */
#define R5  0x05 /* Address for CPU register R5. */
#define R6  0x06 /* Address for CPU register R6. */
#define R7  0x07 /* Address for CPU register R7. */
#define R8  0x08 /* Address for CPU register R8. */
#define R9  0x09 /* Address for CPU register R9. */
#define R10 0x0A /* Address for CPU register R10. */
#define R11 0x0B /* Address for CPU register R11. */
#define R12 0x0C /* Address for CPU register R12. */
#define R13 0x0D /* Address for CPU register R13. */
#define R14 0x0E /* Address for CPU register R14. */
#define R15 0x0F /* Address for CPU register R15. */

#define R16 0x10 /* Address for CPU register R16. */
#define R17 0x11 /* Address for CPU register R17. */
#define R18 0x12 /* Address for CPU register R18. */
#define R19 0x13 /* Address for CPU register R19. */
#define R20 0x14 /* Address for CPU register R20. */
#define R21 0x15 /* Address for CPU register R21. */
#define R22 0x16 /* Address for CPU register R22. */
#define R23 0x17 /* Address for CPU register R23. */
#define R24 0x18 /* Address for CPU register R24. */
#define R25 0x19 /* Address for CPU register R25. */
#define R26 0x1A /* Address for CPU register R26. */
#define R27 0x1B /* Address for CPU register R27. */
#define R28 0x1C /* Address for CPU register R28. */
#define R29 0x1D /* Address for CPU register R29. */
#define R30 0x1E /* Address for CPU register R30. */
#define R31 0x1F /* Address for CPU register R31. */


#define CPU_REGISTER_ADDRESS_WIDTH 32 /* 32 CPU registers in control unit. */
#define CPU_REGISTER_DATA_WIDTH    32  /* 8 bit data width per CPU register. */
#define IO_REGISTER_DATA_WIDTH     32  /* 8 bit data width per I/O location. */

#define I 5 /* Interrupt flag in status register. */
#define S 4 /* Signed flag in status register. */
#define N 3 /* Negative flag in status register. */
#define Z 2 /* Zero flag in status register. */
#define V 1 /* Overflow flag in status register. */
#define C 0 /* Carry flag in status register. */


#endif /* CPU_H_ */
