#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char ot[127][15];

typedef struct Node
{
    int number;
    int character;
    struct Node *leftNode;
    struct Node *rightNode;
    struct Node *parents;
}node;

void bublesort(node alpha[],int leftNode,int rightNode);
void printcode(node *arr,char *codeString,int level,FILE *op);

int main (){
    int c,i,A=0,C=0,D=0,temp=0;
    int symbol[256]={};
    char bitsGenerate;
    char name[10];
    char ans[500];
    node n[500];
    FILE *ptrTxt,*ptrCode,*ptrOutput;

    //input file
    printf("Enter the file name(the same package with .txt):\n");
    scanf("%s",name);
	
	//open file
    ptrTxt= fopen(name,"r");

    if(ptrTxt== NULL){
        printf("Loading Failed!");
    }
    else{
        do{
            c = fgetc(ptrTxt);
            symbol[c]=symbol[c]+1;
          } while (c != EOF);
    }

    //read to the array
    for(i=0;i<128;i++){
        n[A].number=symbol[i];
        n[A].character=i;
        n[A].leftNode=NULL;
        n[A].rightNode=NULL;
        A++;
        if(symbol[i]>0)
        temp++;
    }

    //sort the node and store the node
    printf("A:%d",A);
    for(C=0;C<temp-1;){
        bublesort(n,0+D,A+C-1);
        if(n[D].number>0){
                n[A+C].number=n[D].number+n[D+1].number;
                n[A+C].leftNode=&n[D];
                n[A+C].rightNode=&n[D+1];
                n[A+C].character=15;
                n[A+C].parents=NULL;
                n[D].parents=&n[A+C];
                n[D+1].parents=&n[A+C];
                D=D+2;
                C++;
            }
        else
            D++;
    }
	
    ptrCode= fopen("Codebook.txt","w");
    ptrOutput= fopen("Output.txt","wb");
    printf("\nEncode the code and, calculate the counts:\n");
	//input Codebook
    printcode(&n[A+temp-2],&ans[0],0,ptrCode);
	//find the unit in array
    fseek(ptrTxt,0,SEEK_SET);
    while ((c = fgetc(ptrTxt)) != EOF){
     fprintf(ptrOutput,"%s",ot[c]);
    }
    fclose(ptrOutput);
    fclose(ptrCode);
    ptrOutput=fopen("Output.txt","r");
    ptrTxt=fopen("Zip.bin","wb");

    //transfer stored data into bitstream
    i=getc(ptrOutput);
    for(;i>0;i=getc(ptrOutput)){
        bitsGenerate=bitsGenerate*2+i-'0';
        c++;

        if(c==8){
            fwrite(&bitsGenerate,sizeof(char),1,ptrTxt);
            c=0;
            bitsGenerate=0;
        }
    }
    fwrite(&bitsGenerate,sizeof(char),1,ptrTxt);
    bitsGenerate=8-c;
    fwrite(&bitsGenerate,sizeof(char),1,ptrTxt);
    printf("Finish the Compression!");
    fclose(ptrOutput);
    fclose(ptrTxt);
    return 0;
}

void bublesort(node alpha[],int leftNode,int rightNode)
{
    int i;
    node a;
    for(i=leftNode;i<rightNode;i++)
        if(alpha[i].number>alpha[i+1].number){
            a=alpha[i];
            alpha[i]=alpha[i+1];
            alpha[i+1]=a;
            i=leftNode-1;
        }
}

void printcode(node *arr,char *codeString,int level,FILE *op)
{

    if((arr->leftNode)!=NULL){
        codeString[level]='0';
        printcode(arr->leftNode,codeString,level+1,op);
        codeString[level]='1';
        printcode(arr->rightNode,codeString,level+1,op);
    }
    else{
        int a=arr->character;
        printf("< %c > have the code",arr->character);
        codeString[level]='\0';
        printf(":%-20s,",codeString);
        printf("and it has %d counts.\n",arr->number);
        fprintf(op,"%c:%s\n",arr->character,codeString);
        strcpy(ot[a],codeString);
    }
}
