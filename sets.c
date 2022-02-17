#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sets.h"

char *string;
int i;
int length;

// <expression> -> <atomic> <expression tail>
bool expression() {
    if (!atomic()) return 0;
    if (!expr_tail()) return 0;

    // parse tree handling 
    return 1;

}

// <expression tail> -> U <expression> | ^ <expression> | ϵ
bool expr_tail() { // could cause problems
    if (lookahead('U')) {
        match('U');
        if (!expression()) return 0;
        return 1;
    } else if (lookahead('^')) {
        match('^');
        if (!expression()) return 0;
        return 1;   
    } else {
        return 1; 
    }
}

// <atomic> -> (<expression>) | <set>
bool atomic() {
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
bool set() {
    if (!match('{')) return 0;
    if (!set_tail()) return 0;

    return 1;
}

// <set> -> { <set tail>
bool set_tail() {
    if (lookahead('}')) {
        return match('}');
    } else {
        if (!elements()) return 0;
        if (!match('}')) return 0;
        return 1; 
    }
}

// <elements> -> <element> <elements tail>
bool elements() {
   if (!element()) return 0;
   if (!elements_tail()) return 0;
   return 1;
}

// <elements tail> -> , <elements> | ϵ
bool elements_tail() { // causing problems?
    if (lookahead(',')) {
        match(','); // could be formalized like the other if statements
        if (!elements()) return 0; // could be condensed 
        return 1;

    } else {
        return 1;
    }
}

// <element> -> <number>
bool element() {
    return number();
}

// <Number> -> <digit> <number tail>
bool number() {
    if (!digit()) return 0;
    if (!number_tail()) return 0;
    return 1;
}

// <number tail> -> <number> | ϵ
bool number_tail() { // might cause problems
    if (is_digit()) { // lookahead in this scenario
        number();
        return 1;
    } else {
        return 1;
    }
}

// <digit> -> 0 | 1 | · · · | 9
bool digit() {
    bool isDigit = is_digit();
    if (isDigit) return match(string[i]);
    return 0;
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
    return expression() && length == i; // may cause problems
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