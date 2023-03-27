#include "program_memory.h"

#define main 8
#define main_loop 9
#define led1_toggle 10
#define led1_off        13 
#define led1_on         19 
#define setup           25 
#define ISR_PCINT0      33 
#define ISR_PCINT0_end  37 
#define end             38 

#define LED1 PORTB0 
#define BUTTON1 PORTB5
#define led_enable 100

static inline uint32_t assemble(const uint8_t op_code, const uint8_t op1, const uint8_t op2);

static uint32_t data[PROGRAM_MEMORY_ADDRESS_WIDTH];

void program_memory_write(void)
{
	static bool program_memory_initialized = false;
	if (program_memory_initialized) return;


	data[0] = assemble(JMP, main, 0x00);
	data[1] = assemble(NOP, 0x00, 0x00);

	data[2] = assemble(JMP, ISR_PCINT0, 0x00);
	data[3] = assemble(NOP, 0x00, 0x00);
	data[4] = assemble(NOP, 0x00, 0x00);
	data[5] = assemble(NOP, 0x00, 0x00);
	data[6] = assemble(NOP, 0x00, 0x00);
	data[7] = assemble(NOP, 0x00, 0x00);

	data[8] = assemble(CALL, setup, 0x00);
	data[9] = assemble(JMP, main_loop, 0x00);

	data[10] = assemble(LDS, R16, led1_enabled);
	data[11] = assemble(CPI, R16, 0x00);
	data[12] = assemble(BREQ, led1_on, 0x00);

	data[13] = assemble(IN, R16, PORTB);
	data[14] = assemble(ANDI, R16, ~(1 << LED1));
	data[15] = assemble(OUT, PORTB, R16);
	data[16] = assemble(LDI, R16, 0x00);
	data[17] = assemble(STS, led1_enabled, R16);
	data[18] = assemble(RET, 0x00, 0x00);

	data[19] = assemble(IN, R16, PORTB);
	data[20] = assemble(ORI, R16, (1 << LED1));
	data[21] = assemble(OUT, PORTB, R16);
	data[22] = assemble(LDI, R16, 0x01);
	data[23] = assemble(STS, led1_enabled, R16);
	data[24] = assemble(RET, 0x00, 0x00);

	data[25] = assemble(LDI, R16, (1 << LED1));
	data[26] = assemble(OUT, DDRB, R16);
	data[27] = assemble(LDI, R17, (1 << BUTTON1));
	data[28] = assemble(OUT, PORTB, R17);
	data[29] = assemble(SEI, 0x00, 0x00);
	data[30] = assemble(STS, PCICR, R16);
	data[31] = assemble(STS, PCMSK0, R17);
	data[32] = assemble(RET, 0x00, 0x00);

	data[33] = assemble(IN, R16, PINB);
	data[34] = assemble(ANDI, R16, (1 << BUTTON1));
	data[35] = assemble(BREQ, ISR_PCINT0_end, 0x00);
	data[36] = assemble(CALL, led1_toggle, 0x00);
	data[37] = assemble(RETI, 0x00, 0x00);

	program_memory_initialized = true;
	return;
}

uint32_t program_memory_read(const uint8_t address)
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

const char* program_memory_subroutine_name(const uint8_t address)
{
	if (address >= RESET_vect && address < PCINT0_vect)    return "RESET_vect";
	else if (address >= PCINT0_vect && address < main)     return "PCINT0_vect";
	else if (address >= main && address < led1_toggle)     return "main";
	else if (address >= led1_toggle && address < led1_off) return "led1_toggle";
	else if (address >= led1_off && address < led1_on)     return "led1_off";
	else if (address >= led1_on && address < setup)        return "led1_on";
	else if (address >= setup && address < ISR_PCINT0)     return "setup";
	else if (address >= ISR_PCINT0 && address < end)       return "ISR_PCINT0";
	else                                                   return "Unknown";
}
static inline uint32_t assemble(const uint8_t op_code,
	const uint8_t op1,
	const uint8_t op2)
{
	const uint32_t instruction = (op_code << 16) | (op1 << 8) | op2;
	return instruction;
}