#include "control_unit.h"

static void monitor_interrupts(void);
static void pci_check_event(
    const uint8_t pin_reg,
    const uint8_t mask_reg,
    const uint8_t pcie_bit,
    const uint8_t flag_bit,
    uint8_t* pin_reg_last_value);

static void pci_check_for_interrupt_requests(void);
static void generate_interrupt(const uint8_t interrupt_request);
static void return_from_interrupt(void);

/// STATIC VARIABLES
static uint32_t ir; 
static uint8_t pc;  
static uint8_t mar; 
static uint8_t sr;  

static uint8_t op_code;
static uint8_t op1;
static uint8_t op2;

static enum cpu_state state;
static uint8_t pinb_last_value;
static uint8_t pinc_last_value;
static uint8_t pind_last_value;

static uint8_t reg[CPU_REGISTER_ADDRESS_WIDTH];

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
    
    for (uint8_t i = 0; i < CPU_REGISTER_ADDRESS_WIDTH; ++i)
    {
        reg[i] = 0x00;
    }
    pinb_last_value = 0x00;
    pinc_last_value = 0x00;
    pind_last_value = 0x00;

    data_memory_reset = 0x00;
    stack_reset();
    program_memory_write();
    return;
}

void control_unit_run_next_state(void)
{
    switch (state)
    {
        case CPU_STATE_FETCH
        {
            ir = program_memory_write(pc)
            mar = pc;
            pc++;
            state = CPU_STATE_DECODE;
            break;
        }
            case CPU_STATE_DECODE
        {
            op_code = ir >> 16;
            op1 = ir >> 8;
            op2 = ir;
            state = CPU_STATE_EXECUTE;
            break;
        }
            case CPU_STATE_EXECUTE
        {
                     switch (op_code) /* Checks the OP code.*/
         {
            case NOP: /* NOP => do nothing. */
            {
               break;
            }
            case LDI: /* LDI R16, 0x01 => op_code = LDI, op1 = R16, op2 = 0x01 */
            {
               reg[op1] = op2;
               break;
            }
            case MOV: /* MOV R17, R16 => op_code = MOV, op1 = R17, op2 = R16 */
            {
               reg[op1] = reg[op2];
               break;
            }
            case OUT: /* OUT DDRB, R16 => op_code = OUT, op1 = DDRB, op2 = R16 */
            {
               data_memory_write(op1, reg[op2]);
               break;
            }
            case IN: /* IN R16, PINB => op_code = IN, op1 = R16, op2 = PINB */
            {
               reg[op1] = data_memory_read(op2);
               break;
            }
            case STS: /* STS counter, R16 => op_code = STS, op1 = counter, op2 = R16 */
            {
               data_memory_write(op1 + 256, reg[op2]);
               break;
            }
            case LDS: /* LDS R16, counter => op_code = LDS, op1 = R16, op2 = counter*/
            {
               reg[op1] = data_memory_read(op2 + 256);
               break;
            }
            case ORI: /* ORI R16, 0x01 => op_code = ORI, op1 = R16, op2 = 0x01 */
            {
               reg[op1] = alu(OR, reg[op1], op2, &sr);
               break;
            }
            case ANDI: /* ANDI R17, 0x20 => op_code = ANDI, op1 = R17, op2 = 0x20 */
            {
               reg[op1] = alu(AND, reg[op1], op2, &sr);
               break;
            }
            case XORI: /* XORI R18, 0x05 => op_code = XORI, op1 = R18, op2 = 0x05 */
            {
               reg[op1] = alu(XOR, reg[op1], op2, &sr);
               break;
            }
            case OR: /* OR R16, R17 => op_code = OR, op1 = R16, op2 = R17 */
            {
               reg[op1] = alu(OR, reg[op1], reg[op2], &sr);
               break;
            }
            case AND: /* AND R16, R17 => op_code = AND, op1 = R16, op2 = R17 */
            {
               reg[op1] = alu(AND, reg[op1], reg[op2], &sr);
               break;
            }
            case XOR: /* XOR R16, R17 => op_code = XOR, op1 = R16, op2 = R17 */
            {
               reg[op1] = alu(XOR, reg[op1], reg[op2], &sr);
               break;
            }
            case ADDI: /* ADDI R16, 0x10 => op_code = ADDI, op1 = R16, op2 = 0x10 */
            {
               reg[op1] = alu(ADD, reg[op1], op2, &sr);
               break;
            }
            case SUBI: /* SUBI R17, 0x05 => op_code = SUBI, op1 = R17, op2 = 0x05 */
            {
               reg[op1] = alu(SUB, reg[op1], op2, &sr);
               break;
            }
            case ADD: /* ADD R16, R17 => op_code = ADD, op1 = R16, op2 = R17 */
            {
               reg[op1] = alu(ADD, reg[op1], reg[op2], &sr);
               break;
            }
            case SUB: /* SUB R17, R18 => op_code = SUB, op1 = R17, op2 = R18 */
            {
               reg[op1] = alu(SUB, reg[op1], reg[op2], &sr);
               break;
            }
            case INC: /* INC R16 => op_code = INC, op1 = R16 */
            {
               reg[op1] = alu(ADD, reg[op1], 0x01, &sr);
               break;
            }
            case DEC: /* DEC R17 => op_code = INC, op1 = R17 */
            {
               reg[op1] = alu(SUB, reg[op1], 0x01, &sr);
               break;
            }
            case CPI: /* CPI R16, 0x01 => op_code = CPI, op1 = R16, op2 = 0x01 */
            {
               (void)alu(SUB, reg[op1], op2, &sr);
               break;
            }
            case CP: /* CPI R16, R17 => op_code = CP, op1 = R16, op2 = R17 */
            {
               (void)alu(SUB, reg[op1], reg[op2], &sr);
               break;
            }
            case JMP: /* JMP 0x05 => op_code = JMP, op1 = 0x05 */
            {
               pc = op1;
               break;
            }
            case BREQ: /* BREQ 0x10 => op_code = BREQ, op1 = 0x10 */
            {
               if (read(sr, Z)) pc = op1;
               break;
            }
            case BRNE: /* BRNE 0x20 => op_code = BRNE, op1 = 0x20 */
            {
               if (!read(sr, Z)) pc = op1;
               break;
            }
            case BRGE: /* BRGE 0x30 => op_code = BRGE, op1 = 0x30 */
            {
               if (!read(sr, S)) pc = op1;
               break;
            }
            case BRGT: /* BRGT 0x40 => op_code = BRGT, op1 = 0x40 */
            {
               if (!read(sr, S) && !read(sr, Z)) pc = op1;
               break;
            }
            case BRLE: /* BRLE 0x50 => op_code = BRLE, op1 = 0x50 */
            {
               if (read(sr, S) || (read(sr, Z))) pc = op1;
               break;
            }
            case BRLT: /* BRLT 0x60 => op_code = BRLT, op1 = 0x60 */
            {
               if (read(sr, S)) pc = op1;
               break;
            }
            case CALL: /* CALL 0x10 => op_code = CALL, op1 = 0x10 */
            {
               stack_push(pc);
               pc = op1;
               break;
            }
            case RET: /* RET => op_code = RET */
            {
               pc = stack_pop();
               break;
            }
            case RETI: /* RETI => op_code = RETI */
            {
               return_from_interrupt();
               break;
            }
            case PUSH: /* PUSH R16 => op_code = PUSH, op1 = R16 */
            {
               stack_push(reg[R16]);
               break;
            }
            case POP: /* POP R16 => op_code = POP, op1 = R16 */
            {
               reg[op1] = stack_pop();
               break;
            }
            case SEI: /* SEI => op_code = SEI */
            {
               set(sr, I);
               break;
            }
            case CLI: /* CLI => op_code = CLI */
            {
               clr(sr, I);
               break;
            }
            case SKY:
            {
                break;
            }
            case NCE:
            {
                //todo drivrutiner till 7 seg. så att den ger ut 69 på displayen
                break;
            }
            default:  /* System reset if error occurs. */
            {
               control_unit_reset();
               break;
            }
        }
        pci_break_for_interrupt_requests();
        state = CPU_STATE_FETCH;
        break;
        }
        default:
        {
            control_unit_reset;
            break;
        }
    }
    monitor_interrupts();
    return;
}

