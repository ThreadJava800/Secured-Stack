#include "stack.h"

int proveCanary(Elem_t *data, size_t capacity) {
    char canaryBytes[sizeof(CANARY_CONSTANT)];
    memcpy(canaryBytes, (((char *)data) - sizeof(CANARY_CONSTANT)), sizeof(CANARY_CONSTANT));
    size_t *firstCanaryPtr = (size_t *) (canaryBytes);
    size_t firstCanary = *firstCanaryPtr;

    memcpy(canaryBytes, (((char *)data) + sizeof(Elem_t) * capacity), sizeof(CANARY_CONSTANT));
    firstCanaryPtr = (size_t *) (canaryBytes);
    size_t secondCanary = *firstCanaryPtr;

    return firstCanary == CANARY_CONSTANT && secondCanary == CANARY_CONSTANT;
}

int verifyStack(Stack_t *stack) {
    if (stack == nullptr) return STACK_NULL;
    if (stack->data == nullptr) return DATA_NULL;
    if (stack->size > stack->capacity) return SIZE_BIGGER_CAPACITY;
    if (stack->stackCanary1 != CANARY_CONSTANT || stack->stackCanary2 != CANARY_CONSTANT) return STACK_CANARY_BROKEN;
    if (!proveCanary(stack->data, stack->capacity)) return BUFFER_CANARY_BROKEN;

    return OK;
}