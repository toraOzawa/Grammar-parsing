#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "sets.h"
#include "Stack.h"

char *string; // string pointer passed in to parsing function
int i; // string index
int length; // string length
TREE root; // root of the parse tree used globablly in function

char input[60]; // actual input string (60 char limit)

typedef struct ParseTable* ParseTable;

struct ParseTable {
    int** table; // Traditional "parse table"
    int** dictionary; // i = syntactic category label/number; dictionary[i][j] = jth terminal/category to push on stack for ith category
    int* row_lengths; // keeps track of row lengths of dictionary
};

// Functions below each represent the syntactic categories above theisr names 

// ⟨Expr⟩ → ⟨Atomic⟩ ⟨ExprTail⟩                   
TREE expression() {
    TREE atom = atomic();
    if (atom == NULL) return NULL;
    TREE tail  = expr_tail();
    if (tail == NULL)  { 
        TREE_free(atom);
        return NULL; 
    }

    return makeNode4('E', atom, tail, NULL, NULL);

}
             
// ⟨ExprTail⟩ → U ⟨Expr ⟩ | ^ ⟨Expr ⟩ | ϵ    
TREE expr_tail() { // could cause problems
    if (lookahead('U')) {
        match('U');
        TREE exp = expression();
        if (exp == NULL) return NULL;
        return makeNode4('e', makeNode0('U'), exp, NULL, NULL); 
    } else if (lookahead('^')) {
        match('^');
        TREE exp = expression();
        if (exp == NULL) return NULL;
        return makeNode4('e', makeNode0('^'), exp, NULL, NULL);   
    } else {
        return makeNode1('e', makeNode0(':')); 
    }
}

// ⟨Atomic⟩ → ( ⟨Expr ⟩ ) | ⟨Set⟩
TREE atomic() {
    if (lookahead('(')) {
        if (!match('(')) return NULL;
        TREE exp = expression();
        if (exp == NULL) return NULL;
        if (!match(')')) {
            TREE_free(exp);
            return NULL;
        }
        return makeNode4('a', makeNode0('('), exp, makeNode0(')'), NULL);
    } else {
        TREE s = set();
        if (s == NULL) return NULL; 
        return makeNode1('a', s); 
    }
}

// ⟨Set⟩ → { ⟨SetTail⟩                        
TREE set() {
    if (!match('{')) return NULL;
    TREE tail = set_tail();
    if (tail == NULL) return NULL;

    return makeNode4('S', makeNode0('{'), tail, NULL, NULL);
}

// ⟨SetTail⟩ → } | ⟨Elements⟩ }
TREE set_tail() {
    if (lookahead('}')) {
        match('}');
        return makeNode1('s', makeNode0('}'));
    } else {
        TREE elem = elements();
        if (elem == NULL) return NULL;
        if (!match('}'))  { 
            TREE_free(elem);
            return NULL; 
        }
        return makeNode4('s', elem, makeNode0('}'), NULL, NULL); 
    }
}

// 5: ⟨Elements⟩ → ⟨Element⟩ ⟨ElementsTail⟩       
TREE elements() {
   TREE elem = element();
   if (elem == NULL) return NULL;
   TREE tail = elements_tail();
   if (tail == NULL)  { 
       TREE_free(elem);
       return NULL; 
    }

   return makeNode4('L', elem, tail, NULL, NULL);
}
     
// 6: ⟨ElementsTail⟩ → , ⟨Elements⟩ | ϵ  
TREE elements_tail() { // causing problems?
    if (lookahead(',')) {
        match(','); // could be formalized like the other if statements
        TREE elems = elements();
        if (elems == NULL) return NULL; // could be condensed 

        return makeNode4('t', makeNode0(','), elems, NULL, NULL);

    } else {
        return makeNode1('t', makeNode0(':'));
    }
}

// ⟨Element⟩ → ⟨Number⟩                      
TREE element() {
    TREE num = number();
    if (num == NULL) return NULL;
    return makeNode1('l', num);
}

// ⟨Number⟩ → ⟨Digit⟩ ⟨NumberTail⟩           
TREE number() {
    TREE dig = digit();
    if (dig == NULL) return NULL;
    TREE tail = number_tail();
    if (tail == NULL)  { 
        TREE_free(dig);
        return NULL; 
    }

    
    return makeNode4('N', dig, tail, NULL, NULL);
}

