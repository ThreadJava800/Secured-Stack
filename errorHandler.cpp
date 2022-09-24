#include "stack.h"

int verifyStack(Stack_t *stack) {
    if (stack == nullptr) return NULL_POINTER;
    if (stack->data == nullptr) return NULL_POINTER;
    if (stack->size > stack->capacity) return SIZE_BIGGER_CAPACITY;

    return OK;
}