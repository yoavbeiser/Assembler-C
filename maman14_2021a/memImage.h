typedef struct 
{
     int data : 12;
     char ARE;      /* gets A , R or E according to rules*/
}memWord;

typedef struct 
{
     memWord firstword;
     memWord src;
     memWord dst;
     unsigned int srcMet : 2;       /*0 , 1 , 2 or 3 according to the "שיטת מיעון"*/
     unsigned int dstMet : 2; 
     unsigned int length : 2;        /*1 , 2 ,or 3 according to how many words the instruction takes*/
     int ic;  

}instructionBinary;
int insertInt(memWord arr[] , int dc , char* input);
int insertStr(memWord arr[] , int dc , char* input);
int putSrcDstMetLIC(instructionBinary* inst , int L , int IC);