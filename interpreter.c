#include <stdio.h>
#include "tokenizer.h"
#include "value.h"
#include "linkedlist.h"
#include "parser.h"
#include "talloc.h"
#include "interpreter.h"
#include <string.h>

/*
    PRIMITIVES
*/

void bind(char *name, Value *(*function)(struct Value *), Frame *frame){
    Value *func = talloc(sizeof(Value));
    func->type = PRIMITIVE_TYPE;
    func->pf = function;
    Value *sym = talloc(sizeof(Value));
    sym->type = SYMBOL_TYPE;
    sym->s = name;
    Value *bin = cons(func, makeNull());
    bin = cons(sym, bin);
    frame->bindings = cons(bin, frame->bindings);
}

Value *primitiveMult(Value *args){
    if(length(args) < 2){
        printf("Not enough args in *\n");
        texit(0);
    }
    int intProd = 1;
    double doubleProd = 1;
    while(args->type != NULL_TYPE){
        Value *curArg = car(args);
        if(curArg->type != INT_TYPE && curArg->type != DOUBLE_TYPE){
            printf("Non-numeric args in *\n");
            texit(0);
        }
        if(curArg->type == INT_TYPE){
            intProd *= curArg->i;
        }
        else{
            doubleProd *= curArg->d;
        }
        args = cdr(args);
    }
    Value *result = talloc(sizeof(Value));
    if(doubleProd == 1){
        result->type = INT_TYPE;
        result->i = intProd;
    }
    else{
        result->type = DOUBLE_TYPE;
        result->d = intProd * doubleProd;
    }
    return result;
}

/*
    Takes a list of args and checks to see if they are all int or double types. If not, returns 0.
*/
int numericCheck(Value *args){
    while(args->type!=NULL_TYPE){
        Value *curArg = car(args);
        if(curArg->type != INT_TYPE && curArg->type != DOUBLE_TYPE){
            return 0;
        }
        args = cdr(args);
    }
    return 1;
}


Value *primitiveMinus(Value *args){
    if(length(args) != 2){
        printf("Wrong number of args in -\n");
        texit(0);
    }
    
    Value *numOne = car(args);
    Value *numTwo = car(cdr(args));
    Value *diff = talloc(sizeof(Value));
    if(!numericCheck(args)){
        printf("Non-numeric types in -\n");
        texit(0);
    }
    if(numOne->type == INT_TYPE && numTwo->type == INT_TYPE){
        diff->type = INT_TYPE;
        diff->i = numOne->i - numTwo->i;
    }
    else{
        double one;
        double two;
        if(numOne->type == INT_TYPE){
            one = numOne->i;
        }
        else{
            one = numOne->d;
        }
        if(numTwo->type == INT_TYPE){
            two = numTwo->i;
        }
        else{
            two = numTwo->d;
        }
        diff->type = DOUBLE_TYPE;
        diff->d = one - two;
    }
    return diff;
}

Value *primitiveMod(Value *args){
    if(length(args) != 2){
        printf("Wrong number of args in modulo\n");
        texit(0);
    }
    Value *numOne = car(args);
    Value *numTwo = car(cdr(args));
    if(numOne->type != INT_TYPE || numTwo->type != INT_TYPE){
        printf("Non-ints given to modulo\n");
        texit(0);
    }
    
    Value *answer = talloc(sizeof(Value));
    answer->type = INT_TYPE;
    answer->i = numOne->i % numTwo->i;
    return answer;
}


Value *primitiveDivide(Value *args){
    Value *numOne = car(args);
    Value *numTwo = car(cdr(args));
    if(!numericCheck(args)){
        printf("Non-numeric types in -\n");
    }
    
    Value *answer = talloc(sizeof(Value));
    if(numOne->type == INT_TYPE && numTwo->type == INT_TYPE){
        int one = numOne->i;
        int two = numTwo->i;
        if(one%two == 0){
            answer->type = INT_TYPE;
            answer->i = one/two;
            return answer;
        }
    }
    double one;
    if(numOne->type == INT_TYPE){
        one = numOne->i;
    }
    else{
        one = numOne->d;
    }
    double two;
    if(numTwo->type == INT_TYPE){
        two = numTwo->i;
    }
    else{
        two = numTwo->d;
    }
    answer->type = DOUBLE_TYPE;
    answer->d = one/two;
    return answer;
}


