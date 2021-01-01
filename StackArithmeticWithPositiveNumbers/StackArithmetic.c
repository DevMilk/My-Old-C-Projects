#include <stdlib.h>
#include <stdio.h>
#define IS_DIGIT(a) (a>='0' && a<='9')
#define SIZE 50
#define MAX_INPUT_SIZE 1000
int desimal(char arr[],int size){
	int sum=0,i;
	for(i=0;i<size;i++)
		sum=(arr[i]-'0')+10*sum;
	return sum;
}
typedef struct{
	int curr;
	char array[SIZE];
}charStack;
typedef struct{
	int curr;
	float array[SIZE];
}numStack;
int isEmpty(numStack* stack){return (stack->curr==-1);}
int isEmpty_c(charStack* stack){return (stack->curr==-1);}
void push(numStack* stack, float element){stack->array[++stack->curr]=element;}
void push_c(charStack* stack,char element){stack->array[++stack->curr]=element;}
float pop(numStack* stack){
	if(isEmpty(stack))
		return -1;
	float tmp = stack->array[stack->curr];
	stack->array[stack->curr--]='\0';
	return tmp;
}
char pop_c(charStack* stack){
	if(isEmpty_c(stack))
		return -1;
	char tmp = stack->array[stack->curr];
	stack->array[stack->curr--]='\0';
	return tmp;
}
int oncelik(char ilk, char ikinci){ //returning 1 means its priority is first
	return ((ilk=='*' || ilk == '/') && (ikinci=='+' || ikinci == '-'));
}
float islem(numStack* sayiStack, char isaret){ 
	float sayi2=pop(sayiStack);
	float sayi1=pop(sayiStack);
	switch(isaret){
		case '*':
			return sayi1*sayi2;
			break;
		case '/':
			return sayi1/sayi2;
			break;
		case '+':
			return sayi1+sayi2;
			break;
		case '-':
			return sayi1-sayi2;
			break;
		default:
			break;
	}
}
void printStack(numStack* stack){
	int i=0;
	int size = stack->curr;
	printf("Sayi Stack: ");
	for(i=0;i<=size;i++)
			printf("%.1f ",stack->array[i]);
	printf("\n");
}
void printStack_c(charStack* stack){
	int i=0;
	int size = stack->curr;
	printf("Isaret Stack: ");
	for(i=0;i<=size;i++)
		printf("%c ",stack->array[i]);
	printf("\n");
}

int main(){
	numStack* sayiStack=malloc(sizeof(numStack));
	sayiStack->curr=-1;
	
	charStack* isaretStack = malloc(sizeof(charStack));
	isaretStack->curr=-1;
	
	int i=0, k=0,l=0;
	char tmp[4];
	char buffer[MAX_INPUT_SIZE];
	printf("Enter the process: ");
	gets(buffer);
	
	char tmpn;
	//Stringi stacka koy
	while(buffer[i]!='\0' && buffer[i]!= '\n'){
		if(IS_DIGIT(buffer[i])){
			k=0;
			while(IS_DIGIT(buffer[i]))
				tmp[k++]=buffer[i++];
			
			i--;
			push(sayiStack,desimal(tmp,k));
		}
		else{
			switch(buffer[i]){
				case '(':
					push_c(isaretStack,buffer[i]);
					break;
				case ')':
					while((tmpn=pop_c(isaretStack))!='(')
						push(sayiStack,islem(sayiStack,tmpn));
					
					break;
				case ' ':
					break;
				default:
					if( isEmpty_c(isaretStack) || isaretStack->array[isaretStack->curr]=='(' || oncelik(isaretStack->array[isaretStack->curr],buffer[i])==0)
						push_c(isaretStack,buffer[i]);
					else{
						do{
							char a = pop_c(isaretStack);

							push(sayiStack,islem(sayiStack,a));		
			
						}while( isEmpty_c(isaretStack)!=1 && oncelik(isaretStack->array[isaretStack->curr],buffer[i])==1);
						push_c(isaretStack,buffer[i]);
					}	
					break;
					
					
			}
		}
		printStack(sayiStack);
		printStack_c(isaretStack);
		printf("\n");
		i++;
	}
	while(!isEmpty_c(isaretStack)){
		push(sayiStack,islem(sayiStack,pop_c(isaretStack)));
		printStack(sayiStack);
		printStack_c(isaretStack);
	}
	printf("Output: %.1f" ,sayiStack->array[0]);
	return 0;	

}
