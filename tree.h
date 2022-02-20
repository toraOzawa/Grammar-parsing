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
extern void TREE_pretty_print(TREE t, int spaces);
extern void TREE_free(TREE root);
// TREE parseTree; /* holds the result of the parse */
// char *nextTerminal; /* current position in input string */