#include "talloc.h"
#include "linkedlist.h"


Value *pointerList;

void pointerListCons(Value *value){
    Value *temp = malloc(sizeof(Value));
    temp->type = CONS_TYPE;
    temp->c.cdr = pointerList;
    temp->c.car = value;
    pointerList = temp;
}

void *talloc(size_t size){
    if(!pointerList){
        pointerList = malloc(sizeof(Value));
        pointerList->type = NULL_TYPE;
    }
    Value *temp = malloc(sizeof(Value));
    temp->type = PTR_TYPE;
    void *pointTemp = malloc(size);
    temp->p = pointTemp;
    pointerListCons(temp);
    return pointTemp;
}   
    

void cleanup(Value *list){
    if ((*list).type == CONS_TYPE){
        cleanup(car(list));
        cleanup(cdr(list));
    }
    else if((*list).type != NULL_TYPE){
        free(list->p);
    }
    free(list);
}

void tfree(){
    if(pointerList){
        cleanup(pointerList);
        pointerList = 0;
    }
}

void texit(int status){
    tfree();
    exit(status);
}