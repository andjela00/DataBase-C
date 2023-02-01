#ifndef SLOG_H_INCLUDED
#define SLOG_H_INCLUDED
#define FBLOK 3
#define END_OF_FILE -1
typedef struct{
int minut;
int sat;
int dan;
int mesec;
int godina;
}DAT;
typedef struct {
    int sifraLeta;
    DAT datum;
    char tipAviona[7];
    int trajanjeLeta;
    int udaljenostLeta;
    char mestoDolaska[21];
    int deleted;
} SLOG;
typedef struct Blok {
    SLOG slogovi[FBLOK];
} BLOK;

typedef struct{
char mesto[21];
char tipovi[50][7];
int cnt;
}M;


#endif // SLOG_H
