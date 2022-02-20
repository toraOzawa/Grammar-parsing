#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "tree.h"

// TREE parseTree; /* holds the result of the parse */
// char *nextTerminal; /* current position in input string */


// void preorder(pNODE n) {
//     pNODE c; /* a child of node n */
//     printf("%c\n", n->nodeLabel);
//     c = n->leftmostChild;
//     while (c != NULL) {
//         preorder(c);
//         c = c->rightSibling;    
//     }
// }

// void postorder(pNODE n) {
//     pNODE c; /* a child of node n */
//     c = n->leftmostChild;
//     while (c != NULL) {
//         postorder(c);
//         c = c->rightSibling;
//     }
//     printf("%c\n", n->nodeLabel);
// }

// int main() {
//     nextTerminal = "()()"; /* in practice, a string of terminals would be read from input */
//     parseTree = B();
// }

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
    // printf("call start\n");
    for (int i = 0; i < spaces; i++) {
        printf("  ");
    }
    // printf("spaces inserted\n");
    printf("%c\n", t->label);

    if (t->leftmostChild != NULL) TREE_pretty_print(t->leftmostChild, spaces + 1);
    if (t->rightSibling != NULL) TREE_pretty_print(t->rightSibling, spaces);

}

// int main() {

// }

