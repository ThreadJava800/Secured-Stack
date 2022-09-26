#include "stack.h"


// void test(FILE *file, char *format, ...) {
//     va_list args;
//     va_start(args, format);
//     vfprintf(file, format, args);
// }

int main(void) {
    Stack_t stack = {};
    stackCtor(&stack, 1, nullptr);

    for (int i = 0; i < 40; i++) {
        stackPush(&stack, i);
    }

    for (int i = 0; i < 24; i++) {
        stackPop(&stack);
    }

    stackDtor(&stack);

    // int a = 345;
    // test(stderr, "%d %d", a, 2356);

    return 0;
}