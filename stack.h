#include "cpu.h"

#ifndef STACK_H_
#define STACK_H_

#define STACK_ADDRESS_WIDTH 100 /* 1024 unique addresses on the stack. */

/********************************************************************************
* stack_reset: Clears content on the entire stack and sets the stack pointer
*              to the top of the stack.
********************************************************************************/
void stack_reset(void);

/********************************************************************************
* stack_push: Pushes 8 bit value to the stack, unless the stack is full.
*             Success code 0 is returned after successful push, otherwise
*             error code 1 is returned if the stack is already full.
*
*             - value: 32 bit value to push to the stack.
********************************************************************************/
int stack_push(const uint32_t value);

/********************************************************************************
* stack_pop: Returns 32 bit value popped from the stack. If the stack is empty,
*            the value 0x00 is returned.
********************************************************************************/
uint32_t stack_pop(void);


#endif /* STACK_H_ */
