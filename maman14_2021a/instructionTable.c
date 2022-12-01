#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructionTable.h"

/*****************************************************************
* addInstruction function
* Populate the instructDef object
*****************************************************************/

void addInstruction(instructList* list , char* _name , int _opcode , int _funct , int _legalSrcMet, int _legalDstMet , int _numOfOperants){     /*this function allows us to add a definition of an instruction to the list*/
    instructDef* newInstruction = NULL;
    newInstruction = (instructDef*)malloc(sizeof(instructDef));
    newInstruction -> name = _name;
    newInstruction -> opcode = _opcode;
    newInstruction -> funct = _funct;
    newInstruction -> legalSrcMet = _legalSrcMet;
    newInstruction -> legalDstMet = _legalDstMet;
    newInstruction -> numOfOperants = _numOfOperants;
    newInstruction -> next = list -> head;
    list -> head = newInstruction;
}

/*****************************************************************
* fillTable function
* Fills for every instrucion - its definition and properties
*****************************************************************/

void fillTable(instructList* list){
    addInstruction(list , "mov" , 0 , 0 , 11 , 10 , 2);
    addInstruction(list , "cmp" , 1 , 0 , 11 , 11 , 2);
    addInstruction(list , "add" , 2 , 10 , 11 , 10 , 2);
    addInstruction(list , "sub" , 2 , 11 , 11 , 10 , 2);
    addInstruction(list , "lea" , 4 , 0 , 2 , 10 , 2);
    addInstruction(list , "clr" , 5 , 10 , 0 , 10 , 1);
    addInstruction(list , "not" , 5 , 11 , 0 , 10  , 1);
    addInstruction(list , "inc" , 5 , 12 , 0 , 10  , 1);
    addInstruction(list , "dec" , 5 , 13 , 0 , 10 , 1);
    addInstruction(list , "jmp" , 9 , 10 , 0 , 6 , 1);
    addInstruction(list , "bne" , 9 , 11 , 0 , 6 , 1);
    addInstruction(list , "jsr" , 9 , 12 , 0 , 6 , 1);
    addInstruction(list , "red" , 12 , 0 , 0 , 10 , 1);
    addInstruction(list , "prn" , 13 , 0 , 0 , 11 , 1);
    addInstruction(list , "rts" , 14 , 0 , 0 , 0 , 0);
    addInstruction(list , "stop" , 15 , 0 , 0 , 0 , 0);
}


/*****************************************************************
* isDefined function
* goes through the list of instructions, if we find the instruction, 
* we return its position in the list, else  we return -1
*****************************************************************/

int isDefined( instructList* list, char* name) {
    instructDef* temp = list -> head;
    int i = 0;
    for(i = 0 ; i < 16 ; i++){
        if (strcmp(name, temp -> name) == 0)
            return i;
        temp = temp -> next;
    }
    return -1;
}


/*****************************************************************
* isLegalSrcMet function
* num is the src method we want to know is legal or not. 
* this function takes num and the corresponding instruction's definition, 
* and returns 1 if num is a legal src method 
*****************************************************************/

int isLegalSrcMet(instructDef* def, int num){
   int i = 0;
    for(i = 0 ; i < 4 ; i++){
        if (num == i  && ((1 << i) & (def -> legalSrcMet)) != 0)    /*checks if the bit in position num is on in the instruction's legal source method field - that means it is a legal method*/
            return 1;
    }
    return -1;
}

/*****************************************************************
* isLegalDstMet function
* num is the destination method we want to know is legal or not. 
* this function takes num and the corresponding instruction's definition, 
* and returns 1 if num is a legal destination method 
*****************************************************************/

int isLegalDstMet(instructDef* def, int num){
   int i = 0;
    for(i = 0 ; i < 4 ; i++){
        if (num == i  && ((1 << i) & (def -> legalDstMet)) != 0)
            return 1;
    }
    return -1;
}

/*****************************************************************
* getDef function
* goes through the list of instructions, if we find the instruction, 
* we return its definition from the list, else  we return -1
*****************************************************************/

instructDef* getDef(instructList* list, char* name){
    instructDef* temp = list->head;
    int i = 0;
    for(i = 0 ; i < 16 ; i++){      /*iterates 16 times because the list is constant and will allways have 16 elements*/
        if (strcmp(name, temp -> name) == 0)
            return temp;
        temp = temp -> next;
    }
    return NULL;
}
