typedef struct instructDef{     /*this struct is for saving the properties of every instruction and using as reference in different functions*/
	char* name;
    unsigned int opcode : 4;    /*the opcode of instruction*/
    unsigned int funct : 4;     /*the funct of instruction*/
    unsigned int legalSrcMet : 4;   /*if the bit is on , then the method is legal*/
    /* changed-if 0 - no source , if 1- legal addresing mode 1 , if 2 - legal addresimg modes 0, 1, 3*/
    unsigned int legalDstMet : 4;    /*if the bit is on , then the method is legal*/
    unsigned int numOfOperants : 2;     /*indicates number of allowed operands, 0, 1, 2*/
    struct instructDef* next;

}instructDef;

typedef struct{
    instructDef* head;
}instructList;      /*we save the definitions in a list*/
void addInstruction(instructList* list , char* _name , int _opcode , int _funct , int _legalSrcMet, int _legalDstMet , int numOfOperants);
void fillTable(instructList* list);
int isDefined( instructList* list, char* name);
int isLegalSrcMet(instructDef* def, int num);
int isLegalDstMet(instructDef* def, int num);
instructDef* getDef(instructList* list, char* name);