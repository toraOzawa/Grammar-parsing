
#include <stdlib.h>
#include <stdbool.h>

extern bool expression();

extern bool expr_tail();

extern bool atomic();

extern bool set();

extern bool set_tail();

extern bool elements();

extern bool element();

extern bool elements_tail();


extern bool number();

extern bool number_tail();


extern bool digit();

// checks if the current char is the char inputed
extern bool lookahead(char c);

// checks if the current char is the char inputed and consumes it if it is
extern bool match(char c);

// checks if the input string is at the end 
extern at_end();
