#include <stdio.h>
#include "tokenizer.h"
#include "value.h"
#include "linkedlist.h"
#include "parser.h"
#include "talloc.h"
#include "interpreter.h"
#include <unistd.h>

int main() {
    if(isatty(fileno(stdin))){
        printf("isatty\n");
    }
    else{
        printf("not isatty\n");
    }
    Value *list = tokenize(stdin);
    //displayTokens(list);
    Value *tree = parse(list);
   //printf("Here\n");
    //printTree(tree);
    interpret(tree);
    tfree();
    return 0;
}