#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>


//Callback funkcija prikazuje i nazive kolona i vrednosti polja
/*static int callback(void *data, int br_kolona, char **vrednost, char **ime_kolone) {
	int i;
	for (i = 0; i < br_kolona; i++) {
			printf("%-8s = %12s\n", ime_kolone[i], vrednost[i] ? vrednost[i] : "null");
	}
	printf("\n");
	return 0;
	}*/

//Druga callback funkcija prikazuje samo vrednosti polja ( ali ne i nazive kolona)
static int callback(void *data, int br_kolona, char **vrednost, char **Ime_Kolone) {
	int i;
	for (i = 0; i < br_kolona; i++) {
		printf("%-15s",vrednost[i] ? vrednost[i] : "NULL");
	}
	printf("\n");
	return 0;
}

//Funkcija koja ispisuje zaglavlje(nazive kolona)
void naslov() {
	printf("\n");
	printf("SQLite + C\n");
	printf("%-15s%-15s%-15s%-15s%-15s\n", "Indeks", "Ime", "Prezime", "Smer", "Prosek");
	printf("_____________________________________________________________________________\n");
}

int meni() {
	printf("\nSQLite meni:\n\
_______________________________________\n\
1. Insert - Ubaci studenta\n\
2. Select - Prikazi sve podatke\n\
3. Update - Azuriraj prosek studentu\n\
4. Delete - Obrisi odredjenog studenta\n\
0. Izlaz\n\
Vas izbor: ");
	int izbor;
	scanf("%d", &izbor); getchar();
	return izbor;
}

