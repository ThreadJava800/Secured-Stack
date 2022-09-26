#include "stack.h"

int proveCanary(Elem_t *data, size_t capacity) {
    if (!data) return 0;

    char canaryBytes[sizeof(CANARY_CONSTANT)];
    memcpy(canaryBytes, (((char *)data) - sizeof(CANARY_CONSTANT)), sizeof(CANARY_CONSTANT));
    size_t *firstCanaryPtr = (size_t *) (canaryBytes);
    size_t firstCanary = *firstCanaryPtr;

    memcpy(canaryBytes, (((char *)data) + sizeof(Elem_t) * capacity), sizeof(CANARY_CONSTANT));
    firstCanaryPtr = (size_t *) (canaryBytes);
    size_t secondCanary = *firstCanaryPtr;

    return firstCanary == CANARY_CONSTANT && secondCanary == CANARY_CONSTANT;
}

size_t countHash(void *ptr, size_t size) {
    if (!ptr) return 0;

    size_t hash = 0;
    char *tmp = (char *) ptr;

    for (size_t i = 0; i < size; i++) {
        hash += (size_t) tmp[i];
    }

    return hash;
}

int checkDataHash(Stack_t *stack) {
    if (!stack) return 0;
    if (!stack->data) return 0;

    size_t oldHash = stack->bufferHash;
    size_t newHash = countHash(stack->data - sizeof(CANARY_CONSTANT) / sizeof(Elem_t), stack->capacity * sizeof(Elem_t) + 2 * sizeof(CANARY_CONSTANT));

    return oldHash == newHash;
}

int checkStackHash(Stack_t *stack) {
    if (!stack) return 0;

    size_t oldHash = stack->stackHash;
    stack->stackHash = 0;

    size_t newHash = countHash(stack, sizeof(Stack_t));
    stack->stackHash = oldHash;

    return oldHash == newHash;
}

int verifyStack(Stack_t *stack) {
    if (stack == nullptr) return STACK_NULL;
    if (stack->data == nullptr) return DATA_NULL;
    if (stack->size > stack->capacity) return SIZE_BIGGER_CAPACITY;
    if (stack->stackCanary1 != CANARY_CONSTANT || stack->stackCanary2 != CANARY_CONSTANT) return STACK_CANARY_BROKEN;
    if (!proveCanary(stack->data, stack->capacity)) return BUFFER_CANARY_BROKEN;
    if (!checkDataHash(stack)) return BUFFER_HASH_CHANGED;
    if (!checkStackHash(stack)) return STACK_HASH_CHANGED;

    return OK;
}