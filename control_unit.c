#include "control_unit.h"

/* Static functions: */
static void monitor_interrupts(void);
static void check_for_irq(void);
static void generate_interrupt(const uint16_t interrupt_vector);
static inline void monitor_pcint(void);
static void control_unit_io_reset(void);
static void control_unit_io_update(void);
static inline void return_from_interrupt(void);


/* Static variables: */
static uint64_t ir; /* Instruction register, stores next instruction to execute. */
static uint16_t pc;  /* Program counter, stores address to next instruction to fetch. */
static uint32_t mar;

static uint8_t sr;  /* Status register, stores status bits ISNZVC. */

static volatile uint16_t op_code; /* Stores OP-code, for example LDI, OUT, JMP etc. */
static uint16_t op1;     /* Stores first operand, most often a destination. */
static uint32_t op2;     /* Stores second operand, most often a value or read address. */

static enum cpu_state state;                    /* Stores current state. */
static uint32_t reg[CPU_REGISTER_ADDRESS_WIDTH]; /* CPU-registers R0 - R31. */

static uint32_t pina_previous; /* Stores previous input values of PINB (for monitoring). */


/********************************************************************************
* control_unit_reset: Resets control unit registers and corresponding program.
********************************************************************************/
void control_unit_reset(void)
{
    ir = 0x00;
    pc = 0x00;
    mar = 0x00;
    sr = 0x00;

    op_code = 0x00;
    op1 = 0x00;
    op2 = 0x00;

    state = CPU_STATE_FETCH;

    pina_previous = 0x00;

    for (uint32_t i = 0; i < CPU_REGISTER_ADDRESS_WIDTH; ++i)
    {
        reg[i] = 0x00;
    }


    data_memory_reset();
    stack_reset();
    program_memory_write();
	control_unit_io_reset();
    return;
}

