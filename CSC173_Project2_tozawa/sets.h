#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"


// ⟨Expression⟩ → ⟨Expression⟩ U ⟨Expression⟩
// ⟨Expression⟩ → ⟨Expression⟩ ^ ⟨Expression⟩
// ⟨Expression⟩ → ( ⟨Expression⟩ )
// ⟨Expression⟩ → ⟨Set⟩
// ⟨Set⟩ → {}
// ⟨Set⟩ → { ⟨Elements⟩ }
// ⟨Elements⟩ → ⟨Element⟩
// ⟨Elements⟩ → ⟨Elements⟩ , ⟨Element⟩
// ⟨Element⟩ → ⟨Number ⟩
// ⟨Number ⟩ → ⟨Digit⟩
// ⟨Number ⟩ → ⟨Number ⟩ ⟨Digit⟩
// ⟨Digit⟩ → 0 | 1 | · · · | 9            

// Functions which represent the syntactice cateogires above, returning a subtree when successful 
// and NULL when parse fails.
extern TREE expression();
extern TREE expr_tail();
extern TREE atomic();
extern TREE set();
extern TREE set_tail();
extern TREE elements();
extern TREE element();
extern TREE elements_tail();
extern TREE number();
extern TREE number_tail();
extern TREE digit();

// Checks if the current char is a digit
extern bool is_digit();

// checks if the current char is the char inputed
extern bool lookahead(char c);

// checks if the current char is the char inputed and consumes it if it is
extern bool match(char c);

// checks if the input string is at the end 
extern bool at_end();

extern bool isTerminal(char c);

