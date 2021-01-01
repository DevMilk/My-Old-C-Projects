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
(Projenin açýklamasýnda verildiði üzere büyük harflerle(Caps Lock açýkken) girilen R,E ve X geçerlidir.
R ile önceki duruma geçer (önceki durumun öncekine geçemez)
X ile sýfýrlanýr
E ile oyunu sonlandýrýr
Yön tuþlarý ile yön tuþuna göre iþlem yapar
*/

//Matrisi sola çevirir
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

//Matrisi saat yönünde çevirir
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

//Yukarý doðru seçenekle uygulanan fonksiyon
void islem(short int tahta[][4],int* score){
	short int i,j,k;
	for(i=0;i<4;i++){
		for(j=0;j<3;j++){
			k=j+1;
			while(k<4 && tahta[k][i]==0){ //Bir sayýya denk geldiðinde dur
				k++;	
			}
			if(k<4 && tahta[j][i]==tahta[k][i]){
				tahta[j][i]*=2; //Aynýysa 2 katýdýr zaten.
				*score+=tahta[j][i];
				tahta[k][i]=0;
			}
		}
	}
	
	//KAYDIRMA ÝÞLEMÝ
	for(i=0;i<4;i++){
		for(j=0;j<3;j++){
			k=j+1;
			while(k<4 && tahta[k][i]==0){ //Bir sayýya denk geldiðinde dur
				k++;
			}
			if(tahta[j][i]==0 && k<4){
				tahta[j][i]=tahta[k][i];
				tahta[k][i]=0;
			}
		}
	}
}

//Oyunun bitip bitmediðini belirleyen fonksiyon
short int bittimi(short int tahta[][4]){
	short int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			if(!tahta[i][j] || tahta[i][j]==2048){ //Tahta doldu mu(yapýlacak hamle kalmamýþsa), 2048 varsa oyun biter.
				return -1;
			}
		}
	}
	return 1;
}

//Verilen tahtayý yazdýrýr (Öncekini yazdýrmak için kullanýlacak
void tahtayaz(short int tahta[][4],short int x, short int y){ //tahta ve yeni sayýnýn koordinatlarýný alýyor
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
					//Sayýnýn basamaðý artýnca tablodaki | lerin kaymamasý için:
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
	int score=0,highscore=0; //Skor sayaçlarý
	char c=0;
	
	//Yazýlarýn renklerini deðiþtirmek için.
	system("COLOR B");
	
	srand(time(NULL));
	do{


		if(score>highscore)
			highscore = score;
		

		//Rastgele koordinat verilmesi
		if(c!='R'){//Eðer geri alýnma iþlemi yapýlmýþsa yeniden random atýlmasýn
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
		
		//Ekraný temizlemek
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

					//Yýldýzýn konumunu kaybetmemek için.
					x=prev_x;
					y=prev_y;
					
					if(adim>1){ //Oyunun baþýnda deðilse
					system("cls");
					printf("\n\t\t\t\t       ______________________________");
					printf("\n\n\t\t\t\t\t");
					printf("SCORE: %d	HIGHSCORE: %d\n",score,highscore);
					printf("\t\t\t\t       ______________________________");
					tahtayaz(onceki,prev_x,prev_y);
					memcpy(tahta,onceki,sizeof(tahta)); //memcpy(tahta, onceki, sizeof(tahta))  ile ayný þey.
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
				
				else if(c==0x48){ //Yukarý
					//yukarý basýldýðýnda, ayný sütundaki ayný sayýlar birbiriyle toplanýp ayný sayýya denk gelmeyene kadar yukarý gidecek
					prev_x=x;
					prev_y=y;
					memcpy(onceki,tahta,sizeof(tahta));
					islem(tahta,&score);
					}
				
				else if(c==0x50){ //Aþaðý
						prev_x=x;
						prev_y=y;
						memcpy(onceki,tahta,sizeof(tahta));
						solacevir(tahta);
						solacevir(tahta);
						islem(tahta,&score);
						sagacevir(tahta);
						sagacevir(tahta);
						}
						
			else if(c==0x4B){//Sað
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
			
		}while(c==0);//Farklý bir input girilirse yeniden giriþ 

	}	while(bittimi(tahta)==-1);

	printf("\n\t\t\t\t\t\tGAME OVER\n\n\n");
	printf("Cikmak icin herhangi bir tusa basiniz...\n ");
	getch();
	return 0;
}

