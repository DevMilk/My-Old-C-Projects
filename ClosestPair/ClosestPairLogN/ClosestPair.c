#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include<locale.h> 

#define FARKx(a,b) dots[(a)].x-dots[(b)].x //2 nokta aras�ndaki x'lerin fark�n� al�r
#define FARKy(a,b) dots[(a)].y-dots[(b)].y //2 nokta aras�ndaki y'lerin fark�n� al�r
#define FARK(a,b) pow(FARKx(a,b),2)+pow(FARKy(a,b),2) //2 Nokta aras�ndaki fark� al�r (de�erler kar��la�t�r�laca�� i�in sadece yazd�r�rken karek�k kulland�m)
#define pair(a,b,c) pair[0]=a; pair[1]=b; pair[2]=c;
#define MAX_DOT_SIZE 100 //Noktalar dizisindeki maximum nokta say�s�
#define MAX_FILE_NAME 50 //E�er girdiler dosyadan okunursa dosyan�n ismindeki max. kelime say�s�

typedef struct{
	int x;
	int y;
}dot; //Nokta yap�s�

int* pair_Recursive(dot*,int,int); //Verilen s�n�rlar i�inde en yak�n 2 noktan�n indexlerini pair[0] ve pair[1]'e, aralar�ndaki mesafeyi pair[2]'ye atar ve o diziyi d�nd�r�r
void merge(dot*,int,int,int); //Sol, pivot ve sa� indexi verilen diziyi merge eder
void firstSort(dot*, int,int); //Verilen noktalar dizisini x koordinatlar�na g�re s�ralayan fonksiyon
dot* loadFromFile(char*); //Verilen dosya isminden, noktalar k�mesini �ekecek olan fonksiyon
int* brute(dot*,int,int); //E�er kalan nokta say�s� 2 veya 3 ise brute ile arlar�ndaki mesafe en az olan 2 noktay� pair[0] ve pair[1]'e atar ve aralar�ndaki mesafeti pair[2]'ye at�p d�nd�r�r
void ySirala(dot*,int*,int,int); //Noktalar� y'ye g�re s�ralamk i�in
void yMerge(dot*,int*,int,int,int); //Noktalar� y'ye g�re s�ralamada yard�mc� olan merge fonksiyonu
int n=1; //Noktalar dizisindeki ele veya  ve man say�s�n� tutan global de�i�ken

