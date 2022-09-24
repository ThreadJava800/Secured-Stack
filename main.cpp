#include "stack.h"


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

    return 0;
}