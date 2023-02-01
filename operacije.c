#include "slog.h"
#include "operacije.h"
#include <stdbool.h>

FILE *otvoriDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "rb+");
	if (fajl == NULL) {
		printf("Datoteka ne postoji \n");
	} else {
		printf("Datoteka otvorena.\n" );
	}
	return fajl;
}

void kreirajDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "wb");
	if (fajl == NULL) {
		printf("Doslo je do greske prilikom kreiranja datoteke \"%s\"!\n", filename);
	} else {
		BLOK blok;
		blok.slogovi[0].sifraLeta=END_OF_FILE;
		fwrite(&blok, sizeof(BLOK), 1, fajl);
		printf("Datoteka \"%s\" uspesno kreirana.\n", filename);
		fclose(fajl);
	}
}

SLOG *pronadjiSlog(FILE *fajl, int sifraLeta) {
	if (fajl == NULL) {
		return NULL;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOK; i++) {
			if (sifraLeta==END_OF_FILE) {
				return NULL;
			}

			if (blok.slogovi[i].sifraLeta==sifraLeta && !blok.slogovi[i].deleted) {

				SLOG *slog = (SLOG *)malloc(sizeof(SLOG));
				memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
				return slog;
			}
		}
	}

	return NULL;
}

void dodajSlog(FILE *fajl, SLOG *slog) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena\n");
		return;
	}

	SLOG *prosliSlog = pronadjiSlog(fajl, slog->sifraLeta);
	if (prosliSlog != NULL) {
        printf("Vec postoji slog sa tim evid brojem\n");
        return;
    }

	BLOK b;
	fseek(fajl, -sizeof(BLOK), SEEK_END);
	fread(&b, sizeof(BLOK), 1, fajl);

	int i = 0;
	while(i<FBLOK) {
		if (b.slogovi[i].sifraLeta==END_OF_FILE) {
			memcpy(&b.slogovi[i], slog, sizeof(SLOG));
			break;
		}
		i++;
	}

	i++;

	if (i < FBLOK) {
        b.slogovi[i].sifraLeta=END_OF_FILE;
		fseek(fajl, -sizeof(BLOK), SEEK_CUR);
		fwrite(&b, sizeof(BLOK), 1, fajl);
		fflush(fajl);
	} else {
        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
		fwrite(&b, sizeof(BLOK), 1, fajl);

		BLOK noviBlok;
		noviBlok.slogovi[0].sifraLeta=END_OF_FILE;
		fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
	}

	if (ferror(fajl)) {
		printf("Greska pri upisu u fajl\n");
	} else {
		printf("Upis novog sloga zavrsen.\n");
	}
}

void ispisiSveSlogove(FILE *fajl) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	int rbBloka = 0;
	printf("Sifra leta\t\tDatum\t\t  Tip aviona\t\tTrajanje leta\t\tUdaljenost leta\t\tMesto dolaska\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOK; i++) {
			if (blok.slogovi[i].sifraLeta== END_OF_FILE) {
				printf("B%d S%d *\n", rbBloka, i);
                break;
			}
			if (!blok.slogovi[i].deleted) {
                printf("B%d S%d ", rbBloka, i);
                ispisiSlog(&blok.slogovi[i]);
                printf("\n");
            }
		}

		rbBloka++;
	}
}

void ispisiSlog(SLOG *slog) {
	printf("%d\t\t%d-%d-%d %d:%d\t\t  %s\t\t\t%d\t\t\t%d\t\t\t%s\n",
        slog->sifraLeta,
        slog->datum.dan,
        slog->datum.mesec,
        slog->datum.godina,
        slog->datum.sat,
        slog->datum.minut,
        slog->tipAviona,
        slog->trajanjeLeta,
        slog->udaljenostLeta,
        slog->mestoDolaska);

}

