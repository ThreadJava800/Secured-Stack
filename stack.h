#ifndef SECURED_STACK_H
#define SECURED_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>

#define Elem_t int
#define MAX_DEB_LINE 4096
#define POISON_VALUE 2147483647

#define ASSERT_OK(stack) {\
    int errorCode = verifyStack(stack);\
    \
    if (errorCode) {\
        FILE *file = fopen("log.txt", "a");\
        DUMP(stack, file, errorCode);\
        fclose(file);\
    }\
}\

#define DUMP(stack, file, errCode) {\
    fprintf(file, "\nASSERTION FAILED with error code: %d\n", errCode);\
    fprintf(file, "TIMESTAMP: %lus\n", (unsigned long)time(NULL));\
    fprintf(file, "in %s at %s(%d)\n", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);\
    \
    if (stack && errCode != STACK_NULL)  {\
        fprintf(file, "Stack_t[%p] '%s' at %s at %s(%d)\n{\n", &stack, stack->name, stack->createFunc, stack->createFile, stack->createLine);\
        fprintf(file, "\tsize = %lu\n", stack->size);\
        fprintf(file, "\tcapacity = %lu\n", stack->capacity);\
        \
        if (stack->data) {\
            fprintf(file, "\tdata[%p]\n\t{\n", &stack->data);\
            \
            for (size_t i = 0; i < stack->size; i++) {\
                Elem_t dumpValue = stack->data[i];\
                if (dumpValue != POISON_VALUE) fprintf(file, "\t\t*[%lu] = %d\n", i, dumpValue);\
                else fprintf(file, "\t\t[%lu] = %d\n", i, POISON_VALUE);\
            }\
            \
            fprintf(file, "\t}\n}\n");\
        }\
        else fprintf(file, "data[0x00000000] - NULLPTR\n");\
    }\
    else fprintf(file, "Stack[0x00000000] - NULLPTR");\
}\

const double resizeCoefficient = 1.61; //http://artem.sobolev.name/posts/2013-02-10-std-vector-growth.html https://github.com/facebook/folly/blob/main/folly/docs/FBVector.md

/**
 *
 * Stack itself
 *
 * @param data - stack values (array of them)
 * @param size - amount of values in stack (and pointer to nearest free place)
 * @param capacity - all of capacity available to data
**/
struct Stack_t {
    Elem_t *data = {};
    size_t size = 0;
    size_t capacity = 0;

    const char *createFunc = {};
    const char *createFile = {};
    const char *name = {};
    int createLine = 0;
};

enum ErrorCodes {
    OK                          =  0,
    UNABLE_TO_ALLOCATE_MEMORY   = -1,
    DATA_NULL                   = -2,
    INVALID_CAPACITY            = -3,
    SIZE_BIGGER_CAPACITY        = -4,
    STACK_NULL                  = -5,
    INVALID_SIZE                = -6,
};

/**
 *
 * Pushes value of Elem_t type to stack
 *
 * @param stack - pointer to stack where to push element
 * @param capacity - wanted capacity of stack
 * @param err - pointer to int where error code is saved
 **/
void _stackCtor(Stack_t *stack, size_t capacity, int *err = nullptr);

#define stackCtor(stack, ...) {\
    if (stack) {\
        (stack)->createFunc = __PRETTY_FUNCTION__;\
        (stack)->createFile = __FILE_NAME__;\
        (stack)->createLine = __LINE__;\
        (stack)->name = #stack;\
    }\
    _stackCtor(stack, ##__VA_ARGS__);\
}\

/**
 *
 * Pushes value of Elem_t type to stack
 *
 * @param stack - pointer to stack where to push element
 * @param elem - value to push
 * @param err - pointer to int where error code is saved
 **/
void stackPush(Stack_t *stack, Elem_t elem, int *err = nullptr);

/**
 *
 * Takes the last value of stack and returns it
 *
 * @param stack - pointer to stack where to pop element
 * @param err - pointer to int where error code is saved
 *
 * @return Elem_t - value 
 **/
Elem_t stackPop(Stack_t *stack, int *err = nullptr);

void stackResize(Stack_t *stack, size_t size, int *err = nullptr);

void stackShrinkToFit(Stack_t *stack, int *err = nullptr);

void stackDtor(Stack_t *stack, int *err = nullptr);

int verifyStack(Stack_t *stack);

#endif