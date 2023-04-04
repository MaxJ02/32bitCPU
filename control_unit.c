/********************************************************************************
* control_unit.c: Contains static variables and function definitions for 
*                 implementation of an 8-bit control unit.
********************************************************************************/
#include "control_unit.h"

/* Static variables: */
static uint64_t ir; /* Instruction register, stores next instruction to execute. */
static uint16_t pc;  /* Program counter, stores address to next instruction to fetch. */
static uint16_t mar; /* Memory address register, stores address for current instruction. */
static uint8_t sr;  /* Status register, stores status bits ISNZVC. */

static uint16_t op_code; /* Stores OP-code, for example LDI, OUT, JMP etc. */
static uint16_t op1;     /* Stores first operand, most often a destination. */
static uint32_t op2;     /* Stores second operand, most often a value or read address. */

static enum cpu_state state;                    /* Stores current state. */
static uint32_t reg[CPU_REGISTER_ADDRESS_WIDTH]; /* CPU-registers R0 - R31. */

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

   for (uint32_t i = 0; i < CPU_REGISTER_ADDRESS_WIDTH; ++i)
   {
      reg[i] = 0x00;
   }

   
   data_memory_reset();
   stack_reset();
   program_memory_write();
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
               data_memory_write(op1, reg[op2]); /* Writes to address 0 - 255. */
               break;
            }
            case IN: /* IN R16, PINB => op_code = IN, op1 = R16, op2 = PINB */
            {
               reg[op1] = data_memory_read(op2); /* Reads from address 0 - 255. */
               break;
            }
            case STS: /* STS counter, R16 => op_code = STS, op1 = counter, op2 = R16 */
            {
               data_memory_write(op1 + 256, reg[op2]); /* Offset 256 => writes to address 256 - 511. */
               break;
            }
            case LDS: /* LDS R16, counter => op_code = LDS, op1 = R16, op2 = counter */
            {
               reg[op1] = data_memory_read(op2 + 256); /* Offset 256 => reads from address 256 - 511. */
               break;
            }
            case CLR: /* CLR R16 => op_code = CLR, op1 = R16 */
            {
               reg[op1] = 0x00; /* Clears content of CPU register. */
               break;
            }
            case ORI: /* ORI R16, 0x01 => op_code = ORI, op1 = R16, op2 = 0x01 */
            {
               reg[op1] = alu(OR, reg[op1], op2, &sr);
               break;
            }
            case ANDI: /* ANDI R16, 0x01 => op_code = ANDI, op1 = R16, op2 = 0x01 */
            {
               reg[op1] = alu(AND, reg[op1], op2, &sr);
               break;
            }
            case XORI: /* XORI R16, 0x01 => op_code = XORI, op1 = R16, op2 = 0x01 */
            {
               reg[op1] = alu(XOR, reg[op1], op2, &sr);
               break;
            }
            case OR:  /* OR R16, R17 => op_code = OR, op1 = R16, op2 = R17 */
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
            case ADDI: /* ADDI R16, 100 => op_code = ADDI, op1 = R16, op2 = 100 */
            {
               reg[op1] = alu(ADD, reg[op1], op2, &sr);
               break;
            }
            case SUBI: /* SUBI R16, 50 => op_code = SUBI, op1 = R16, op2 = 50 */
            {
               reg[op1] = alu(SUB, reg[op1], op2, &sr);
               break;
            }
            case ADD: /* ADD R16, R17 => op_code = ADD, op1 = R16, op2 = R17 */
            {
               reg[op1] = alu(ADD, reg[op1], reg[op2], &sr);
               break;
            }
            case SUB: /* SUB R16, R17 => op_code = SUB, op1 = R16, op2 = R17 */
            {
               reg[op1] = alu(SUB, reg[op1], reg[op2], &sr);
               break;
            }
            case INC: /* INC R16 => op_code = INC, op1 = R16 */
            {
               reg[op1] = alu(ADD, reg[op1], 1, &sr);
               break;
            }
            case DEC: /* DEC R16 => op_code = DEC, op1 = R16 */
            {
               reg[op1] = alu(SUB, reg[op1], 1, &sr);
               break;
            }
            case CPI: /* CPI R16, 5 => op_code = CPI, op1 = R16, op2 = 5 */
            {
               (void)alu(SUB, reg[op1], op2, &sr);
               break;
            }
            case CP: /* CP R16, R17 => op_code = CP, op1 = R16, op2 = R17 */
            {
               (void)alu(SUB, reg[op1], reg[op2], &sr);
               break;
            }
            case CALL: /* CALL setup => op_code = CALL, op1 = setup */
            {
               stack_push(pc); /* Stores the return address on the stack. */
               pc = op1;       /* Jumps to specified address. */
               break;
            }
            case RET: /* RET => op_code = RET */
            {
               /* Pops the return address from the stack and assigns to the program counter. */
               pc = stack_pop();
               break;
            }
            case PUSH: /* PUSH R24 => op_code = PUSH, op1 = R24 */
            {
               /* Pushes content of a CPU register to the stack. */
               stack_push(reg[op1]);
               break;
            }
            case POP: /* POP R24 => op_code = POP, op1 = R24 */
            {
               /* Pops content from the stack to a CPU register. */
               reg[op1] = stack_pop();
               break;
            }
            case JMP: /* JMP 0x05 => op_code = JMP, op1 = 0x05 */
            {
               pc = op1; /* Jumps to specified address. */
               break;
            }
            case BREQ: /* BREQ 0x20 => op_code = BREQ, op1 = 0x20 */
            {
               if (read(sr, Z)) pc = op1; /* Equal to 0 => Z = 1. */
               break;
            }
            case BRNE: /* BRNE 0x20 => op_code = BRNE, op1 = 0x20 */
            {
               if (!read(sr, Z)) pc = op1; /* Not equal to 0 => Z = 0. */
               break;
            }
            case BRGE: /* BRGE 0x20 => op_code = BRGE, op1 = 0x20 */
            {
               if (!read(sr, S)) pc = op1; // R >= 0 => S = 0.
               break;
            }
            case BRGT: /* BRGT 0x20 => op_code = BRLE, op1 = 0x20 */
            {
               if (!read(sr, S) && !read(sr, Z)) pc = op1; /* R > 0 if S = 0 and Z = 0. */
               break;
            }
            case BRLE: /* BRLE 0x20 => op_code = BRLE, op1 = 0x20 */
            {
               if (read(sr, S) || read(sr, Z)) pc = op1; /* R <= 0 => S = 1 or Z = 1. */
               break;
            }
            case BRLT: /* BRLT 0x20 => op_code = BRLT, op1 = 0x20 */
            {
               if (read(sr, S)) pc = op1; /* R < 0 if S = 1. */
               break;
            }
            case LSL: /* LSL R16 => op_code = LSL, op1 = R16 */
            {
               reg[op1] = reg[op1] << 1; /* Shifts content of CPU register one step to the left. */
               break;
            }
            case LSR: /* LSR R1R => op_code = LSR, op1 = R16 */
            {
               reg[op1] = reg[op1] >> 1; /* Shifts content of CPU register one step to the right. */
               break;
            }
            default:
            {
               control_unit_reset(); /* System reset if error occurs. */
               break;
            }
         }

         state = CPU_STATE_FETCH; /* Fetches next instruction during next clock cycle. */
         break;
      }
      default: /* System reset if error occurs. */
      {
         control_unit_reset();
         break;
      }
   }
   return;
}

/********************************************************************************
* control_unit_run_next_state: Runs next CPU instruction cycle, i.e. fetches
*                              a new instruction from program memory, decodes
*                              and executes it.
********************************************************************************/
void control_unit_run_next_instruction_cycle(void)
{
   do
   {
      control_unit_run_next_state();
   } while (state != CPU_STATE_EXECUTE);
   return;
}



