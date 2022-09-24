#include "stack.h"

void stackCtor(Stack_t *stack, size_t capacity, int *err) {
    if (err) *err = OK;
    if (!stack && err) {
        *err = NULL_POINTER;
        return;
    }
    if (capacity < 1 && err) {
        *err = INVALID_CAPACITY;
        return;
    }

    stack->data = (Elem_t *) calloc(capacity, sizeof(Elem_t));
    stack->size = 0;
    stack->capacity = capacity;

    if (!stack->data && err) *err = UNABLE_TO_ALLOCATE_MEMORY;

    ASSERT_OK(stack);
}


void stackPush(Stack_t *stack, Elem_t elem, int *err) {
    if (err) *err = OK;
    if (!stack && err) {
        *err = NULL_POINTER;
        return;
    }

    ASSERT_OK(stack);

    stack->data[stack->size] = elem;
    stack->size++;

    ASSERT_OK(stack);
}

Elem_t stackPop(Stack_t *stack, int *err) {
    if (err) *err = OK;
    if (!stack && err) {
        *err = NULL_POINTER;
        return POISON_VALUE;
    }
    ASSERT_OK(stack);

    stack->size--;
    Elem_t value = stack->data[stack->size];
    stack->data[stack->size] = 0;

    ASSERT_OK(stack);

    return value;
}