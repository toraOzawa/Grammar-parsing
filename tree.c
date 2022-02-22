#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "tree.h"
// Code Taken from chapter 11 of Foundations of Computer Science, by Al Aho and Jeff Ullman

TREE makeNode0(char x) { 
    TREE root;
    root = (TREE) malloc(sizeof(struct NODE));
    root->label = x;
    root->leftmostChild = NULL;
    root->rightSibling = NULL;
    return root;
}

TREE makeNode1(char x, TREE t) {
    TREE root;
    root = makeNode0(x);
    root->leftmostChild = t;
    return root;
}

TREE makeNode4(char x, TREE t1, TREE t2, TREE t3, TREE t4) {
    TREE root;
    root = makeNode1(x, t1);
    if (t1 == NULL) return root;
    t1->rightSibling = t2;
    if (t2 == NULL) return root;
    t2->rightSibling = t3;
    if (t3 == NULL) return root;
    t3->rightSibling = t4;
    return root;
}

void TREE_free(TREE root) {
    if (root->leftmostChild != NULL) TREE_free(root->leftmostChild);
    if (root->rightSibling!= NULL) TREE_free(root->rightSibling);
    free(root);
}

void TREE_pretty_print(TREE t, int spaces) {
    for (int i = 0; i < spaces; i++) {
        printf("  ");
    }
    printf("%c\n", t->label);

    if (t->leftmostChild != NULL) TREE_pretty_print(t->leftmostChild, spaces + 1);
    if (t->rightSibling != NULL) TREE_pretty_print(t->rightSibling, spaces);

}

