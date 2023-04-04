#ifndef ALU_H_
#define ALU_H_

/* Include directives: */
#include "cpu.h"

/********************************************************************************
* alu: Performs calculation with specified operands and returns the result.
*      The status flags SNZVC of the referenced status register are updated
*      in accordance with the result.
*
*      - operation: The operation to perform (OR, AND, XOR, ADD or SUB).
*      - a        : First operand.
*      - b        : Second operand.
*      - sr       : Reference to status register containing SNZVC flags.
********************************************************************************/
uint32_t alu(const uint16_t operation,
             const uint32_t a,
             const uint32_t b,
             uint8_t* sr);

#endif /* ALU_H_ */
