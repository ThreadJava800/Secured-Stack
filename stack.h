#ifndef SECURED_STACK_H
#define SECURED_STACK_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>

#ifndef HASH_PROTECT
#define HASH_PROTECT 1
#endif

#ifndef CANARY_PROTECT
#define CANARY_PROTECT 1
#endif

#ifndef _DEBUG
#define _DEBUG 0
#endif

typedef int Elem_t;

/**
 *
 * I use this value to replace old values with it
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

#if CANARY_PROTECT
/**
 *
 * I use this constant for CANARY protect, i put it to the beginning of array and struct
 *
 **/
const size_t CANARY_CONSTANT = 8350650019957897075;
#endif

/**
 *
 * This function checks if stack OK and DUMPS stack values on error (if _DEBUG)
 *
 *
 * @param stack - Stack_t structure with all needed staff for stack
 * @param printEl - function to print element in array
 **/
#if _DEBUG

    #define ASSERT_OK(stack, printEl) {\
        int errorCode = verifyStack(stack);\
        \
        if (errorCode) {\
            FILE *file = fopen("log.txt", "a");\
            if (errorCode == SIZE_BIGGER_CAPACITY) exit(0);\
            \
            DUMP(stack, file, errorCode, printEl);\
            \
            fclose(file);\
        }\
    }\

#else

    #define ASSERT_OK(stack, printEl) {\
        int errorCode = verifyStack(stack);\
        \
        if (errorCode) {\
            if (errorCode == SIZE_BIGGER_CAPACITY) exit(0);\
        }\
    }\

#endif

/**
 *
 * This function DUMPS all info about stack if error
 *
 *
 * @param stack - Stack_t structure with all needed staff for stack
 * @param file - pointer to file where to DUMP
 * @param errCode - program error code
 * @param printEl - function to print element in array
 **/
#define DUMP(stack, file, errCode, printEl) {\
    mprintf(file, "\nASSERTION FAILED with error code: %d\n", errCode);\
    mprintf(file, "TIMESTAMP: %lus\n", (unsigned long)time(NULL));\
    mprintf(file, "in %s at %s(%d)\n", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);\
    \
    if (stack && errCode != STACK_NULL)  {\
        mprintf(file, "Stack_t[%p] '%s' at %s at %s(%d)\n{\n", stack, (stack)->name, (stack)->createFunc, (stack)->createFile, (stack)->createLine);\
        mprintf(file, "\tsize = %lu\n", (stack)->size);\
        mprintf(file, "\tcapacity = %lu\n", (stack)->capacity);\
        \
        if ((stack)->data) {\
            mprintf(file, "\tdata[%p]\n\t{\n", &(stack)->data);\
            \
            mprintf(file, "\t\t%d\n", (stack)->data[-1]);\
            for (size_t i = 0; i < (stack)->size; i++) {\
                Elem_t dumpValue = (stack)->data[i];\
                printEl(file, i, dumpValue);\
            }\
            mprintf(file, "\t\t%d\n", (stack)->data[(stack)->capacity]);\
            \
            mprintf(file, "\t}\n}\n");\
        }\
        else mprintf(file, "%s", "data[0x00000000] - NULLPTR\n");\
    }\
    else mprintf(file, "%s", "Stack[0x00000000] - NULLPTR");\
    \
    fflush(file);\
}\

#define getType()

/**
 *
 * Stack itself
 *
 * @param stackCanary1 - canary constant in the beginning
 * @param data - stack values (array of them)
 * @param size - amount of values in stack (and pointer to nearest free place)
 * @param capacity - all of capacity available to data
 * @param createFunc - name of function where stack was created
 * @param createFile - name of file where stack was created
 * @param name - name of stack variable
 * @param createLine - number of line where stack was created in @createFunc
 * @param stackHash - hash value of stack
 * @param bufferHash - hash value of data
 * @param stackCanary2 - canary constant in the end
**/
struct Stack_t {
    #if CANARY_PROTECT
    size_t stackCanary1 = CANARY_CONSTANT;
    #endif

    Elem_t *data = {};
    size_t size = 0;
    size_t capacity = 0;

    #if _DEBUG
    const char *createFunc = {};
    const char *createFile = {};
    const char *name = {};
    int createLine = 0;
    #endif

    #if HASH_PROTECT
    size_t stackHash = 0;
    size_t bufferHash = 0;
    #endif

    #if CANARY_PROTECT
    size_t stackCanary2 = CANARY_CONSTANT;
    #endif
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
    STACK_CANARY_BROKEN         = -8,
    BUFFER_CANARY_BROKEN        = -9,
    BUFFER_HASH_CHANGED         = -10,
    STACK_HASH_CHANGED          = -11,
};

/**
 *
 * Ads canaries to end and beginning of array data
 *
 * @param data - elements
 * @param capacity - capacity of array
 * @param err - pointer to int where error code is saved
 **/
void canaryData(Elem_t **data, size_t capacity, int *err = nullptr);

/**
 *
 * Updates hashes for stack and data
 *
 * @param stack - pointer to stack to hash
 * @param err - pointer to int where error code is saved
 **/
void updateHashes(Stack_t *stack, int *err = nullptr);

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
#if _DEBUG

    #define stackCtor(stack, ...) {\
        if (stack) {\
            (stack)->createFunc = __PRETTY_FUNCTION__;\
            (stack)->createFile = __FILE_NAME__;\
            (stack)->createLine = __LINE__;\
            (stack)->name = #stack;\
        }\
        _stackCtor(stack, ##__VA_ARGS__);\
    }\

#else 

    #define stackCtor(stack, ...) {\
        _stackCtor(stack, ##__VA_ARGS__);\
    }\

#endif

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
 * Checks if canaries in array are not broken
 *
 * @param data - array to check
 * @param err - pointer to int where error code is saved
 *
 * @return int - 1 if everything ok and 0 if canary broken
 **/
int proveCanary(Elem_t *data, size_t capacity);

/**
 *
 * Counts hash of a value
 *
 * @param ptr - pointer to value
 * @param size - size of value in bytes
 *
 * @return size_t - hash value
 **/
size_t countHash(void *ptr, size_t size);

/**
 *
 * Checks if array hash is not broken
 *
 * @param stack - pointer to stack where hash saved
 *
 * @return int - 1 if everything ok and 0 if canary broken
 **/
int checkDataHash(Stack_t *stack);

/**
 *
 * Checks if stack hash is not broken
 *
 * @param stack - pointer to stack
 *
 * @return int - 1 if everything ok and 0 if hash broken
 **/
int checkStackHash(Stack_t *stack);

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

/**
 *
 * Prints values using vfprintf
 *
 * @param file - pointer to file
 * @param fmt - format string
 * @param ... - arguments to print
 **/
void mprintf(FILE *file, const char *fmt...);

/**
 *
 * Prints element according to its type
 *
 * @param file - pointer to file
 * @param index - index of value in array
 * @param value - value itself
 **/
void printElemT(FILE *file, size_t index, Elem_t value);

#endif