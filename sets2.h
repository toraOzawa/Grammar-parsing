
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


// ⟨Expr ⟩ → ⟨Atomic⟩ ⟨ExprTail⟩              (add to tree)
// ⟨ExprTail⟩ → U ⟨Expr ⟩ | ^ ⟨Expr ⟩ | ϵ
// ⟨Atomic⟩ → ( ⟨Expr ⟩ ) | ⟨Set⟩
// ⟨Set⟩ → { ⟨SetTail⟩                        (add to tree)
// ⟨SetTail⟩ → } | ⟨Elements⟩ }
// ⟨Elements⟩ → ⟨Element⟩ ⟨ElementsTail⟩      (add to tree)
// ⟨ElementsTail⟩ → , ⟨Elements⟩ | ϵ
// ⟨Element⟩ → ⟨Number⟩                      (add to tree)
// ⟨Number ⟩ → ⟨Digit⟩ ⟨NumberTail⟩           (add to tree)
// ⟨NumberTail⟩ → ⟨Number ⟩ | ϵ
// ⟨Digit⟩ → 0 | 1 | · · · | 9                 (add to tree)

extern TREE expression();

extern TREE expr_tail();

extern TREE atomic();

extern TREE set();

extern TREE set_tail();

extern TREE elements();

extern TREE element();

extern bool elements_tail();


extern bool number();

extern bool number_tail();


extern bool digit();

extern bool is_digit();

// checks if the current char is the char inputed
extern bool lookahead(char c);

// checks if the current char is the char inputed and consumes it if it is
extern bool match(char c);

// checks if the input string is at the end 
extern bool at_end();

#include <stdlib.h>
#include <stdbool.h>


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


// ⟨Expr ⟩ → ⟨Atomic⟩ ⟨ExprTail⟩              (add to tree)
// ⟨ExprTail⟩ → U ⟨Expr ⟩ | ^ ⟨Expr ⟩ | ϵ
// ⟨Atomic⟩ → ( ⟨Expr ⟩ ) | ⟨Set⟩
// ⟨Set⟩ → { ⟨SetTail⟩                        (add to tree)
// ⟨SetTail⟩ → } | ⟨Elements⟩ }
// ⟨Elements⟩ → ⟨Element⟩ ⟨ElementsTail⟩      (add to tree)
// ⟨ElementsTail⟩ → , ⟨Elements⟩ | ϵ
// ⟨Element⟩ → ⟨Number⟩                      (add to tree)
// ⟨Number ⟩ → ⟨Digit⟩ ⟨NumberTail⟩           (add to tree)
// ⟨NumberTail⟩ → ⟨Number ⟩ | ϵ
// ⟨Digit⟩ → 0 | 1 | · · · | 9                 (add to tree)

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

extern bool is_digit();

// checks if the current char is the char inputed
extern bool lookahead(char c);

// checks if the current char is the char inputed and consumes it if it is
extern bool match(char c);

// checks if the input string is at the end 
extern bool at_end();
