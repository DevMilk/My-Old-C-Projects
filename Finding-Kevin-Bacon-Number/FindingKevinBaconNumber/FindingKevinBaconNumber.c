#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#define MAX_FILE_NAME 50
#define MAX_ACTOR_COUNT 176680
#define MAX_MOVIE_COUNT 15000
#define MAX_ACTOR_NAME 120
#define MAX_MOVIE_NAME 200
#define M (MAX_ACTOR_COUNT+MAX_MOVIE_COUNT)
#define R 7
#define SAME 0
struct Node{
	short int isEmpty;
	char* name;
	struct Node** list;
	short int listCounter;
	struct Node* src; //To print way backward easily, we need that
};
typedef struct Node Node;
int NodeLimit=MAX_ACTOR_COUNT+MAX_MOVIE_COUNT;
int nodeCounter=0;
Node* GraphList[M]; //Head array of Graph


int getDistance(char*,char*); //Function returns distance of given 2 actor name-username
int readFile(char*); //Reads and parses file to fill graph 
int addMovie(char*); //Adds movie to graph and returns GraphList index of added movies
void addActor(int,char*); //Adds actor to graph
unsigned long long int horner(char*); //Returns key of string
int getIndex(char*,int); //Gets index of given Actor/Movie name
int isHarf(char); //Checks if given character is suitable cahracter for a name
void insertToNode(Node*,Node*); //Inserts node to other node's list
int findNode(Node*, Node*); //Checks if a node is in other node's list
void allocate(Node*); //Allocate list of a node
int main(){
	setlocale(LC_ALL, "Turkish"); 
	
	int i,j;
	for(i=0;i<M;i++){
		GraphList[i]=(Node*)malloc(sizeof(Node));
		GraphList[i]->isEmpty=1;
		GraphList[i]->listCounter=0;			
	}
	char shortansw='0';
	char answer[MAX_FILE_NAME];
	char answer2[MAX_FILE_NAME];
	
	int res;
	printf("Enter text file: ");
	scanf("%s",answer);
	if(readFile(answer)!=1){
		exit(-1);
	}
	printf("File read.\n");
	gets(answer); //For fixing bug
	while(shortansw!='3'){
		strcpy(answer,"");
		strcpy(answer2,"");
		printf("--------------------------MENU--------------------------\n1.Find Kevin Bacon number of given actor\n2.Find distance between given 2 actors\n3.Exit\n--------------------------------------------------------\nAnswer: ");
		shortansw=getch();
		printf("%c\n",shortansw);
		switch(shortansw){
			case '1':
				printf("Enter Actor's name and surname: ");
				gets(answer);
				printf("\n");
				res = getDistance(answer,"Kevin Bacon");

				if(res>0 && res<=6)
					printf("Distance between %s and Kevin Bacon: %d\n",answer,res);
				else if(res==-1 || res>6)
					printf("There is no relation between %s and Kevin Bacon\n",answer);
				printf("\n\n");
				break;
			case '2':
				printf("Enter first Actor's name and surname: ");
				gets(answer);
				printf("\n");
				printf("Enter second Actor's name and surname: ");
				gets(answer2);
				printf("\n");			
				res = getDistance(answer,answer2);
				if(res>0)	
					printf("Distance between %s and %s: %d\n",answer,answer2,res);
				else if(res==-1 || res>6)
					printf("There is no relation between %s and %s\n",answer,answer2);
				printf("\n\n");
				break;
			case '3':
				break;
			default:
				printf("Invalid answer!!\n\n\n");
				break;
		}
		
		
	}
	for(i=0;i<M;i++)
		free(GraphList[i]);
	return 0;
}
int readFile(char* fileName){
	//Parse File
	char surname[MAX_ACTOR_NAME];
	char name[MAX_ACTOR_NAME];
	char movie[MAX_MOVIE_NAME];
	char c;
	FILE* dosya = fopen(fileName,"r");
	if(!dosya){
		printf("File can't be read\n");
		return 0;
	}
	
	//Assign all file to a buffer at the beggining
  	fseek(dosya,0,SEEK_END);
  	int dosyasize=ftell(dosya)+1;
  	rewind(dosya);
  	char* buffer = (char*)malloc(dosyasize*sizeof(char));
  	fread(buffer,1,dosyasize,dosya);
  	fclose(dosya);
  	
  	
  	int bufCount=0;
	while(bufCount<dosyasize){
		int i=0,j=0,k=0,finished=0;;
		strcpy(movie,"");
		strcpy(name,"");
		strcpy(surname,"");
		while(buffer[bufCount]!='/' && bufCount<dosyasize){
			movie[i++]=buffer[bufCount];
			bufCount++;
		}
		bufCount++;
		movie[i]='\0';

		int movieIndex= addMovie(movie);
		while(!finished && bufCount<dosyasize){
			
			int comma=0;
			while(buffer[bufCount]!='/' && buffer[bufCount]!='(' && bufCount<dosyasize){
				if(isHarf(buffer[bufCount]))
					surname[j++]=buffer[bufCount];
				else if(buffer[bufCount]==',' && comma==0)
					comma=j;
				bufCount++;				
			}
			if(buffer[bufCount]=='('){
				do{
					bufCount--;
					if(isHarf(buffer[bufCount]))
						j--;
				}while(buffer[bufCount]!='\n');
				finished=1;
			}
			surname[j]='\0';
			if(comma==0)//If comma is 0 then don't have surname
				strcpy(name,surname);
			else{
				int s;
				for(s=0;s<j-comma-1;s++)
					name[s]=surname[s+comma+1];
				name[j-comma-1]=' ';
				name[j-comma]='\0';
				surname[comma]='\0';
				strcat(name,surname);
				
			}
			addActor(movieIndex,name);
			k=0;
			j=0;
			bufCount++;
		}

	}
	return 1;
}
int getDistance(char* actor1,char* actor2){
	int index  = getIndex(actor1,0);
	int index2 = getIndex(actor2,0);
	if(GraphList[index]->isEmpty || GraphList[index2]->isEmpty){
		printf("One of actors can't found.\n");
		return -2;
	}
	int i,j,distance=0;
	int found=0;
	int q=0;
	Node** queue=(Node**)malloc(NodeLimit*sizeof(Node*));
	short int *visited=(short int*)malloc(M*sizeof(short int));
	for(i=0;i<M;i++){
		visited[i]=0;
	}
	int front=0;
	queue[q++]=GraphList[index];	
	visited[index]=1;
	int tab=0;
	while(!found && q-front!=0){
		distance++;
		
		int current=q;
		for(j=front;j<current && !found;j++){
			//Look the list of jth node of queue		
			for(i=0;i<queue[j]->listCounter && !found;i++){
				//Get index
				int movieIndex= getIndex(queue[j]->list[i]->name,1);
				//Control if visited or not
				if(!visited[movieIndex]){
					//Add nodes in list of jth node of queue to queue
					queue[q++]=queue[j]->list[i];
					queue[q-1]->src=queue[j];
					//Check visited
					visited[movieIndex]=1;		
					found = findNode(queue[q-1],GraphList[index2]);	
					if(found)
						GraphList[index2]->src=queue[q-1];
								
				}

			}
			front++;
		}
	
		
		current=q;
		j=front;
		while(j<current && !found){
			//Look the list of jth node of queue			
			for(i=0;i<queue[j]->listCounter && !found;i++){

				//Get index
				int Actorindex= getIndex(queue[j]->list[i]->name,0);
				//Control if visited or not
				
				if(!visited[Actorindex]){
					//Add nodes in list of jth node of queue to queue
					queue[q++]=queue[j]->list[i];
					queue[q-1]->src=queue[j];
					//Check visited
					visited[Actorindex]=1;	
				}

			}
			front++;
			j++;			
		}

	}		
		

	if(!found)
		return -1;
	
	//Print way with backward
	Node* ptr = (Node*)malloc(sizeof(Node));
	ptr =GraphList[index2];
	i=0;
	while(strcmp(ptr->name,actor1)!=SAME){
		printf("%s,",ptr->name);

		i++;
		if(i%3==0){
			printf("\n%s,",ptr->name);
			i++;
		}
				ptr=ptr->src;
	}
	printf("%s\n\n",ptr->name);
	
	return distance;
	
}
int addMovie(char* movieName){
	int i;
	int movieIndex = getIndex(movieName,1);
	GraphList[movieIndex]->isEmpty=0;
	GraphList[movieIndex]->name=(char*)malloc(strlen(movieName)*sizeof(char));
	GraphList[movieIndex]->list=NULL;
		GraphList[movieIndex]->src=(Node*)malloc(sizeof(Node));
		GraphList[movieIndex]->src=NULL	;
	strcpy(GraphList[movieIndex]->name,movieName);
	return movieIndex;	
}
void addActor(int movieIndex,char* actorName){
	int i;
	int actorIndex = getIndex(actorName,0);
	if(GraphList[actorIndex]->isEmpty){
		GraphList[actorIndex]->name=(char*)malloc(strlen(actorName)*sizeof(char));		
		strcpy(GraphList[actorIndex]->name,actorName);	
		GraphList[actorIndex]->isEmpty=0;
		GraphList[actorIndex]->list=NULL;		
		GraphList[actorIndex]->src=(Node*)malloc(sizeof(Node));
		GraphList[actorIndex]->src=NULL;
	}
	//Add Actor To Film
	insertToNode(GraphList[movieIndex],GraphList[actorIndex]);
	//Add Film to Actor's list
	insertToNode(GraphList[actorIndex],GraphList[movieIndex]);
}
unsigned long long int horner(char* word){
	unsigned long long int h=1; 
	int i;
	int a= strlen(word);
	for(i=0;i<a;i++)
		h = (h*R + word[i]);
	return h;	
}
int getIndex(char* nodeName,int isMovie){
	int i=0;
	int limit;
	if(isMovie)
		limit=MAX_MOVIE_COUNT;
	else
		limit=MAX_ACTOR_COUNT;
	unsigned long long int key = horner(nodeName);
	int h1 = key%limit;
	int h2 = 1+key%(limit-1);

	unsigned long long int h;
	
	do{
		//if iteration is too long, forget about double hashing
		if(i>1000)
			h2=1;
		h=(h1+i*h2)%limit;
		if(!isMovie)
			h+=MAX_MOVIE_COUNT; //0...MAX_MOVIE_COUNT indexes are the indexes with Movies so after that there will be actor indexes
		i++;
	}while(GraphList[h]->isEmpty==0 && strcmp(GraphList[h]->name,nodeName)!=SAME);
	
	return h;
}
int isHarf(char c){
	return (c!=',' && c!='/' && c!='\n');
}
void insertToNode(Node* node,Node* nodeInsert){
	allocate(node);
	node->list[node->listCounter-1]=nodeInsert;
}
int findNode(Node* node, Node* nodeFind){
	int i;
	while((i)<node->listCounter && strcmp(node->list[i]->name,nodeFind->name)!=SAME)
		i++;
	if(i<node->listCounter)
		return 1;
	return 0;
}
void allocate(Node* node){
	node->listCounter++;
	if(node->listCounter==1)
		node->list=(Node**)malloc(sizeof(Node*));
	else{
		Node** neww=(Node**)malloc(node->listCounter*sizeof(Node*));;
		memcpy(neww,node->list,(node->listCounter-1)*sizeof(Node*));
		node->list=neww;	
	}
	
}

