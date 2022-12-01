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
* CheckLabel function
* we call CheckLabel when we want to define a label and to check if it is legal.
* CheckLabel checks if a word fits the parameter of a label and if it does it calls 
* doesLabelLegal with the parameter isDefined on  to check that it is a valid label.
*****************************************************************/

int CheckLabel(symbolList* list , char* word){
	int i = 0;
	char temp[40];
	resetString(temp , 40);
    if(word[strlen(word) - 1] == ':' && strlen(word) < 32){
		for(i = 0 ; i < strlen(word) - 1 ; i++)
			temp[i] = word[i];
		/*printf(".%s." , temp);*/
		if(doesLabelLegal(list , temp , 1) != 0)
			return 1;
		return -1;
	}
    return -2;
}

/*****************************************************************
* checkStoringData function
* check if word is .data or .string
*****************************************************************/

int checkStoringData(char* word){
    if(strcmp(word , ".data") == 0)
        return 1;
    if(strcmp(word , ".string") == 0)
        return 2;
    return 0;
}

/*****************************************************************
* checkExternEntry function
* check if word is .extern or .entry
*****************************************************************/

int checkExternEntry(char* word){
    if(strcmp(word , ".extern") == 0)
        return 1;
    if(strcmp(word , ".entry") == 0)
        return 2;
    return 0;
}

/*****************************************************************
* doesLabelLegal function
* check if word is one of the reserved words of assembler 
* check that word is only letters and numbers and that the first char is a letter
*****************************************************************/

int doesLabelLegal(symbolList* list , char* word , int isDef){
    char commands[16][4]= {"mov", "cmp" , "add" , "sub" , "lea" , "clr" , "not" , "inc" , "dec" , "jmp" , "bne" , "jsr" , "red" , "prn" , "rts" , "stop"};
    int i = 0;
	if((word[i] > 'z' || word[i] < 'a') && (word[i] > 'Z' || word[i] < 'A'))
		return 0;
	for(i = 1 ; i < strlen(word) ; i++){
		if((word[i] > 'z' || word[i] < 'a') && (word[i] > 'Z' || word[i] < 'A') && (word[i] > '9' || word[i] < '0'))
		return 0;
	}
	if (isDef == 1 && findInTable(list , word) != -1)		/*if isDef == 1 then we want to define a new label, but if we find it already exist then it is Illegal multiple attempts to redefine*/
		return 0;
	if(strlen(word) == 2 && word[0] == 'r' && '0' <= word[1] && word[1] <= '7' )
		return 0;
    if(strlen(word) > 31 || checkStoringData(word) != 0 || checkExternEntry(word) != 0)
        return 0;
    for(i = 0 ; i < 16 ; i++){
        if(strcmp(word , commands[i]) == 0)
            return 0;
    } 
    return 1;
}

/*****************************************************************
* numberVerification function
* Validates that the string is actually a number
*****************************************************************/

int numberVerification(char* str){ 
	int i = 0;
	int size = 0;
	if(str == NULL)
		return 1;
	size = strlen(str);
	for(i = 0; i < size ; i++)
		if(((str[i] < '0' || str[i] > '9') && str[i] != '-'  && str[i] != '+') || (i != 0 && str[i] == '-') || (i != 0 && str[i] == '+')) /* make sure that there is +/- sign then they can only be the first position*/
			return 0;
	return 1;
}

/*****************************************************************
* immediateVerification function
* Validates that the string is actually an immediate (format: #number)
*****************************************************************/

int immediateVerification(char* str){ 
	int i = 0;
	int size = 0;
	if(str == NULL)
		return 1;
	size = strlen(str);	
	for(i = 1; i < size ; i++){
		if( str[0] != '#')	/*first char is #*/
			return 0;
		if(((str[i] < '0' || str[i] > '9') && str[i] != '-'  && str[i] != '+') || (i != 1 && str[i] == '-') || (i != 1 && str[i] == '+')) /* make sure that there is +/- sign then they can only be the first position(after #)*/
			return 0;	
	}
	return 1;
}

/*****************************************************************
* ConvertStringToNumber function
* takes a string that we know represents a number, and returns said number
* imm is 1 if we want to convert an immediate(format: #number), and 0 if regular number
*****************************************************************/

