#include <stdio.h>
#include <string.h> //strlen() için
#include <stdlib.h> //exit(0) için
#define OKU FILE* veritabani=fopen("veritabani.dat","r")
#define YAZ FILE* veritabani=fopen("veritabani.dat","r+")
#define KAPA fclose(veritabani)
#define CIKIS(a) if(strcmp(a,"-1")==0) return

//Yeni Fonksiyonlar: 
//fseek(dosya,(hesapno-1)*sizeof(veri),SEEK_SET)
//fwrite(&bosveri,sizeof(veri),1,dosya);
//fread(&bosveri,sizeof(veri),1,dosya);


//ÜYELERİN HESABI, DOSYAYA HESAP NUMARASINDAKİ SATIRA YERLEŞTİRİLİR.(KOLAY ERİŞMEK İÇİN)
//Üyelerin yapısı
typedef struct{
	long long int para;
	char isim[20];
	char soyad[20];
	char id[20];
	char pw[20];
	char phone[12];
	unsigned int hesapno;
} uye;
//C'de bool tanımı
typedef enum{false,true} bool;

//üye structını hesap numarasına göre veritabanında konumlandıran/güncelleştiren fonksiyon
void guncelle(uye tmp){
	YAZ;
	fseek(veritabani,(tmp.hesapno-1)*sizeof(uye),SEEK_SET);
	fwrite(&tmp,sizeof(uye),1,veritabani);
	KAPA;
}

//Kayıt işlemi için kontrol
//True dönerse zaten böyle bir kullanıcı adı ya da telefon var demektir.
bool regkontrol(char id[20], char phone[12]){ 
	int i=1;
	uye tmp;
	OKU;
	if(veritabani==NULL){ //dosya varsa zaten yanlışlanacak bir şey de yok
		return false;
	}
	rewind(veritabani);
	while(!feof(veritabani)){ //dosya bitene kadar okuyan döngü
		fread(&tmp,sizeof(uye),1,veritabani);
		if(strcmp(id,tmp.id)==0 && strcmp(phone,tmp.phone)==0)
			return true;
			
	}
	KAPA;
	return false;
}

//Giriş işlemi için kontrol
//Hesap numarasını döndürecek, bulamazsa -1 döndürecek
int logkontrol(char id[20], char pw[20]){
	uye tmp;
	OKU;
	rewind(veritabani);
	while(!feof(veritabani)){
		fread(&tmp,sizeof(uye),1,veritabani);
		if(strcmp(id,tmp.id)==0 && strcmp(pw,tmp.pw)==0)
			return tmp.hesapno;
	}
	KAPA;
	return -1;
}

//Hesaba giriş yaptıktan sonraki kontrolller
void islem(uye tmp){ 
printf("\n\n			ISLEM\n------------------------------------------------------\n");
	uye tmn;
	int hesapno;
	long long int a;
	char phone[12];
	int cevap=0,i;
	while(1){
		printf("\n\n\t\t%s %s\nBakiyeniz: %lld\n\nYapabileceginiz islemler: \n1.Para cek\n2.Para yatir\n3.Para transferi\n4.Hesapdan cik\n5.Programdan cik\nCevap: ",tmp.isim,tmp.soyad,tmp.para);
		scanf("%d",&cevap);
		switch(cevap){
			case 1:
				printf("Cekmek istediginiz tutari giriniz: ");
				scanf("%lld",&a);
				if(tmp.para>=a){
				printf("Hesabinizdan %lld tl para cekilmistir.",a);
				tmp.para -=a;
				guncelle(tmp);
				}
				else{
					printf("Yeterli paraniz yoktur.");
				}
				break;
			case 2:
				printf("Yatirmak istediginiz tutari giriniz: ");
				scanf("%lld",&a);
				tmp.para+=a;
				guncelle(tmp);
				printf("Hesabiniza %lld tl para yuklenmistir.",a);
				break;
			case 3:
				printf("Havale yapacaginiz hesabin numarasini giriniz: ");
				scanf("%d",&hesapno);
				printf("Havale yapmak istediginiz para miktarini giriniz: ");
				scanf("%lld",&a);
				OKU;
				fseek(veritabani,hesapno*sizeof(uye),0);
				fread(&tmn,sizeof(uye),1,veritabani);
				if(tmn.hesapno==0){
					printf("Boyle bir hesap kayitli degildir.");
					KAPA;
					break;
				}
				tmp.para-=a;
				tmn.para+=a;
				KAPA;
				guncelle(tmn);
				guncelle(tmp);
				printf("Transfer basariyla tamamlandi.");
				break;
			case 4:
				return;
				break;
			case 5:
				exit(0);
		}
	}
}

