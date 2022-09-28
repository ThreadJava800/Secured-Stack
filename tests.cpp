#include "tests.h"
#include "stack.h"

void testPushPop() {
    srand(time(NULL));
    int testCount = 10000, succeed = 0;
    int values[10000] = {};
    Stack_t stack = {};
    stackCtor(&stack, 1);

    for(int i = 0; i < testCount; i++){
        values[i] = (rand() % DEFAULT_POISON_VALUE) + 1;
    }

    for(int i = 0; i < testCount; i++){
        stackPush(&stack, values[i]);
    }

    for(int i = 0; i < testCount; i++){
        int *err = 0;
        int returned = stackPop(&stack, err);
        if (values[testCount - i - 1] == returned) {
            succeed++;
        } else {
            printf("Test FAILED:\n");
            printf("\tWanted: %d, GOT: %d\n", values[testCount - i], returned);
        }
    }

    // for(int i = 0; i < testCount; i++){
    //     printf("%d ", values[testCount - i - 1]);
    // }
    // printf("\n");
    // for(int i = 0; i < testCount + 2; i++){
    //     printf("%lu ", stackPop(&stack), stack.size);
    // }

    printf("PASSED tests: %d/%d\n", succeed, testCount);
}

void testResize() {
    srand(time(NULL));
    int testCount = 10000, succeed = 0;
    int values[10000] = {};
    Stack_t stack = {};
    stackCtor(&stack, 1);

    for(int i = 0; i < testCount; i++){
        values[i] = (rand() % DEFAULT_POISON_VALUE) + 1;
    }

    for(int i = 0; i < testCount; i++){
        stackPush(&stack, values[i]);

        size_t waited = 0;
        if (stack.size == stack.capacity - 1) {
            waited = (size_t) ceil(stack.size * RESIZE_COEFFICIENT);
        } else {
            waited = stack.capacity;
        }

        if (waited == stack.capacity) {
            succeed++;
        }

    }

    for(int i = 0; i < testCount; i++){
        stackPop(&stack);
        size_t waited = 0;
        if (stack.size < (size_t)(floor(stack.capacity / (RESIZE_COEFFICIENT * RESIZE_COEFFICIENT)))) {
            stack.size = (size_t)(floor(stack.capacity / RESIZE_COEFFICIENT));
        } else {
            waited = stack.capacity;
        }

        if (waited == stack.capacity) {
            succeed++;
        } else {
            printf("Test FAILED:\n");
            printf("\tWanted: %d, GOT: %d\n", stack.capacity, waited);
        }
    }

    // for(int i = 0; i < testCount; i++){
    //     printf("%d ", values[testCount - i - 1]);
    // }
    // printf("\n");
    // for(int i = 0; i < testCount + 2; i++){
    //     printf("%lu ", stackPop(&stack), stack.size);
    // }


    printf("PASSED tests: %d/%d\n", succeed, testCount);
}

void runTests() {
    printf("PUSH POP\n");
    testPushPop();

    printf("RESIZE\n");
    testResize();
}