// ⟨NumberTail⟩ → ⟨Number ⟩ | ϵ
TREE number_tail() { // might cause problems
    if (is_digit()) { // lookahead in this scenario
        return makeNode1('n', number());
    } else {
        return makeNode1('n', makeNode0(':'));
    }
}

// ⟨Digit⟩ → 0 | 1 | · · · | 9 
TREE digit() {
    bool isDigit = is_digit();
    if (isDigit) { 
        match(string[i]);
        return makeNode1('D', makeNode0(string[i-1]));
    } else {
        return NULL;
    }
}

bool is_digit() {
    return string[i] == '0' || string[i] == '1' || string[i] == '2' || string[i] == '3' || string[i] == '4' || string[i] == '5' || string[i] == '6' || string[i] == '7' || string[i] == '8' || string[i] == '9';
}

// Function to parse by recursive descent
TREE parse_set_alg(char *input) {
    string = input;
    i = 0;
    length = strlen(string);
    printf("input length: %d\n", length);
    root = expression();
    if (length != i)  { 
        if (root == NULL) return NULL;
        TREE_free(root);
        return NULL; 
    }
    return root;
}

bool lookahead(char c) {
    return string[i] == c && i < length;
}

bool match(char c) {
    if (c == ':') return true; // special case used in table driven parser
    i++;
    return string[i - 1] == c && (i - 1) < length;
}