int main(){
	setlocale(LC_ALL, "Turkish"); //T�rk�e karakterler i�in
	int i=0; //Saya�
	dot* dots; //Noktalar k�mesi
	while(n){ //0'a bas�lmad��� s�rece program devam eder
		n=1; //Ba�ta n'e 1 ver
		
		printf("Girdileri nas�l se�eceginizi giriniz: \n0=Cikis\n1=El ile\n2=dosyadan\n");
		scanf("%d",&n);
		
		
		if(n==1){// NOKTALARI MANUEL OLARAK G�RMEK ���N
				printf("Nokta say�s�n� giriniz: ");
				scanf("%d",&n);
				dots = (dot*)malloc(n*sizeof(dot));
				for(i=0;i<n;i++){
					printf(" %d indexindeki Noktalan�n x ve y koordinatlar�n� giriniz: ",i);
					scanf("%d",&dots[i].x);
					scanf("%d",&dots[i].y);
				}
		}
		else if(n==2){// NOKTALARI �SM� VER�LEN DOSYADAN OKUMAK ���N
			char fileName[MAX_FILE_NAME];
			printf("Dosyan�n ad�n� giriniz: ");
			scanf("%s",fileName);
			dots= loadFromFile(fileName); //Dosyadan OKU
		}
		else{ //Giri� 0 1 veya 2 de�ilse program� sonland�r
			printf("Gecersiz giris");
			exit(0);
		}
		firstSort(dots,0,n-1); //Noktalar� X de�erlerine g�re s�rala
	
		//Sonu�lar� yazd�r
		int* pair = pair_Recursive(dots,0,n-1);
		printf("-------Birbirlerine en yakin noktalar: (%d,%d) ve (%d,%d)-------\n\
				-------Birbirine En yakin i noktalarin arasindaki mesafe: %.3f-------\n\n",
							dots[pair[0]].x,dots[pair[0]].y,dots[pair[1]].x,dots[pair[1]].y,
							sqrt(pair[2]));
	}
}
int* brute(dot* dots,int start, int end){
	int dif[3];
	int* pair=(int*)malloc(3*sizeof(int));	
	switch(end-start+1){
		case 2:
			dif[2]=FARK(start,end);
			pair(start,end,dif[2]);
			break;
		case 3:
			dif[0]=FARK(start,start+1);
			dif[1]=FARK(start+1,end);
			dif[2]=FARK(start,end);
			
			if(dif[0]<dif[1]){					
				if(dif[0]<dif[2]){
					//dif[0] en k�����
					pair(start,start+1,dif[0]);
				}

				else{
					//dif[2] en k�����
					pair(start,end,dif[2]);
				}
			}
			else{
				if(dif[1]<dif[2]){
					//dif[1] en k�����
				   pair(start+1,end,dif[1]);
				}
				else{
				   //dif[2] en k�����
					pair(start,end,dif[2]);
				} 					
			}
			break;		
	}
	return pair;
}
int* pair_Recursive(dot* dots,int start, int end){
//	printf("\nStart-End De�erleri: %d %d\n",start,end);
	int* pair;	
	//E�er 2 veya 3 nokta kalm��sa brute fonksiyonuna y�nlendir
	if(2==end-start+1 || end-start+1==3){
		pair=brute(dots,start,end);
	}
	else{
		pair =(int*)malloc(3*sizeof(int));
		//Medyan eleman� belirle	
		int mIndex = (start+end)/2;
		
		int* left = pair_Recursive(dots,start,(start+end)/2); //Sola do�ru rekursif
		int* right= pair_Recursive(dots,(start+end)/2+1,end); //Sa�a do�ru rekursif
		
		int dmin = left[2]>right[2] ? right[2] : left[2]; //Hangisi daha k���kse dmin'e ata
		dmin = sqrt(dmin);
		if(right[2]>left[2]){
			pair(left[0],left[1],left[2]);
		}
		else{
			pair(right[0],right[1],right[2]);
		}
		int* serit=(int*)malloc(sizeof(int)*(end-start));
		int i,j,k=0; //k medyan ve medyan�n solunda yer alan noktalar� sayacak
		int l=0;//r medyan ve medyan�n solunda yer alan noktalar� sayacak
		while(mIndex+i<=end && abs(FARKx(mIndex,mIndex+i))<=dmin){
				serit[k++]=mIndex+i; 
			i++; //indeci artt�r
		}
		i=1;
		serit[k+(l++)]=mIndex;
		//Dikd�rtgenin sol taraf�ndaki nokta adaylar�n� ara
		while(mIndex-i>=0 && abs(FARKx(mIndex,mIndex-i))<dmin){
				serit[k+l]=mIndex-i;
				l++;
			i++;
		}
		ySirala(dots,serit,0,k-1);
		ySirala(dots,serit,k,k+l-1);
		for(i=0;i<k;i++){
			//j'li loop en fazla 6 kere d�ner
			for(j=k;j<l && 2*abs(FARKy(serit[j],serit[i]))<dmin;j++)
				if(FARK(serit[i],serit[j])<dmin){
					pair(i,j,FARK(serit[i],serit[j]));
				}
		}
		
	}
//	printf("\nStart ve End de�erleri %d %d olan a�aman�n d�nd�rd��� dizinin g�sterdi�i noktalar ve mesafe(karek�k� al�nmad�): (%d,%d) (%d,%d) %d \n\n",start,end,dots[pair[0]].x,dots[pair[0]].y,dots[pair[1]].x,dots[pair[1]].y,pair[2]);
	return pair;
}
void ySirala(dot* dots, int* serit,int l, int r) { 
    if (l<r) { 
        int mid = l+(r-l)/2; 
        ySirala(dots,serit,l,mid); 
        ySirala(dots,serit,mid+1,r); 
        yMerge(dots,serit,l,mid,r); 
    } 
} 
void yMerge(dot* dots,int* serit,int l,int mid,int r) { 
    int i,j,k; 
    int lCount= mid-l+1; //dizinin mid'e g�re solundaki eleman say�s�
    int rCount= r-mid;  //dizinin mid'e g�re sa��ndaki eleman say�s�
  	
  	//Merge edilecek dizi
    int* left= (int*)malloc(lCount*sizeof(int));
	int* right= (int*)malloc(rCount*sizeof(int)); 
  
    //dizinin sol ve sa� taraf�n� ata
    for (i=0;i<lCount;i++) 
        left[i]= serit[l+i]; 
    for (j=0; j<rCount;j++) 
        right[j] = serit[mid+1+j]; 
    i= 0;
    j= 0; 
    k= l; // Merge eidlmi� dizinin indexini gezecek de�i�ken
    while (i<lCount && j<rCount){ 
        if (dots[left[i]].y<=dots[right[j]].y){ 
            serit[k]= left[i]; 
            i++; 
        } 
        else{ 
            serit[k]= right[j]; 
            j++; 
        } 
        k++; 
    } 
  
   	//solda Geri kalan elemanlar� serit dizisine ata
    while (i<lCount){ 
        serit[k]= left[i]; 
        i++; 
        k++; 
    } 
  
    //sa�da geri kalan elemanlar� serit dizisine ata
    while (j< rCount){ 
        serit[k]= right[j]; 
        j++; 
        k++; 
    } 
} 
void merge(dot* dots,int l,int mid,int r) { 
    int i,j,k; 
    int lCount= mid-l+1; //dizinin mid'e g�re solundaki eleman say�s�
    int rCount= r-mid;  //dizinin mid'e g�re sa��ndaki eleman say�s�
  	
  	//Merge edilecek dizi
    dot* left= (dot*)malloc(lCount*sizeof(dot));
	dot* right= (dot*)malloc(rCount*sizeof(dot)); 
  
    //dizinin sol ve sa� taraf�n� ata
    for (i=0;i<lCount;i++) 
        left[i]= dots[l+i]; 
    for (j=0; j<rCount;j++) 
        right[j] = dots[mid+1+j]; 
    i= 0;
    j= 0; 
    k= l; // Merge eidlmi� dizinin indexini gezecek de�i�ken
    while (i<lCount && j<rCount){ 
        if (left[i].x<=right[j].x){ 
            dots[k]= left[i]; 
            i++; 
        } 
        else{ 
            dots[k]= right[j]; 
            j++; 
        } 
        k++; 
    } 
  
   	//solda Geri kalan elemanlar� dots dizisine ata
    while (i<lCount){ 
        dots[k]= left[i]; 
        i++; 
        k++; 
    } 
  
    //sa�da geri kalan elemanlar� dots dizisine ata
    while (j< rCount){ 
        dots[k]= right[j]; 
        j++; 
        k++; 
    } 
} 
void firstSort(dot* dots, int l, int r) { 
    if (l<r) { 
        int mid = l+(r-l)/2; 
        firstSort(dots,l,mid); 
        firstSort(dots,mid+1,r); 
        merge(dots,l,mid,r); 
    } 
} 
dot* loadFromFile(char* fileName){
	n=0;
	dot* dott=(dot*)malloc(sizeof(dot)*MAX_DOT_SIZE); //dizi i�in yer a�
	FILE* file = fopen(fileName,"r"); //OKuma modunda a�
	if(file == NULL){
		printf("B�yle bir dosya bulunmuyor");
		exit(1);
	}
	//Dosya bitene kadar x ve y de�erlerini oku
	while(!feof(file)){
		fscanf(file,"%d %d\n",&dott[n].x,&dott[n].y);
		n++;
	}
	fclose(file); //Dosyay� kapat
	return dott; //noktalar dizisini d�nd�r
}
