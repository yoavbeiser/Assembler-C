#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"
#include "instructionTable.h"
#include "memImage.h"
#include "transition1OfAssembler.h"
#include "utilities.h"
#include "transition2OfAssembler.h"

/*****************************************************************
* main function
* calls the program for each file in the parameters
*****************************************************************/

int main(int argc, char* argv[]){
    int i = 1;
	if(argv[1] == NULL){
		printf("\033[0;31m");
		printf("ERROR:");
		printf("\033[0m");
		printf(" no file name recieved\n");
		return 1;
	}
    while (argv[i] != NULL){
    	firstTransition(argv[i]);
    	i++;
    }
    return 1;
}
