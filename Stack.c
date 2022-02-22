#include <stdlib.h>
#include "Stack.h"


Stack new_Stack() {
    Stack new = (Stack)malloc(sizeof(struct Stack));
    new->stack = new_LinkedList();
    return new;
}

void Stack_free(Stack this, bool free_data_also) {
    LinkedList_free(this->stack, free_data_also);
    free(this);
}


bool Stack_isEmpty(const Stack this) {
    return LinkedList_isEmpty(this->stack);
}

void Stack_push(Stack this, void *value) {
    LinkedList_add_at_front(this->stack, value);
}

void *Stack_pop(Stack this) {
    return LinkedList_pop(this->stack);
}

void *Stack_peek(Stack this, int i) {
    return LinkedList_elementAt(this->stack, i);
}