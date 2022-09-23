#include "stack.h"

void stackCtor(Stack_t *stack, size_t capacity, int *err) {
    stack->data = (Elem_t *) calloc(capacity, sizeof(Elem_t));
}


void stackPush(Stack_t *stack, Elem_t elem, int *err) {
    stack->data[stack->size] = elem;
    stack->size++;
}

Elem_t stackPop(Stack_t *stack, int *err) {
    stack->size--;
    Elem_t value = stack->data[stack->size];
    stack->data[stack->size] = 0;

    return value;
}