/********************************************************************************
* control_unit_run_next_state: Runs next state in the CPU instruction cycle:
********************************************************************************/
void control_unit_run_next_state(void)
{
    switch (state)
    {
    case CPU_STATE_FETCH:
    {
        ir = program_memory_read(pc); /* Fetches next instruction. */
        mar = pc;                     /* Stores address of current instruction. */
        pc++;                         /* Program counter points to next instruction. */
        state = CPU_STATE_DECODE;     /* Decodes the instruction during next clock cycle. */
        break;
    }
    case CPU_STATE_DECODE:
    {
        op_code = ir >> 48;           /* Bit 23 downto 16 consists of the OP code. */
        op1 = ir >> 32;                /* Bit 15 downto 8 consists of the first operand. */
        op2 = ir;                     /* Bit 7 downto 0 consists of the second operand. */
        state = CPU_STATE_EXECUTE;    /* Executes the instruction during next clock cycle. */
        break;
    }
    case CPU_STATE_EXECUTE:
    {
        switch (op_code) /* Checks the OP code.*/
        {
        case NOP: /* NOP => do nothing. */
        {
            break;
        }
        case LDI: /* Loads constant into specified CPU register. */
        {
            reg[op1] = op2;
            break;
        }
        case MOV: /* Copies value to specified CPU register. */
        {
            reg[op1] = reg[op2];
            break;
        }
        case OUT: /* Writes value to I/O location (address 0 - 255) in data memory. */
        {
            if (op1 == PINA)
			{
				const uint32_t data = data_memory_read(PORTA);
				data_memory_write(PORTA, data ^ reg[op2]);
			}
			else
			{
				data_memory_write(op1, reg[op2]);
			}
            break;
        }
        case IN: /* Reads value from I/O location (address 0 - 255) in data memory. */
        {
            reg[op1] = data_memory_read(op2);
            break;
        }
        case STS: /* Stores value to data memory (address 256 - 511, hence an offset of 256). */
        {
            data_memory_write(op1, reg[op2]);
            break;
        }
        case LDS: /* Loads value from data memory (address 256 - 511, hence an offset of 256). */
        {
            reg[op1] = data_memory_read(op2);
            break;
        }
        case CLR: /* Clears content of CPU register. */
        {
            reg[op1] = 0x00;
            break;
        }
        case ORI: /* Performs bitwise OR with a constant. */
        {
            reg[op1] = alu(OR, reg[op1], op2, &sr);
            break;
        }
        case ANDI: /* Performs bitwise AND with a constant. */
        {
            reg[op1] = alu(AND, reg[op1], op2, &sr);
            break;
        }
        case XORI: /* Performs bitwise XOR with a constant. */
        {
            reg[op1] = alu(XOR, reg[op1], op2, &sr);
            break;
        }
        case OR: /* Performs bitwise OR with content in CPU register. */
        {
            reg[op1] = alu(OR, reg[op1], reg[op2], &sr);
            break;
        }
        case AND: /* Performs bitwise AND with content in CPU register. */
        {
            reg[op1] = alu(AND, reg[op1], reg[op2], &sr);
            break;
        }
        case XOR: /* Performs bitwise AND with content in CPU register. */
        {
            reg[op1] = alu(XOR, reg[op1], reg[op2], &sr);
            break;
        }
        case ADDI: /* Performs addition with a constant. */
        {
            reg[op1] = alu(ADD, reg[op1], op2, &sr);
            break;
        }
        case SUBI: /* Performs subtraction with a constant. */
        {
            reg[op1] = alu(SUB, reg[op1], op2, &sr);
            break;
        }
        case ADD: /* Performs addition with a CPU register. */
        {
            reg[op1] = alu(ADD, reg[op1], reg[op2], &sr);
            break;
        }
        case SUB: /* Performs subtraction with a CPU register. */
        {
            reg[op1] = alu(SUB, reg[op1], reg[op2], &sr);
            break;
        }
        case INC: /* Increments content of a CPU register. */
        {
            reg[op1] = alu(ADD, reg[op1], 1, &sr);
            break;
        }
        case DEC: /* Decrements content of a CPU register. */
        {
            reg[op1] = alu(SUB, reg[op1], 1, &sr);
            break;
        }
        case CPI: /* Compares content between CPU register with a constant. */
        {
            (void)alu(SUB, reg[op1], op2, &sr); /* Return value is not stored. */
            break;
        }
        case CP: /* Compares content between two CPU registers. */
        {
            (void)alu(SUB, reg[op1], reg[op2], &sr); /* Return value is not stored. */
            break;
        }
        case JMP: /* Jumps to specified address. */
        {
            pc = op1;
            break;
        }
        case BREQ: /* Branches to specified address i Z flag is set. */
        {
            if (read(sr, Z)) pc = op1;
            break;
        }
        case BRNE: /* Branches to specified address if Z flag is cleared. */
        {
            if (!read(sr, Z)) pc = op1;
            break;
        }
        case BRGE: /* Branches to specified address if S flag is cleared. */
        {
            if (!read(sr, S)) pc = op1;
            break;
        }
        case BRGT: /* Branches to specified address if both S and Z flags are cleared. */
        {
            if (!read(sr, S) && !read(sr, Z)) pc = op1;
            break;
        }
        case BRLE: /* Branches to specified address if S or Z flag is set. */
        {
            if (read(sr, S) || read(sr, Z)) pc = op1;
            break;
        }
        case BRLT: /* Branches to specified address if S flag is set. */
        {
            if (read(sr, S)) pc = op1;
            break;
        }
        case CALL: /* Stores the return address on the stack and jumps to specified address. */
        {
            stack_push(pc);
            pc = op1;
            break;
        }
        case RET: /* Jumps to return address stored on the stack. */
        {
            pc = stack_pop();
            break;
        }
        case RETI: /* Pops the return address from the stack and sets the global interrupt flag. */
        {
            pc = stack_pop();
            set(sr, I);
            break;
        }
        case PUSH: /* Stores content of specified CPU register on the stack. */
        {
            stack_push(reg[op1]);
            break;
        }
        case POP: /* Loads value from the stack to a CPU-register. */
        {
            reg[op1] = stack_pop();
            break;
        }
        case LSL: /* Shifts content of CPU register on step to the left. */
        {
            reg[op1] = reg[op1] << 1;
            break;
        }
        case LSR: /* Shifts content of CPU register on step to the right. */
        {
            reg[op1] = reg[op1] >> 1;
            break;
        }
        case SEI: /* Sets the global interrupt flag in the status register. */
        {
            set(sr, I);
            break;
        }
        case CLI: /* Clears the global interrupt flag in the status register. */
        {
            clr(sr, I);
            break;
        }
        case STIO:  /* Stores value to referenced I/O location (no offset). */
        {
            const uint16_t address = reg[op1] | (reg[op1 + 1] << 8);
            data_memory_write(address, reg[op2]);
            break;
        }
        case LDIO: /* Loads value from referenced I/O location (no offset). */
        {
            const uint16_t address = reg[op2] | (reg[op2 + 1] << 8);
            reg[op1] = data_memory_read(address);
            break;
        }
        case ST: /* Stores value to referenced data location (offset = 256). */
        {
            const uint16_t address = reg[op1] | (reg[op1 + 1] << 8);
            data_memory_write(address, reg[op2]);
            break;
        }
        case LD: /* Loads value from referenced data location (offset = 256). */
        {
            const uint16_t address = reg[op2] | (reg[op2 + 1] << 8);
            reg[op1] = data_memory_read(address);
            break;
        }
        default:
        {
            control_unit_reset(); /* System reset if error occurs. */
            break;
        }
        }

        state = CPU_STATE_FETCH;    /* Fetches next instruction during next clock cycle. */
        check_for_irq();            /* Checks for interrupt request after each execute cycle. */
        break;
    }
    default:                       /* System reset if error occurs. */
    {
        control_unit_reset();
        break;
    }
    }

    control_unit_io_update();
    monitor_interrupts();            /* Monitors interrupts each clock cycle. */
    return;
}

