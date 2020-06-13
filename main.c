#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//szóval...
//e jelentése érték
//w jelentése írhatóság
typedef struct t{
	int e;
	int w;
}kocka,*pk;
//felhasznált lépések (azé long mert tipikusan 32000-nél nagyobb
long int stp;
//kirajzoló függvény
void rajz(pk);
//olyan értelemben nézi, hogy jó, hogy elvileg be lehet-e írni
int jo(pk,int);
//sort, oszlopot, négyzetet ellenõrzõ függvény
int sell(pk,int);
int oell(pk,int);
int nell(pk,int);

void main(void) {
	//a térkép olyan struktokból áll, amiknek van értéke meg írhatósága
	pk map;
	int lehetetlen=0,kesz=0,muk;
	//ciklusokhoz változók
	int i;
	int p;
	//fájlból tölti be a pályát
	//fájl felépítése: xkoordináta szóköz ykoordináta szóköz érték enter stb
	FILE *fp;
	int a,b,c;
	//a map nem kétdimenziós tömb, hanem egy.
	//miért?
	//így könnyebb az elõzõ ill következõre lépkedés
	//tehát az indexek 0 tól 80 ig futnak
	//minden sorszám/9 megadja az y-t
	//minden sorszám%9 megadja az oszlopot,

	//matematikai kitérõ:
	//tehát a leképzés bijektív->létezik inverz fv
	//->létezik átváltás (majd késõbb a nell fv-ben bebüntet)
	//a 2Ds tömb számossága <=Alef0, mert 81 elemû
	//a 2Ds tömb akkor invertálható, ha töltesz bele elemket, különben csupa 0
	//kitérõ vége
	//memóriafoglalás 99 elemnek
	map=(pk)malloc(sizeof(kocka)*9*9);
	//feltoltes
	//mindenkit 0 ra és írhatóra tesz
	for(i=0;i<81;i++) {
		map[i].e=0;
		map[i].w=1;
	}
	//aki a fileból jön, nem írható
	fp=fopen("map.dat","r");
	while(fscanf(fp,"%d%d%d",&a,&b,&c)==3) {
		map[b*9+a].e=c;
		map[b*9+a].w=0;
	}
	//itt ellenõrzöd, hogy az-e amit kitölteni akarsz, billentyûleütésre tovább megy
	rajz(map);
	//megkérdezi, hogy szertnéd-e mûködés közben látni
	
	printf("\nlepesenkent akarod latni?(i/n)");
	muk=getchar();
	muk=(muk=='i')?1:0;
	printf("\nnyomj gombot, ha mehet!");
	//ez a getkar csak várakoztat, ne érdekeljen
	getchar();
	//kitöltés
	//a p az egy cucc, amivel rohangál az indexek között
	p=0; 
	//lépések száma alapból
	stp=0;
	//addig megy, amég nem lehetetlen v nincs kész, de úgyis kibrékel belõle, tehát írhatsz while(1)-t is
	while(!lehetetlen && !kesz) {
		//növeljük meg, amíg nem lesz jó, de ne 0ról kezdje ellenõrizni
		do {
			if(map[p].w==1) map[p].e++;
			stp++;
			if(muk)rajz(map);
		}while(!jo(map,p));
		//túlpörgött, tehát abba a pozícióba semmit nem lehet tenni
		if(map[p].e==10) {
			map[p].e=0;
			//visszalép
			do {
				p--;
				//ha kiszaladt az elsõ mezõn is, gáz van
				if(p<0) {lehetetlen=1;break;}
			}while(map[p].w!=1);//azt nézi, írhatóra megy-e
			continue;
		} else {
			//továbblép
			do {
				p++;
				//ha a 81. mezõ is klappol, akkor hátradõl a programozó és örül, hogy
				//a www.websudoku.com evil fokozatát 0 mp alatt solvolja
				if(p>80) {kesz=1;break;}
			}while(map[p].w!=1);
			continue;
		}

		
	}
	//Óz a nagy varázsló olyat mutat, hogy a kisgyerekek ilyet kérnek Karira
	//precíz kis space enter tördelt áttekinthetõ táblázat, majdnem excel
	rajz(map);
	getchar();
	
	

}

void rajz(pk a) {
	int i;
	//ez itt ANSI szerint duplán javasolt és szabványosított megoldás :)
	system("cls");
	for(i=0;i<81;i++) {
		printf("%d ",a[i].e);
		if(i%9==8)printf("\n\n");
	}
	printf("\neddig %d lepes telt el",stp);
}
//akkor jó, ha mind jó
int jo(pk a,int s) {
	//idáig megy, ha mindegyik jó, akkor visszaadja, hogy jó, ha bármelyik nem stimmel, azt is
	if((sell(a,s) && oell(a,s) && nell(a,s))==1)return 1;
	//ebbe az ágba csak abban az esetben lép be, ha az elõzõbõl nem tért vissza
	//ergo nem kell else
	return 0;
	
}
//sorellenõrzõ
int sell(pk a,int s) {
	int i,elso;
	//s a sorszám, azt 9-el osztva és 9-el megszorozva megkapod a sor elején álló elemet
	elso=s/9*9;
	//ez egészosztás, tehát nehogy letöröld mert X/9*9=X mert nem annyi 9 bõl 8 esetben
	for(i=elso;i<elso+9;i++) {
		//ne magával vizsgálja, mert akkor sosem lesz jó
		if(i==s)continue;
		//ha vki ugyanaz a sorban, mint a vizsgált, hamisat ad vissza
		if(a[s].e==a[i].e)return 0;
	}
	//megint csak akkor tér vissza, ha  elõbb nem
	return 1;
}
//oszlopot hasonlít
int oell (pk a, int s) {
	int elso,i;
	//a  mod 9 a 9s maradék, ez a sor legtetején állók sorszáma
	elso=s%9;
	//tehát 9esével ugrál lefelé
	//pl a 4-es alatt áll a 4+9=13, az alatt a 13+9=22
	for(i=elso;i<=elso+9*8;i+=9) {
		//fut egészen a legfölsõ+72-ig (a 72-t rajzon meglátod simán, de mondhatod, hogy
		//8-al van alatta az utolsó és minden leugrás 9-et jelent sorszámilag
		if(i==s)continue;
		if(a[s].e==a[i].e)return 0;
	}
	return 1;
}
//Álmaink netovábbja, a mod mûvelet és X*/*/ együttes használata
//komoly metróablakonkibámulási idõt emésztett fel a megalkotása
//mûködik viszont
int nell(pk a, int s) {
	//kicsit több a változó
	int x,y,fs,i;
	//fs=fölsõ sarok.Az a sorszám, amelyik a 3*3as négyzeten belül a bal fölsõ
	//az össszeg elsõ része a négyzeten belüli legfölsõ sor elsõ eleme
	//a második rész a jobbra lépkedés a bal fölsõ sarkáig a négyzetnek
	fs=s/9/3*3*9+(s%9)/3*3;
	for(x=fs;x<fs+3;x++) {
		for(y=0;y<3;y++) {
			//úgy vizsgál, hogy lépeget a sorban, közben lefelé megnéz 3-at
			//az i pedig a kérdéses sorszám
			i=x+y*9;
			if(i==s)continue;
			if(a[s].e==a[i].e)return 0;
		}
	}
	return 1;
}