void obrisiSlogFizicki(FILE *fajl, int sifraLeta) {

    SLOG *slog = pronadjiSlog(fajl, sifraLeta);
    if (slog == NULL) {
        printf("Slog koji zelite obrisati ne postoji\n");
        return;
    }

    BLOK b, sledeciBlok;
    int sifraLetaObrisati = sifraLeta;

    fseek(fajl, 0, SEEK_SET);
    while (fread(&b, 1, sizeof(BLOK), fajl)) {
        for (int i = 0; i < FBLOK; i++) {

            if (b.slogovi[i].sifraLeta == END_OF_FILE) {

                if (i == 0) {
                    printf("Skracujem datoteku\n");
                    fseek(fajl, -sizeof(BLOK), SEEK_END);
					long bytesToKeep = ftell(fajl);
					ftruncate(fileno(fajl), bytesToKeep);
					fflush(fajl);
                }

                printf("Slog je fizicki obrisan.\n");
                return;
            }

            if (b.slogovi[i].sifraLeta == sifraLetaObrisati) {

                for (int j = i+1; j < FBLOK; j++) {
                    memcpy(&(b.slogovi[j-1]), &(b.slogovi[j]), sizeof(SLOG));
                }

                int podatakaProcitano = fread(&sledeciBlok, sizeof(BLOK), 1, fajl);

                if (podatakaProcitano) {

                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);

                    memcpy(&(b.slogovi[FBLOK-1]), &(sledeciBlok.slogovi[0]), sizeof(SLOG));
                    sifraLetaObrisati= sledeciBlok.slogovi[0].sifraLeta;
                }

                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&b, sizeof(BLOK), 1, fajl);
                fflush(fajl);

                if (!podatakaProcitano) {
                    printf("Slog je fizicki obrisan.\n");
                    return;
                }
                break;
            }

        }
    }
}

SLOG* zadatak2(FILE *fajl){
    if (fajl == NULL) {
            printf("Datoteka nije otvorena\n");
            return NULL;
        }
        double najvecaBrzina=-1;

        SLOG *najbrzi = (SLOG *)malloc(sizeof(SLOG));
        fseek(fajl, 0, SEEK_SET);
        BLOK b;
        double trenutna;

        while (fread(&b, sizeof(BLOK), 1, fajl)) {

            for (int i = 0; i < FBLOK; i++) {
                if (b.slogovi[i].sifraLeta== END_OF_FILE) {
                    break;
                }
                if (!b.slogovi[i].deleted) {
                   trenutna=(double)b.slogovi[i].udaljenostLeta/(double)b.slogovi[i].trajanjeLeta;
                   if(trenutna>najvecaBrzina){
                        najvecaBrzina = trenutna;
                        memcpy(najbrzi,&b.slogovi[i], sizeof(SLOG));
                   }
                }
            }

        }
        return najbrzi;
}

void zadatak4(FILE *fajl){
if (fajl == NULL) {
		printf("Datoteka nije otvorena\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK b;
	M m[100];

	bool tipAviona= false;
	int i,j,k, cm, idx = 0;
	for(i; i<FBLOK; i++){
            cm++;
        if(b.slogovi[i].sifraLeta == END_OF_FILE){
            break;
        }
	}

        for(i=0; i<FBLOK; i++){
        if(b.slogovi[i].sifraLeta == END_OF_FILE){
            break;
        }
        else{
            if(b.slogovi[i].deleted == 0){
                for (j; j<cm; j++){
                    if (strcmp(b.slogovi[i].mestoDolaska, m[j].mesto)==0){
                        for (k; k<m[j].cnt; k++){
                            if(strcmp(b.slogovi[i].tipAviona, m[j].tipovi)==0){
                                tipAviona=false;
                            }
                        }
                            if(tipAviona==true){
                                strcpy(m[j].tipovi[m[j].cnt], b.slogovi[i].tipAviona);

                            }
                        }
                    }
                }
            }

        }


	for(i=0; i<cm; i++){
        printf("%s-",m[i].mesto);
         for( j=0;j<m[i].cnt;j++)
         {
             printf("%s ,",m[i].tipovi[j]);
         }
	}
}





