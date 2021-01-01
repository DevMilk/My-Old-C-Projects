#include <stdio.h>
#include <string.h>
#include <time.h>
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
/*
(Projenin a��klamas�nda verildi�i �zere b�y�k harflerle(Caps Lock a��kken) girilen R,E ve X ge�erlidir.
R ile �nceki duruma ge�er (�nceki durumun �ncekine ge�emez)
X ile s�f�rlan�r
E ile oyunu sonland�r�r
Y�n tu�lar� ile y�n tu�una g�re i�lem yapar
*/

//Matrisi sola �evirir
void solacevir( short int tahta[][4]){
	short int tmp[4][4],i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			tmp[3-j][i]=tahta[i][j];
		}
	}
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			tahta[i][j]=tmp[i][j];
}

//Matrisi saat y�n�nde �evirir
void sagacevir(short int tahta[][4]){
	short int tmp[4][4],i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			tmp[j][3-i]=tahta[i][j];
		}
	}
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				tahta[i][j]=tmp[i][j];
}

//Yukar� do�ru se�enekle uygulanan fonksiyon
void islem(short int tahta[][4],int* score){
	short int i,j,k;
	for(i=0;i<4;i++){
		for(j=0;j<3;j++){
			k=j+1;
			while(k<4 && tahta[k][i]==0){ //Bir say�ya denk geldi�inde dur
				k++;	
			}
			if(k<4 && tahta[j][i]==tahta[k][i]){
				tahta[j][i]*=2; //Ayn�ysa 2 kat�d�r zaten.
				*score+=tahta[j][i];
				tahta[k][i]=0;
			}
		}
	}
	
	//KAYDIRMA ��LEM�
	for(i=0;i<4;i++){
		for(j=0;j<3;j++){
			k=j+1;
			while(k<4 && tahta[k][i]==0){ //Bir say�ya denk geldi�inde dur
				k++;
			}
			if(tahta[j][i]==0 && k<4){
				tahta[j][i]=tahta[k][i];
				tahta[k][i]=0;
			}
		}
	}
}

//Oyunun bitip bitmedi�ini belirleyen fonksiyon
short int bittimi(short int tahta[][4]){
	short int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(!tahta[i][j] || tahta[i][j]==2048){ //Tahta doldu mu(yap�lacak hamle kalmam��sa), 2048 varsa oyun biter.
				return -1;
			}
		}
	}
	return 1;
}

//Verilen tahtay� yazd�r�r (�ncekini yazd�rmak i�in kullan�lacak
void tahtayaz(short int tahta[][4],short int x, short int y){ //tahta ve yeni say�n�n koordinatlar�n� al�yor
	short int i,j,payda;
	printf("\n\n\n\n\n\t\t\t\t  _______________________________________\n");
		for(i=0;i<4;i++){
			printf("\t\t\t\t");
			for(j=0;j<4;j++){

				if(tahta[i][j]!=0){
					printf("  |  %d",tahta[i][j]);
					if(i==x && j==y)
						printf("*   ");
					else 
						printf("    ");
						
					payda = 10;
					//Say�n�n basama�� art�nca tablodaki | lerin kaymamas� i�in:
					while(tahta[i][j]/payda>=1){
					payda*=10;
					printf("\b");
					}
					
				}
				else{
					printf("  |       ");
				}

			}
			
			printf("|\n\t\t\t\t  _______________________________________\n");	
		}
}
short int main(){
	static short int tahta[4][4], onceki[4][4];
	short int x,y,i,j,k,prev_x=0,prev_y=0,payda,random,adim=0;
	int score=0,highscore=0; //Skor saya�lar�
	char c=0;
	
	//Yaz�lar�n renklerini de�i�tirmek i�in.
	system("COLOR B");
	
	srand(time(NULL));
	do{


		if(score>highscore)
			highscore = score;
		

		//Rastgele koordinat verilmesi
		if(c!='R'){//E�er geri al�nma i�lemi yap�lm��sa yeniden random at�lmas�n
		if(adim<2)
			adim++; 
		do{
		x = rand()%4;
		y = rand()%4;			
		}while(tahta[x][y]!=0);
		
		if(adim==1){
			prev_x=x;
			prev_y=y;
		}
		// Rastgele 2 ya da 4 verilmesi
		tahta[x][y]=(rand()%2+1)*2;
		
		//Ekran� temizlemek
		system("cls");
		printf("\n\t\t\t\t       ______________________________");
		printf("\n\n\t\t\t\t\t");
		printf("SCORE: %d	HIGHSCORE: %d\n",score,highscore);
		printf("\t\t\t\t       ______________________________");
		tahtayaz(tahta,x,y);
	}	


		do{
			c = getch();
				if(c=='r' || c=='R'){ //Geri almak

					//Y�ld�z�n konumunu kaybetmemek i�in.
					x=prev_x;
					y=prev_y;
					
					if(adim>1){ //Oyunun ba��nda de�ilse
					system("cls");
					printf("\n\t\t\t\t       ______________________________");
					printf("\n\n\t\t\t\t\t");
					printf("SCORE: %d	HIGHSCORE: %d\n",score,highscore);
					printf("\t\t\t\t       ______________________________");
					tahtayaz(onceki,prev_x,prev_y);
					memcpy(tahta,onceki,sizeof(tahta)); //memcpy(tahta, onceki, sizeof(tahta))  ile ayn� �ey.
					}
				}
				if(c=='e' || c=='E'){ //Kapatmak
				printf("\n\t\t\t\t\t\tGAME OVER\n\n\n");
					return 0;
					}
				if(c=='x' || c=='X'){ //Resetlemek
					for(i=0;i<4;i++){
						for(j=0;j<4;j++){
							tahta[i][j]=0;
							onceki[i][j]=0;
							score = 0;
						}
					}
					adim=0;
					}
				
				else if(c==0x48){ //Yukar�
					//yukar� bas�ld���nda, ayn� s�tundaki ayn� say�lar birbiriyle toplan�p ayn� say�ya denk gelmeyene kadar yukar� gidecek
					prev_x=x;
					prev_y=y;
					memcpy(onceki,tahta,sizeof(tahta));
					islem(tahta,&score);
					}
				
				else if(c==0x50){ //A�a��
						prev_x=x;
						prev_y=y;
						memcpy(onceki,tahta,sizeof(tahta));
						solacevir(tahta);
						solacevir(tahta);
						islem(tahta,&score);
						sagacevir(tahta);
						sagacevir(tahta);
						}
						
			else if(c==0x4B){//Sa�
					prev_x=x;
					prev_y=y;				
					memcpy(onceki,tahta,sizeof(tahta));
					sagacevir(tahta);
					islem(tahta,&score);
					solacevir(tahta);
					}
						
				else if(c==0X4D){ //Sol
					prev_x=x;
					prev_y=y;				
					memcpy(onceki,tahta,sizeof(tahta));
					solacevir(tahta);
					islem(tahta,&score);
					sagacevir(tahta);
				}
					
				else{
					c=0;
				}
			
		}while(c==0);//Farkl� bir input girilirse yeniden giri� 

	}	while(bittimi(tahta)==-1);

	printf("\n\t\t\t\t\t\tGAME OVER\n\n\n");
	printf("Cikmak icin herhangi bir tusa basiniz...\n ");
	getch();
	return 0;
}

