Tora Ozawa
tozawa@u.rochester.edu
32086188
Project 2 CSC 173 
Partners: none 

Project can be built and run by using a standard compile command for all the files and running the produced executable.

In other words a command such as: "gcc -std=c99 -Wall -Werror sets.c LinkedList.c tree.c Stack.c"
followed by running the resulting executable normally. 

The LinkedList file is the same file used and provided for project 1, and the Stack file is a simple wrapped structure of the 
LinkedList struct. Moreover as noted in the comments, tree.c is taken from the textbook. 

As noted in the comments of the file, the ParseTable uses multiple tables to represent several things in the parse tables seen 
in class. It uses a 2d array for integers to represent the characters which are to represent syntactic categories and possible 
inputs. It is very similar to a transition table from project 1. Of course this does make for some extra used space, however
given the fixed size and the fact that it is storing integers I deemed it as negligible. It uses another 2d array, where each 
row represents a numbered syntactic category, with the entire row holding the ASCII values of the characters to be pushed on 
to the stack. Finally, an array of ints is also used to keep track of the size of each row. The explicit table for the grammar
in the assignment is hard coded in the function set_parse_table() (I do apologize that it is done line by line and not by 
function calls, making it quite long). 

The main function is in the file sets.c as instructed. Users will able to execute a REPL when project is built. 

