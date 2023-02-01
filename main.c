#include <stdio.h>
#include <stdlib.h>
#include "operacije.h"

int main()
{
     int running = 1;
	int in;

	FILE *fajl = NULL;

    do{
		printf("\n----------------ODABERITE OPCIJU----------------\n");
		printf("\n\t1. Otvaranje datoteke\n");
		printf("\t2. Formiranje datoteke\n");
		printf("\t3. Pronadji slog\n");
        printf("\t4. Dodaj slog\n");
		printf("\t5. Ispisi sve slogove\n");
		printf("\t6. Fizicki izbrisi slog\n");
		printf("\t7. Prikazi tip aviona sa najvecom brzinom\n");
		printf("\t8. Prikazi za svako mesto dolaska razlicite tipove aviona\n");
		printf("\t0. Izlaz\n");
		printf("\n-------------------------------------------------\n");
		if (fajl == NULL) {
			printf("\n\nDatoteka jos uvek nije otvorena\n");
		}
		scanf("%d", &in);
		getc(stdin);

		switch(in) {
			case 1:
				{
					char imeDatoteke[20];
					printf("Unesite ime datoteke koju zelite da otvorite: ");
					scanf("%s", &imeDatoteke[0]);
					fajl = otvoriDatoteku(imeDatoteke);
					break;
				}

			case 2:
				{
					char imeDatoteke[20];
					printf("Unesite ime datoteke za kreiranje: ");
					scanf("%s", imeDatoteke);
					kreirajDatoteku(imeDatoteke);
					break;
				}
            case 3:
				{
					int sifraLeta;
				    printf("Unesi sifru leta za slog koji zelite da vidite: ");
				    scanf("%d",&sifraLeta);
				    SLOG* s = pronadjiSlog(fajl,sifraLeta);
				    if(s==NULL){
                        printf("Slog sa siform leta %d nije pronadjen", sifraLeta);
				    }
				    else
                        ispisiSlog(s);
				    break;
				}
			case 4:
				{
				    SLOG s;
                    printf("Unesite sifru leta: ");
                    scanf("%d",&s.sifraLeta);
                    printf("Unesite datum u obliku <dd-mm-yyyy hh:mm>: ");
                    scanf("%d-%d-%d %d:%d",
                          &s.datum.dan,
                          &s.datum.mesec,
                          &s.datum.godina,
                          &s.datum.sat,
                          &s.datum.minut);
                    printf("Unesite tip aviona: ");
                    scanf("%s",s.tipAviona);
                    printf("Unesite trajanje leta u minutima: ");
                    scanf("%d",&s.trajanjeLeta);
                    printf("Unesite udaljenost leta: ");
                    scanf("%d",&s.udaljenostLeta);
                    printf("Unesite mesto dolaska: ");
                    scanf("%s",s.mestoDolaska);
                     if (s.datum.godina==2021 && s.datum.mesec==12 && s.datum.dan==31) {
                        if(s.datum.sat*60+s.datum.minut+s.trajanjeLeta>24*60){
                        strcat(s.mestoDolaska,"*");

                    }
                }
                    s.deleted=0;
                    dodajSlog(fajl,&s);
					break;
				}
            case 5:
				{
                    ispisiSveSlogove(fajl);
                    break;
				}
            case 6:
				{
				    int sifraLeta;
				    printf("Unesite sifru leta za brisanje izabranog sloga: ");
				    scanf("%d",&sifraLeta);
                    obrisiSlogFizicki(fajl,sifraLeta);
					break;
				}
			case 7:
				{
				    SLOG* s;
				    s = zadatak2(fajl);
				    if(s!= NULL) {
				    printf("Najbrzi tip aviona je: \n");
				    ispisiSlog(s);
				    }
					break;
				}
            case 8:
                {
                    zadatak4(fajl);
                    break;
                }
            case 0:
                {
                    fclose(fajl);
                    printf("Datoteka uspesno zatvorena\n");
                    return 0;
                }
            default:
                    printf("Izabrali ste opciju koja ne postoji\n");
		}
	} while(1);
}
