#include "stack.h"

void canaryData(Elem_t **data, size_t capacity, int *err) {
    if (err) *err = OK;
    if (!data && err) *err = DATA_NULL;
    if (!*data && err) *err = DATA_NULL;

    char *temp = (char *) *data;
    *data = (Elem_t*) memcpy(temp, &CANARY_CONSTANT, sizeof(CANARY_CONSTANT));
    *data += sizeof(CANARY_CONSTANT) / sizeof(Elem_t);

    temp += capacity * sizeof(Elem_t) + sizeof(CANARY_CONSTANT);
    memcpy(temp, &CANARY_CONSTANT, sizeof(CANARY_CONSTANT));
}

void updateHashes(Stack_t *stack, int *err) {
    if (!stack && err) *err = STACK_NULL;
    if (!stack->data && err) *err = DATA_NULL;

    stack->stackHash = 0;
    stack->bufferHash = countHash(stack->data - sizeof(CANARY_CONSTANT) / sizeof(Elem_t), stack->capacity * sizeof(Elem_t) + 2 * sizeof(CANARY_CONSTANT));
    stack->stackHash = countHash(stack, sizeof(Stack_t));
}

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

    stack->data = (Elem_t *) calloc(1, sizeof(Elem_t) * capacity + 2 * sizeof(CANARY_CONSTANT));
    stack->size = 0;
    stack->capacity = capacity;

    if (!stack->data) {
        if (err) *err = UNABLE_TO_ALLOCATE_MEMORY;
    } else {
        canaryData(&stack->data, capacity, err);
    }

    updateHashes(stack, err);

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

    updateHashes(stack, err);

    if (stack->size >= stack->capacity - 1) {
        size_t coef = (size_t)ceil((double)stack->size * RESIZE_COEFFICIENT);
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

    size_t toLower = (size_t)(floor((double)stack->capacity / (RESIZE_COEFFICIENT * RESIZE_COEFFICIENT)));

    updateHashes(stack, err);

    if (stack->size < toLower) {
        stackResize(stack, (size_t)floor((double)stack->capacity / RESIZE_COEFFICIENT), err);
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

    stack->data -= sizeof(CANARY_CONSTANT) / sizeof(Elem_t);
    stack->data = (Elem_t *) recalloc(stack->data, 1, (size + 1) * sizeof(Elem_t) + 2 * sizeof(CANARY_CONSTANT), stack->size);
    stack->capacity = size + 1;

    canaryData(&stack->data, stack->capacity, err);

    if (!stack->data) *err = UNABLE_TO_ALLOCATE_MEMORY;

    updateHashes(stack, err);
    
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
        stack->data -= sizeof(CANARY_CONSTANT) / sizeof(Elem_t);
        free(stack->data);
        stack->data = nullptr;
    }

    stack->size = 0;
    stack->capacity = 0;

    stack->createFunc = {};
    stack->createFile = {};
    stack->name = {};
    stack->createLine = 0;

    stack->stackHash = 0;
    stack->bufferHash = 0;
}

void *recalloc(void *ptr, size_t amount, size_t elemSize, size_t currentAmount, int *err) {
    if (err) *err = OK;
    if (!ptr) {
        return calloc(amount, elemSize);
    }

    char *newPtr = (char *) realloc(ptr, amount * elemSize);

    if (!newPtr) {
        if (err) {
            *err = UNABLE_TO_ALLOCATE_MEMORY;
        }

        return ptr;
    }

    if (currentAmount < amount) {
        for (size_t i = currentAmount; i < amount; i++) {
            newPtr[i * elemSize] = 0;
        }
    }

    ptr = newPtr;

    return ptr;
}