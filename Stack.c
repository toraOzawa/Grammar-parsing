#include "Stack.h"


Stack new_Stack() {
    Stack new = (Stack)malloc(sizeof(struct Stack));
    new->stack = new_LinkedList();
    return newl
}

void Stack_free(Stack this, bool free_data_also) {
    LinkedList_free(this->stack, free_data_also);
    free(this);
}


bool Stack_isEmpty(const Stack this) {
    LinkedList_isEmpty(this->stack);
}

void Stack_push(Stack this, void *value) {
    LinkedList_add_at_front(this, value);
}

void *Stack_pop(Stack this) {
    return LinkedList_pop(this);
}

void *Stack_peek(Stack this, int i) {
    return LinkedList_elementAt(this->stack, i);
}