static void control_unit_io_reset(void)
{
	
	DDRB = 0;
	DDRC = 0;
	DDRD = 0;
	
    PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	return;
}
static void control_unit_io_update(void)
{
	const uint32_t ddra = data_memory_read(DDRA);
	const uint32_t porta = data_memory_read(PORTA);
	const uint32_t pina = PIND | ((uint32_t)(PINB) << 8) | ((uint32_t)(PINC) << 16);
	
	data_memory_write(PINA, pina);
	
	DDRB = (uint8_t)(ddra >> 8);
	DDRC = (uint8_t)(ddra >> 16);
	DDRD = (uint8_t)(ddra);
	
	PORTB = (uint8_t)(porta >> 8);
	PORTC = (uint8_t)(porta >> 16);
	PORTD = (uint8_t)(porta);
	return;
	
}

static inline void cpu_registers_clear(void)
{
	for (uint32_t* i = reg; i < reg + CPU_REGISTER_ADDRESS_WIDTH; ++i)
	{
		*i = 0x00;
	}
	return;
}

static void monitor_interrupts(void)
{
	monitor_pcint();
	return;
}

/********************************************************************************
* check_for_irq: Checks for interrupt requests and generates an interrupt if
*                the I flag in status register i set, a specific interrupt flag,
*                such as PCIF0 in PCIFR, is set and the corresponding interrupt
*                enable bit, such as PCIE0 in PCICR, i set. Before an interrupt
*                is generated, the corresponding flag bit is cleared to
*                terminate the interrupt request. Otherwise the interrupt
*                will be generated again and again). A jump is made to the
*                corresponding interrupt vector, such as PCINT0_vect.
********************************************************************************/

static void check_for_irq(void)
{
    if (read(sr, I))
    {
        const uint32_t pcifr = data_memory_read(IFR);
        const uint32_t pcicr = data_memory_read(ICR);

        if (read(pcifr, PCIFA) && read(pcicr, PCIEA))
        {
            data_memory_clear_bit(IFR, PCIFA);
            generate_interrupt(PCINT_vect);
        }
    }
    return;
}

/********************************************************************************
* generate_interrupt: Generates and interrupt by jumping to specified interrupt
*                     vector. Before the jump, the return address is stored
*                     on the stack and the I-flag in the status register is
*                     cleared so that no new interrupts are generated while
*                     the current interrupt is executed.
*
*                     - interrupt_vector: Jump address for generating interrupt.
********************************************************************************/
static void generate_interrupt(const uint16_t interrupt_vector)
{
    stack_push(pc);
    clr(sr, I);
    pc = interrupt_vector;
    return;
}

/********************************************************************************
* monitor_pcint0: Monitors pin change interrupts on I/O port B. All pins where
*                 pin change monitoring is enabled (corresponding mask bit in
*                 PCMSK0 i set) is monitored by comparing current input signal
*                 with the previous one. If they don't match, the corresponding
*                 interrupt flag PCIF0 in the PCIFR register i set to generate
*                 an interrupt request (IRQ).
********************************************************************************/
static inline void monitor_pcint(void)
{
    const uint32_t pina_current = data_memory_read(PINA);
    const uint32_t pcmsk = data_memory_read(PCMSKA);

    for (uint32_t i = 0; i < CPU_REGISTER_DATA_WIDTH; ++i)
    {
        if (read(pcmsk, i))
        {
            if (read(pina_current, i) != read(pina_previous, i))
            {
                data_memory_set_bit(IFR, PCIF0);
                break;
            }
        }
    }

    pina_previous = pina_current;
    return;
}

static inline void return_from_interrupt(void)
{
	pc = stack_pop();
	set(sr, I);
	return;
}