#include <stdio.h>
typedef enum {false,true} bool;
#define kontrol(r,c) if(mtrx[r][c]){rec(mtrx,mtrx2,r,c);}
int n,m;
int g=1;

void print(int** mtrx){
int i,j;
printf("\n\n");
for(i=0;i<n;i++){
	for(j=0;j<m;j++){
		printf("%d ",mtrx[i][j]);
	}
	printf("\n");
}
}

void rec(int** mtrx,int** mtrx2,int r,int c){
	mtrx[r][c]=0;
	mtrx2[r][c]=g;
	//	print(mtrx); //Aþama aþama görmek için bunu kaldýrabilirsiniz

	if(r+1<n) kontrol(r+1,c);//aþaðý bak
	if(c+1<m) kontrol(r,c+1);//saða bak

	if(c-1>=0) kontrol(r,c-1);//sola bak
	if(r-1>=0) kontrol(r-1,c) //yukarý bak

}

void Connect(int** mtrx,int** mtrx2,int row, int col){
	
int i,j;
for(i=0;i<row;i++){
	for(j=0;j<col;j++){
		if(mtrx[i][j]){
		 rec(mtrx,mtrx2,i,j);
		 g++;
	}
		
	}
}
}

int main(){
	int i,j;
	printf("Satir sayisini giriniz: ");
	scanf("%d",&n);
	printf("\nSutun sayisini giriniz: ");
	scanf("%d",&m);

	//ilk matrise yer açma
	int** matrix;
	matrix=(int**)calloc(n,sizeof(int*));
	for(i=0; i<n;i++){
	matrix[i]=(int*)calloc(m,sizeof(int));
	}
	
	
	//ilk matrisin deðerlerini atama
	for(i=0; i<n;i++){
		for(j=0; j<m;j++){
			int deger;
			printf("\nMatrisin %d. satirindaki %d.sutunundaki degeri giriniz: ",i+1,j+1);
			scanf("%d",&deger);
			matrix[i][j]=deger;
		}
	}

	//Kopya matrisi tanýmlama
	int** mtrx2;
	mtrx2=(int**)calloc(n,sizeof(int*));
	for(i=0; i<n;i++){
	mtrx2[i]=(int*)calloc(m,sizeof(int));
	}
	for(i=0; i<n;i++){
		for(j=0; j<m;j++){
			mtrx2[i][j]=matrix[i][j];
		}
	}
	Connect(matrix,mtrx2,n,m);
	print(mtrx2);
	return 0;
}
