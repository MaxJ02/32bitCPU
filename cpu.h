/********************************************************************************
* cpu.h: Contains miscellaneous definitions and declarations for implementation
*        of a 32-bit processor
********************************************************************************/

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
#define PCINT0_vect 0x02 /* Pin change interrupt vector 0 (for I/O port B). */
#define PCINT1_vect 0x04 /* Pin change interrupt vector 0 (for I/O port C). */
#define PCINT2_vect 0x06 /* Pin change interrupt vector 0 (for I/O port D). */

#define DDRA  0x00 /* Data direction register for I/O port B. */
#define PORTA 0x01 /* Data register for I/O port B. */
#define PINA  0x02 /* Pin input register for I/O port B. */

#define PCICR 0x09 /* Pin change interrupt control register for all I/O ports. */
#define PCIFR 0x0A /* Pin change interrupt flag register for all I/O ports. */

#define PCMSK0 0x10 /* Pin change interrupt mask register for I/O port B. */
#define PCMSK1 0x11 /* Pin change interrupt mask register for I/O port C. */
#define PCMSK2 0x12 /* Pin change interrupt mask register for I/O port D. */

#define PCIE0 0 /* Pin change interrupt enable bit for I/O port B. */
#define PCIE1 1 /* Pin change interrupt enable bit for I/O port C. */
#define PCIE2 2 /* Pin change interrupt enable bit for I/O port D. */

#define PCIF0 0 /* Pin change interrupt flag bit for I/O port A. */

#define PORTA0 0 /* Bit number for pin 0 at I/O port A. */
#define PORTA1 1 /* Bit number for pin 1 at I/O port A. */
#define PORTA2 2 /* Bit number for pin 2 at I/O port A. */
#define PORTA3 3 /* Bit number for pin 3 at I/O port A. */
#define PORTA4 4 /* Bit number for pin 4 at I/O port A. */
#define PORTA5 5 /* Bit number for pin 5 at I/O port A. */
#define PORTA6 6 /* Bit number for pin 6 at I/O port A. */
#define PORTA7 7 /* Bit number for pin 7 at I/O port A. */
#define PORTA8 8 /* Bit number for pin 8 at I/O port A. */



#endif /* CPU_H_ */