void control_unit_run_next_instruction_cycle(void)
{
    do
    {
        control_unit_run_next_state();
    } while (state != CPU_STATE_EXECUTE);
    return;
}

void control_unit_print(void)
{
    printf("--------------------------------------------------------------------------------\n");
    printf("Current subroutine:\t\t\t\t%s\n", program_memory_subroutine_name(mar));
    printf("Current instruction:\t\t\t\t%s\n", cpu_instruction_name(op_code));
    printf("Current state:\t\t\t\t\t%s\n", cpu_state_name(state));

    printf("Program counter:\t\t\t\t%hu\n", pc);

    printf("Instruction register:\t\t\t\t%s ", get_binary((ir >> 16) & 0xFF, 8));
    printf("%s ", get_binary((ir >> 8) & 0xFF, 8));
    printf("%s\n", get_binary(ir & 0xFF, 8));

    printf("Status register (ISNZVC):\t\t\t%s\n\n", get_binary(sr, 6));

    printf("Content in CPU register R16:\t\t\t%s\n", get_binary(reg[R16], 8));
    printf("Content in CPU register R17:\t\t\t%s\n", get_binary(reg[R17], 8));
    printf("Content in CPU register R18:\t\t\t%s\n", get_binary(reg[R18], 8));
    printf("Content in CPU register R24:\t\t\t%s\n\n", get_binary(reg[R24], 8));

    printf("Content in data direction register DDRB:\t%s\n", get_binary(data_memory_read(DDRB), 8));
    printf("Content in data register PORTB:\t\t\t%s\n", get_binary(data_memory_read(PORTB), 8));
    printf("Content in pin input register PINB:\t\t%s\n", get_binary(data_memory_read(PINB), 8));

    printf("--------------------------------------------------------------------------------\n\n");
    return;
}
static void monitor_interrupts(void)
{
    pci_check_event(PINB, PCMSK0, PCIE0, PCIF0, &pinb_last_value);
    pci_check_event(PINC, PCMSK1, PCIE1, PCIF1, &pinc_last_value);
    pci_check_event(PIND, PCMSK2, PCIE2, PCIF2, &pind_last_value);
    return;
}
static void pci_check_event(const uint8_t pin_reg,
    const uint8_t mask_reg,
    const uint8_t interrupt_enable_bit,
    const uint8_t flag_bit,
    uint8_t* pin_reg_last_value)
{
    const uint8_t pin_reg_new_value = data_memory_read(pin_reg);
    const uint8_t pcicr = data_memory_read(PCICR + 256);
    const uint8_t pcmsk = data_memory_read(mask_reg + 256);

    for (uint8_t i = 0; i < IO_REGISTER_DATA_WIDTH; ++i)
    {
        if (read(pin_reg_new_value, i) != read(*pin_reg_last_value, i))
        {
            if (read(pcicr, interrupt_enable_bit) && read(pcmsk, i))
            {
                uint8_t pcifr = data_memory_read(PCIFR + 256);
                set(pcifr, flag_bit);
                data_memory_write(PCIFR + 256, pcifr);
                break;
            }
        }
    }

    *pin_reg_last_value = pin_reg_new_value;
    return;
}

