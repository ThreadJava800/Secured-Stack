#ifndef SECURED_STACK_H
#define SECURED_STACK_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>

typedef int Elem_t;

/**
 *
 * I use this coefficient to resize stack when needed.
 * I used this link while searching info:
 * http://artem.sobolev.name/posts/2013-02-10-std-vector-growth.html
 *
 **/
const Elem_t POISON_VALUE = 2147483646;

/**
 *
 * I use this coefficient to resize stack when needed.
 * I used this link while searching info:
 * http://artem.sobolev.name/posts/2013-02-10-std-vector-growth.html
 *
 **/
const double RESIZE_COEFFICIENT = 1.61;

#define ASSERT_OK(stack) {\
    int errorCode = verifyStack(stack);\
    \
    if (errorCode) {\
        FILE *file = fopen("log.txt", "a");\
        if (errorCode == SIZE_BIGGER_CAPACITY) exit(0);\
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
        fprintf(file, "Stack_t[%p] '%s' at %s at %s(%d)\n{\n", stack, (stack)->name, (stack)->createFunc, (stack)->createFile, (stack)->createLine);\
        fprintf(file, "\tsize = %lu\n", (stack)->size);\
        fprintf(file, "\tcapacity = %lu\n", (stack)->capacity);\
        \
        if ((stack)->data) {\
            fprintf(file, "\tdata[%p]\n\t{\n", &(stack)->data);\
            \
            for (size_t i = 0; i < (stack)->size; i++) {\
                Elem_t dumpValue = (stack)->data[i];\
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

    const char *createFunc = {};
    const char *createFile = {};
    const char *name = {};
    int createLine = 0;
};

/**
 *
 * Error codes returned from functions
 *
**/
enum ErrorCodes {
    OK                          =  0,
    UNABLE_TO_ALLOCATE_MEMORY   = -1,
    DATA_NULL                   = -2,
    INVALID_CAPACITY            = -3,
    SIZE_BIGGER_CAPACITY        = -4,
    STACK_NULL                  = -5,
    INVALID_SIZE                = -6,
    STACK_EMPTY                 = -7,
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

/**
 *
 * Interlayer between main and _stackCtor, used for debug to get variable name
 *
 **/
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

/**
 *
 * Resizes stack
 *
 * @param stack - pointer to stack to resize
 * @param size - new size of stack
 * @param err - pointer to int where error code is saved
 **/
void stackResize(Stack_t *stack, size_t size, int *err = nullptr);

/**
 *
 * Resizes stack to its actual size
 *
 * @param stack - pointer to stack to resize
 * @param err - pointer to int where error code is saved
 **/
void stackShrinkToFit(Stack_t *stack, int *err = nullptr);

/**
 *
 * Kills stack: frees all allocated memory, nulls all arguments
 *
 * @param stack - pointer to stack to kill
 * @param err - pointer to int where error code is saved
 **/
void stackDtor(Stack_t *stack, int *err = nullptr);

/**
 *
 * Checks stack and returns OK(0) if stack is ok.
 * Else returns errorCode (from -1 to ... -inf)
 *
 * @param stack - pointer to stack to check
 *
 * @return int - errorCode
 **/
int verifyStack(Stack_t *stack);

/**
 *
 * Reallocates memory and sets all new values to 0
 *
 * @param ptr - pointer to object to realloc
 * @param size - new size to realloc
 * @param err - pointer to int where error code is saved
 *
 * @return void * - pointer to new reallocated memory
 **/
void *recalloc(void *ptr, size_t amount, size_t elemSize, size_t currentAmount, int *err = nullptr);

#endif