Interpreter for Racket code made in C. 

It is split up into 3 different sections:
  1) Tokenizer (tokenizer.c)
  2) Parser (parser.c)
  3) Interpreter (interpreter.c)

Takes in a name of a Racket text file as command-line argument and prints results out to stdout. 

talloc.c makes a "tracked allocation" of memory so I don't have to worry about freeing things at the end of execution.
value.h provides a struct for keeping track of our tokens after tokenization. 
linkedlist.h proviedes a struct for a linkedlist of values as one might imagine.

Unfortunately, I was not able to finish a REPL (Read evaluate print loop) so that we could interpret on the fly in the command-line. (repl.c)