int main(int argc, char* argv[]) {
	sqlite3 *BAZA;			//Pokazaivac na bazu podataka
	char *ErrorPoruka = 0;	//Poruka koja se ispisuje usled greske
	int rezultat;			//Rezultat sqlite funkcija
	char sql_iskaz[1000];		//SQL komanda koja se izvrsava
	const char* data = "Poziv callback funkcije";


	/* Otvranje baze podataka */
	//Ukoliko baza pod tim imenom ne postoji, bice kreirana
	rezultat = sqlite3_open("Student.db", &BAZA);
	//Ukoliko je baza uspsesno otvorena povratna vrednost je 0
	if (rezultat != SQLITE_OK) {
		fprintf(stderr, "Greska prilikom otvaranja baze podataka%s\n", sqlite3_errmsg(BAZA));
		return(0);
	}
	else {
		fprintf(stdout, "Baza podataka 'Student' je uspesno otvorena\n");
	}

	/*Brisanje tabele*/
	strcpy(sql_iskaz, "DROP TABLE IF EXISTS STUDENT");
	rezultat = sqlite3_exec(BAZA, sql_iskaz, callback, 0, &ErrorPoruka);
	if (rezultat != SQLITE_OK) {
		fprintf(stderr, "SQL greska: %s\n", ErrorPoruka);
		sqlite3_free(ErrorPoruka);
	}
	else {
		fprintf(stdout, "");
	}

	/*Kreiranje tabele student*/
	strcpy(sql_iskaz, "CREATE TABLE STUDENT("\
		"INDEKS INT PRIMARY KEY		NOT NULL,"\
		"IME				TEXT	NOT NULL,"\
		"PREZIME			TEXT	NOT NULL,"\
		"SMER				TEXT	NOT NULL,"\
		"PROSEK				REAL);");
	rezultat = sqlite3_exec(BAZA, sql_iskaz, callback, 0, &ErrorPoruka);
	if (rezultat != SQLITE_OK) {
		fprintf(stderr, "SQL greska: %s\n", ErrorPoruka);
		sqlite3_free(ErrorPoruka);
	}
	else {
		fprintf(stdout, "Tabela Student(INDEKS,IME,PREZIME,SMER,PROSEK) je uspesno kreirana\n");
	}
	/*Ubacivanje podataka*/
	strcpy(sql_iskaz, "INSERT INTO STUDENT(INDEKS,IME,PREZIME,SMER,PROSEK) "\
		"VALUES(1,'Petar','Miric','ISiT',9.17); "\
		"INSERT INTO STUDENT(INDEKS,IME,PREZIME,SMER,PROSEK) "\
		"VALUES(2,'Lazar','Mitrovic','MiO',8.23);"\
		"INSERT INTO STUDENT(INDEKS,IME,PREZIME,SMER,PROSEK) "\
		"VALUES(3,'Marina','Lazic','ISiT',7.8);"
		"INSERT INTO STUDENT(INDEKS,IME,PREZIME,SMER,PROSEK) "\
		"VALUES(4,'Marko','Dusanovic','ISiT',NULL);"
		"INSERT INTO STUDENT(INDEKS,IME,PREZIME,SMER,PROSEK) "\
		"VALUES(5,'Nikolina','Nikolic','MiO',9.6);");
	rezultat = sqlite3_exec(BAZA, sql_iskaz, callback, 0, &ErrorPoruka);
	if (rezultat != SQLITE_OK) {
		fprintf(stderr, "SQL greska: %s\n", ErrorPoruka);
		sqlite3_free(ErrorPoruka);
	}
	else {
		fprintf(stdout, "Podaci su uspesno uneti u tabelu Student\n");
	}

	int izbor;
	char indeks[10]; char noviprosek[10];
	char ime[30], prezime[30], smer[4];
	double prosek;
	do {
		izbor = meni();
		switch (izbor)
		{
		case 1:/* INSERT/UBACI */
			printf("\nNovi student\n");
			printf("Broj indeksa: "); scanf("%s", indeks); getchar();
			printf("Ime studenta: "); scanf("%s", ime); getchar();
			printf("Prezime studenta: "); scanf("%s", prezime); getchar();
			printf("Smer: "); scanf("%s", smer); getchar();
			printf("Prosek: "); scanf("%lf", &prosek); getchar();

			sprintf(sql_iskaz, "INSERT INTO STUDENT(INDEKS,IME,PREZIME,SMER,PROSEK) VALUES(%s,'%s','%s','%s',%lf); ", indeks, ime, prezime, smer, prosek);

			rezultat = sqlite3_exec(BAZA, sql_iskaz, callback, 0, &ErrorPoruka);
			if (rezultat != SQLITE_OK) {
				fprintf(stderr, "SQL greska: %s\n", ErrorPoruka);
				sqlite3_free(ErrorPoruka);
			}
			else {
				fprintf(stdout, "Student je uspesno unet u tabelu 'Student'\n");
			}

			break;
		case 2:/* SELECT/PRIKAZI */
			strcpy(sql_iskaz, "SELECT* FROM STUDENT");

			/* Izvrsavanje SQL iskaza */
			printf("\n");
			naslov();
			rezultat = sqlite3_exec(BAZA, sql_iskaz, callback, (void*)data, &ErrorPoruka);
			if (rezultat != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", ErrorPoruka);
				sqlite3_free(ErrorPoruka);
			}
			else {
				fprintf(stdout, "\n");
			}
			break;
		case 3:
			printf("Unesite Indeks studenta: ");
			scanf("%s", indeks); getchar();
			printf("Unesite novi prosek: ");
			scanf("%s", noviprosek); getchar();
			//Konkatenacija SQL iskaza
			sprintf(sql_iskaz, "UPDATE STUDENT SET PROSEK=%s WHERE INDEKS=%s", noviprosek, indeks);
			/* Izvrsavanje SQL iskaza */
			rezultat = sqlite3_exec(BAZA, sql_iskaz, callback, (void*)data, &ErrorPoruka);

			if (rezultat != SQLITE_OK) {
				fprintf(stderr, "SQL greska: %s\n", ErrorPoruka);
				sqlite3_free(ErrorPoruka);
			}
			else {
				fprintf(stdout, "\nAzuriranje uspesno izvrseno\n");
			}
			break;
		case 4:/* DELETE/OBRISI */
			printf("\nUnesite Indeks studenta: ");
			scanf("%s", indeks);getchar();
			sprintf(sql_iskaz, "DELETE FROM STUDENT WHERE INDEKS =%s", indeks);
			printf("Da li ste sigurni da zelite da obrise studenta sa brojem indeksa %s?\n1. Da\n2. Ne\n", indeks);
			printf("Vas izbor: "); int odgovor; scanf("%d", &odgovor); getchar();
			if (odgovor == 1) {
				rezultat = sqlite3_exec(BAZA, sql_iskaz, callback, (void*)data, &ErrorPoruka);

				if (rezultat != SQLITE_OK) {
					fprintf(stderr, "SQL greska: %s\n", ErrorPoruka);
					sqlite3_free(ErrorPoruka);
				}
				else {
					fprintf(stdout, "\nBrisanje uspesno izvrseno\n");
				}
			}
			break;
		case 0:
			system("cls");
			printf("KRAJ PROGRAMA\n");
			break;
		default:
			printf("Pogresan izbor, pokusajte ponovo\n");
			break;
		}
	} while (izbor != 0);


	system("pause");
	return 0;
}