static void pci_check_for_interrupt_requests(void)
{
    uint8_t pcifr = data_memory_read(PCIFR + 256);

    for (uint8_t i = PCIF0; i < PCIF2; ++i)
    {
        if (read(pcifr, i) && read(sr, I))
        {
            if (i == PCIF0)
            {
                generate_interrupt(PCINT0_vect);
            }
            else if (i == PCIF1)
            {
                generate_interrupt(PCINT1_vect);
            }
            else if (i == PCIF2)
            {
                generate_interrupt(PCINT2_vect);
            }

            clr(pcifr, i);
            data_memory_write(PCIFR + 256, pcifr);
        }
    }
    return;
}
static void generate_interrupt(const uint8_t interrupt_vector)
{
    stack_push(ir >> 16);
    stack_push(ir >> 8);
    stack_push(ir);
    stack_push(pc);
    stack_push(mar);
    stack_push(sr);

    stack_push(op_code);
    stack_push(op1);
    stack_push(op2);

    stack_push(state);
    stack_push(pinb_last_value);
    stack_push(pinc_last_value);
    stack_push(pind_last_value);

    for (uint32_t i = 0; i < CPU_REGISTER_ADDRESS_WIDTH; ++i)
    {
        stack_push(reg[i]);
    }

    pc = interrupt_vector;
    state = CPU_STATE_FETCH;
    clr(sr, I);
    return;
}
static void return_from_interrupt(void)
{
    for (uint8_t i = 0; i < CPU_REGISTER_ADDRESS_WIDTH; ++i)
    {
        reg[CPU_REGISTER_ADDRESS_WIDTH - 1 - i] = stack_pop();
    }

    pind_last_value = stack_pop();
    pinc_last_value = stack_pop();
    pinb_last_value = stack_pop();
    state = stack_pop();

    op2 = stack_pop();
    op1 = stack_pop();
    op_code = stack_pop();

    sr = stack_pop();
    mar = stack_pop();
    pc = stack_pop();

    ir = stack_pop();
    ir |= stack_pop() << 8;
    ir |= stack_pop() << 16;
    return;
}