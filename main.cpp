#include "stack.h"


int main(void) {
    Stack_t stack = {};
    stackCtor(&stack, 1, nullptr);

    stackPush(&stack, 5);
    stackPush(&stack, 6);
    stackPush(&stack, 7);
    stackPush(&stack, 10);
    stackPush(&stack, 11);
    stackPush(&stack, 12);
    stackPush(&stack, 13);
    stackPush(&stack, 14);
    stackPush(&stack, 15);
    stackPush(&stack, 16);
    stackPush(&stack, 17);
    stackPush(&stack, 18);

    printf("%d\n", stackPop(&stack));
    printf("%d\n", stackPop(&stack));
    printf("%d\n", stackPop(&stack));
    printf("%d\n", stackPop(&stack));
    printf("%d\n", stackPop(&stack));
    printf("%d\n", stackPop(&stack));
    printf("%d\n", stackPop(&stack));
    printf("%d\n", stackPop(&stack));
    printf("%d\n", stackPop(&stack));
    printf("%d\n", stackPop(&stack));

    stackDtor(&stack);

    return 0;
}