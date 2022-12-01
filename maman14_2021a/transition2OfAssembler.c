#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"
#include "symbolTable.h"
#include "instructionTable.h"
#include "memImage.h"
#include "transition1OfAssembler.h"
#include "transition2OfAssembler.h"

/*****************************************************************
* encodeAddressToBinary function
* coding the addresses into instruction binary, 
* based on the properties of 'inst', as well as symbolist.
*****************************************************************/
int encodeAddressToBinary(instructionBinary* inst , symbolList* list , symbolList* externList ,  char* line , int index , int ic){
	char* str =  getWord(line , index+1);	/*getting the first-indexed word*/
	char* str1 = getWord(line , index+2); 	/*getting the second-indexed word*/
	int temp = 0;
	if(inst -> srcMet == 1 && inst -> length == 3 ){ 	/*if source is 1 and length is 3*/
		temp = getVal(list , str); 		/*the value of str according to the symbol table*/
		if(temp != -1){ 		/*if temp exists*/
			inst -> src.data = temp;
			if(isExtern(list , str) == 1){		 /*if is extern*/
				inst -> src.ARE = 'E';
				addSymbol(externList , str , ic +1 , 2 , 0); 		/*adding a symbol to symbol table*/
			}
			else 
				inst -> src.ARE = 'R';
		}
		else
			return 0;
	}
	if(inst -> dstMet == 1 && inst -> length == 3){ 		/*if destination is 1 and length is 3*/
		temp = getVal(list , str1);		 /*the value of str according to the symbol table*/
		if(temp != -1){		 /*if temp exists*/
			inst -> dst.data = temp;
			if(isExtern(list , str1) == 1){ 		/*if is extern*/
				inst->dst.ARE = 'E';
				addSymbol(externList , str1 , ic + 2 , 2 , 0); 		/*adding a symbol to symbol table*/
			}	
			else
				inst -> dst.ARE = 'R';
		}
		else
			return 0;
	}
    if(inst -> dstMet == 1 && inst -> length == 2){ 		/*if destination is 1 and length is 2*/
		temp = getVal(list , str);		 /*the value of str according to the symbol table*/
		if(temp != -1){		 /*if temp exists*/
			inst -> dst.data = temp;
			if(isExtern(list , str) ==1){ 		/*if is extern*/
				inst -> dst.ARE = 'E';
				addSymbol(externList , str , ic + 1 , 2 , 0); 		/*adding a symbol to symbol table*/
			}
			else
				inst -> dst.ARE = 'R';
		}
		else 
			return 0;
	}
	if(inst -> dstMet == 2 && inst -> length == 2){		 /*if destination is 2 and length is 2*/
		if(getVal(list , str) != -1){		 /*if value of str exists*/
			inst -> dst.data = (signed int)(getVal(list , str) - (inst -> ic +1) );
			inst -> dst.ARE = 'A';
		}
		else
			return 0;
	}
	free(str);
	free(str1);
	return 1;
}

/*****************************************************************
* secondTransition function
* doing the second pass according to the MAMAN guidelines
*****************************************************************/

int secondTransition(FILE *f1, instructionBinary* arr , symbolList * list ,symbolList * externlist){ 		/*	symbolDef* temp20; iterator*/
	int error = 0; 		/*error count*/
	char* line , *word;
	char temp[40]; 		/*temporary data*/
	int end = 0, done = 0;
	int i = 0 , index = 0;		 /*indexes*/
	resetString(temp , 40);		 /*reset temp*/
	while (end == 0){ 		/*while the programme is still running*/
		done = 0;
		line = readNextLine(f1); 		/*read line from file*/
		if(line != NULL && line != (char*)EOF){ 		/*if the line is O.K.*/
			word = getWord(line , index); 		/*taking a word from the line*/
			index++; 		/*next time take the next word*/
			if(CheckLabel(list , word) != -2){ 		/*if the label is O.K.*/
				free(word);
				word = getWord(line , index);
				index++;		 /*next time take the next word*/
			}
			if(checkStoringData(word) || checkExternEntry(word) == 1)		 /*if the word includes data or is an extern*/
				done = 1;
			else{
				if(checkExternEntry(word) == 2){		 /*if the word is an entry*/
					free(word);
					word = getWord(line , index);		 /*take next word*/
					index++;		 /*next time take the next word*/
					if(findInTable(list , word) != -1) {		 /*if word is found in table*/
						setEntry(list , word);
					}
					else{
						done = 1 ;
						printf("\033[0;31m");
						printf("ERROR:");
						printf("\033[0m");
						printf(" entry not defined\n");
						if(line != NULL)
							printf("   in line: %s\n" , line);
						error++;
					}	
				}
				else
				if(done == 0){		 /*if not done yet*/
					if(!encodeAddressToBinary(&arr[i] , list , externlist , line , index-1 , arr[i].ic )) {		 /*if cannot code word because the label is illegal and used wrongly*/
						error++;
						done = 1;
						printf("\033[0;31m");
						printf("ERROR:");
						printf("\033[0m");
						printf(" label used as operand is illegal\n");
						if(line != NULL)
							printf("   in line: %s\n" , line);
						
					}
					i++;
				}
			}
		}
		else{
			if( line == (char*)EOF)		 /*if reached file end*/
				end = 1;
		}
		index = 0;
	}
	return error;		 /*error count*/
}
