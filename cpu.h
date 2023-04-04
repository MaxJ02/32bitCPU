/********************************************************************************
* cpu.h: Contains miscellaneous definitions and declarations for implementation
*        of an 8-bit processor based on AVR:s instruction list.
********************************************************************************/
#ifndef CPU_H_
#define CPU_H_

/* Include directives: */
#include <stdio.h>
#include <avr/io.h>
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
#define STIO 0x26 /* Writes to referenced I/O location in data memory (address 0 - 255). */
#define LDIO 0x27 /* Reads from referenced I/O location in data memory (address 0 - 255). */
#define ST   0x28 /* Writes to referenced location in data memory. (address 256 - 1999). */
#define LD   0x29 /* Reads from referenced location in data memory (address 256 - 1999). */

#define RESET_vect  0x00 /* Reset vector. */
#define PCINT_vect 0x02 /* Pin change interrupt vector (for I/O port A). */

#define DDRA  0x00 /* Data direction register for I/O port A. */
#define PORTA 0x01 /* Data register for I/O port A. */
#define PINA  0x02 /* Pin input register for I/O port A. */

#define ICR 0x03 /* Pin change interrupt control register for all I/O ports. */
#define IFR 0x04 /* Pin change interrupt flag register for all I/O ports. */

#define PCMSKA 0x05 /* Pin change interrupt mask register for I/O port A. */
#define PCIEA 0 /* Pin change interrupt enable bit for I/O port A. */
#define PCIFA 0 /* Pin change interrupt flag bit for I/O port A. */

#define PORTA0 0 /* Bit number for pin 0 at I/O port D. */
#define PORTA1 1 /* Bit number for pin 1 at I/O port D. */
#define PORTA2 2 /* Bit number for pin 2 at I/O port D. */
#define PORTA3 3 /* Bit number for pin 3 at I/O port D. */
#define PORTA4 4 /* Bit number for pin 4 at I/O port D. */
#define PORTA5 5 /* Bit number for pin 5 at I/O port D. */
#define PORTA6 6 /* Bit number for pin 6 at I/O port D. */
#define PORTA7 7 /* Bit number for pin 7 at I/O port D. */

#define PORTA8 8 /* Bit number for pin 8 at I/O port B. */
#define PORTA9 9 /* Bit number for pin 9 at I/O port B. */
#define PORTA10 10 /* Bit number for pin 10 at I/O port B. */
#define PORTA11 11 /* Bit number for pin 11 at I/O port B. */
#define PORTA12 12 /* Bit number for pin 12 at I/O port B. */
#define PORTA13 13 /* Bit number for pin 13 at I/O port B. */

#define PORTA14 14 /* Bit number for pin A0 at I/O port A. */
#define PORTA15 15 /* Bit number for pin A1 at I/O port A. */
#define PORTA16 16 /* Bit number for pin A2 at I/O port A. */
#define PORTA17 17 /* Bit number for pin A3 at I/O port A. */
#define PORTA18 18 /* Bit number for pin A4 at I/O port A. */
#define PORTA19 19 /* Bit number for pin A5 at I/O port A. */


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
#define CPU_REGISTER_DATA_WIDTH    32  /* 32 bit data width per CPU register. */
#define IO_REGISTER_DATA_WIDTH     32  /* 32 bit data width per I/O location. */

#define I 5 /* Interrupt flag in status register. */
#define S 4 /* Signed flag in status register. */
#define N 3 /* Negative flag in status register. */
#define Z 2 /* Zero flag in status register. */
#define V 1 /* Overflow flag in status register. */
#define C 0 /* Carry flag in status register. */

/********************************************************************************
* set: Sets bit in specified register without affecting other bits.
*
*      - reg: Reference to the register.
*      - bit: The bit to be set in the referenced register.
********************************************************************************/
#define set(reg, bit)  reg |= (1 << bit)

/********************************************************************************
* clr: Clears bit in specified register without affecting other bits.
*
*      - reg: Reference to the register.
*      - bit: The bit to be cleared in the referenced register.
********************************************************************************/
#define clr(reg, bit)  reg &= ~(1 << (bit))

/********************************************************************************
* read: Reads bit from specified register. The return value is 1 is the bit is
*       high, otherwise 0 if the bit is low.
*
*       - reg: Reference to the register.
*       - bit: The bit to be read in the referenced register.
********************************************************************************/
static inline bool read(const uint32_t reg, const uint8_t bit)
{
   return (bool)(reg & (1 << bit));
}

/********************************************************************************
* cpu_state: Enumeration for the different states of the CPU instructio cycle.
********************************************************************************/
enum cpu_state
{
	CPU_STATE_FETCH,  /* Fetches next instruction from program memory. */
	CPU_STATE_DECODE, /* Decodes the fetched instruction. */
	CPU_STATE_EXECUTE /* Executes the decoded instruction. */
};

#endif /* CPU_H_ */