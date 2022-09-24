#include "stack.h"

void _stackCtor(Stack_t *stack, size_t capacity, int *err) {
    if (err) *err = OK;
    if (!stack && err) {
        *err = STACK_NULL;
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
        *err = STACK_NULL;
        return;
    }

    ASSERT_OK(stack);

    stack->data[stack->size] = elem;
    stack->size++;

    if (stack->size >= stack->capacity - 1) {
        size_t coef = (size_t)ceil((double)stack->size * resizeCoefficient);
        stackResize(stack, coef, err);
    }
}

Elem_t stackPop(Stack_t *stack, int *err) {
    if (err) *err = OK;
    if (!stack && err) {
        *err = STACK_NULL;
        return POISON_VALUE;
    }
    if (stack->size == 0) {
        if (err) *err = STACK_EMPTY;
        return POISON_VALUE;
    }
    ASSERT_OK(stack);

    stack->size--;
    Elem_t value = stack->data[stack->size];
    stack->data[stack->size] = POISON_VALUE;

    size_t toLower = (size_t)(floor((double)stack->capacity / (resizeCoefficient * resizeCoefficient)));
    if (stack->size < toLower) {
        stackResize(stack, (size_t)floor((double)stack->capacity / resizeCoefficient), err);
    }

    return value;
}

void stackResize(Stack_t *stack, size_t size, int *err) {
    if (err) *err = OK;
    if (!stack && err) {
        *err = STACK_NULL;
        return;
    }
    ASSERT_OK(stack);

    stack->data = (Elem_t *) realloc(stack->data, sizeof(Elem_t) * (size + 1));
    stack->capacity = size + 1;
    if (!stack->data) *err = UNABLE_TO_ALLOCATE_MEMORY;

    ASSERT_OK(stack);
}

void stackShrinkToFit(Stack_t *stack, int *err) {
    stackResize(stack, stack->size, err);
}

void stackDtor(Stack_t *stack, int *err) {
    if (err) *err = OK;
    if (!stack && err) {
        *err = STACK_NULL;
        return;
    }
    ASSERT_OK(stack);

    if (stack->data) {
        free(stack->data);
    }

    stack->size = 0;
    stack->capacity = 0;

    stack->createFunc = {};
    stack->createFile = {};
    stack->name = {};
    stack->createLine = 0;
}