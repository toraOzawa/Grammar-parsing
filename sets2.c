#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sets2.h"

char *string;
int i;
int length;
TREE root;

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
    i++;
    return string[i - 1] == c && (i - 1) < length;
}

int main() {
    if (parse_set_alg("{46,789876,2}")) {
        printf("Success case reached. Were we supposed to?");
    }
}