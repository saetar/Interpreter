#include "linkedlist.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "talloc.h"

// Create a new NULL_TYPE value node.
Value *makeNull(){
    Value *value = talloc(sizeof(Value));
    (*value).type = NULL_TYPE;
    return value;
}

// Create a new CONS_TYPE value node.
Value *cons(Value *car, Value *cdr){
    Value *value = talloc(sizeof(Value));
    (*value).type = CONS_TYPE;
    (*value).c.car = car;
    (*value).c.cdr = cdr;
    return value;
    
}

// Display the contents of the linked list to the screen in some kind of
// readable format
void display(Value *list){
    switch((*list).type){
        case NULL_TYPE:
            break;
        case INT_TYPE:
            printf("%i\n", (*list).i);
            break;
        case DOUBLE_TYPE:
            printf("%lf\n",(*list).d);
            break;
        case STR_TYPE:
            printf("%s\n", (*list).s);
            break;
        case SYMBOL_TYPE:
            printf("%s\n", list->s);
            break;
        case CONS_TYPE:
            display((*list).c.car);
            display((*list).c.cdr);
            break;
        default:
            printf("\n");
    }
}

// Return a new list that is the reverse of the one that is passed in. 
Value *reverse(Value *list){
    Value *ptr = 0;
    while(!isNull(list)){
        Value *temp = talloc(sizeof(Value));
        temp->type = CONS_TYPE;
        temp->c.car = car(list);
        if(ptr == 0){
            ptr = makeNull();
        }
        temp->c.cdr = ptr;
        ptr = temp;
        
        list = cdr(list);
    }
    return ptr;
}

// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list){
    if((*list).type == CONS_TYPE){
        return (*list).c.car;
    }
    else{
        return 0;
    }
}

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list){
    if((*list).type == CONS_TYPE){
        return (*list).c.cdr;
    }
    else{
        return 0;
    }
    //What should be returned if not CONS_TYPE?
}

// Utility to check if pointing to a NULL_TYPE value. Use assertions to make sure
// that this is a legitimate operation.
bool isNull(Value *value){
    return ((*value).type == NULL_TYPE);
}

// Measure length of list. Use assertions to make sure that this is a legitimate
// operation.
int length(Value *value){
    if( (*value).type != CONS_TYPE ){
        return 0;
    }
    return 1 + length( cdr(value) );
}
