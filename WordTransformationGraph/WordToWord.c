#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h> //For all characters
#define MAX 10 //Maximum letter count in a word
#define MAX_WORD 2500 //Maximum word count
#define ATA(a,b) strcpy(a,b)  //copy string b to string a
#define AYNI(a,b) strcmp(a,b)==0 //compare string a and string b

typedef struct{
	char word[MAX]; //Word
	short int id; //Word id
}NODE;

typedef struct{
	int size; //Graph counter
	int matrix[MAX_WORD][MAX_WORD]; // Adjacency Matrix
	NODE* nodes[MAX_WORD]; //Array that holding nodes 
}GRAF;

typedef struct{
	int size; //Queue counter
	NODE* word[MAX_WORD]; //Node array
}QUEUE;

typedef struct{
	int top; //Stack counter
	char word[MAX_WORD][MAX]; //string array
}STACK; //2. kelimeye ulaþtýktan sonra, 1 ve 2. kelime arasýndaki kelimeleri yazdýrýrken stack kullanacaðým

GRAF* graph; //Main graph

void push(QUEUE*,NODE*); //Adding node to queue
NODE* dequeue(QUEUE*);	//Dequeue function
int isEmpty(QUEUE*);	//returning if queue is empty or not
void addWord(char[MAX]);//Add Word to queue
int baglanti(NODE*,NODE*);// Checking is there a 1 letter change between 2 given words
int difcount(char[MAX],char[MAX]);	//function that returns 
int donusumVarMi(char[MAX],char[MAX]);//function that returns Is there is a transiction between 2 words
char* getWordById(int);	//returns word of given id
NODE* getNodeByWord(char[MAX]); //returns node of given word
int contains(char[MAX]);	//returns if graf contains given word
void stackPush(STACK*, char[MAX]); //push function for stack
char* pop(STACK*);	//pop function for stack

