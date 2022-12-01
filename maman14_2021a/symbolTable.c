#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"
#include "utilities.h"
#include "instructionTable.h"
#include "memImage.h"
#include "transition1OfAssembler.h"

/*****************************************************************
* addSymbol function
* adding a symbol to a list according to given data
*****************************************************************/

void addSymbol(symbolList* list , char* _name , int _val , int _type , int _isentry){
    symbolDef* new = NULL;      /*creating a new symbol*/
    new = (symbolDef*)malloc(sizeof(symbolDef));        /*allocating memory for the symbol*/
    new -> name = (char*)malloc(sizeof(char)*32);      /*defining new data stats*/
    strcpy(new -> name , _name);
    new -> val = _val;
    new -> type = _type;
    new -> isentry = _isentry;
    new -> next = list -> head;
    list -> head = new;
}

/*****************************************************************
* printTable function
* printing a table by its default order
* into a specified file
*****************************************************************/

void printTable(FILE* dest , symbolList* list){
    symbolDef* temp;    /*iterator*/
    temp = list -> head;
    while(temp != NULL){    /*printing the table by order into a specified file*/
        fprintf(dest , " %s , %d  \n" , temp -> name , temp -> val);
        temp = temp -> next;
    }
}

/*****************************************************************
* findInTable function
* finding an element in the table 
* and returning its location (i) if found or (-1) if not
*****************************************************************/

int findInTable(symbolList* list , char* _name){
    symbolDef* temp = NULL;      /*iterator*/
    int i = 0;      /*element's location*/
    temp = list -> head;
    while(temp != NULL){
        if(strcmp(temp -> name,_name) == 0)         /*if the element is found*/
            return i;
        i++;
        temp = temp -> next;      /*go to the next element*/
    }
    return -1;
}

/*****************************************************************
* findEntry function
* return position if entry found, or -1 if not
*****************************************************************/

int findEntry(symbolList* list ){
    symbolDef* temp = NULL;      /*iterator*/
    int i = 0;      /*element's location*/
    temp = list -> head;
    while(temp != NULL){
        if(temp -> isentry == 1)        /*if the element is found*/
            return i;
        i++;
        temp = temp -> next;      /*go to the next element*/
    }
    return -1;
}

/*****************************************************************
* getVal function
* getting a name and returns its list value
* according to the given list
*****************************************************************/

int getVal(symbolList* list , char* _name){
    symbolDef* temp;        /*iterator*/
    int i = 0 , j = 0;      /*indexes*/
    char ctemp[50];      /**/
    temp = list -> head;
    resetString(ctemp , 50);         /*resetting ctemp*/
    if(_name[0] == '%')       /*if name is similar to the ASCII value of %*/
        for(j = 0 ; j < strlen(_name) ; j++)
            ctemp[j] = _name[j+1];
    while(temp != NULL){        /*find name*/
        if(strcmp(temp -> name,_name) == 0 || strcmp(temp -> name , ctemp) == 0)
            return temp -> val;        /*return the list value*/
        i++;         /*next index*/
        temp = temp -> next;
    }
    return -1;      /*value not found*/
}

/*****************************************************************
* isExtern function
* checking if a given label is an extern
* and returning true (1) or false (0)
*****************************************************************/

int isExtern(symbolList* list , char* _name){
    symbolDef* temp;        /*iterator*/
    int i = 0 , j = 0;      /*indexes*/
    char ctemp[50] ;
    temp = list -> head;
    resetString(ctemp , 50);      /*resetting*/
    if(_name[0] == '%')
        for(j = 0 ; j < strlen(_name) ; j++)        /*copying the name into ctemp*/
            ctemp[j] = _name[j+1];
    while(temp != NULL){         /*iterating list*/
        if(strcmp(temp -> name,_name) == 0 || strcmp(temp->name , ctemp) == 0){      /*if name is found*/
            if(temp -> type == 2)      /*if the type is an extern type*/
                return 1;
        }
        i++;
        temp = temp -> next;
    }
    return 0;
}

/*****************************************************************
* addICF function
* checking if a given label is an extern
* and adding its ICF
*****************************************************************/

void addICF(symbolList* list , int ICF){
    symbolDef* temp = list -> head;        /*iterator*/
    while(temp != NULL){        /*running on temp*/
       if(temp -> type == 0)         /*if temp is data-typed*/
            temp -> val += ICF;
        temp = temp->next;       /*go to next symbol*/
    }
}

/*****************************************************************
* setEntry function
* setting a symbol as a legal entry
*****************************************************************/

void setEntry(symbolList* list , char *name){
    symbolDef* temp = list -> head;     /*iterator*/
    while(temp != NULL){         /*running on temp*/
       if(strcmp(temp -> name, name) == 0)         /*if the name is found*/
         temp -> isentry = 1;         /*setting as an entry*/
       temp = temp -> next;      /*go to next symbol*/
    }    
}

/*****************************************************************
* printEntries function
* printing entries to a specified file
*****************************************************************/

void printEntries(FILE* dest , symbolList* list){
    symbolDef* temp;
    temp = list -> head;       /*iterator*/
    while(temp != NULL){         /*running on temp*/
       if(temp -> isentry == 1)      /*if is entry*/
           fprintf(dest , "%s %04d\n" , temp -> name , temp -> val);         /*put the entry to a file*/
       temp = temp -> next;
    }
}

/*****************************************************************
* emptyTable function
* an internal function that clearTable uses, clearing
* a table from head to tail
*****************************************************************/

void emptyTable(symbolDef* node){
    if(node == NULL || node->next == NULL)
		return;
    if(node->next != NULL)      /*if the list is not over yet*/
        emptyTable(node->next);       /*recursive call*/
    node -> next = NULL;      /*setting next to null*/
	if(node -> name != NULL)
	free(node -> name);       /*free memory*/
    free(node); 
}

/*****************************************************************
* clearTable function
* a function that is clearing
* a table from head to tail using emptyTable
*****************************************************************/

void clearTable(symbolList* list){
	if(list == NULL || list->head == NULL)
		return;
    emptyTable(list -> head);     /*calling emptyTable*/
    list -> head = NULL;         /*setting head to null*/
}
