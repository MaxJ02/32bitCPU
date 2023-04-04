/********************************************************************************
* program_memory.c: Contains function definitions and macro definitions for
*                   implementation of a 0.75 kB program memory, capable of
*                   storing up to 256 24-bit instructions. Since C doesn't
*                   support unsigned 24-bit integers (without using structs or
*                   unions), the program memory is set to 32 bits data width,
*                   but only 24 bits are used.
********************************************************************************/
#include "program_memory.h"

/* Macro definitions: */
#define main      1  /* Start address for subroutine main. */
#define main_loop 2  /* Start address for loop in subroutine main. */
#define led1_off  5  /* Start address for subroutine led1_off. */
#define led1_on   9  /* Start address for subroutine led1_on. */
#define setup     13 /* Start address for subroutine setup. */
#define end       18 /* End address for current program. */

#define LED1    PORTA0 /* LED 1 connected to pin 8 (PORTB0). */
#define BUTTON1 PORTA5 /* Button 1 connected to pin 13 (PORTA5). */

/* Static functions: */
static inline uint64_t assemble(const uint16_t op_code,
                                const uint16_t op1,
                                const uint32_t op2);

/* Static variables: */
static uint64_t data[PROGRAM_MEMORY_ADDRESS_WIDTH]; /* 0.75 kB program memory. */

/********************************************************************************
* program_memory_write: Writes machine code to the program memory. This function
*                       should be called once when the program starts.
********************************************************************************/
void program_memory_write(void)
{
   static bool program_memory_initialized = false;
   if (program_memory_initialized) return;

   /********************************************************************************
   * RESET_vect: Reset vector and start address for the program. A jump is made
   *             to the main subroutine in order to start the program.
   ********************************************************************************/
   data[0] = assemble(JMP, main, 0x00); /* JMP main */

   /*********************************************************************************
   * main: Initiates the I / O ports at start.The program is running as long as
   *       voltage is supplied.BUTTON1 is continuously polled.When BUTTON1
   *       is pressed, LED1 is enabled, otherwise it's disabled.
   *********************************************************************************/
   data[1] = assemble(CALL, setup, 0x00);         /* CALL setup */
   data[2] = assemble(IN, R16, PINA);             /* IN R16, PINB */
   data[3] = assemble(ANDI, R16, (1 << BUTTON1)); /* ANDI R16, (1 << BUTTON1) */
   data[4] = assemble(BRNE, led1_on, 0x00);       /* BRNE led1_on */

   /********************************************************************************
   * led1_off: Disables LED1.
   *********************************************************************************/
   data[5] = assemble(IN, R16, PORTA);          /* IN R16, PORTB */
   data[6] = assemble(ANDI, R16, ~(1 << LED1)); /* ANDI R16, ~(1 << LED1) */
   data[7] = assemble(OUT, PORTA, R16);         /* OUT PORTB, R16 */
   data[8] = assemble(JMP, main_loop, 0x00);    /* JMP main_loop */

   /********************************************************************************
   * led1_on: Enables LED1.
   *********************************************************************************/
   data[9]  = assemble(IN, R16, PORTA);        /* IN R16, PORTB */
   data[10] = assemble(ORI, R16, (1 << LED1)); /* ORI R16, (1 << LED1) */
   data[11] = assemble(OUT, PORTA, R16);       /* OUT PORTB, R16 */
   data[12] = assemble(JMP, main_loop, 0x00);  /* JMP main_loop */

   /********************************************************************************
   * setup: Sets the pin connected to LED1 to outputand enables the internal
   *        pull - up resistor for the pin connected to BUTTON1 so that the input
   *        signal always reads as high(1) or low(0).
   ********************************************************************************/
   data[13] = assemble(LDI, R16, (1 << LED1));    /* LDI R16, (1 << LED1) */
   data[14] = assemble(OUT, DDRA, R16);           /* OUT DDRB, R16 */
   data[15] = assemble(LDI, R16, (1 << BUTTON1)); /* LDI R16, (1 << BUTTON1) */
   data[16] = assemble(OUT, PORTA, R16);          /* OUT PORTB, R16 */
   data[17] = assemble(RET, 0x00, 0x00);          /* RET */

   program_memory_initialized = true;
   return;
}

/********************************************************************************
* program_memory_read: Returns the instruction at specified address. If an
*                      invalid address is specified (should be impossible as
*                      long as the program memory address width isn't increased)
*                      no operation (0x00) is returned.
*
*                      - address: Address to instruction in program memory.
********************************************************************************/
uint64_t program_memory_read(const uint16_t address)
{
   if (address < PROGRAM_MEMORY_ADDRESS_WIDTH)
   {
      return data[address];
   }
   else
   {
      return 0x00;
   }
}

/********************************************************************************
* assemble: Returns instruction assembled to machine code.
********************************************************************************/
static inline uint64_t assemble(const uint16_t op_code,
                                const uint16_t op1,
                                const uint32_t op2)
{
   const uint32_t instruction = ((uint64_t)(op_code) << 48) | ((uint64_t)(op1) << 32) | op2;
   return instruction;
}