int main(){
	setlocale(LC_ALL,"Turkish"); //For all characters
	
	//All alloctions and first assigns of graph
	graph=(GRAF*)malloc(sizeof(GRAF));
	graph->size=-1; 
	
	//Variables for inputs
	int cevap;
	char tmp[MAX];
	char tmp2[MAX];
	//Counters
	int i,j;

	//File read
	FILE* file = fopen("kelime.txt","r");
	if(file!=NULL){
		while(!feof(file)){
			fscanf(file,"%s",tmp); //Read word and assign to tmp
			addWord(tmp); //add word to graph
		}
	}
	else{
		printf("File not found.");
		return -1;
	}
	fclose(file); //Close file


	//Main Menu loop
	while(1){
		printf("\n\nSelect Process:\n1. Is there a transiction between 2 given words\n2. Is there a adjacent between 2 words\n3.Quit\nAnswer: ");
		scanf("%d",&cevap);
		printf("\n");
		switch(cevap){
			case 1:
				printf("Enter first word: ");
				scanf("%s",tmp);
				printf("Enter second word: ");
				scanf("%s",tmp2);
				donusumVarMi(tmp,tmp2);
				break;
			case 2:
				printf("Enter first word: ");
				scanf("%s",tmp);
				printf("Enter second word: ");
				scanf("%s",tmp2);
				printf("Adjacence between 2 words %s exists.", baglanti(getNodeByWord(tmp),getNodeByWord(tmp2)) ? "": "not");
				break;	
			case 3:
				return 0;
				break;
			default:
				printf("Invalid input");
				break;
		}
		
	}
	return 0;
}
void yazdir(STACK* stack){
	int i;
	printf("\n");
	for(i=0;i<stack->top;i++){
		printf("%s-",stack->word[i]);
	}
	printf("\n");
}
void push(QUEUE* queue, NODE* word){
	queue->word[queue->size]=(NODE*)malloc(sizeof(NODE));	//Allocation
	memcpy(queue->word[queue->size],word,sizeof(NODE));	//Assigning word to queue
	queue->size++; //Increase queue s-counter
}
NODE* dequeue(QUEUE* queue){
	int i;
	
	int tmp = queue->word[0]->id;	
	char tmp2[MAX];
	strcpy(tmp2,queue->word[0]->word);
	
	for(i=0;i<queue->size-1;i++)
		memcpy(queue->word[i],queue->word[i+1],sizeof(NODE));
		
	queue->size--;
	
	NODE* tmpNode = (NODE*)malloc(sizeof(NODE));
	tmpNode->id=tmp;
	strcpy(tmpNode->word,tmp2);	
	return tmpNode;
}
int isEmpty(QUEUE* queue){
	return queue->size==0; 
}
int baglanti(NODE* n1, NODE* n2){ 
	return graph->matrix[n1->id][n2->id]; //Checking adjacence between 2 nodes using adjacency matrix
}
void addWord(char word[MAX]){
	int i; //Counter
	
	
	NODE* newNode = (NODE*)malloc(sizeof(NODE)); 
	newNode->id=++graph->size;	
	strcpy(newNode->word,word);	
	

	if(graph->size==0)
		graph->matrix[0][0]=0;
	else{ 
		for(i=0;i<graph->size;i++){
			graph->matrix[i][newNode->id]= difcount(getWordById(i),newNode->word)==1;
			graph->matrix[newNode->id][i]= difcount(getWordById(i),newNode->word)==1;
		}
	}	

	graph->nodes[graph->size]=(NODE*)malloc(sizeof(NODE));
	
	memcpy(graph->nodes[graph->size],newNode,sizeof(NODE));
}
int difcount(char word1[MAX], char word2[MAX]){
	int i=0;
	int dif_count=0; 
	
	if(strlen(word1)!=strlen(word2)){
		return -1;
	}

	int length = strlen(word1);
	
	
	for(i=0;i<length;i++)
		if(word1[i]!=word2[i])
			dif_count++;
	
	
	return dif_count;
}
int donusumVarMi(char word1[MAX],char word2[MAX]){
	//If at least one of word is not ing graph, function doesn't countinue
	if(!contains(word1) || !(contains(word2))){
		printf("\nKELÝME GRAFDA YOK");
		return 0;
	}
	//Allcoation to new Queue
	QUEUE* queue=(QUEUE*)malloc(sizeof(QUEUE));
	queue->size=0;
	
	char out[MAX]; //Array that holds words
	
	int visited[MAX_WORD]; //Variables that holds words that pushed to stack before
	
	int i; 
	
	//First assign of visited array
	for(i=0;i<graph->size;i++)
		visited[i]=-1;	
	
	ATA(out,word1); //Assign word1 to out
	push(queue,getNodeByWord(word1)); //push word1 to queue
	
	//tag word1 as visited
	visited[getNodeByWord(word1)->id]=1;
	
	//Until we reach second word or queue is empty
	while(!isEmpty(queue) && !AYNI(out,word2)){
		ATA(out,dequeue(queue)->word);	//Assign word from dequeue to out
		if(!AYNI(out,word2)){ //If out is not word2
		
			//Add all words that have 1 letter change with out to queue
			for(i=0;i<=graph->size;i++){ 
				//Is word have 1 letter difference from out?
				if(graph->matrix[getNodeByWord(out)->id][i]==1 && visited[i]==-1){
					//If word is not visited
						//Add word's node to queue
						push(queue,getNodeByWord(getWordById(i)));
						//tag word as visited
						visited[i]=1;
					
				}
			}
		}
	}
	

	if(isEmpty(queue) ){
		printf("No Connection between 2 words!");
		return 0;
	}


	//Algorithm to print the way from word1 to word2 
	else{	
		//Allocating stack and first assigns
		STACK* stack=(STACK*)malloc(sizeof(STACK));
		stack->top=-1;
		stackPush(stack,out); //push word2 to stack
		
		NODE* back=(NODE*)malloc(sizeof(NODE)); 
		back= getNodeByWord(word2); //First word2 assigned to back Node
		
		//When we making a backtracking, return visited words to unvisited back.
		visited[back->id]=-1;
		
		//Until we reach word1, try nodes which is visited and have an adjacence with back node to get the path
		while(!AYNI(out,word1)){
			int found=0; //Is there a path
			i=0; 
			//Founding a path
			while(i<=graph->size && !found){
				//If word we trying to find is visited at BFS algorithm adn have a connection with back word taht means we found
				if(graph->matrix[back->id][i]==1 && visited[i]==1){
					found=1;
				} 
				else
					i++;
			}
			//If there is a dead end, we make a back step
			if(found==0){
				back=getNodeByWord(pop(stack)); //assign word which is popped from stack to back node
			}
			//If we found a path, we continue until we reach word1
			else{
				back=getNodeByWord(getWordById(i));
				visited[i]=-1; //visited of node is returns normal
				ATA(out,back->word); //assigning new word to out
				stackPush(stack,out); //pushing new word to stack

			}
		}
		
	
		int donusumSayisi = stack->top; //variable that holds transiction change count
		printf("\n");
		printf("%s-",word1);
		while(stack->top!=0){
			printf("%s-",pop(stack));
		}
		
		printf(" transiction verified with %d transiction change!",donusumSayisi);
		return 1;
	}
		
}
char* getWordById(int id){
	return graph->nodes[id]->word;
}
NODE* getNodeByWord(char word[MAX]){
	int i;
	for(i=0;i<=graph->size;i++)
		if(strcmp(graph->nodes[i]->word,word)==0)
			return graph->nodes[i];
	return NULL;
}
int contains(char word[MAX]){
	int i;
	for(i=0;i<=graph->size;i++)
		if(AYNI(word,(graph->nodes[i])->word))
			return 1;
		
	return 0;		
}
int isEmptyS(STACK* stack){
	if(stack->top==-1)
		return 1;
	return 0;
}
void stackPush(STACK* stack,char word[MAX]){
	
	ATA(stack->word[++stack->top],word);
}
char* pop(STACK* stack){
	return stack->word[--stack->top];
}
