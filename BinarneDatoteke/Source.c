#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define DATOTEKA "Student.bin"
typedef struct student {
	int indeks;
	char ime[15];
	char prezime[15];
	char smer[15];
	double prosek;
}STUDENT;
int meni();
void unesiStudentaUDatoteku(char* nazivDatoteke);
void prikaziStudenta(STUDENT S);
void prikaziSveStudente(char* nazivDatoteke);
int daLiStudentPostojiUDatoteci(int indeks, char* nazivDatoteke);
void obrisiOdredjenogStudenta(char* NazivDatoteke);
void naslov();
void azurirajStudentuProsek(char* nazivDatoteke);
void formirajPocetnoStanje(STUDENT nizS[], int n, char* nazivDatoteke);
int main(void) {
	STUDENT nizS[5] = {
		{1,"Petar","Miric","ISiT",9.17},
		{2, "Lazar", "Mitrovic", "MiO", 8.23},
		{3, "Marina", "Lazic", "ISiT", 7.8},
		{4, "Marko", "Dusanovic", "ISiT", 9.0},
		{5, "Nikolina", "Nikolic", "MiO", 9.6},
	};
	formirajPocetnoStanje(nizS, 5, DATOTEKA);
	int izbor;
	do
	{
		izbor = meni();
		switch (izbor)
		{
		case 1:
			unesiStudentaUDatoteku(DATOTEKA);
			break;
		case 2:
			naslov();
			prikaziSveStudente(DATOTEKA);
			break;
		case 3:
			obrisiOdredjenogStudenta(DATOTEKA);
			break;
		case 4:azurirajStudentuProsek(DATOTEKA);
			break;
		case 0:
			system("cls");
			printf("KRAJ PROGRAMA\n");			
			break;
		default:
			break;
		}
	} while (izbor!=0);



	system("pause");
	return 0;
}

int meni() {
	int izbor;
	printf("\nMeni\n\
_______________________\n\
1. Unesi studenta\n\
2. Prikazi sve studente\n\
3. Obrisi studenta\n\
4. Azuriraj prosek studentu\n\
0. Izlaz\n\
Vas izbor: ");
	scanf("%d", &izbor);getchar();
	return izbor;
}
void unesiStudentaUDatoteku(char* nazivDatoteke) {
	FILE* fptr = fopen(nazivDatoteke, "ab");
	if (fptr == NULL) {
		printf("Greska prilikom otvaranja datoteke\n");
		return;
	}
	STUDENT S;
	printf("\nNovi student\n");
	printf("Indeks studenta: "); 
	scanf("%d", &S.indeks);getchar();
	printf("Ime: ");
	scanf("%s", &S.ime);getchar();
	printf("Prezime: ");
	scanf("%s", &S.prezime);getchar();
	printf("Smer: ");
	scanf("%s", &S.smer);getchar();
	printf("Prosek: ");
	scanf("%lf", &S.prosek);getchar();
	
	if (daLiStudentPostojiUDatoteci(S.indeks, nazivDatoteke) == 0) {
		fwrite(&S, sizeof(STUDENT), 1, fptr);
		printf("Student je uspesno unet u datoteku\n");
	}
	else {
		printf("Studen sa indeksom %d vec postoji u datoteci!\n", S.indeks);
	}
	fclose(fptr);
}
void naslov() {
	printf("\n");
	printf("Binarne datoteke + C\n");
	printf("%-10s%-20s%-20s%-9s%-9s\n", "Indeks", "Ime", "Prezime", "Smer", "Prosek");
	printf("_____________________________________________________________________________\n");
}
void prikaziStudenta(STUDENT S) {
	printf("%-10d%-20s%-20s%-9s%-9.2lf\n",S.indeks ,S.ime, S.prezime, S.smer, S.prosek);
}
void prikaziSveStudente(char* nazivDatoteke) {
	FILE* fptr = fopen(nazivDatoteke, "rb");
	if (fptr == NULL) {
		printf("Greska prilikom otvaranja datoteke\n");
		return;
	}
	STUDENT S;
	while (fread(&S, sizeof(STUDENT), 1, fptr) > 0) {
		prikaziStudenta(S);
	}
	fclose(fptr);
}
int daLiStudentPostojiUDatoteci(int indeks,char* nazivDatoteke) {
	FILE* fptr = fopen(nazivDatoteke, "rb");
	STUDENT S;
	while (fread(&S, sizeof(STUDENT), 1, fptr) > 0) {
		if (S.indeks == indeks)
			return 1;
	}
	return 0;
}
void azurirajStudentuProsek(char* nazivDatoteke) {
	FILE* fptr = fopen(nazivDatoteke, "rb+");
	if (fptr == NULL) {
		printf("Greska prilikom otvaranja datoteke\n");
		return;
	}
	int indeks; double prosek;
	printf("Unesite indeks studenta ciji prosek zelite da azurirate: ");
	scanf("%d", &indeks);getchar();
	printf("Unesite novi prosek: ");
	scanf("%lf", &prosek);getchar();
	STUDENT S;
	while (fread(&S, sizeof(STUDENT), 1, fptr) > 0){
		if (S.indeks == indeks) {
			S.prosek = prosek;

			//Vracanje pokazivaca na prethodni bajt;
			fseek(fptr, 0 - sizeof(STUDENT), SEEK_CUR);

			//Upisivanje uvecanje vrednosti
			fwrite(&S, sizeof(STUDENT), 1, fptr);
			fflush(fptr);
			printf("Azuriranje uspesno izvrseno\n");
			fclose(fptr);
			return;
		}
	}
	printf("Ne postoji student sa zadatim indeksom\n");
	fclose(fptr);
}
void obrisiOdredjenogStudenta(char* NazivDatoteke) {
	STUDENT nizStudenata[1000];
	int n = 0;

	FILE* fptr = fopen(NazivDatoteke, "r");
	if (fptr == NULL) {
		printf("Neuspesno otvaranje datoteke\n");
		return;
	}
	
	fseek(fptr, 0, SEEK_END);
	n = ftell(fptr) / sizeof(STUDENT);
	fseek(fptr, 0, SEEK_SET);
	fread(nizStudenata, n, sizeof(STUDENT), fptr);

	fclose(fptr);

	int indeks;
	printf("Unesite indeks studenta kog zelite da izbrisete: ");
	scanf("%d", &indeks);
	int postoji = 0;
	for (int i = 0;i < n;i++) {
		if (nizStudenata[i].indeks == indeks) {
			for (int j = i;j < n;j++) {
				nizStudenata[j] = nizStudenata[j + 1];
			}
			postoji = 1;
			n--;
		}
	}
	if (!postoji) {
		printf("Student sa indeksom %d ne postoji u datoteci\n", indeks);
	}
	else {
		printf("Student sa indeksom %d je uspesno obrisan\n", indeks);
	}

	fptr = fopen(NazivDatoteke, "w");
	for (int i = 0;i < n;i++) {
		fwrite(&nizStudenata[i], sizeof(STUDENT), 1, fptr);
	}
	fclose(fptr);
}

void formirajPocetnoStanje(STUDENT nizS[], int n, char* nazivDatoteke) {
	FILE* fptr = fopen(nazivDatoteke, "wb");
	if (fptr == NULL) {
		printf("Datoteka nije uspesno otvorena\n");
		return 0;
	}

	for (int i = 0;i < n;i++) {
		fwrite(&nizS[i], sizeof(STUDENT), 1, fptr);
	}
	printf("Datoteka je otvorena\nStudenti su uspesno uneti u datoteku\n");
	fclose(fptr);
}