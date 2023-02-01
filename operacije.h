#ifndef OPERACIJE_H_INCLUDED
#define OPERACIJE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stddef.h>
#include "slog.h"


FILE *otvoriDatoteku(char *filename);
void kreirajDatoteku(char *filename);
SLOG *pronadjiSlog(FILE *fajl, int sifraLeta);
void dodajSlog(FILE *fajl, SLOG *slog);
void ispisiSveSlogove(FILE *fajl);
void ispisiSlog(SLOG *slog);
void obrisiSlogFizicki(FILE *fajl, int sifraLeta);
SLOG* zadatak2(FILE *fajl);
void zadatak4(FILE *fajl);



#endif // OPERACIJE_H_INCLUDED