int ConvertStringToNumber(char* str , int imm){ 	/*converts a string we found is a number to a double , if imm = 1 , the its a -#(number) , if imm=0, then the number is not an immediate*/
	int result = 0;		/*the returned int*/
	int sign = 1, size = 0;
	int i = 0, temp = 0;
	temp += imm;
	if(str == NULL)
		return 0;
	size = strlen(str);
	if(str[0 + imm] == '-'){		/*if the first position is +/- we change sign to match*/
		sign = -1;
		temp++;
	}
	if(str[0 + imm] == '+'){
		sign = 1;
		temp++;
	}
	for( i=temp ; i < size ; i++)
		result = 10 * result + (str[i] - '0');	/*converts string to int*/
	return result * sign;

}
/*****************************************************************
* checkRegister function
* checks if the string represents a register 
*****************************************************************/

int checkRegister(char* str){
	if(strlen(str) == 2 && str[0] == 'r' && '0' <= str[1] && str[1] <= '7' )
		return str[1]-'0';
	else
		return -1;
}

/*****************************************************************
* ChecksStringIsAddress function
* checks if the string represents a relative address(%ADDRESS)
*****************************************************************/

int ChecksStringIsAddress(symbolList* list , char* str){
	int i = 0;
	char temp[40];
	for(i = 0 ; i < 40 ; i++)
		temp[i] = '\0';
	if(str[0] != '%')
		return -1;
	else{
		for(i = 1 ; i < strlen(str) ; i++)
			temp[i - 1] = str[i];
	}
	if(doesLabelLegal(list , temp , 0) != 0)		/*calls isLegalLable to validate correctness*/
		return 1;
	return -1;
}

/*****************************************************************
* doesOperandsValid function
* takes the line of a command and index of first word of command,
* then validates that the operands are legal for the command
*returns the total number of words command takes in memmory, if illegal returns -1
*****************************************************************/

int doesOperandsValid(instructDef* inst ,symbolList* list , char* line , int index){
	 int numOfOperands = inst -> numOfOperants;
	 char* str = NULL;
	 char* str1 = NULL;
	 char* str2 = NULL;
	  str = getWord(line , index + 1);		/*take the needed words from line*/
	  str1 = getWord(line , index + 2);
	  str2 = getWord(line , index + 3);
	 if(numOfOperands == 0 && str == NULL)
		 return 1;
	 if(numOfOperands == 1 && str != NULL && str1 == NULL){ 	/*command has one operand - the destination - we check if the operand entered is legal*/
	 	free(str1);
		free(str2);
		if(str != NULL && immediateVerification(str) && isLegalDstMet(inst , 0) == 1){
			free(str);
			return 2;
		}	
		if(str != NULL && checkRegister(str) != -1 && isLegalDstMet(inst , 3) == 1){
			free(str);
			return 2;
		}
		if(str != NULL && ChecksStringIsAddress(list ,str)== 1 && isLegalDstMet(inst , 2) == 1){
			free(str);
			return 2;
		}
		if(str != NULL && doesLabelLegal(list , str ,0)== 1 && isLegalDstMet(inst , 1) == 1){
			free(str);
			return 2;
		}
	 }
	 if(numOfOperands == 2 && str != NULL && str1 != NULL &&  str2 == NULL){/*command has two operands - the source and destination - we check if the operands entered are legal*/
	 	if(((immediateVerification(str) ==1 && isLegalSrcMet(inst , 0)== 1) || (checkRegister(str)!= -1 && isLegalSrcMet(inst , 3)== 1) || (ChecksStringIsAddress(list , str)== 1 && isLegalSrcMet(inst , 2)== 1) || (doesLabelLegal(list , str , 0)== 1 && isLegalSrcMet(inst , 1)== 1))){
			if(((immediateVerification(str1) ==1 && isLegalDstMet(inst , 0)== 1) || (checkRegister(str1)!= -1 && isLegalDstMet(inst , 3)== 1) || (ChecksStringIsAddress(list , str1)== 1 && isLegalDstMet(inst , 2)== 1) || (doesLabelLegal(list , str1 ,0)== 1 && isLegalDstMet(inst , 1)== 1))){
				free(str);
				free(str1);
				free(str2);
				return 3;
			}
		}
		free(str);
		free(str1);
		free(str2);
		return -1;
	 }
	 free(str);
	 free(str1);
	 free(str2);
	 return -1;
}

