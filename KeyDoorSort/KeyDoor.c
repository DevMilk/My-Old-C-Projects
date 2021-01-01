#include <stdio.h>
#include<stdlib.h>
#define MAX 50 //Max size of array
void sortfunc(int*,int*,int,int); //Main sort function
int partition(int*,int,int,int); /*function that sorts array according to pivot and splits array to 3 parts: lower than pivot, equal to pivot, bigger than pivot*/
void swap(int*, int*); //swap integers function



int main(){
    srand( time(NULL)); //to generate random numbers more randomly
    int N; //D/size of array
	int Lock[MAX],Key[MAX]; //Key and Lock arrays
	
	//Taking user 3 input: size of array, key array and lock array
    printf("Enter array size: ");
    scanf("%d",&N);
    printf("Enter Key array: ");
    int i; //sayaç
    for(i=0;i<N;i++)
        scanf("%d",&Key[i]);
    
    
    printf("Enter Lock array: ");
    for(i=0;i<N;i++)
        scanf("%d",&Lock[i]);
    
    
    sortfunc(Lock,Key,0,N-1);
    
    printf("\n");
    printf("Last form of Key array: ");
    for(i=0;i<N;i++)
        printf("%d  ",Key[i]);
    
    printf("\n");
    printf("Last form of Lock aarray: ");
    for(i=0;i<N;i++)
        printf("%d  ",Lock[i]);
  
    return 0;
}

int partition(int* Arr, int alt, int ust, int select){
	int i; // variable for cursor moving right
	int j;// variable for cursor moving left
	
	int k; //variable for cursor moving right for pivotequal sort
	int l; //variable for cursor moving left for pivotequal sort
    int f;
    i=alt;
    j=ust;
    
    while(i<j ){ 
        while(select>Arr[i])
            i++;
        while(select<Arr[j])
            j--;
        if(Arr[i]==select && Arr[j]==select)
        	j--;
        else
           swap(&Arr[j],&Arr[i]);
		
            

        
    }
    
    
	//swapping pivotequal numbers to middle location of array
	k=0;
	while(Arr[k]<=select)
		k++;
		
	l=ust;
	while(k<l){
		l= ust;
		while(select!=Arr[l])
			l--;
		if(k<l){
			swap(&Arr[k],&Arr[l]);
			l++;		
		}
		
		
	}

    
    return i;//return pivot
    
}
    

void sortfunc(int *Key,int *Lock,int l, int r) {
 int pivot; //cursort that is going to split array in sort function
 int select; // random number from Key array for comparing numbers of Lock array
 if(r>l){

    select=Key[rand()%(r-l+1)+l];//l-r arasýnda random sayý
    pivot=partition(Lock,l,r,select);//Lock küçük, eþit ve büyük sýnfýlandýrma
    partition(Key,l,r,Lock[pivot]);//Key sýnýflandýrma
    //pivota kadar ve pivottan sonrasýný ayýrýp sýralama
    sortfunc(Key,Lock,l,pivot-1);
    sortfunc(Key,Lock,pivot+1,r);
	}
  
}
void swap(int* x, int* y){
	int tmp= *x;
	*x= *y;
	*y = tmp;
	
}
