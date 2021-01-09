#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#define MAX_FRIEND_SIZE 500 //Facebooku baz alarak belirledim

typedef struct Tree{
	long int kimlikNo;
	char ad[20];
	char soyad[20];
	long int arkadasKimlik[MAX_FRIEND_SIZE];
	struct Tree* right;
	struct Tree* left;
}Tree;

Tree* insertNewUser(long int); //inserts  user of given id
void deleteUser(long int ); //deletes user 
int contains(long int);	  //returns if user of given id is exists in tree
void friends(long int);	//prints friends of users
int size(Tree*);   //returns node count of tree
void printNext(long int); //prints subtree of user of given id
void printGreater(long int); //prints users whose ids are bigger than user's id of given id
void printInOrder();	//prints tree sorted by user ids
Tree* bul(long int); 	//returns user of given id from tree
void copy(Tree*,Tree*); // copies one user node to another
long int desimal(char[],int); //converts array to decimal number
void updateFriends(long int,Tree*); //deletes user from all his/her friends's friend array
Tree* oncekinibul(long int); //returns previous node of user of given id
Tree* agac;	//Global main tree

int main(){
	//Tree's allocation and first values
	agac=(Tree*)malloc(sizeof(Tree));
	agac->right=NULL;
	agac->left=NULL;
	agac->kimlikNo=-1;
	
	//variables are going to be used in text-parse
	char sayitmp[20];
	char ad[20];
	char soyad[20];
	char tmp;
	Tree* yeni;
	
	//Deðerleri içinde tutacak deðiþkenlerin indisi
	int i;
	//Birinin arkadaþ dizisinin indisini tutacak deðiþken
	int k;
	
	//Okuma modunda açmal
	FILE* dosya=fopen("Input.txt","r");
	
	//to print turkish characters	
	setlocale(LC_ALL, "Turkish"); 
	
	//Check if file is found or not
	if(dosya==NULL){
		printf("File not found.");
		exit(1);
	}
	//set cursor of text
	fseek(dosya,3,SEEK_CUR);
	
	//TEXT PARSE
	while(!feof(dosya)){
		
		i=0;
		//taking id
		while((tmp=fgetc(dosya))!=',')
			sayitmp[i++]=tmp;
		//new user	
		yeni=insertNewUser(desimal(sayitmp,i));
		
		i=0;
		//taking name
		while((tmp=fgetc(dosya))!=' ' && isalpha(tmp))
			*(yeni->ad+(i++))=tmp;
		*(yeni->ad+i)='\0'; //null at end
		
		
		i=0;
		//take surname
		while((tmp=fgetc(dosya))!=',' && tmp != '\n' && isalpha(tmp))
			*(yeni->soyad+(i++))=tmp;
		*(yeni->soyad+i)='\0'; //null at end
		
			
		//If there is a comma after surname, that means he/she have friends	
		if(tmp==','){
			i=0;
			k=0;
			//If there is not a \n oro ' ' character, continue parsing
			while((tmp=fgetc(dosya))!='\n' && tmp!= ' '){
				//- symbol means friend's id is finished
				if(tmp=='-'){
					yeni->arkadasKimlik[k++]=desimal(sayitmp,i);
					i=0;
				}
				//assigning number to array
				else
					sayitmp[i++]=tmp;
			}
			//assign last friend
			yeni->arkadasKimlik[k++]=desimal(sayitmp,i);
		}
		//continue text from next paragraph
		fscanf(dosya, "\n");
	}	
	//Main Menu loop
	while(1){
		char cevap; //for Process number
		long int cevap2; //for id
		printf("\n\n\t\t\tMENU\n-----------------------------------------------------------");
		printf("\nChose Process: \n1.Delete User\n2.Look at user's friends\n3.Print user's next users\n4.Print users whose ids are bigger than given id\n5.Print sorted tree\n6.Show that given user exists\n7.Add user\n8.Quit\n9.Print user count\nAnswer: ");
		cevap = getch();
		printf("%c",cevap);
		printf("\n");
		switch(cevap){
			case '1':
				printf("\nEnter user id: ");
				scanf("%d",&cevap2);
				printf("\n");				
				deleteUser(cevap2);
				break;
			case '2':
				printf("\nEnter user id: ");
				scanf("%d",&cevap2);	
				printf("\n");
							
				friends(cevap2);
				break;
			case '3':
				printf("\nEnter user id: ");
				scanf("%d",&cevap2);
				printf("\n");			
				printNext(cevap2);
				break;
			case '4':
				printf("\nEnter user id: ");
				scanf("%d",&cevap2);
				printf("\n");		
				printGreater(cevap2);
				break;
			case '5':
				printInOrder(agac);
				break;
			case '6':
				printf("\nEnter user id: ");
				scanf("%d",&cevap2);	
				printf("\n");
						
				if(contains(cevap2)==0){
					printf("User not found");
				}
				else
					printf("User exists");
				break;
			case '7':
				printf("\nEnter user id: ");
				scanf("%d",&cevap2);		
				printf("\n");
						
				yeni= insertNewUser(cevap2);
				if(yeni!=NULL){
					printf("Enter user name: ");
					scanf("%s",yeni->ad);
					printf("Enter user surname: ");
					scanf("%s",yeni->soyad);	
					printf("User added");
				}

				break;
			case '8':
				exit(0);
				break;
			case '9':
				printf("\nTotal user count of tree: %d\n",size(agac));
				break;
		}
	}

	return 0;
}

