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
    if (!verifyStack(stack)) {\
        FILE *file = fopen("log.txt", "a");\
        DUMP(stack, file);\
        fclose(file);\
    }\
}\

#define DUMP(stack, file) {\
    fprintf(file, "\nASSERTION FAILED\n");\
    fprintf(file, "TIMESTAMP: %lus\n", (unsigned long)time(NULL));\
    fprintf(file, "in %s at %s(%d)\n", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);\
    \
    if (stack)  {\
        fprintf(file, "Stack_t[%p]\n{\n", &stack);\
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

    const char *breakFunc = {};
    const char *breakFile = {};
    int breakLine = 0;
};

enum ErrorCodes {
    OK                          =  0,
    UNABLE_TO_ALLOCATE_MEMORY   = -1,
    NULL_POINTER                = -2,
    INVALID_CAPACITY            = -3,
    SIZE_BIGGER_CAPACITY        = -4,
};

/**
 *
 * Pushes value of Elem_t type to stack
 *
 * @param stack - pointer to stack where to push element
 * @param capacity - wanted capacity of stack
 * @param err - pointer to int where error code is saved
 **/
void stackCtor(Stack_t *stack, size_t capacity, int *err = nullptr);

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

/**
 *
 * Prints error to file by error code
 *
 * @param file - pointer to file
 * @param errCode - code of error
 **/
void printError(FILE *file, int errCode);

int verifyStack(Stack_t *stack);

#endif