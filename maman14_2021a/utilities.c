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
* resetString function
* initializes a string and resetting it
*****************************************************************/

void resetString(char str[] , int length){ 
    int i = 0;	/*creating a string index*/
	if(str == NULL || str[0] == '\0')
		return;
    for (i = 0; i < length ; i++){
        str[i] = '\0';	/*resetting the string*/
    }
}

/*****************************************************************
* readNextLine function
* reading the next line off of a file
*****************************************************************/

char* readNextLine(FILE * fp){
	char* line;	/*the returned string*/
	char c = '\0';
	int i = 0;
    line = (char*)malloc(500 * sizeof(char));
    resetString(line , 500);
    c = fgetc(fp);
    while(c == ' '){	/*skips any spaces in beginning*/
    	c = fgetc(fp);
    }
    if(c == ';'){	/*skips line if it is comment(returns null)*/
    	while(c != '\n' && c != EOF && c != '\0'){
    		c = fgetc(fp);
        }
		if(c == EOF)	/*return special char if EOF*/
			return (char*)EOF;
        return NULL;
    }
            
    while(c != '\n' && c != EOF){
        line[strlen(line)] = c;
        c = fgetc(fp);
    }

	if(c == EOF && strlen(line) == 0)
		return (char*)EOF;
	for(i = 0 ; i < 500 ; i++){	/*removes any \n from line*/
		if(line[i] == '\n')
			line[i] ='\0';
	} 
    if(line[0] != '\0')
        return line;
    else
        return NULL;
}

/*****************************************************************
* getWord function
* grabs a word out of a given string and
* then returns the requested word
*****************************************************************/

char* getWord(char* line , int index){ 
	char* ret = malloc(sizeof(char)*500);	/*the final returned string*/
	char* iterator = line;
	char* endOfLine = line + strlen(line); /*a char that marks the end of a line*/
	int wordCount = 0;	/*finds the index*/
	int i = 0;
	resetString(ret, 500);	/*resetting the string*/
	while (( *iterator == ' '|| *iterator == 9) && *iterator != '\0')	/*finding the 0-indexed word*/
		iterator++;
	if ( *iterator == '\0'){	/*if we reached the end of the string and didn't find a word*/
		free(ret);
		return NULL;
	}
	while (*iterator != '\0') {		/*running until the end og the string*/
		if (wordCount > index){		/*if the word does'nt exist*/
			free(ret);
			return NULL;
		}
		if (wordCount == index) { 	/*if we found the indexed word*/
			i = 0;
			while (*iterator != ' '&& *iterator != 9 && *iterator != '\0' && *iterator != ',' && *iterator != '\n'){ 	/*run until the end of the word*/
				ret[i] = *iterator;	/*put char in the final return state*/
				iterator++;
				i++;
			}
			return ret; 	/*return the word*/
		}
		else {
			while (*iterator != ' '&& *iterator != 9 && *iterator != '\0' && *iterator != ',' && *iterator != '\n' && iterator < endOfLine) 	/*run to next space*/
				iterator++;

			while ((*iterator == ' ' || *iterator == 9 || *iterator == '\0' || *iterator == ',' || *iterator == '\n') && iterator < endOfLine ){	/*run to next word*/ 
				iterator++;
				if (*iterator == '\0'){ 	/*if we reached the end of the string and didn't find a word*/
					free(ret);
					return NULL;
				}
			}
		}
		wordCount++; 	/*next word in queue*/
	}
	free(ret);
	return NULL; 	/*if we didn't found nothing*/
}

/*****************************************************************
* checkComma function
* checks for illegal commas based on word count, 
* then reuturns true (1) or false (0)
*****************************************************************/

