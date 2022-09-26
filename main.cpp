#include "stack.h"


// void test(FILE *file, char *format, ...) {
//     va_list args;
//     va_start(args, format);
//     vfprintf(file, format, args);
// }

int main(void) {
    //runTests();

    Stack_t stack = {};
    stackCtor(&stack, 4, nullptr);
    stackPush(&stack, 34);
    stackPush(&stack, 4);

    for (int i = 0; i < 40; i++) {
        stackPush(&stack, i);
    }

    for (int i = 0; i < 24; i++) {
        stackPop(&stack);
    }

    stackDtor(&stack);
    //printf("%lu\n", countHash(stack.data - sizeof(CANARY_CONSTANT) / sizeof(Elem_t), stack.capacity * sizeof(Elem_t) + 2 * sizeof(CANARY_CONSTANT)));

    //printf("%lu\n", countHash(stack.data, stack.capacity * sizeof(Elem_t)));

    // int a = 345;
    // test(stderr, "%d %d", a, 2356);

    return 0;
}