Value *primitiveGreaterThan(Value *args){
    if(!numericCheck(args)){
        printf("Non-numeric values used in >.\n");
        texit(0);
    }
    Value *numOne = car(args);
    Value *numTwo = car(cdr(args));
    double one;
    if(numOne->type == INT_TYPE){
        one = numOne->i;
    }
    else{
        one = numOne->d;
    }
    double two;
    if(numTwo->type == INT_TYPE){
        two = numTwo->i;
    }
    else{
        two = numTwo->d;
    }
    Value *boo = talloc(sizeof(Value));
    boo->type = BOOL_TYPE;
    if(one > two){
        boo->s = "#t";
    }
    else{
        boo->s = "#f";
    }
    return boo;
}


Value *primitiveLessThan(Value *args){
    if(!numericCheck(args)){
        printf("Non-numeric values used in <.\n");
        texit(0);
    }
    Value *numOne = car(args);
    Value *numTwo = car(cdr(args));
    double one;
    if(numOne->type == INT_TYPE){
        one = numOne->i;
    }
    else{
        one = numOne->d;
    }
    double two;
    if(numTwo->type == INT_TYPE){
        two = numTwo->i;
    }
    else{
        two = numTwo->d;
    }
    Value *boo = talloc(sizeof(Value));
    boo->type = BOOL_TYPE;
    if(one < two){
        boo->s = "#t";
    }
    else{
        boo->s = "#f";
    }
    return boo;
}


Value *primitiveEquals(Value *args){
    if(!numericCheck(args)){
        printf("Non-numeric values used in =.\n");
        texit(0);
    }
    Value *numOne = car(args);
    Value *numTwo = car(cdr(args));
    double one;
    if(numOne->type == INT_TYPE){
        one = numOne->i;
    }
    else{
        one = numOne->d;
    }
    double two;
    if(numTwo->type == INT_TYPE){
        two = numTwo->i;
    }
    else{
        two = numTwo->d;
    }
    Value *boo = talloc(sizeof(Value));
    boo->type = BOOL_TYPE;
    if(one == two){
        boo->s = "#t";
    }
    else{
        boo->s = "#f";
    }
    return boo;
}

Value *primitiveCar(Value *args){
    if(length(args) != 1){
        printf("Wrong number of args in car\n");
        texit(0);
    }
    if(car(args)->type != CONS_TYPE){
        printf("Wrong type of argument for car\n");
        texit(0);
    }
    return car(car(args));
}

Value *primitiveCdr(Value *args){
    if(length(args) != 1){
        printf("Wrong number of args in car\n");
        texit(0);
    }
    if(car(args)->type != CONS_TYPE){
        printf("Wrong type of argument for car\n");
        texit(0);
    }
    return cdr(car(args));
}

Value *primitiveCons(Value *args){
    if(length(args) != 2){
        printf("Wrong number of args in cons\n");
        texit(0);
    }
    Value *first = car(args);
    Value *list = car(cdr(args));
    
    if(list->type != CONS_TYPE){
        printf("Bad form for cons\n");
        texit(0);
    }
    
    Value *newList = makeNull();
    list = reverse(list);
    while(list->type != NULL_TYPE){
        newList = cons(car(list), newList);
        list=cdr(list);
    }
    newList = cons(first, newList);
    return newList;
}

Value *primitiveNullQ(Value *args){
    args = car(args);
    if(args->type == NULL_TYPE){
        Value *boole = talloc(sizeof(Value));
        boole->type = BOOL_TYPE;
        boole->s = "#t";
        return boole;
    }
    else if(args->type != CONS_TYPE){
        printf("Not right type for null?\n");
        texit(0);
    }
    Value *boole = talloc(sizeof(Value));
    boole->type = BOOL_TYPE;
    boole->s = "#f";
    return boole;
}