int checkComma(char* sen, int wordCount) {		 /*checks for illegal commas based on word count*/
	char* senext = &sen[1];		/*a next-pointer to check spaces*/
	if(wordCount == 0 && CheckAnyCommas(sen) == 0) 		/*if there are no more commas and the specified amount is 0*/
		return 1;
	if (*senext == '\0') 		/*if the sentence is a null*/
		return 0;
    while (*senext != '\0') { 		/*until the end of the sentence*/
		if (*senext == ' '&& *sen != ' ') { 		/*if reached an end of a word*/
			sen++;
			senext++;
			sen++;
		 	senext++;
		 	break;
	 	}
	 	else { 			/*advance until an end*/
	  		sen++;
	  		senext++;
	 	}
	}
	if (*senext == '\0' || *sen == ',') 		/*if reached an end or found an illegal comma*/
		return 0;
	else {
		while (*sen != '\0') { 			/*until the end of the sentence*/
			if (*sen == ',') { 			/*if a comma is found*/
				if (*senext == ',' || *senext == '\0')		 /*and there are double commas or there is a null*/
					return 0;
				senext++; /*advance*/
				sen++;
				if (*sen != ' ' && *sen != ',') {}			/*if found a word and passed it (neutral 'if')*/
				else if (*senext == ',' || *senext == '\0') 	/*of the next is a null or a comma and the amount of words is over*/
					return 0;
				wordCount--; 		/*next word check*/
			}
			else { 			/*going to a next comma*/
				sen++;
				senext++;
			}
			if (CheckAnyCommas(sen) == 0) { 		/*if there are no more commas*/
				wordCount--;
				break;
			}
		}
		if (wordCount > 0 || wordCount < 0) 		/*if there are not enough words*/
			return 0;
		return 1;
	}
}

/*****************************************************************
* CheckAnyCommas function
* checks if there are any more commas in string,
* then reuturns true (1) or false (0)
*****************************************************************/

int CheckAnyCommas(char* c) {
	while (*c != '\0') { 	/*running until the end of the sentence*/
		if (*c == ',') 		/*if a comma is found*/
			return 1;
		else
			c++; 		/*advance*/
	}
	return 0;
}

/*****************************************************************
* CheckValidity1 function
* checks the validity of a .string formatting,
* then reuturns true (1) or false (0)
*****************************************************************/

int CheckValidity1(char*sen) { /*checks the validity of a string*/
	char* comp = ".string \""; /*how a string should be*/
	while (*sen != '.' && *sen != '\0') /*reaching the '.string' part*/
		sen++;
	if (*sen == '\0') /*if end of string reached*/
		return 0;
	else {
		while (*comp != '\0' && *sen != '\0' && *comp == *sen) {	 /*while both are functioning and equal*/
			if (*sen == ' ' && *comp == ' ') { 		/*if there are spaces between '.string' and '"'*/
					sen++;
				if (*sen != ' ') 	/*if reached '"'*/
					comp++;
			}
			else { 		/*if not reached the spaces*/
				comp++;
				sen++;
			}
		}
		if (*sen == '\0' || *comp != '\0') 		/*if the string is not O.K.*/
			return 0;
		return 1;
	}
}

/*****************************************************************
* cutSentence function
* taking a requested string part off of a .string,
* then reuturning it
*****************************************************************/

char* cutSentence(char* sen, int index) { 		/*taking words off of a string*/
	char* reti = malloc(sizeof(char)*500);			/*returned word*/
	char* ret = reti;
	char* fin = reti;	 	/*returning value*/
	int isIndex = 0; 		/*word comparison index*/
	while (*sen != '\0') { 		/*reaching a word*/
		if (*sen == '"') {
			isIndex++;
			break;
		}
		sen++;
	}
	if (isIndex <= 0){ 		/*if there is no word*/
		free(reti); 
		return NULL;
	}
	else {
		while (*sen != '\0') {		 /*until the end of the string*/
			if (isIndex == index) { 		/*if that is the word we are searching for*/
				*ret = *sen;
				sen++;
				ret++;
				while (*sen != '"' && *sen != '\0') {		 /*copying the word*/
					*ret = *sen;
					sen++;
					ret++;
				}
				if (*sen == '\0'){	 	/*if we reached a null*/
					free(reti);
					return NULL;
				}
				else {		 /*finalizing the returned string*/
					*ret++ = '\"';
					*ret++ = '\0';
					return fin;
				}
			} 
			else {		 /*if not reached the willed index*/
				sen++;
				while (*sen != '"' && *sen != '\0') 		 /*running until the end of the in-string word*/
					sen++;
				sen++;
				if (*sen == '\0') {		/*if reached a null*/
					free(reti);
					return NULL;
				}
				else{
					while (*sen != '\0' && *sen != '"') 		/*going to the next word opening*/
						sen++;
					if (*sen == '\0'){ 		/*until the end of the sentence*/
						free(reti);
						return NULL;
					}
					isIndex++;	 	/*searched word index comparison is up by 1*/
				}
			}
		}
	}
	free(reti);
	return NULL;
}
