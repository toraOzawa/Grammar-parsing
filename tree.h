#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct NODE *TREE;

struct NODE {
    char label;
    TREE leftmostChild, rightSibling;
};


extern TREE makeNode0(char x);
extern TREE makeNode1(char x, TREE t);
extern TREE makeNode4(char x, TREE t1, TREE t2, TREE t3, TREE t4);
// TREE parseTree; /* holds the result of the parse */
// char *nextTerminal; /* current position in input string */