#ifndef SECURED_STACK_H
#define SECURED_STACK_H

#include <stdio.h>
#include <malloc.h>

#define Elem_t int

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

 * @return Elem_t - value 
 **/
Elem_t stackPop(Stack_t *stack, int *err = nullptr);

#endif