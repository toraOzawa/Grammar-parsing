#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sets2.h"
#include "Stack.h"

char *string;
int i;
int length;
TREE root;

typedef struct ParseTable* ParseTable;

ParseTable {
    int** table; // "transition table"
    int** dictionary; // i = syntactic category label; dictionary[i][j] = jth terminal/category to push on stack for ith category
    int* row_lengths; // keeps track of row lengths of dictionary
};

// <expression> -> <atomic> <expression tail>
Tree expression() {
    if (!atomic()) return 0;
    if (!expr_tail()) return 0;

    // parse tree handling 
    return 1;

}

// <expression tail> -> U <expression> | ^ <expression> | ϵ
Tree expr_tail() { // could cause problems
    if (lookahead('U')) {
        match('U');
        TREE exp = expression();
        if (exp == NULL) return 0;
        return 1;
    } else if (lookahead('^')) {
        match('^');
        TREE exp = expression();
        if (exp == NULL) return 0;
        return 1;   
    } else {
        return 1; 
    }
}

// <atomic> -> (<expression>) | <set>
Tree atomic() {
    if (lookahead('(')) {
        if (!match('(')) return 0;
        if (!expression()) return 0;
        if (!match(')')) return 0;
        return 1;
    } else {
        if (!set()) return 0; 
        return 1; // could be condensed to return set()
    }
}

// <set> -> { <set tail>
Tree set() {
    if (!match('{')) return 0;
    if (!set_tail()) return 0;

    return 1;
}

// <set> -> { <set tail>
Tree set_tail() {
    if (lookahead('}')) {
        return match('}');
    } else {
        if (!elements()) return 0;
        if (!match('}')) return 0;
        return 1; 
    }
}

// <elements> -> <element> <elements tail>
Tree elements() {
   TREE elem = element();
   if (elem == NULL) return NULL;
   TREE tail = elements_tail();
   if (tail == NULL) return NULL;

    if (tail->label == 'ϵ') { 
        // free tail   
        return makeNode1('E', elem); 
    
    }

    TREE rightMost = tail->leftmostChild;
    TREE middle = makeNode0(',');
    TREE leftMost = makeNode1('E', elem);
    tail->leftmostChild = leftMost;
    leftMost->rightSibling = middle;
    middle->rightSibling = rightMost;
   return tail->leftmostChild;
}

// <elements tail> -> , <elements> | ϵ
Tree elements_tail() { // causing problems?
    if (lookahead(',')) {
        match(','); // could be formalized like the other if statements
        TREE elems = elements();
        if (elems == NULL) return NULL; // could be condensed 

        return elems;

    } else {
        return makeNode0('ϵ');
    }
}

// <element> -> <number>
Tree element() {
    TREE num = number();
    if (num == NULL) return NULL;
    return makeNode1('e', num);
}

// <number> -> <digit> <number tail>
Tree number() {
    TREE dig = digit();
    if (dig == NULL) return NULL;
    TREE tail = number_tail();
    if (tail == NULL) return NULL;


    if (tail->label == 'ϵ') return makeNode4('N', dig, NULL, NULL, NULL);

    TREE leftMost = makeNode1('N', dig);
    TREE rightMost = tail->leftmostChild;

    tail -> leftmostChild = leftMost;
    leftMost->rightSibling = rightMost;
    
    return tail;
}

// <number tail> -> <number> | ϵ
TREE number_tail() { // might cause problems
    if (is_digit()) { // lookahead in this scenario
        return number();
    } else {
        return makeNode0('ϵ');
    }
}

// <digit> -> 0 | 1 | · · · | 9
Tree digit() {
    bool isDigit = is_digit();
    if (isDigit) { 
        match(string[i])
        return makeNode1( 'd', makeNode0(string[i]));
    } else {
        return NULL;
    }
}

bool is_digit() {
    return string[i] == '0' || string[i] == '1' || string[i] == '2' || string[i] == '3' || string[i] == '4' || string[i] == '5' || string[i] == '6' || string[i] == '7' || string[i] == '8' || string[i] == '9';
}