Tree* bul(long int kimlikno){
	Tree* ptr = agac; //Pointer that will surf on tree
	
	

	while(ptr!=NULL){
		if(kimlikno == ptr->kimlikNo)
			return ptr;
		else if(ptr->kimlikNo > kimlikno){
			ptr=ptr->left;
		}
		else {
			ptr=ptr->right;
	}
}
	
	//If nothing found, return NULL
	return NULL;
}
Tree* insertNewUser(long int kimlikNo){
	Tree* ptr=agac; //Pointer that will surf on tree
	Tree* onceki;	//Previous node of ptr
	
	//If user already exists
	if(contains(kimlikNo)){
		printf("User already exists");
		return NULL;
	}
	
	//If tree is empty
	if(ptr->kimlikNo==-1){
		ptr->kimlikNo=kimlikNo;
		ptr->left=NULL;
		ptr->right=NULL;
		return;
	}

	//Eklenmesi için uygun olan yeri arayan döngü
	while(ptr!=NULL){
	    onceki = ptr;
		if(kimlikNo>(ptr->kimlikNo)){ //If given id is bigger than current node's id
			ptr=ptr->right;
		}
		else{
			ptr=ptr->left;
		}
	}
	
	ptr = (Tree*)malloc(sizeof(Tree));	//Allocation of new user

	if(kimlikNo>onceki->kimlikNo)
		onceki->right=ptr;
	else
		onceki->left=ptr;
		
	//Bulunan yere kullanýcý deðerlerinin eklenmesi
	ptr->kimlikNo=kimlikNo;
	ptr->left=NULL;
	ptr->right=NULL;
	printf("\nUser added\n");
	return ptr;
}
Tree* oncekinibul(long int kimlikNo){
	Tree* ptn = agac; 
	Tree* onceki=NULL;
	while(kimlikNo!=ptn->kimlikNo){
		onceki=ptn;
		 if(ptn->kimlikNo > kimlikNo)
			ptn=ptn->left;
		else 
			ptn=ptn->right;
	}
	
	return onceki;
}
void updateFriends(long int kimlikNo,Tree* root){
	Tree* ptr = root;
	if(ptr==NULL)
		return;
	int i=0;

	while(*(ptr->arkadasKimlik+i)!='\0' && *(ptr->arkadasKimlik+i)!=kimlikNo){
		i++;
	}

	if(*(ptr->arkadasKimlik+i)==kimlikNo){
		while(*(ptr->arkadasKimlik+i)!='\0'){
			*(ptr->arkadasKimlik+i)=*(ptr->arkadasKimlik+i+1);
			i++;
		}
	}
	

	updateFriends(kimlikNo, ptr->right);
	updateFriends(kimlikNo, ptr->left);
}
void deleteUser(long int kimlikNo){
	Tree* ptr = bul(kimlikNo);	
	if(ptr==NULL){
		printf("User added\n");
		return;
	}
	if(size(agac)==1){
		agac->kimlikNo=-1;
		int i=0;
		
		//nullify name and surnames
		while(agac->ad[i]!='\0')
			agac->ad[i++]='\0';
		i=0;
		while(agac->soyad[i]!='\0')
			agac->soyad[i++]='\0';
			
			
		return;
	}
	Tree* onceki;
	
	
	//If user's both right and left node's are empty
	if(ptr->left==NULL && ptr->right==NULL){
		onceki = oncekinibul(kimlikNo);
		if(onceki!=NULL){
			if(onceki->kimlikNo>ptr->kimlikNo)
				onceki->left=NULL;
			else
				onceki->right=NULL;
		}
		free(ptr);
	}
	
	//If user has at least 1 right or left node
	else{
		//If left node exists, left node becomes node that going to be deleted
		if(ptr->left!=NULL && ptr->right==NULL){
			copy(ptr,ptr->left);
			ptr->left=NULL;
			free(ptr->left);
		}
		
		//If Right node exists, Right node becomes node that going to be deleted
		else if(ptr->left==NULL && ptr->right!=NULL){
			copy(ptr,ptr->right);
			ptr->right=NULL;
			free(ptr->right);
		}
	
		//If both right and left nodes exists, left end of right node becomes node that going to be deleted

		else{
			Tree* sonyaprak = ptr->right;
			while(sonyaprak->left!=NULL){
				onceki=sonyaprak;
				sonyaprak=sonyaprak->left;
			}
			
			copy(ptr,sonyaprak);	
			onceki->left=NULL;
			free(sonyaprak);
		}
		
	}
	

	updateFriends(kimlikNo,agac);
	printf("\nUser deleted.\n");
}
int contains(long int kimlikNo){
	if(bul(kimlikNo)!=NULL)
		return 1;
	return 0;
}
void friends(long int kimlikNo){
	if(!(contains(kimlikNo))){
		printf("User not exists");
		return;
	}
	int i=0;
	long int* ptr=(bul(kimlikNo))->arkadasKimlik; 
	while(*(ptr+i)!='\0'){
		printf("%s %s\n",bul(ptr[i])->ad, bul(ptr[i])->soyad);
		i++;
	}
}
int size(Tree* ptr){ 

	//If it is null, return 0
	if(ptr==NULL || ptr->kimlikNo==-1)
		return 0;
	else
		return 1+ size(ptr->right)+ size(ptr->left);
}
void printAdSoyad(Tree* ptr){
	Tree* ptr2 = ptr; 
	if(ptr2!=NULL){
		printAdSoyad(ptr->left);
		printf("%d Name-Surname: %s %s",ptr2->kimlikNo, ptr2->ad, ptr2->soyad);
		printAdSoyad(ptr2->right);
	}
}
void printNext(long int kimlikNo){
	Tree* ptr = bul(kimlikNo);
	if(!contains(kimlikNo)){
		printf("User not exists");
		return;
	}
		printInOrder(ptr->right);
		printInOrder(ptr->left);
}
void printGreater(long int kimlikNo){
	Tree* ptr = agac; 
	if(!(contains(kimlikNo))){
		printf("User not exists");
		return;
	}
	
	while(ptr->kimlikNo!=kimlikNo){
		//If current node's id is bigger than given id,
		//1. print node's right
		//2. print node
		//3. COntinue from node's left
		if(ptr->kimlikNo>kimlikNo){
			printInOrder(ptr->right);
			printf("Id: %d Name-Surname: %s %s\n",ptr->kimlikNo, ptr->ad, ptr->soyad);
			ptr=ptr->left;
		}
		//If current node's id is lower than given id, continue from node's right
		else if(ptr->kimlikNo<kimlikNo){
			ptr=ptr->right;
		}
	}
	//print right nodes of found user.
	printInOrder(ptr->right);
	
}
void printInOrder(Tree* ptr){
	Tree* pn = ptr; 
	int i =0;
	if(pn!=NULL){
		printInOrder(pn->left);
		printf("No: %d Name-Surname %s %s ",pn->kimlikNo, pn->ad, pn->soyad);
		printf("\n");
		printInOrder(pn->right);
	}
}
void copy(Tree* node1, Tree* node2){
		node1->kimlikNo=node2->kimlikNo; //assign id
		strcpy(node1->ad,node2->ad);	//assign name
		strcpy(node1->soyad,node2->soyad); //assign surname
		memcpy(node1->arkadasKimlik,node2->arkadasKimlik,sizeof(node2->arkadasKimlik)); //assign friend array
}
long int desimal(char arr[],int size){
	long int sum=0,i;
	for(i=0;i<size;i++)
		sum=(arr[i]-'0')+10*sum;
	return sum;
}
