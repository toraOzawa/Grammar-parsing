#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char *string;
int index;
int length;

bool expression() {
    if (!atomic()) return 0;;
    if (!expr_tail()) return 0;

    // parse tree handling 
    return 1;

}

bool expr_tail() { // could cause problems
    if (lookahead('U')) {
        match('U');
        if (!expression()) return 0;
        return 1;
    } else if (lookadhead('^')) {
        match('^');
        if (!expression()) return 0;
        return 1;   
    } else {
        return 1; 
    }
}

// <atomic>  -> (<expression>) | <set>
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

bool set() {
    if (!match('{')) return 0;
    if (!set_tail()) return 0;

    return 1;
}

bool set_tail() {
    if (lookahead('}')) {
        return match('}');
    } else {
        if (!elements()) return 0;
        if (!match('}')) return 0;
        return 1; 
    }
}

bool elements() {
   if (!element()) return 0;
   if (!elements_tail()) return 0;
   return 1;
}

bool element() {
    return number();
}

bool elements_tail() {
    if (lookahead(',')) {
        match(','); // could be formalized like the other if statements
        if (!elements()) return 0; // could be condensed 
        return 1;

    } else {
        return 1
    }
}

bool number() {
    if (!digit()) return 0;
    if (!number_tail()) return 0;
    return 1;
}

bool number_tail() { // might cause problems 
    if (!number()) return 0;
    return 1;
}

// <Digit> -> 0 | 1 | · · · | 9
bool digit() {
    bool isDigit = string[index] == 0 || string[index] == 1 || string[index] == 2 || string[index] == 3 || string[index] == 4 ||
           string[index] == 5 || string[index] == 6 || string[index] == 7 || string[index] == 8 || string[index] == 9;
    if (isDigit) return match(string[index]);
    return 0;
}

bool parse_set_alg(char *c) {
    string = input;
    index = 0;
    length = strlen(c);
    return expression();
}

bool lookahead(char c) {
    return string[index] == c && index < length;
}

bool match(char c) {
    index++;
    return string[index - 1] == c && (index - 1) < length;
}

int main() {
    if (parse_set_alg("")) {
        printf("Success case? did we succeed>");
    }
}