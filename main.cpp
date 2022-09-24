#define Elem_t int

#include "stack.h"


int main(void) {
    Stack_t stack = {};
    stackCtor(&stack, 6);

    stackPush(&stack, 5);
    stackPush(&stack, 6);
    stackPush(&stack, 7);
    stackPush(&stack, 10);

    printf("%d\n", stackPop(&stack));
    printf("%d\n", stackPop(&stack));
    printf("%d\n", stackPop(&stack));

    free(stack.data);

    return 0;
}