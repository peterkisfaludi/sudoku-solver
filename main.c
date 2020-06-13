#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//sz�val...
//e jelent�se �rt�k
//w jelent�se �rhat�s�g
typedef struct t{
	int e;
	int w;
}kocka,*pk;
//felhaszn�lt l�p�sek (az� long mert tipikusan 32000-n�l nagyobb
long int stp;
//kirajzol� f�ggv�ny
void rajz(pk);
//olyan �rtelemben n�zi, hogy j�, hogy elvileg be lehet-e �rni
int jo(pk,int);
//sort, oszlopot, n�gyzetet ellen�rz� f�ggv�ny
int sell(pk,int);
int oell(pk,int);
int nell(pk,int);

void main(void) {
	//a t�rk�p olyan struktokb�l �ll, amiknek van �rt�ke meg �rhat�s�ga
	pk map;
	int lehetetlen=0,kesz=0,muk;
	//ciklusokhoz v�ltoz�k
	int i;
	int p;
	//f�jlb�l t�lti be a p�ly�t
	//f�jl fel�p�t�se: xkoordin�ta sz�k�z ykoordin�ta sz�k�z �rt�k enter stb
	FILE *fp;
	int a,b,c;
	//a map nem k�tdimenzi�s t�mb, hanem egy.
	//mi�rt?
	//�gy k�nnyebb az el�z� ill k�vetkez�re l�pked�s
	//teh�t az indexek 0 t�l 80 ig futnak
	//minden sorsz�m/9 megadja az y-t
	//minden sorsz�m%9 megadja az oszlopot,

	//matematikai kit�r�:
	//teh�t a lek�pz�s bijekt�v->l�tezik inverz fv
	//->l�tezik �tv�lt�s (majd k�s�bb a nell fv-ben beb�ntet)
	//a 2Ds t�mb sz�moss�ga <=Alef0, mert 81 elem�
	//a 2Ds t�mb akkor invert�lhat�, ha t�ltesz bele elemket, k�l�nben csupa 0
	//kit�r� v�ge
	//mem�riafoglal�s 99 elemnek
	map=(pk)malloc(sizeof(kocka)*9*9);
	//feltoltes
	//mindenkit 0 ra �s �rhat�ra tesz
	for(i=0;i<81;i++) {
		map[i].e=0;
		map[i].w=1;
	}
	//aki a fileb�l j�n, nem �rhat�
	fp=fopen("map.dat","r");
	while(fscanf(fp,"%d%d%d",&a,&b,&c)==3) {
		map[b*9+a].e=c;
		map[b*9+a].w=0;
	}
	//itt ellen�rz�d, hogy az-e amit kit�lteni akarsz, billenty�le�t�sre tov�bb megy
	rajz(map);
	//megk�rdezi, hogy szertn�d-e m�k�d�s k�zben l�tni
	
	printf("\nlepesenkent akarod latni?(i/n)");
	muk=getchar();
	muk=(muk=='i')?1:0;
	printf("\nnyomj gombot, ha mehet!");
	//ez a getkar csak v�rakoztat, ne �rdekeljen
	getchar();
	//kit�lt�s
	//a p az egy cucc, amivel rohang�l az indexek k�z�tt
	p=0; 
	//l�p�sek sz�ma alapb�l
	stp=0;
	//addig megy, am�g nem lehetetlen v nincs k�sz, de �gyis kibr�kel bel�le, teh�t �rhatsz while(1)-t is
	while(!lehetetlen && !kesz) {
		//n�velj�k meg, am�g nem lesz j�, de ne 0r�l kezdje ellen�rizni
		do {
			if(map[p].w==1) map[p].e++;
			stp++;
			if(muk)rajz(map);
		}while(!jo(map,p));
		//t�lp�rg�tt, teh�t abba a poz�ci�ba semmit nem lehet tenni
		if(map[p].e==10) {
			map[p].e=0;
			//visszal�p
			do {
				p--;
				//ha kiszaladt az els� mez�n is, g�z van
				if(p<0) {lehetetlen=1;break;}
			}while(map[p].w!=1);//azt n�zi, �rhat�ra megy-e
			continue;
		} else {
			//tov�bbl�p
			do {
				p++;
				//ha a 81. mez� is klappol, akkor h�trad�l a programoz� �s �r�l, hogy
				//a www.websudoku.com evil fokozat�t 0 mp alatt solvolja
				if(p>80) {kesz=1;break;}
			}while(map[p].w!=1);
			continue;
		}

		
	}
	//�z a nagy var�zsl� olyat mutat, hogy a kisgyerekek ilyet k�rnek Karira
	//prec�z kis space enter t�rdelt �ttekinthet� t�bl�zat, majdnem excel
	rajz(map);
	getchar();
	
	

}

