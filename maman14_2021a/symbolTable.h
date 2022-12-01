typedef struct symbolDef{
	char* name;
    int val;
    int type;       /*0 - .data , 1-. code , 2- extern*/
    int isentry;
    struct symbolDef* next;

}symbolDef;

typedef struct{
    symbolDef* head;
}symbolList;

void addSymbol(symbolList* list , char* _name , int _val , int _type , int _isentry);
void printTable(FILE* dest , symbolList* list  );
void addICF(symbolList* list , int ICF);
int findInTable(symbolList* list , char* _name);
int findEntry(symbolList* list );
int getVal(symbolList* list , char* _name);
void setEntry(symbolList* list , char *name);
int isExtern(symbolList* list , char* _name);
void emptyTable(symbolDef* node);
void clearTable(symbolList* list);
void printEntries(FILE* dest , symbolList* list);