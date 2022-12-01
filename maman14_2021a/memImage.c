#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memImage.h"
#include "instructionTable.h"
#include "symbolTable.h"
#include "transition1OfAssembler.h"

/*****************************************************************
* insertInt function
* convert string to number and insert into 
* array in position dc if the number is within boundaries
*****************************************************************/

int insertInt(memWord arr[] , int dc , char* input){
    int num = ConvertStringToNumber(input , 0);     /*converting the input to a number*/
    if(num < 2048 && num > -2049){  /*if the num is in range*/
        arr[dc].data = num;
        return dc + 1;
    }
    else
        return -1;
}

/*****************************************************************
* insertStr function
* Insert into data array, every char of the string input
* Returns the data counter in end of string
*****************************************************************/

int insertStr(memWord arr[] , int dc , char* input){
    int size = strlen(input);   /*the size of the given string*/
    int i = 0;
    if(input[0] != '"' || input[size - 1] != '"')    /*if the input is not a well-formatted string*/
        return -1;
    if(dc + size > 100)      /*if overflowing the array bounds*/
        return -1;
    for (i = 1; i < size - 1 && dc < 99 ; i++){      /*putting the input in dynamically*/
        arr[dc].data = input[i];
        dc++;
    }
    arr[dc].data = '\0';        /*"closing" the string*/
    dc++;
    return dc;
}

/*****************************************************************
* putSrcDstMetLIC function
* Takes the instructions binary L and IC and puts the correct
* values for the source and destination methods, the IC, the length
* and also codes the ARE field to be A
*****************************************************************/

int putSrcDstMetLIC(instructionBinary* inst , int L , int IC){
    unsigned int temp = 0;
    temp = ((unsigned)(inst -> firstword.data)) % 4;
    inst -> firstword.ARE = 'A';     /*defining A.R.E status*/
    inst -> dstMet = temp;       /*defining source and destination stats*/
    inst -> srcMet = (((unsigned)(inst -> firstword.data)) % 16 - temp) >> 2;
    inst -> length = L;     /*defining length*/
    inst -> ic = IC;     /*defining IC*/
    return 1;
}