TREE table_parse_set_alg(ParseTable tb, char *input) {
    string = input;
    i = 0;
    length = strlen(string);
    Stack stack = new_Stack();

    root = makeNode0('E');
    Stack_push(stack, root);
    TREE cur_root = root; // cur root represents highest syntactic category in stack when in use

    // While stack in not empty....
    while (!Stack_isEmpty(stack)) {
        TREE cur = Stack_pop(stack); // Pop the stack
        if (isTerminal(cur->label)) { // If it is a terminal....
            bool result = match(cur->label); // Try to match the input
            if (!result) { // If the match fails
                Stack_free(stack, false);
                TREE_free(root);
                return NULL; // return null if the match fails
            }
        } else { // If the top of the stack was not a terminal......
            cur_root = cur;
            int category = tb->table[(int)cur->label][(int)string[i]]; // Find the number of the syntactic category
            if (category == -1) { 
                Stack_free(stack, false);
                TREE_free(root);
                return NULL; // Return null if category is not found
            }

            int children_num = tb->row_lengths[category];
            for (int j = 0; j < children_num; j++) {
                char cur_char = (char) tb->dictionary[category][j];
                Stack_push(stack, makeNode0(cur_char));
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
    Stack_free(stack, false);
    printf("%d\n", i);
    if (length != i) { 
        if (root == NULL) return NULL;
        TREE_free(root);
        return NULL; 
    }
    return root;

}

// Checks if input char is terminal
bool isTerminal(char c) {
    return c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '^' || c == '{' || c == '}' || c == 'U' || c == ',' || c == ':' || c == '(' || c == ')';
}

// Creates and returns parse table for Set algebra expressions (must free manually)
// Table is designed in a way which creating a framework to create grammars would be relatively similar 
// Thus some excess space (both in lines of code and memory) is used, in return for some speed and flexibility, 
// although the latter is not needed for this project
ParseTable set_parse_table() {
    ParseTable set_alg = (ParseTable)malloc(sizeof(struct ParseTable));
    set_alg->table = (int**)malloc(128 * sizeof(int*));
    set_alg->dictionary = (int**)malloc(128 * sizeof(int*));
    set_alg->row_lengths = (int*)malloc(25 * sizeof(int*));


    // allocating and setting parsing table defaults
    for (int i = 0; i < 128; i++) {
        set_alg->table[i] = (int*)malloc(128 * sizeof(int));
        for (int j = 0; j < 128; j++) {
            set_alg->table[i][j] = -1;
        }
    }

    // manually setting parsing table and dictionary for set of alegbra expressions
    int E = (int) 'E';  // Expr
    int e = (int) 'e';  // ExprTail
    int a = (int) 'a'; // atomic
    int S = (int) 'S'; // Set
    int s = (int) 's'; // SetTail
    int L = (int) 'L'; // Elements
    int t = (int) 't'; // ElementsTail
    int l = (int) 'l'; // Element
    int N = (int) 'N'; // Number
    int n = (int) 'n'; // NumberTail
    int D = (int) 'D'; // Digit

    int epsilon = (int) ':';

    // 0: ⟨Expr⟩ → ⟨Atomic⟩ ⟨ExprTail⟩
    set_alg->row_lengths[0] = 2;
    set_alg->dictionary[0] = (int*)malloc(2 * sizeof(int));
    set_alg->dictionary[0][0] = e;
    set_alg->dictionary[0][1] = a;


    set_alg->table[E][(int)'('] = 0;
    set_alg->table[E][(int)'{'] = 0;


    // 1: ⟨ExprTail⟩ → U ⟨Expr⟩ 
    // 2: ⟨ExprTail⟩ → ^ ⟨Expr⟩
    // 3: ⟨ExprTail⟩ → ϵ
    set_alg->row_lengths[1] = 2;
    set_alg->dictionary[1] = (int*)malloc(2 * sizeof(int));
    set_alg->dictionary[1][0] = E;
    set_alg->dictionary[1][1] = (int)'U';

    set_alg->row_lengths[2] = 2;
    set_alg->dictionary[2] = (int*)malloc(2 * sizeof(int));
    set_alg->dictionary[2][0] = E;
    set_alg->dictionary[2][1] = (int)'^';

    set_alg->row_lengths[3] = 1;
    set_alg->dictionary[3] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[3][0] = epsilon;

    for (int i = 0; i < 128; i++) {
        set_alg->table[e][i] = 3; 
    }
    set_alg->table[e][(int)'U'] = 1;
    set_alg->table[e][(int)'^'] = 2;


    // 4: ⟨Atomic⟩ → ( ⟨Expr⟩ )
    // 5: ⟨Atomic⟩ → ⟨Set⟩
    set_alg->row_lengths[4] = 3;
    set_alg->dictionary[4] = (int*)malloc(3 * sizeof(int));
    set_alg->dictionary[4][0] = (int)')';
    set_alg->dictionary[4][1] = E;
    set_alg->dictionary[4][2] = (int)'(';

    set_alg->row_lengths[5] = 1;
    set_alg->dictionary[5] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[5][0] = S;


    set_alg->table[a][(int)'('] = 4;
    set_alg->table[a][(int)'{'] = 5;


    // 6: ⟨Set⟩ → { ⟨SetTail⟩
    set_alg->row_lengths[6] = 2;
    set_alg->dictionary[6] = (int*)malloc(2 * sizeof(int));
    set_alg->dictionary[6][0] = s;
    set_alg->dictionary[6][1] = (int) '{';

    set_alg->table[S][(int)'{'] = 6;     


    // 7: ⟨SetTail⟩ → } 
    // 8: ⟨SetTail⟩ → ⟨Elements⟩ }
    set_alg->row_lengths[7] = 1;
    set_alg->dictionary[7] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[7][0] = (int) '}';

    set_alg->row_lengths[8] = 2;
    set_alg->dictionary[8] = (int*)malloc(2 * sizeof(int));
    set_alg->dictionary[8][0] = (int) '}';
    set_alg->dictionary[8][1] = L;


    set_alg->table[s][(int)'}'] = 7; 
    set_alg->table[s][(int)'0'] = 8; 
    set_alg->table[s][(int)'1'] = 8; 
    set_alg->table[s][(int)'2'] = 8; 
    set_alg->table[s][(int)'3'] = 8; 
    set_alg->table[s][(int)'4'] = 8; 
    set_alg->table[s][(int)'5'] = 8; 
    set_alg->table[s][(int)'6'] = 8; 
    set_alg->table[s][(int)'7'] = 8; 
    set_alg->table[s][(int)'8'] = 8; 
    set_alg->table[s][(int)'9'] = 8; 

    
    // 9: ⟨Elements⟩ → ⟨Element⟩ ⟨ElementsTail⟩  
    set_alg->row_lengths[9] = 2;
    set_alg->dictionary[9] = (int*)malloc(2 * sizeof(int));
    set_alg->dictionary[9][0] = t;
    set_alg->dictionary[9][1] = l;


    set_alg->table[L][(int)'0'] = 9; 
    set_alg->table[L][(int)'1'] = 9; 
    set_alg->table[L][(int)'2'] = 9; 
    set_alg->table[L][(int)'3'] = 9; 
    set_alg->table[L][(int)'4'] = 9; 
    set_alg->table[L][(int)'5'] = 9; 
    set_alg->table[L][(int)'6'] = 9; 
    set_alg->table[L][(int)'7'] = 9; 
    set_alg->table[L][(int)'8'] = 9; 
    set_alg->table[L][(int)'9'] = 9; 


    // 10: ⟨ElementsTail⟩ → , ⟨Elements⟩
    // 11: ⟨ElementsTail⟩ → ϵ
    set_alg->row_lengths[10] = 2;
    set_alg->dictionary[10] = (int*)malloc(2 * sizeof(int));
    set_alg->dictionary[10][0] = L;
    set_alg->dictionary[10][1] = (int)',';

    set_alg->row_lengths[11] = 1;
    set_alg->dictionary[11] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[11][0] = epsilon;


    for (int i = 0; i < 128; i++) {
        set_alg->table[t][i] = 11; 
    }
    set_alg->table[t][(int)','] = 10; 


    // 12: ⟨Element⟩ → ⟨Number⟩    
    set_alg->row_lengths[12] = 1;
    set_alg->dictionary[12] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[12][0] = N;


    set_alg->table[l][(int)'0'] = 12; 
    set_alg->table[l][(int)'1'] = 12; 
    set_alg->table[l][(int)'2'] = 12; 
    set_alg->table[l][(int)'3'] = 12; 
    set_alg->table[l][(int)'4'] = 12; 
    set_alg->table[l][(int)'5'] = 12; 
    set_alg->table[l][(int)'6'] = 12; 
    set_alg->table[l][(int)'7'] = 12; 
    set_alg->table[l][(int)'8'] = 12; 
    set_alg->table[l][(int)'9'] = 12; 


    // 13: ⟨Number⟩ → ⟨Digit⟩ ⟨NumberTail⟩   
    set_alg->row_lengths[13] = 2;
    set_alg->dictionary[13] = (int*)malloc(2 * sizeof(int));
    set_alg->dictionary[13][0] = n;
    set_alg->dictionary[13][1] = D;


    set_alg->table[N][(int)'0'] = 13; 
    set_alg->table[N][(int)'1'] = 13; 
    set_alg->table[N][(int)'2'] = 13; 
    set_alg->table[N][(int)'3'] = 13; 
    set_alg->table[N][(int)'4'] = 13; 
    set_alg->table[N][(int)'5'] = 13; 
    set_alg->table[N][(int)'6'] = 13; 
    set_alg->table[N][(int)'7'] = 13; 
    set_alg->table[N][(int)'8'] = 13; 
    set_alg->table[N][(int)'9'] = 13; 


    // 14: ⟨NumberTail⟩ → ⟨Number⟩
    // 15: ⟨NumberTail⟩ → ϵ
    set_alg->row_lengths[14] = 1;
    set_alg->dictionary[14] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[14][0] = N;

    set_alg->row_lengths[15] = 1;
    set_alg->dictionary[15] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[15][0] = epsilon;


    for (int i = 0; i < 128; i++) {
        set_alg->table[n][i] = 15; 
    }
    set_alg->table[n][(int)'0'] = 14;
    set_alg->table[n][(int)'1'] = 14; 
    set_alg->table[n][(int)'2'] = 14; 
    set_alg->table[n][(int)'3'] = 14; 
    set_alg->table[n][(int)'4'] = 14; 
    set_alg->table[n][(int)'5'] = 14; 
    set_alg->table[n][(int)'6'] = 14; 
    set_alg->table[n][(int)'7'] = 14; 
    set_alg->table[n][(int)'8'] = 14; 
    set_alg->table[n][(int)'9'] = 14; 


    // 16: ⟨Digit⟩ → 0   
    // 17: ⟨Digit⟩ → 1  
    // 18: ⟨Digit⟩ → 2 
    // 19: ⟨Digit⟩ → 3 
    // 20: ⟨Digit⟩ → 4 
    // 21: ⟨Digit⟩ → 5 
    // 22: ⟨Digit⟩ → 6
    // 23: ⟨Digit⟩ → 7
    // 24: ⟨Digit⟩ → 8
    // 25: ⟨Digit⟩ → 9
    set_alg->row_lengths[16] = 1;
    set_alg->dictionary[16] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[16][0] = (int)'0';

    set_alg->row_lengths[17] = 1;
    set_alg->dictionary[17] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[17][0] = (int)'1';

    set_alg->row_lengths[18] = 1;
    set_alg->dictionary[18] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[18][0] = (int)'2';

    set_alg->row_lengths[19] = 1;
    set_alg->dictionary[19] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[19][0] = (int)'3';

    set_alg->row_lengths[20] = 1;
    set_alg->dictionary[20] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[20][0] = (int)'4';

    set_alg->row_lengths[21] = 1;
    set_alg->dictionary[21] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[21][0] = (int)'5';

    set_alg->row_lengths[22] = 1;
    set_alg->dictionary[22] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[22][0] = (int)'6';

    set_alg->row_lengths[23] = 1;
    set_alg->dictionary[23] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[23][0] = (int)'7';

    set_alg->row_lengths[24] = 1;
    set_alg->dictionary[24] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[24][0] = (int)'8';

    set_alg->row_lengths[25] = 1;
    set_alg->dictionary[25] = (int*)malloc(1 * sizeof(int));
    set_alg->dictionary[25][0] = (int)'9';


    set_alg->table[D][(int)'0'] = 16; 
    set_alg->table[D][(int)'1'] = 17; 
    set_alg->table[D][(int)'2'] = 18; 
    set_alg->table[D][(int)'3'] = 19; 
    set_alg->table[D][(int)'4'] = 20; 
    set_alg->table[D][(int)'5'] = 21; 
    set_alg->table[D][(int)'6'] = 22; 
    set_alg->table[D][(int)'7'] = 23; 
    set_alg->table[D][(int)'8'] = 24; 
    set_alg->table[D][(int)'9'] = 25; 

    return set_alg;
}


int main() {

    // Printing key for parse tree
    printf("Key for parse tree:\n");
    printf("E = Expression,\n");
    printf("e = ExprTail,\n");
    printf("a = atomic,\n");
    printf("S = Set,\n");
    printf("s = SetTail,\n");
    printf("L = Elements,\n");
    printf("t = ElementsTail,\n");
    printf("l = element,\n");
    printf("N = Number,\n");
    printf("n = NumberTail,\n");
    printf("D = Digit,\n");
    printf(": = epsilon (using \":\" to test for failures may cause undefined behavior)\n\n");

   // Recursive descent parser REPL
    printf("Recursive descent parser: \n");
    printf("Enter an expression to parse (\"next\" to move on): ");
    scanf("%59s", input);
    printf("\n");
    while (strcmp(input,"next") != 0) {
        TREE result = parse_set_alg(input);
        if (result == NULL) {
            printf("Parse failed...\n");
        } else {
            printf("Parse successful!\n");
            printf("Parse Tree:\n");
            TREE_pretty_print(result, 0);
            printf("\n");
            TREE_free(result);
        }
        printf("Enter next expression (\"next\" to move on):");
        scanf("%59s", input);
    }
    printf("\n\n");


    // Table driven parsing REPL
    ParseTable set_alg = set_parse_table();
    printf("Table driven parser: \n");
    printf("Enter an expression to parse (\"done\" to move terminate program): ");
    scanf("%59s", input);
    printf("\n");
    while (strcmp(input,"done") != 0) {
        TREE result = table_parse_set_alg(set_alg, input);
        if (result == NULL) {
            printf("Parse failed...\n");
        } else {
            printf("Parse successful!\n");
            printf("Parse Tree:\n");
            TREE_pretty_print(result, 0);
            printf("\n");
            TREE_free(result);
        }
        printf("Enter next expression (\"done\" to terminate program):");
        scanf("%59s", input);
    }

    // Freeing parse table manually
    free(set_alg->row_lengths);
    for (int i = 0; i < 128; i++) {
        free(set_alg->table[i]);
    }
    free(set_alg->table);

    for (int i = 0; i < 26; i++) {
        free(set_alg->dictionary[i]);
    }
    free(set_alg->dictionary);
    free(set_alg);

}