/*****************************************************************
* codeFirstWord function
* takes the line of a command and index of first word of command,
* then codes the first word of command according to them, and returns it.
*****************************************************************/

memWord codeFirstWord(instructDef* inst ,symbolList* list , char* line , int index){
	memWord result;		/*returned struct*/
	char* str = NULL, str1 = NULL;
	result.data = 0;
	result.ARE = 'A';
	result.data = (inst -> opcode) << 8;	/*puts opcode and funct in the firstword*/
	result.data += (inst -> funct) << 4;
	if (inst -> numOfOperants == 1){	/*there is only a destination operand*/
		str1 = getWord(line , index + 1);
		/*completes the first memmory word of command with the addresing methods*/
		if(immediateVerification(str1) && isLegalDstMet(inst , 0)== 1)
			result.data += 0;	
		if(checkRegister(str1) != -1 && isLegalDstMet(inst , 3)== 1)
		 	result.data += 3;
		if(ChecksStringIsAddress(list ,str1) == 1 && isLegalDstMet(inst , 2)== 1)
			result.data += 2;
		if(doesLabelLegal(list , str1 ,0)== 1 && isLegalDstMet(inst , 1)== 1)
			result.data += 1;
		if(str1 != NULL){
			free(str1);
			str1 = NULL;
		}
	}
	if(inst -> numOfOperants == 2){		/*there are both dest and src*/ /*completes the first memory word of command with the addresing methods*/ 
	 	str = getWord(line , index + 1);
		str1 = getWord(line , index + 2);	 	
		if(immediateVerification(str) && isLegalSrcMet(inst , 0)== 1)
			result.data += 0 << 2;
		if(checkRegister(str) != -1 && isLegalSrcMet(inst , 3)== 1)
		 	result.data += 3<<2;
		if(doesLabelLegal(list , str ,0)== 1 && isLegalSrcMet(inst , 1)== 1)
		 	result.data += 1<<2;
		if(immediateVerification(str1) && isLegalDstMet(inst , 0)== 1)
		 	result.data += 0;
		if(checkRegister(str1) != -1 && isLegalDstMet(inst , 3)== 1)
		 	result.data += 3;
		if(doesLabelLegal(list , str1 ,0)== 1 && isLegalDstMet(inst , 1)== 1)
		 	result.data += 1;
	}
	if(str != NULL)
		free(str);
	if(str1 != NULL)
		free(str1);
	return result;
}

/*****************************************************************
* codeImmOrReg function
* takes the line of a command and index of first word of command,
* and what we have coded so far, and codes the second and third memmory words 
* if they contain an immediate or register
*****************************************************************/

int codeImmOrReg(instructionBinary* inst ,  char* line , int index){
	int temp = 0;
	char* tempw;
	inst -> src.data = 0;
	inst -> dst.data = 0;
	if(inst -> srcMet == 0 && inst -> length == 3 ){ 	/*code the source if it is immediate and function has 2 operands*/
		tempw = getWord(line , index + 1);
		temp = ConvertStringToNumber(tempw , 1);
		if(2048 > temp  && temp > -2049){
			if(temp >= 0){
				inst -> src.data = ConvertStringToNumber(tempw , 1);
				inst -> src.ARE = 'A';
			}
			else{
				inst -> src.data = codeNegNum(temp);
				inst -> src.ARE = 'A';
			}
		}
		else
			return 0;
		if(tempw != NULL)
			free(tempw);
		tempw = NULL;
	}
	if(inst -> srcMet == 3 && inst -> length == 3){		/*code the source if it is register and function has 2 operands*/
		temp = getWord(line , index + 1)[1] - '0';
		inst -> src.data = 1 << temp;
		inst -> src.ARE = 'A';
	}
	if(inst -> dstMet == 0 && inst -> length == 3){		/*code the destination if it is immediate and function has 2 operands*/
		tempw = getWord(line , index + 2);
		temp = ConvertStringToNumber(tempw , 1);
		if(2048 > temp  && temp > -2049){
			if(temp >= 0){
				inst -> dst.data = ConvertStringToNumber(tempw , 1);
				inst -> dst.ARE = 'A';
			}
			else{
				inst -> dst.data = codeNegNum(temp);
				inst -> dst.ARE = 'A';
			}
		}
		else
			return 0;
		free(tempw);
	}
	if(inst -> dstMet == 3 && inst -> length == 3){		/*code the destination if it is register and function has 2 operands*/
		temp = getWord(line , index + 2)[1] - '0';
		inst -> dst.data = 1 << temp;
		inst -> dst.ARE = 'A';
	}
	if(inst -> dstMet == 0 && inst -> length == 2){		/*code the destination if it is immediate and function has 1 operand*/
		tempw = getWord(line , index + 1);
		temp = ConvertStringToNumber(tempw , 1);
		if(2048 > temp  && temp > -2049){
			if(temp >= 0){
				inst -> dst.data = ConvertStringToNumber(tempw , 1);
				inst -> dst.ARE = 'A';
			}
			else{
				inst -> dst.data = codeNegNum(temp);
				inst -> dst.ARE = 'A';
			}
		}
		else
			return 0;
		free(tempw);
	}
	if(inst -> dstMet == 3 && inst -> length == 2){		/*code the destination if it is register and function has 1 operand*/
		temp = getWord(line , index + 1)[1] - '0';
		inst -> dst.data = 1 << temp;
		inst -> dst.ARE = 'A';
	}
	return 1;
}

