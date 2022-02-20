
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


// 0: ⟨Expr⟩ → ⟨Atomic⟩ ⟨ExprTail⟩              
// 1: ⟨ExprTail⟩ → U ⟨Expr ⟩ | ^ ⟨Expr ⟩ | ϵ
// 2: ⟨Atomic⟩ → ( ⟨Expr ⟩ ) | ⟨Set⟩
// 3: ⟨Set⟩ → { ⟨SetTail⟩                        
// 4: ⟨SetTail⟩ → } | ⟨Elements⟩ }
// 5: ⟨Elements⟩ → ⟨Element⟩ ⟨ElementsTail⟩      
// 6: ⟨ElementsTail⟩ → , ⟨Elements⟩ | ϵ
// 7: ⟨Element⟩ → ⟨Number⟩                      
// 8: ⟨Number⟩ → ⟨Digit⟩ ⟨NumberTail⟩           
// 9: ⟨NumberTail⟩ → ⟨Number ⟩ | ϵ
// 10: ⟨Digit⟩ → 0 | 1 | · · · | 9                 

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

extern bool is_digit();

// checks if the current char is the char inputed
extern bool lookahead(char c);

// checks if the current char is the char inputed and consumes it if it is
extern bool match(char c);

// checks if the input string is at the end 
extern bool at_end();

extern bool isTerminal(char c);