Value *primitiveAdd(Value *args){
    int sum = 0;
    double dub = 0;
    while(args->type != NULL_TYPE){
        Value *curArg = car(args);
        if(curArg->type == DOUBLE_TYPE){
            dub+=curArg->d;
        }
        else if(curArg->type == INT_TYPE){
            sum+=curArg->i;
        }
        else{
            printf("Int or double not used in sum\n");
        }
        args = cdr(args);
    }
    
    Value *result = talloc(sizeof(Value));
    if(dub == 0){
        result->type = INT_TYPE;
        result->i = sum;
    }
    else{
        result->type = DOUBLE_TYPE;
        result->d = sum+dub;
    }
    return result;
}


void printEval2(Value *tree){
    switch(tree->type){
        case CONS_TYPE:
            if(car(tree)->type == CONS_TYPE || car(tree)->type == NULL_TYPE){
                printf("(");
            }
            printEval2(car(tree));
            if(cdr(tree)->type != NULL_TYPE){
                printf(" ");
            }
            printEval2(cdr(tree));
            break;
        
        case NULL_TYPE:
            printf(")");
            break;
        
        case PRIMITIVE_TYPE:
            printf("#<primitive>");
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


void printEval(Value *tree){
    if(tree->type == CONS_TYPE){
        printf("(");
    }
    else if(tree->type == VOID_TYPE || tree->type == NULL_TYPE){
        return;
    }
    printEval2(tree);
    if(tree->type != VOID_TYPE){
        printf("\n");
    }
}

void interpret(Value *tree){
    Frame *main = talloc(sizeof(Frame));
    main->bindings = makeNull();
    main->parent = 0;
    bind("+",primitiveAdd,main);
    bind("car",primitiveCar,main);
    bind("cdr",primitiveCdr,main);
    bind("cons",primitiveCons,main);
    bind("null?",primitiveNullQ,main);
    bind("*",primitiveMult,main);
    bind("-",primitiveMinus,main);
    bind("modulo",primitiveMod,main);
    bind("/",primitiveDivide,main);
    bind(">",primitiveGreaterThan,main);
    bind("<",primitiveLessThan,main);
    bind("=",primitiveEquals,main);
    Value *expressions = tree;
    while(expressions->type != NULL_TYPE){
        Value *curExpress;
        if(expressions->type == CONS_TYPE){
            curExpress = car(expressions);
            expressions = cdr(expressions);
        }
        else{
            curExpress = expressions;
        }
        Value *result = eval(curExpress, main);
        printEval(result);
    }
}

Value *lookUpSymbol(Value *symbol, Frame *frame){
    while(frame != 0){
        Value *binds = frame->bindings;
        while(binds->type != NULL_TYPE){
            Value *curbind = car(binds);
            Value* key = car(curbind);
            Value* value = car(cdr(curbind));
            if(!strcmp(key->s, symbol->s)){
                return eval(value, frame);
            }
            binds = cdr(binds);
        }
        frame = frame->parent;
    }
    printf("No value found for %s.\n", symbol->s);
    texit(0);
    return 0;
}

/*
    Takes arguments and a frame. Evaluates the if
*/
Value *evalIf(Value *args, Frame *frame){
    if(length(args) != 3){
        printf("Incorrect number of args in if\n");
    }
    Value* boo = eval(car(args), frame);
    char* boolean = boo->s;
    Value *results = cdr(args);
    Value *tr = car(results);
    Value *fs = car(cdr(results));
    if(boolean[1] == 't'){
        return eval(tr, frame);
    }
    else{
        return eval(fs, frame);
    }
}


Value *evalLet(Value *args, Frame *frame){
    if(length(args) < 2){
        printf("Wrong number of args in let\n");
        texit(0);
    }
    Frame *newFrame = talloc(sizeof(Frame));
    newFrame->parent = frame;
    Value *bindings = makeNull();
    Value *binds = car(args);
    while(binds->type != NULL_TYPE){
        Value *curbind = car(binds);
        Value *bindInMaking = makeNull();
        bindInMaking = cons(eval(car(cdr(curbind)), frame), bindInMaking);
        bindInMaking = cons(car(curbind), bindInMaking);
        bindings = cons(bindInMaking, bindings);
        binds = cdr(binds);
    }
    newFrame->bindings = bindings;
    
    Value* curTerm = cdr(args);
    while(cdr(curTerm)->type != NULL_TYPE){
        curTerm = cdr(curTerm);
    }
    return eval(car(curTerm), newFrame);
}

/*
    Takes arguments and a frame. Evaluates the let*
*/
Value *evalLetStar(Value *args, Frame *frame){
    if(length(args) < 2){
        printf("Wrong number of args in let\n");
        texit(0);
    }
    Frame *newFrame = talloc(sizeof(Frame));
    newFrame->parent = frame;
    Value *bindings = car(args);
    newFrame->bindings = bindings;
    Value *curTerm = cdr(args);
    while(cdr(curTerm)->type != NULL_TYPE){
        curTerm = cdr(curTerm);
    }
    return eval(car(curTerm), newFrame);
}

Value *evalSetBang(Value *args, Frame *frame){
    Value *symbol = car(args);
    lookUpSymbol(symbol, frame);
    Value *newVal = eval(car(cdr(args)), frame);
    Value *newBinding = cons(newVal, makeNull());
    newBinding = cons(symbol, newBinding);
    frame->bindings = cons(newBinding, frame->bindings);
    Value *returnVal = talloc(sizeof(Value));
    returnVal->type = VOID_TYPE;
    return returnVal;
}

Value *evalBegin(Value *args, Frame *frame){
    if(length(args) == 0){
        printf("Returning null from begin\n");
        Value *returnVal = talloc(sizeof(Value));
        returnVal->type = VOID_TYPE;
        return returnVal;
    }
    
    while(cdr(args)->type != NULL_TYPE){
        Value *curArg = car(args);
        eval(curArg, frame);
        args = cdr(args);
    }
    return eval(car(args), frame);
}

Value *evalCond(Value *args, Frame *frame){
    while(args->type != NULL_TYPE){
        Value *curCond = car(args);
        Value *boo = car(curCond);
        Value *response = car(cdr(curCond));
        if(boo->type == SYMBOL_TYPE){
            if(!strcmp(boo->s,"else")){
                return eval(response,frame);
            }
        }
        boo = eval(boo, frame);
        if(boo->type != BOOL_TYPE){
            printf("Condition not boolean in cond\n");
            texit(0);
        }
        char* boolean = boo->s;
        if(boolean[1] == 't'){
            return eval(response, frame);
        }
        
        args = cdr(args);
    }
    
    Value *returnVal = talloc(sizeof(Value));
    returnVal->type = VOID_TYPE;
    return returnVal;
}



Value *evalOr(Value *args, Frame *frame){
    Value *answer = talloc(sizeof(Value));
    answer->type = BOOL_TYPE;
    if(length(args) == 0){
        answer->s = "#f";
        return answer;
    }
    while(args->type != NULL_TYPE){
        Value *curArg = eval(car(args), frame);
        if(curArg->type != BOOL_TYPE){
            printf("Bool type not given in or\n");
            texit(0);
        }
        char* boolean = curArg->s;
        if(boolean[1]=='t'){
            answer->s = "#t";
            return answer;
        }
        args = cdr(args);
    }
    answer->s = "#f";
    return answer;
}

Value *evalAnd(Value *args, Frame *frame){
    Value *answer = talloc(sizeof(Value));
    answer->type = BOOL_TYPE;
    if(length(args) == 0){
        answer->s = "#t";
        return answer;
    }
    while(args->type != NULL_TYPE){
        Value *curArg = eval(car(args),frame);
        if(curArg->type != BOOL_TYPE){
            printf("Bool not given in and. Type given: %i\n", curArg->type);
            texit(0);
        }
        char* boolean = curArg->s;
        if(boolean[1]=='f'){
            answer->s = "#f";
            return answer;
        }
        args = cdr(args);
    }
    answer->s = "#t";
    return answer;
}


/*
    Takes arguments and a frame. Returns argument
*/
Value *evalQuote(Value *args, Frame *frame){
    if(length(args) != 1){
        printf("Wrong number of args in quote\n");
        texit(0);
    }
    return car(args);
}

Value *evalDef(Value *args, Frame *frame){
    if(frame->parent != 0){
        printf("Define not in outer-most frame\n");
        texit(0);
    }
    if(length(args) != 2){
        printf("Wrong number of args in define");
        texit(0);
    }
    frame->bindings = cons(args,frame->bindings);
    Value *returnVal = talloc(sizeof(Value));
    returnVal->type = VOID_TYPE;
    return returnVal;
}

Value *evalLambda(Value *args, Frame *frame){
    if(length(args) != 2){
        printf("Wrong number of args in lambda\n");
        texit(0);
    }
    Value *newVal = talloc(sizeof(Value));
    newVal->type = CLOSURE_TYPE;
    newVal->cl.paramNames = car(args);
    newVal->cl.functionCode = car(cdr(args));
    newVal->cl.frame = frame;
    return newVal;
}

void evaluationError(){
    printf("Evaluation Error\n");
    texit(0);
}

Value *evalEach(Value *args, Frame *frame){
    Value *evaled = makeNull();
    while(args->type != NULL_TYPE){
        Value *curArg = car(args);
        evaled = cons(eval(curArg, frame), evaled);
        args = cdr(args);
    }
    return reverse(evaled);
}

Value *apply(Value *function, Value *args){
    Frame *newFrame = talloc(sizeof(Frame));
    newFrame->parent = function->cl.frame;
    newFrame->bindings = makeNull();
    Value *names = function->cl.paramNames;
    if(length(args) != length(names)){
        printf("Wrong number of args\n");
        texit(0);
    }
    while(args->type != NULL_TYPE){
        Value *curArg = car(args);
        Value *curBind = cons(curArg, makeNull());
        Value *curName = car(names);
        curBind = cons(curName, curBind);
        newFrame->bindings = cons(curBind, newFrame->bindings);
        args = cdr(args);
        names = cdr(names);
    }
    return eval(function->cl.functionCode, newFrame);
}

Value *primApply(Value *func, Value* args){
    return (func->pf)(args);
}

Value *eval(Value *tree, Frame *frame) {
    Value *result;
    switch (tree->type)  {
       case INT_TYPE:
       case DOUBLE_TYPE:
       case STR_TYPE:
       case BOOL_TYPE:
       case PRIMITIVE_TYPE:
       case NULL_TYPE:
            return tree;
            break;
        
       case SYMBOL_TYPE:
            return lookUpSymbol(tree, frame);
            break;
       
       case CONS_TYPE:{
            Value *first = car(tree);
            Value *args = cdr(tree);
            Value *result;
            if(first->type != SYMBOL_TYPE){
                printf("Not a valid operator: %i\n", first->type);
            }
       
            else if (strcmp(first->s,"if") == 0) {
                result = evalIf(args,frame);
            }
            
            else if (strcmp(first->s,"let") == 0 || 
                    strcmp(first->s,"letrec")==0) {
                result = evalLet(args,frame);
            }
           
           else if (strcmp(first->s, "quote") == 0){
               result = evalQuote(args, frame);
           }
           
           else if (strcmp(first->s, "define") == 0){
               result = evalDef(args, frame);
           }
           
           else if (strcmp(first->s, "lambda") == 0){
               result = evalLambda(args, frame);
           }
           
           else if (strcmp(first->s, "let*") == 0){
               result = evalLetStar(args, frame);
           }
           
           else if (strcmp(first->s, "set!") == 0){
               result = evalSetBang(args, frame);
           }
           
           else if (strcmp(first->s, "begin") == 0){
               result = evalBegin(args, frame);
           }
           
           else if (strcmp(first->s, "cond") == 0){
               result = evalCond(args, frame);
           }
           
           else if (strcmp(first->s, "and") == 0){
               result = evalAnd(args, frame);
           }
           
           else if (strcmp(first->s, "or") == 0){
               result = evalOr(args, frame);
           }
           
           else {
               Value *evaledOperator = eval(first, frame);
               Value *evaledArgs = evalEach(args, frame);
               if(evaledOperator->type == CLOSURE_TYPE){
                   result = apply(evaledOperator, evaledArgs);
               }
               if(evaledOperator->type == PRIMITIVE_TYPE){
                   result = primApply(evaledOperator, evaledArgs);
               }
           }
           return result;
           break;
        }
        default:
            evaluationError();
     }
    return 0;
}