/*****************************************************************
* codeNegNum function
* takes negative number, and returns it in its representation in 2's compliment in 12 bits
*****************************************************************/

int codeNegNum(int num){
	int result;
	int i = 0;
	result = 0;
	if(num < 0){
		num = num * -1;
		for(i = 11 ; i >= 0 ; i--)
			result += (!((num >> i) &1)) << i;
		result++;
	}
	return result;
}

/*****************************************************************
* resetInstructBinary function
* initalize instructBinary with neutral values
*****************************************************************/

void resetInstructBinary(instructionBinary* inst){
	inst -> dst.data = 0;
	inst -> dst.ARE = '\0';
	inst -> dstMet = 0;
	inst -> src.data = 0;
	inst -> src.ARE = '\0';
	inst -> srcMet = 0;
	inst -> firstword.data = 0;
	inst -> firstword.ARE = '\0';
	inst -> ic = 0;
	inst -> length = 0;

}

/*****************************************************************
* firstTransition function
* this function implements the first pass of the assembler 
* as described in maman 14.
* it recieves the filename we need to read from, and will print to the terminal if any errors occour.
* In case of ERROR - maximum 1 error is displayed per line.  
*****************************************************************/

void firstTransition(char* fileName){
	FILE *f1 , *fob , *fext , *fent;	/*the file pointers we need*/
	int error = 0; 		/*error counter*/
    symbolList list;		/*list to contain all symbols*/
	symbolList externList;		/*list to hold the externals and theire values for the output files*/
    char* line , *word  ,*tempWord;		/*strings we will use*/
	char temp[40], ctemp, tempFileName[100];
    instructionBinary arr[100];		/*array to hold the binary code of the instructions - assumed max size of 100 instructions*/
	memWord dataImage[100];		/*array to hold the binary code of the data - assumed max size of 100 data words*/
    int dc = 0 , ic = 100 , L = 0 , instructionCounter = 0 , icf = 0 , dcf =0;		/*counters we need*/
    instructList ilist;		/*the instruction list for reference*/
    instructDef* tempInst;		/*temporaries we use*/
    int i = 0 , index = 0 , number = 0 , temporary = 0, end = 0 , isLabelFlag = 0 , isEntryFlag = 0 , done = 0 , stopSignRead = 0 , stopWithLable = 0 ;		/*flags we use*/
    unsigned int num = 0;
    list.head = NULL;		/*initializing the variables*/
	externList.head = NULL;
	resetString(tempFileName , 100);
	strcpy(tempFileName , fileName);
	strcat(tempFileName , ".as");
	f1 = fopen(tempFileName , "r+");
	if(f1 == NULL){/*if file not found give error and end*/
		printf("ERROR:");
		printf(" file \"%s\" not found\n" , fileName);
		return;
	}
	else{
		printf("***********************\n");
		printf("START PROCESSING FILE: %s\n", fileName);
		printf("***********************\n");
	}	
	fillTable(&ilist);
	for(i = 0 ; i<100 ; i++){
		resetInstructBinary(&arr[i]);
		dataImage[i].data = 0;
		dataImage[i].ARE = 'A';
	}
	resetString(temp , 40);
	while (end == 0){		/*enter the loop*/
		stopWithLable = 0;
		if(instructionCounter >= 100 || dc >= 100){		/*we check if we exceeded the max length of code allowed*/
			printf("ERROR:");
			printf(" code too long\n");
			error++;
			done = 1;
			end = 1;
		}
		line = readNextLine(f1);
		if(line != NULL && line != (char*)EOF){		/*if line is not a comment or EOF, then start analyzing*/	
			word = getWord(line , index);
			index++;
			if(CheckLabel(&list , word) == 1){			/*check for lable, turn on flag*/
				isLabelFlag = 1;
				free(word);
				word =getWord(line , index);
			}
			if(done == 0 && word == NULL && isLabelFlag == 1){
				printf("ERROR:");
				printf(" missing arguments\n");
				done = 1;
				error++;
				if(line != NULL)
					printf("   in line: %s\n" , line);		
			}
			if(done == 0 && checkStoringData(word)){		/*if we are storing data, enter*/
				tempWord = getWord(line , 0);
				if(isLabelFlag){		/*if we have a lable, add it to the list*/
					tempWord[strlen(tempWord)-1] = '\0';
					if(checkStoringData(tempWord) == 0)
						addSymbol(&list , tempWord , dc , 0 ,isEntryFlag);
				}
				if(strcmp(word , ".data") == 0){		/*we need to insert integers*/
					if(checkStoringData(tempWord) == 0)
						index++;
					free(word);
					word =getWord(line , index);
					i = 0;
					while(word != NULL && numberVerification(word)){		/*as long as the strings are actual integers keep going*/
						number = insertInt(dataImage , dc , word );
						if (number > dc)
							dc = number;
						index++;
						free(word);
						word =getWord(line , index);
						i++;
					}
					if(done == 0 && word != NULL && numberVerification(word) == 0 ){
						error++;
						done = 1;
						printf("ERROR:");
						printf(" string inserted to .data command is not real num\n");
						if(line != NULL)
							printf("   in line: %s\n" , line);		
					}
					if(done == 0 && line != NULL && checkComma(line , i) == 0){		/*check commas in line*/
						printf("ERROR:");
						printf(" bad commas in list\n");
						error++;
						done = 1;
						if(line != NULL)
							printf("   in line: %s\n" , line);		
					}
					i = 0;
					done = 1;
				}
				if(word != NULL && strcmp(word , ".string") == 0 && done == 0){		/*we need to insert a string to the data image*/
					i = 1;
					index++;
					free(word);
					word = cutSentence(line , i)		/*get the string to insert*/
					if(done == 0 && CheckValidity1(line) == 0){		/*check it for validity*/
						error++;
						done = 1;
						printf("ERROR:");
						printf(" bad character/s in .string funct\n");
						if(line != NULL)
							printf("   in line: %s\n" , line);		
					}
					if(word != NULL){		/*if we have the string, insert it, check for errors*/
						number = insertStr(dataImage , dc ,word);
						if (number > dc)
							dc = number;
						index++;
						i++;
						free(word);
						word = cutSentence(line , i);
					}
					else{
						if(done == 0){
							printf("ERROR:");
							printf(" illegal string to enter\n");
							error++;
							done = 1;
							if(line != NULL)
								printf("   in line: %s\n" , line);		
						}
					}
					ctemp = line[strlen(line) - 1];
					if(ctemp !='\"' && done == 0){
						printf("ERROR:");
						printf(" extra text at end of line\n");
						error++;
						if(line != NULL)
							printf("   in line: %s\n" , line);		
					}
					done = 1;
				}
			}
			if(word != NULL){	/*line is not storing data*/
				i = 0;
				temporary = checkExternEntry(word);
				i = strcmp(word , ".extern");
				if(done == 0 && temporary > 0){		/*if entry/extern, enter*/
					if(temporary == 2)
						done = 1;
					if (isLabelFlag && done == 0){		/*check for lable before entry/extern*/
						done = 1;
						printf("ERROR:");
						printf(" label before entry/extern  \n");
						error++;
						if(line != NULL)
							printf("   in line: %s\n" , line);		
					}
					if(word != NULL)
						free(word);		/*checking for errors*/
					word = getWord(line , index);
					index++;
					if(word != NULL){
						num = temporary;
						temporary = doesLabelLegal(&list , word , 0);
					}
					else if(done == 0){
						done = 1;
						printf("ERROR:");
						printf(" missing name after .extern  \n");
						if(line != NULL)
							printf("   in line: %s\n" , line);	
						error++;
					}
					if(tempWord != NULL)
						free(tempWord);
					tempWord = getWord(line , index);
					if(tempWord != NULL && done == 0){
						error++;
						done = 1;
						printf("ERROR:");
						printf(" too many arguments for .extern/.entry funct.\n");
						if(line != NULL)
							printf("   in line: %s\n" , line);		
					}
					if(checkComma(line , 0) == 0 && done == 0){
						error++;
						done = 1;
						printf("ERROR:");
						printf(" illegal comma in line.\n");
						if(line != NULL)
							printf("   in line: %s\n" , line);		
					}
					if(done == 0 &&   temporary != 0 && findInTable(&list , word) == -1 ){		/*if lable okay, add it as extern*/
						if(i == 0)
						addSymbol(&list , word, 0 , 2 , 0);
						else{
							done = 1;
						}
						done = 1;
					}
					else{		/*report errors found*/
						if(done == 0 && word != NULL && findInTable(&list , word) != -1 && num != 2){
							printf("ERROR:");
							printf(" trying to redefine existing variable\n");
							error++;
							done = 1;
							if(line != NULL)
								printf("   in line: %s\n" , line);		
						}
					}
					free(tempWord);
					if(temporary == 0 && done == 0){
						printf("ERROR:");
						printf(" illegal label\n");
						if(line != NULL)
							printf("   in line: %s\n" , line);		
						done = 1;
						error++;
					}
				}
			}
			if(word != NULL && strcmp(word , "stop")==0){		/*command is stop - code it and turn on flag*/
				if(word != NULL ){
					free(word);
					word = NULL;
				}
				word = getWord(line , index + 1);
				if (word == NULL){
					arr[instructionCounter].firstword = codeFirstWord(getDef(&ilist , "stop") , &list , line , index - 1 + isLabelFlag);
					arr[instructionCounter].ic = ic;
					arr[instructionCounter].length = 1;
					instructionCounter++;
					ic++;
					stopSignRead = 1;
					stopWithLable = 1;
				}
				done = 1;
				if(word  != NULL){
					free(word);
					word = NULL;
				}
			}
			
			if(isLabelFlag && (done == 0 || stopWithLable == 1)){		 /*we are in a command line, if we have a lable, add it with type - code*/
				tempWord = getWord(line , 0);
				if(tempWord != NULL && strlen(tempWord) > 0){
					tempWord[strlen(tempWord) - 1] = '\0';
					addSymbol(&list , tempWord , ic - stopWithLable , 1 ,isEntryFlag);		/*new, from now*/
					stopWithLable = 0;
				}
			}
			else{		/*catch errors*/
				if(done == 0){
					tempWord = getWord(line , 0);
					temporary = CheckLabel(&list , tempWord);
					if(temporary == -1){
						printf("in first illegal label\n");
						printf("ERROR:");
						printf(" illegal label\n");
						error++;
						if(line != NULL)
							printf("   in line: %s\n" , line);		
						done = 1;
						/*end =1;*/
					} 
				}
				
			}
			if(done == 0){		/*coding the command*/
			temporary = isDefined(&ilist , word);
				if(temporary == -1){		/*check the command name*/
					printf("ERROR:");
					printf(" undefined command name\n");
					error++;
					if(line != NULL)
						printf("   in line: %s\n" , line);
					done = 1;
				}
				else{
					tempInst = getDef(&ilist , word);
					temporary = doesOperandsValid(tempInst , &list , line , index - 1 + isLabelFlag);		/*check validity of operands*/
					if(temporary != -1){
						L = temporary;
						if(checkComma(line , L - 1)|| (checkComma(line , L - 2) && L == 2)){		/*if line is good, code it into the instruction array*/
							arr[instructionCounter].firstword = codeFirstWord(tempInst , &list , line , index - 1 + isLabelFlag);
							putSrcDstMetLIC(&arr[instructionCounter] , L , ic);
							number =  codeImmOrReg(&arr[instructionCounter] , line , index - 1 + isLabelFlag);
							if(number == 1){
								instructionCounter++;
								ic += L;
								done =1;
							}
							else if(done == 0){/*catch errors*/
								error++;
								printf("ERROR:");
								printf(" invalid value in immidiate\n");
								if(line != NULL)
									printf("   in line: %s\n" , line);			
							}
						}
						else if(done == 0){
							printf("ERROR:");
							printf(" bad commas\n");
							error++;
							if(line != NULL)
								printf("   in line: %s\n" , line);
						}
						done = 1;
					}
					else if(done == 0){
						printf("ERROR:");
						printf(" illegal operands for command\n");
						error++;
						if(line != NULL)
							printf("   in line: %s\n" , line);
						done = 1;
					}
				}
			}
			if(end == 0){
				isLabelFlag = 0;
				index = 0;
				done = 0;
			}
		}
		else{
			if(done == 0 && line == (char*)EOF && stopSignRead == 0){
				end = 1;
				error++;
				printf("ERROR:");
				printf(" end of file reached without \"stop\" command\n");
			}
			if(line == (char*)EOF && stopSignRead == 1)
				end = 1;
		}
		
	}
	/*end of loop - start second pass and output files*/
	printf("**************\n");
	printf("NUMBER OF ERRORS FOUND IN THE FIRST PASS: %d\n" , error);
	printf("**************\n");
	if(error == 0){
		icf = ic;
		dcf = dc;
		fclose(f1);
		f1 = fopen(tempFileName , "r+");
		addICF(&list , icf);
		number = secondTransition(f1 , &arr[0] , &list , &externList);
		printf("FIRST PASS SUCCESS\n");
		if(number == 0){
			printf("SECOND PASS SUCCESS\n");
			resetString(tempFileName , 100);
			strcpy(tempFileName , fileName);
			strcat(tempFileName , ".ob");
			if(tempFileName != NULL)
				fob = fopen(tempFileName , "w");
			if(fob != NULL){ 		/*print the .ob file*/	
				fprintf(fob , "   %d %d \n" , icf-100 , dcf);
				i = 0;
				while(arr[i].length != 0 || arr[i].firstword.data != 0 || arr[i].ic != 0){
					num = 0;
					fprintf(fob , "%04d %03X %c \n" ,arr[i].ic, arr[i].firstword.data & 0xFFF , arr[i].firstword.ARE);
					if(arr[i].length == 3){
						num = 1;
						fprintf(fob , "%04d %03X %c \n", arr[i].ic+num , arr[i].src.data & 0xFFF , arr[i].src.ARE);
					}
					if(arr[i].length > 1 )
						fprintf(fob ,"%04d %03X %c \n", arr[i].ic+1+num , arr[i].dst.data & 0xFFF , arr[i].dst.ARE);
					i++;
				}
				for(i=0 ; i < dcf ; i++)
					fprintf(fob , "%04d %03X %c \n" , i+icf  , dataImage[i].data &0xFFF , dataImage[i].ARE);
			}
			else{
				printf("**************\n");
				printf("FAILED TO OPEN .OB FILE\n");
				printf("**************\n");
			}	
			resetString(tempFileName , 100);/*create .ent and .ext files*/
			strcpy(tempFileName , fileName);
			strcat(tempFileName , ".ent");
			if(findEntry(&list) != -1){
				remove(tempFileName);
				fent = fopen(tempFileName , "w");
				if(fent != NULL){
					printEntries(fent , &list);
					fclose(fent);
				}
				else{
					printf("**************\n");
					printf("FAILED TO OPEN .ENT FILE\n");
					printf("**************\n");
				}
			}
			resetString(tempFileName , 100);
			strcpy(tempFileName , fileName);
			strcat(tempFileName , ".ext");
			if(externList.head != NULL && tempFileName != NULL){
				remove(tempFileName);
				fext = fopen(tempFileName , "w");
				if(fext != NULL){
					printTable(fext , &externList);
					fclose(fext);
				}
				else{
					printf("**************\n");
					printf("FAILED TO OPEN .EXT FILE\n");
					printf("**************\n");
				}
			}
			/*close and clear things*/
			fclose(f1);
			clearTable(&list);
			clearTable(&externList);
		}
		else{
			printf("**************\n");
			printf("NUMBER OF ERRORS FOUND IN THE SECOND PASS: %d\n" , number);/*notify about errors*/
			printf("**************\n");
			printf("TOTAL NUMBER OF ERRORS FOUND: %d" , error + number);
			printf("**************\n");
				
		}
	}
	else{
		printf("**************\n");
		printf("ERRORS FOUND IN FIRST PASS - EXITING\n");
		printf("**************\n");
	}
}


