#include "stack.h"

int verifyStack(Stack_t *stack) {
    if (stack == nullptr) return STACK_NULL;
    if (stack->data == nullptr) return DATA_NULL;
    if (stack->size > stack->capacity) return SIZE_BIGGER_CAPACITY;

    return OK;
}