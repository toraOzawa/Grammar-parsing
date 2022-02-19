
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


// 0: ⟨Expr ⟩ → ⟨Atomic⟩ ⟨ExprTail⟩              (add to tree)
// 1: ⟨ExprTail⟩ → U ⟨Expr ⟩ | ^ ⟨Expr ⟩ | ϵ
// 2: ⟨Atomic⟩ → ( ⟨Expr ⟩ ) | ⟨Set⟩
// 3: ⟨Set⟩ → { ⟨SetTail⟩                        (add to tree)
// 4: ⟨SetTail⟩ → } | ⟨Elements⟩ }
// 5: ⟨Elements⟩ → ⟨Element⟩ ⟨ElementsTail⟩      (add to tree)
// 6: ⟨ElementsTail⟩ → , ⟨Elements⟩ | ϵ
// 7: ⟨Element⟩ → ⟨Number⟩                      (add to tree)
// 8: ⟨Number ⟩ → ⟨Digit⟩ ⟨NumberTail⟩           (add to tree)
// 9: ⟨NumberTail⟩ → ⟨Number ⟩ | ϵ
// 10: ⟨Digit⟩ → 0 | 1 | · · · | 9                 (add to tree)

extern TREE expression();

extern bool expr_tail();

extern bool atomic();

extern TREE set();

extern bool set_tail();

extern TREE elements();

extern TREE element();

extern bool elements_tail();


extern TREE number();

extern bool number_tail();


extern TREE digit();

extern TREE is_digit();

// checks if the current char is the char inputed
extern TREE lookahead(char c);

// checks if the current char is the char inputed and consumes it if it is
extern TREE match(char c);

// checks if the input string is at the end 
extern TREE at_end();
