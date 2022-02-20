#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

struct Stack {
    LinkedList stack;
};

typedef struct Stack *Stack;

/**
 * Allocate, initialize and return a new (empty) Stack.
 */
extern Stack new_Stack();

/**
 * Free the memory used for the given Stack.
 * If boolean free_data_also is true, also free the data associated with
 * each element.
 */
extern void Stack_free(Stack list, bool free_data_also);

/**
 * Return true if the given Stack is empty.
 */
extern bool Stack_isEmpty(const Stack this);

/**
 * Push the given element (void*) on the Stack.
 */
extern void Stack_push(Stack this, void *value);

/**
 * Pop and return the first element from the given Stack.
 * Returns NULL if the Stack is empty.
 */
extern void *Stack_pop(Stack this);

/**
 * Returns (but does NOT pop) the ith element from the given Stack.
 */
extern void *Stack_peek(Stack this, int i); 

