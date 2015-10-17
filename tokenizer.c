#include "tokenizer.h"
#include "value.h"
#include "talloc.h"
#include "linkedlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//Value *master;
char charRead;

void nextChar(){
    charRead = fgetc(stdin);
}

int isNumeric(char s){
    char nums[12] = ".0123456789";
    for(int i = 0; i < 10; i++){
        if(s == nums[i]){
            return 1;
        }
    }
    return 0;
}

Value *string(){
    char* str = talloc(sizeof(char)*300);
    int i = 0;
    nextChar();
    while(charRead != '\"'){
        if(charRead == '\\'){
            nextChar();
            if(charRead == 'n'){
                str[i] = '\n';
                i++;
                nextChar();
                continue;
            }
            if(charRead == 't'){
                str[i] = '\t';
                i++;
                nextChar();
                continue;
            }
            str[i] = charRead;
            i++;
            nextChar();
            continue;
        }
        str[i] = charRead;
        nextChar();
        i++;
    }
    str[i] = '\0';
    Value *temp = talloc(sizeof(Value));
    temp->type = STR_TYPE;
    temp->s = str;
    return temp;
}


Value *open(){
    if(charRead == '('){
        Value *temp = talloc(sizeof(Value));
        temp->type = OPEN_TYPE;
        return temp;
        //list = cons(temp, list);
        //displayTokens(temp);
    }
    else{
        printf("Could not match: (");
        texit(0);
        return 0;
    }
}

Value *close(){
    Value *temp;
    if(charRead == ')'){
        temp = talloc(sizeof(Value));
        temp->type = CLOSE_TYPE;
        return temp;
    }
    else{
        printf("Could not match: )");
        texit(0);
        return 0;
    }
}


Value *number(){
    char strNum[20];
    int i = 0;
    int intType = 1;
    while(isNumeric(charRead)){
        if(i >= 19){
            printf("Int too big");
            texit(0);
        }
        if(charRead == '.'){
            intType = 0;
        }
        strNum[i] = charRead;
        i++;
        nextChar();
    }
    strNum[i] = '\0';
    Value *temp = talloc(sizeof(Value));
    if(intType){
        temp->type = INT_TYPE;
        int numStr = atoi(strNum);
        temp->i = numStr;
    }
    else{
        temp->type = DOUBLE_TYPE;
        double numStr = atof(strNum);
        temp->d = numStr;
    }
    return temp;
}

void comment(){
    while(charRead != '\n' && charRead != EOF){
        nextChar();
    }
}

Value *boolean(){
    char *boo = talloc(sizeof(char)*3);
    if(charRead == '#'){
        boo[0] = charRead;
        nextChar();
        if(charRead != 'f' && charRead != 't'){
            printf("# not followed by t or f\n");
            texit(0);
        }
        boo[1] = charRead;
        boo[2] = '\0';
        nextChar();
        if(charRead != ' ' && charRead != ')' && charRead != '\n' && charRead != EOF){
            printf("# not followed by t or f\n");
            texit(0);
        }
        Value *temp = talloc(sizeof(Value));
        temp->type = BOOL_TYPE;
        temp->s = boo;
        return temp;
    }
    return 0;
}

int isValidSymbolChar(char s){
    char invalidSymbols[28] = "@#$%^&~`[]{}+.,\\|() ";
    for(int i = 0; i < 26; i++){
        if(s == invalidSymbols[i]){
            return 0;
        }
    }
    return 1;
}

Value *sign(){
    //printf("Signing: %c\n", charRead);
    char *num = talloc(sizeof(char)*11);
    Value *temp = talloc(sizeof(Value));
    int isNum = 0;
    int isDecimal = 0;
    int i = 0;
    while(charRead != ' ' && charRead != '\n' && charRead != ')'){
        num[i] = charRead;
        if(isNumeric(charRead)){
           // printf("I think it's numeric: %c\n", charRead);
            isNum = 1;
        }
        if(charRead == '.'){
            isDecimal = 1;
        }
        nextChar();
        i++;
    }
    num[i] = '\0';
    if(!isNum){
        temp->type = SYMBOL_TYPE;
        temp->s = num;
    }
    else if(isNum && !isDecimal){
        temp->type = INT_TYPE;
        temp->i = atoi(num);
    }
    else if(isNum && isDecimal){
        temp->type = DOUBLE_TYPE;
        temp->d = atof(num);
    }
    else{
        printf("Error parsing sign\n");
        texit(0);
    }
    return temp;
}
    

Value *symbol(){
    char *str = talloc(sizeof(char)*11);
    int i = 0;
    
    while(isValidSymbolChar(charRead) && charRead != '(' &&  charRead != '\n' && charRead!=EOF){
        str[i] = charRead;
        nextChar();
        i++;
    }
    
    if(!isValidSymbolChar(charRead) && charRead != ' ' && charRead != '(' && charRead != ')'){
        printf("Invalid char in symbol: %c\n", charRead);
        texit(0);
    }
    str[i] = '\0';
    Value *temp = talloc(sizeof(Value));
    temp->type = SYMBOL_TYPE;
    temp->s = str;
    return temp;
}
    
Value *tokenize(){
    Value *master = makeNull();
    nextChar();
    
    while(charRead != EOF){
        switch(charRead){
            case '(':
                master = cons(open(), master);
                nextChar();
                break;
            
            case ')':
                master = cons(close(), master);
                nextChar();
                break;
            
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
                master = cons(number(), master);
                break;
            
            case '+':
            case '-':
                master = cons(sign(), master);
                break;
            
            case ' ':
            case '\n':
                nextChar();
                break;
            
            case '\"':
                master = cons(string(), master);
                nextChar();
                break;
            
            case ';':
                comment();
                break;
            
            case '#':
                master = cons(boolean(), master);
                break;
                
            default:
                master = cons(symbol(), master);
        }
    }
    return reverse(master);
}

// Displays the contents of the linked list as tokens, with type information
void displayTokens(Value *list){
    switch((*list).type){
        case CONS_TYPE:
            displayTokens((*list).c.car);
            displayTokens((*list).c.cdr);
            break;
        case NULL_TYPE:
            break;
        case INT_TYPE:
            printf("%i: int\n", (*list).i);
            break;
        case DOUBLE_TYPE:
            printf("%f: double\n",(*list).d);
            break;
        case STR_TYPE:
            printf("%s: string\n", (*list).s);
            break;
        case OPEN_TYPE:
            printf("(: open\n");
            break;
        case CLOSE_TYPE:
            printf("): close\n");
            break;
        case BOOL_TYPE:
            printf("%s: bool\n", (*list).s);
            break;
        case SYMBOL_TYPE:
            printf("%s: symbol\n", (*list).s);
            break;
        default:
            printf("\n");
    }
}
