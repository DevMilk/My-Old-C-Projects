#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include<locale.h> 

#define FARKx(a,b) dots[(a)].x-dots[(b)].x //2 nokta arasýndaki x'lerin farkýný alýr
#define FARKy(a,b) dots[(a)].y-dots[(b)].y //2 nokta arasýndaki y'lerin farkýný alýr
#define FARK(a,b) pow(FARKx(a,b),2)+pow(FARKy(a,b),2) //2 Nokta arasýndaki farký alýr (deðerler karþýlaþtýrýlacaðý için sadece yazdýrýrken karekök kullandým)
#define pair(a,b,c) pair[0]=a; pair[1]=b; pair[2]=c;
#define MAX_DOT_SIZE 100 //Noktalar dizisindeki maximum nokta sayýsý
#define MAX_FILE_NAME 50 //Eðer girdiler dosyadan okunursa dosyanýn ismindeki max. kelime sayýsý

typedef struct{
	int x;
	int y;
}dot; //Nokta yapýsý

int* pair_Recursive(dot*,int,int); //Verilen sýnýrlar içinde en yakýn 2 noktanýn indexlerini pair[0] ve pair[1]'e, aralarýndaki mesafeyi pair[2]'ye atar ve o diziyi döndürür
void merge(dot*,int,int,int); //Sol, pivot ve sað indexi verilen diziyi merge eder
void firstSort(dot*, int,int); //Verilen noktalar dizisini x koordinatlarýna göre sýralayan fonksiyon
dot* loadFromFile(char*); //Verilen dosya isminden, noktalar kümesini çekecek olan fonksiyon
int* brute(dot*,int,int); //Eðer kalan nokta sayýsý 2 veya 3 ise brute ile arlarýndaki mesafe en az olan 2 noktayý pair[0] ve pair[1]'e atar ve aralarýndaki mesafeti pair[2]'ye atýp döndürür
void ySirala(dot*,int*,int,int); //Noktalarý y'ye göre sýralamk için
void yMerge(dot*,int*,int,int,int); //Noktalarý y'ye göre sýralamada yardýmcý olan merge fonksiyonu
int n=1; //Noktalar dizisindeki ele veya  ve man sayýsýný tutan global deðiþken