void rajz(pk a) {
	int i;
	//ez itt ANSI szerint dupl�n javasolt �s szabv�nyos�tott megold�s :)
	system("cls");
	for(i=0;i<81;i++) {
		printf("%d ",a[i].e);
		if(i%9==8)printf("\n\n");
	}
	printf("\neddig %d lepes telt el",stp);
}
//akkor j�, ha mind j�
int jo(pk a,int s) {
	//id�ig megy, ha mindegyik j�, akkor visszaadja, hogy j�, ha b�rmelyik nem stimmel, azt is
	if((sell(a,s) && oell(a,s) && nell(a,s))==1)return 1;
	//ebbe az �gba csak abban az esetben l�p be, ha az el�z�b�l nem t�rt vissza
	//ergo nem kell else
	return 0;
	
}
//sorellen�rz�
int sell(pk a,int s) {
	int i,elso;
	//s a sorsz�m, azt 9-el osztva �s 9-el megszorozva megkapod a sor elej�n �ll� elemet
	elso=s/9*9;
	//ez eg�szoszt�s, teh�t nehogy let�r�ld mert X/9*9=X mert nem annyi 9 b�l 8 esetben
	for(i=elso;i<elso+9;i++) {
		//ne mag�val vizsg�lja, mert akkor sosem lesz j�
		if(i==s)continue;
		//ha vki ugyanaz a sorban, mint a vizsg�lt, hamisat ad vissza
		if(a[s].e==a[i].e)return 0;
	}
	//megint csak akkor t�r vissza, ha  el�bb nem
	return 1;
}
//oszlopot hasonl�t
int oell (pk a, int s) {
	int elso,i;
	//a  mod 9 a 9s marad�k, ez a sor legtetej�n �ll�k sorsz�ma
	elso=s%9;
	//teh�t 9es�vel ugr�l lefel�
	//pl a 4-es alatt �ll a 4+9=13, az alatt a 13+9=22
	for(i=elso;i<=elso+9*8;i+=9) {
		//fut eg�szen a legf�ls�+72-ig (a 72-t rajzon megl�tod sim�n, de mondhatod, hogy
		//8-al van alatta az utols� �s minden leugr�s 9-et jelent sorsz�milag
		if(i==s)continue;
		if(a[s].e==a[i].e)return 0;
	}
	return 1;
}
//�lmaink netov�bbja, a mod m�velet �s X*/*/ egy�ttes haszn�lata
//komoly metr�ablakonkib�mul�si id�t em�sztett fel a megalkot�sa
//m�k�dik viszont
int nell(pk a, int s) {
	//kicsit t�bb a v�ltoz�
	int x,y,fs,i;
	//fs=f�ls� sarok.Az a sorsz�m, amelyik a 3*3as n�gyzeten bel�l a bal f�ls�
	//az �ssszeg els� r�sze a n�gyzeten bel�li legf�ls� sor els� eleme
	//a m�sodik r�sz a jobbra l�pked�s a bal f�ls� sark�ig a n�gyzetnek
	fs=s/9/3*3*9+(s%9)/3*3;
	for(x=fs;x<fs+3;x++) {
		for(y=0;y<3;y++) {
			//�gy vizsg�l, hogy l�peget a sorban, k�zben lefel� megn�z 3-at
			//az i pedig a k�rd�ses sorsz�m
			i=x+y*9;
			if(i==s)continue;
			if(a[s].e==a[i].e)return 0;
		}
	}
	return 1;
}