bool parse_set_alg(char *input) {
    string = input;
    i = 0;
    length = strlen(string);
    printf("input length: %d\n", length);
    // bool result = true;
    // while(i < length) {
    //     result = expression 
    // }
    return expression() != null && length == i; // may cause problems
}

bool lookahead(char c) {
    return string[i] == c && i < length;
}

bool match(char c) {
    if (c == '*') return true; // special case used in table driven parser
    i++;
    return string[i - 1] == c && (i - 1) < length;
}



TREE table_parse_set_alg(ParseTable tb, char *input) {
    string = input;
    i = 0;
    length = strlen(string);
    Stack stack = new_Stack();

    root = makeNode0('E');
    Stack_push(root);
    TREE cur_root = root;

    while (!Stack_isEmpty(stack)) {
        TREE cur = Stack_pop(stack);
        if (isTerminal(cur->label)) {
            bool result = match(cur->label);
            if (!result) return NULL;
        } else {
            cur_root = cur;
            int category = tb->table[cur->label][(int)string[i]];
            if (category == -1) return NULL;

            int children_num = tb->row_lengths[category];
            for (int j = 0; j < children_num; j++) {
                char cur_char = (char) tb->dictionary[category][j];
                Stack_push(makeNode0(cur_char));
            }

            // create sub tree with cur_root as root
            TREE prev_child;
            for (int i = 0; i < children_num; i++) {
                TREE child = Stack_peek(stack, i);
                if (cur_root->leftmostChild == NULL) {
                    cur_root->leftmostChild = child;
                    prev_child = child;
                    continue;
                }
                prev_child->rightSibling = child;
                prev_child = child;

            }

        }
    }

    // free stack

    if (length != i) return NULL;
    return root;

}

bool isTerminal(char c) {
    return c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '^' || c == '{' || c == '}' || c == 'U' || c == ',' || c == '*';
}



int main() {
    if (parse_set_alg("{46,789876,2}")) {
        printf("Success case reached. Were we supposed to?");
    }

    // creating explicit parse table
    ParseTable set_alg = (ParseTable)malloc(sizeof(struct ParseTable));
    set_alg->table = (int**)malloc(128 * sizeof(*int));


    // setting parsing table
    for (int i = 0; i < 128; i++) {
        set_alg->table[i] = (int*)malloc(128 * sizeof(int));
        for (int j = 0; j < 128; j++) {
            set_alg->table[i][j] = -1;
        }
    }
    
    // 0: ⟨Expr⟩ → ⟨Atomic⟩ ⟨ExprTail⟩
    int E = (int)'E';         
    // for (int i = 0; i < 128; i++) {
    //     set_alg->table[E][i] = 0;
    // }     
    set_alg->table[E][(int)'('] = 1;

    // 1: ⟨ExprTail⟩ → U ⟨Expr ⟩ | ^ ⟨Expr ⟩ | ϵ
    // 2: ⟨ExprTail⟩ → ^ ⟨Expr ⟩ | ϵ
    // -2: ⟨ExprTail⟩ → ϵ
    int e = (int)'e';
    for (int i = 0; i < 128; i++) {
        set_alg->table[E][i] = -2; // special value where nothing is pushed to stack
    }

    // 2: ⟨Atomic⟩ → ( ⟨Expr ⟩ ) | ⟨Set⟩
    // 3: ⟨Set⟩ → { ⟨SetTail⟩                        
    // 4: ⟨SetTail⟩ → } | ⟨Elements⟩ }
    // 5: ⟨Elements⟩ → ⟨Element⟩ ⟨ElementsTail⟩      
    // 6: ⟨ElementsTail⟩ → , ⟨Elements⟩ | ϵ
    // 7: ⟨Element⟩ → ⟨Number⟩                      
    // 8: ⟨Number⟩ → ⟨Digit⟩ ⟨NumberTail⟩           
    // 9: ⟨NumberTail⟩ → ⟨Number ⟩ | ϵ
    // 10: ⟨Digit⟩ → 0 | 1 | · · · | 9  


}