int main(){
	setlocale(LC_ALL, "Turkish"); //Türkçe karakterler için
	int i=0; //Sayaç
	dot* dots; //Noktalar kümesi
	while(n){ //0'a basýlmadýðý sürece program devam eder
		n=1; //Baþta n'e 1 ver
		
		printf("Girdileri nasýl seçeceginizi giriniz: \n0=Cikis\n1=El ile\n2=dosyadan\n");
		scanf("%d",&n);
		
		
		if(n==1){// NOKTALARI MANUEL OLARAK GÝRMEK ÝÇÝN
				printf("Nokta sayýsýný giriniz: ");
				scanf("%d",&n);
				dots = (dot*)malloc(n*sizeof(dot));
				for(i=0;i<n;i++){
					printf(" %d indexindeki Noktalanýn x ve y koordinatlarýný giriniz: ",i);
					scanf("%d",&dots[i].x);
					scanf("%d",&dots[i].y);
				}
		}
		else if(n==2){// NOKTALARI ÝSMÝ VERÝLEN DOSYADAN OKUMAK ÝÇÝN
			char fileName[MAX_FILE_NAME];
			printf("Dosyanýn adýný giriniz: ");
			scanf("%s",fileName);
			dots= loadFromFile(fileName); //Dosyadan OKU
		}
		else{ //Giriþ 0 1 veya 2 deðilse programý sonlandýr
			printf("Gecersiz giris");
			exit(0);
		}
		firstSort(dots,0,n-1); //Noktalarý X deðerlerine göre sýrala
	
		//Sonuçlarý yazdýr
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
					//dif[0] en küçüðü
					pair(start,start+1,dif[0]);
				}

				else{
					//dif[2] en küçüðü
					pair(start,end,dif[2]);
				}
			}
			else{
				if(dif[1]<dif[2]){
					//dif[1] en küçüðü
				   pair(start+1,end,dif[1]);
				}
				else{
				   //dif[2] en küçüðü
					pair(start,end,dif[2]);
				} 					
			}
			break;		
	}
	return pair;
}
int* pair_Recursive(dot* dots,int start, int end){
//	printf("\nStart-End Deðerleri: %d %d\n",start,end);
	int* pair;	
	//Eðer 2 veya 3 nokta kalmýþsa brute fonksiyonuna yönlendir
	if(2==end-start+1 || end-start+1==3){
		pair=brute(dots,start,end);
	}
	else{
		pair =(int*)malloc(3*sizeof(int));
		//Medyan elemaný belirle	
		int mIndex = (start+end)/2;
		
		int* left = pair_Recursive(dots,start,(start+end)/2); //Sola doðru rekursif
		int* right= pair_Recursive(dots,(start+end)/2+1,end); //Saða doðru rekursif
		
		int dmin = left[2]>right[2] ? right[2] : left[2]; //Hangisi daha küçükse dmin'e ata
		dmin = sqrt(dmin);
		if(right[2]>left[2]){
			pair(left[0],left[1],left[2]);
		}
		else{
			pair(right[0],right[1],right[2]);
		}
		int* serit=(int*)malloc(sizeof(int)*(end-start));
		int i,j,k=0; //k medyan ve medyanýn solunda yer alan noktalarý sayacak
		int l=0;//r medyan ve medyanýn solunda yer alan noktalarý sayacak
		while(mIndex+i<=end && abs(FARKx(mIndex,mIndex+i))<=dmin){
				serit[k++]=mIndex+i; 
			i++; //indeci arttýr
		}
		i=1;
		serit[k+(l++)]=mIndex;
		//Dikdörtgenin sol tarafýndaki nokta adaylarýný ara
		while(mIndex-i>=0 && abs(FARKx(mIndex,mIndex-i))<dmin){
				serit[k+l]=mIndex-i;
				l++;
			i++;
		}
		ySirala(dots,serit,0,k-1);
		ySirala(dots,serit,k,k+l-1);
		for(i=0;i<k;i++){
			//j'li loop en fazla 6 kere döner
			for(j=k;j<l && 2*abs(FARKy(serit[j],serit[i]))<dmin;j++)
				if(FARK(serit[i],serit[j])<dmin){
					pair(i,j,FARK(serit[i],serit[j]));
				}
		}
		
	}
//	printf("\nStart ve End deðerleri %d %d olan aþamanýn döndürdüðü dizinin gösterdiði noktalar ve mesafe(karekökü alýnmadý): (%d,%d) (%d,%d) %d \n\n",start,end,dots[pair[0]].x,dots[pair[0]].y,dots[pair[1]].x,dots[pair[1]].y,pair[2]);
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
    int lCount= mid-l+1; //dizinin mid'e göre solundaki eleman sayýsý
    int rCount= r-mid;  //dizinin mid'e göre saðýndaki eleman sayýsý
  	
  	//Merge edilecek dizi
    int* left= (int*)malloc(lCount*sizeof(int));
	int* right= (int*)malloc(rCount*sizeof(int)); 
  
    //dizinin sol ve sað tarafýný ata
    for (i=0;i<lCount;i++) 
        left[i]= serit[l+i]; 
    for (j=0; j<rCount;j++) 
        right[j] = serit[mid+1+j]; 
    i= 0;
    j= 0; 
    k= l; // Merge eidlmiþ dizinin indexini gezecek deðiþken
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
  
   	//solda Geri kalan elemanlarý serit dizisine ata
    while (i<lCount){ 
        serit[k]= left[i]; 
        i++; 
        k++; 
    } 
  
    //saðda geri kalan elemanlarý serit dizisine ata
    while (j< rCount){ 
        serit[k]= right[j]; 
        j++; 
        k++; 
    } 
} 
void merge(dot* dots,int l,int mid,int r) { 
    int i,j,k; 
    int lCount= mid-l+1; //dizinin mid'e göre solundaki eleman sayýsý
    int rCount= r-mid;  //dizinin mid'e göre saðýndaki eleman sayýsý
  	
  	//Merge edilecek dizi
    dot* left= (dot*)malloc(lCount*sizeof(dot));
	dot* right= (dot*)malloc(rCount*sizeof(dot)); 
  
    //dizinin sol ve sað tarafýný ata
    for (i=0;i<lCount;i++) 
        left[i]= dots[l+i]; 
    for (j=0; j<rCount;j++) 
        right[j] = dots[mid+1+j]; 
    i= 0;
    j= 0; 
    k= l; // Merge eidlmiþ dizinin indexini gezecek deðiþken
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
  
   	//solda Geri kalan elemanlarý dots dizisine ata
    while (i<lCount){ 
        dots[k]= left[i]; 
        i++; 
        k++; 
    } 
  
    //saðda geri kalan elemanlarý dots dizisine ata
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
	dot* dott=(dot*)malloc(sizeof(dot)*MAX_DOT_SIZE); //dizi için yer aç
	FILE* file = fopen(fileName,"r"); //OKuma modunda aç
	if(file == NULL){
		printf("Böyle bir dosya bulunmuyor");
		exit(1);
	}
	//Dosya bitene kadar x ve y deðerlerini oku
	while(!feof(file)){
		fscanf(file,"%d %d\n",&dott[n].x,&dott[n].y);
		n++;
	}
	fclose(file); //Dosyayý kapat
	return dott; //noktalar dizisini döndür
}
