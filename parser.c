#include "tokenizer.h"
#include "value.h"
#include "talloc.h"
#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void throwError(char *str){
    printf("%s\n", str);
    texit(0);
}

Value *addToParseTree(Value *tree, int depth, Value *token){
    Value *current = tree;
    if(token->type == CLOSE_TYPE){
        return tree;
    }
    if(current->type == NULL_TYPE){
        //make Cons cell 
        // whose cdr is tree; return Cons cell
        if(token->type == OPEN_TYPE){
            return cons(makeNull(),current);
        }
        else if(token->type != CLOSE_TYPE){
            return cons(token, current);
        }
    }
    
    while(cdr(current)->type != NULL_TYPE){
        current = cdr(current);
    }
    
    if(current->type == CONS_TYPE){
        if(depth == 0){
            if(token->type != OPEN_TYPE && token->type !=                            CLOSE_TYPE){
                Value *temp = talloc(sizeof(Value));
                temp->type = CONS_TYPE;
                temp->c.cdr = current->c.cdr;
                temp->c.car = token;
                current->c.cdr = temp;

            }
            else if(token->type == OPEN_TYPE){
                Value *temp = talloc(sizeof(Value));
                temp->type = CONS_TYPE;
                temp->c.cdr = cdr(current);
                temp->c.car = makeNull();
                current->c.cdr = temp;
                return tree;
            }
        }
        else{
            current->c.car = addToParseTree(car(current),depth-1, token);
        } 
    }
    else{
        throwError("We messed up somewhere\n");
    }
    return tree;
}

void printTree2(Value *tree){
    switch(tree->type){
        case CONS_TYPE:
            if(car(tree)->type == CONS_TYPE || car(tree)->type == NULL_TYPE){
                printf("(");
            }
            printTree2(car(tree));
            if(cdr(tree)->type != NULL_TYPE){
                printf(" ");
            }
            printTree2(cdr(tree));
            break;
        
        case NULL_TYPE:
            printf(")");
            break;
        
        case PRIMITIVE_TYPE:
            printf("Primitive\n");
            break;
        
        case SYMBOL_TYPE:
            printf("%s", tree->s);
            break;
        
        case INT_TYPE:
            printf("%i", tree->i);
            break;
        
        case DOUBLE_TYPE:
            printf("%f", tree->d);
            break;
        
        case STR_TYPE:
            printf("\"%s\"", tree->s);
            break;
        
        case BOOL_TYPE:
            printf("%s", tree->s);
            break;
        
        case VOID_TYPE:
            break;
        
        case CLOSURE_TYPE:
            printf("#<procedure>");
            break;
        
        default:
            printf("Messed up");
    }
}


void printTree(Value* tree){
    //printf("(");
    //printTree2(car(tree));
    if(tree->type == VOID_TYPE){
        return;
    }
    if(tree->type != CONS_TYPE){
        printTree2(tree);
        printf("\n");
    }
    else{
        Value *current = tree;
        Value *sub = car(current);
        while(sub->type != NULL_TYPE){
            if(sub->type == CONS_TYPE){
                printf("(");
            }
            printTree2(sub);
            printf("\n");
            current = cdr(current);
            if(current->type != CONS_TYPE){
                break;
            }
            sub = car(current);
        }
    }
}

Value *parse(Value *tokens){
    
    Value *root = makeNull();
    Value *current = tokens;
    int depth = 0;
    //assert(current != NULL && "Error (parse): null                          pointer");
    while(current->type != NULL_TYPE){
        Value *token = car(current);
        if(token->type == CLOSE_TYPE && depth-1 == -1){
            throwError("Too many close parens");
        }
        root = addToParseTree(root,depth,token);
        //printTree(root);
        if(token->type == OPEN_TYPE){
            depth++;
        }
        if(token->type == CLOSE_TYPE){
            depth--;
        }
        current = cdr(current);
    }
    //depth--;
    if(depth > 0){
        throwError("Too few closing parens");
    }
    if(depth < 0){
        throwError("Too many closing parens");
    }
    return root;
}
