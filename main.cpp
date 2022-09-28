#include "stack.h"

int main(void) {
    Stack_t stack = {};
    stackCtor(&stack, 4, printElemT, DEFAULT_POISON_VALUE, nullptr);
    stackPush(&stack, 34);
    stackPush(&stack, 4);

    for (int i = 0; i < 40; i++) {
        stackPush(&stack, i);
    }

    for (int i = 0; i < 24; i++) {
        stackPop(&stack);
    }

    stackDtor(&stack);

    return 0;
}