//Hesap kaydı
void reg(){
	printf("\n\n			KAYIT\n------------------------------------------------------\n");
	uye tmp;
	uye tmn;
	char tmppw[20],d;
	int c,i=0;
	printf("\n...Kullanici adi ve sifre 6 karakterden buyuk olmalidir...\n\n");
	do{
	printf("Adiniz(Lutfen sadece 1 isminizi giriniz): ");
	scanf("%s",tmp.isim);
	CIKIS(tmp.isim); //Eğer -1 girerse fonksiyondan çıkar
	printf("Soyadiniz: ");
	scanf("%s",tmp.soyad);
	CIKIS(tmp.soyad);
	printf("Telefon Numaraniz: ");
	scanf("%s",tmp.phone);
	CIKIS(tmp.phone);
	printf("Kullanici adi belirleyiniz: ");
	scanf("%s",tmp.id);
	CIKIS(tmp.id);
	printf("Sifre belirleyiniz: ");
	scanf("%s",tmp.pw);
	CIKIS(tmp.pw);
	printf("Tekrar sifre giriniz: ");
	scanf("%s",tmppw);
	CIKIS(tmppw);
	printf("Hesap numarasi giriniz: (en az 1)");
	scanf("%d",&tmp.hesapno);
	i=0;
	if(regkontrol(tmp.id,tmp.phone)==true){ //Veritabanında böyle bir kullanıcı adı ya da telefon var mı??
		printf("\nBoyle bir kullanici adi veya telefon zaten kayitli.Cikmak icin 0, devam etmek icin herhangi bir karakter giriniz.");
		 scanf("%d",&c);
		 if(c==0) return;
	}
	else if(tmp.hesapno<1) printf("Hesap numarasi 1'den buyuk olmalidir."); //hesap numarası 1'den küçük olamaz.
	else if(strlen(tmp.phone)<10) printf("Telefon numarasi eksik girilmis\n"); //Telefon numarası 11 rakam olmalı.(baştaki 0 dahil)
	else if(strlen(tmp.id)<=6) printf("Kullanici adi 6 karakterden uzun olmalidir.");
	else if(strlen(tmp.pw)<=6) printf("Sifre 6 karakterden uzun olmalidir.");
	else if(strcmp(tmp.pw,tmppw)!=0) printf("Sifreler eslesmiyor."); //İlk şifreyi yanlış yazarsa diye tekrar şifrenin girilmesi
	tmp.para=0;
	
	//Hesap numarası var mı diye kontrol etmek için
	OKU;
	fseek(veritabani,(tmp.hesapno-1)*sizeof(uye),SEEK_SET);
	fread(&tmn,sizeof(uye),1,veritabani);
	KAPA;
	if (tmn.hesapno!=0) printf("Boyle bir hesap numarasi zaten mevcut.");
	printf("\n\n");
	}while(regkontrol(tmp.id,tmp.phone)==true || strlen(tmp.id)<=6 || strlen(tmp.pw)<=6 || strcmp(tmp.pw,tmppw)!=0||tmn.hesapno!=0);
	
	
	FILE* veritabani;
	
	//Eğer dosya yoksa yeni bir dosya yapması için 'write' modunda açılır.
	if(!fopen("veritabani.dat","r")){
		veritabani=fopen("veritabani.dat","w");
	}
	guncelle(tmp);
	printf("Hesabiniz basariyla olusturuldu.");
}

//Hesaba giriş
void login(){
	printf("\n\n			GIRIS\n------------------------------------------------------\n");
	char id[20],pw[20];
	int no,i=0;
	uye tmp;
	printf("\nKullanici adinizi giriniz: ");
	scanf("%s",id);
	printf("\nSifrenizi giriniz: ");
	scanf("%s",pw);
	if(logkontrol(id,pw)==-1){
		 printf("Boyle bir kullanici adi bulunamadi\n");
		 return;
	}
	printf("\n\n");
	
	//Kullanıcı adı ve şifreyle eşleşen kullanıcıyı veritabanından bul
	OKU;
	fseek(veritabani,(logkontrol(id,pw)-1)*sizeof(uye),SEEK_SET);
	fread(&tmp,sizeof(uye),1,veritabani);
	KAPA;
	
	//Hesabı işlem fonksiyonuna parametre olarak yaz
	islem(tmp);
}


int main(int argv, char* argc){
	short int cevap=3;
	while(1){
	printf("\n---------------------------\nBankamiza hosgeldiniz. Yapmak istediginiz islemi seciniz:\n1.Yeni Hesap Ac\n2.Hesabina Giris Yap\n3.Cikis Yap\n---------------------------\nCevap: ");
	scanf("%d",&cevap);
	switch(cevap){
		case 1:
			reg(); //Kayıt
			break;
		case 2:
			login(); //Giriş
			break;
		case 3:
			return 0;
		default:
			break;
	}
	}
return 0;	
}
