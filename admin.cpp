#include "server_funksionet.h"
#include <sys/stat.h>

void fshijFile(char* emri, char* rezultati) {
    while (emri[0] == ' ') {
        for (int i = 0; i < (int)strlen(emri); i++) {
            emri[i] = emri[i+1];
        }
    }
    
    if (strcmp(emri, "server.exe") == 0 || strcmp(emri, "klient.exe") == 0) {
        sprintf(rezultati, "GABIM: Nuk mund te fshihet file-i i sistemit '%s'!\n", emri);
        return;
    }
    
    if (remove(emri) == 0) {
        sprintf(rezultati, "SUKSES: File-i '%s' u fshi!\n", emri);
    } else {
        sprintf(rezultati, "GABIM: Nuk mund te fshihet '%s'\n", emri);
    }
}

void kerkoFile(char* fjala, char* rezultati) {
    while (fjala[0] == ' ') {
        for (int i = 0; i < (int)strlen(fjala); i++) {
            fjala[i] = fjala[i+1];
        }
    }
    
    sprintf(rezultati, "========== REZULTATET E KERKIMIT ('%s') ==========\n", fjala);
    int gjetur = 0;
    
    system("dir /b > temp_kerko.txt");
    ifstream file("temp_kerko.txt");
    char rreshti[200];
    while (file.getline(rreshti, 200)) {
        if (strstr(rreshti, fjala) != NULL) {
            strcat(rezultati, "  ");
            strcat(rezultati, rreshti);
            strcat(rezultati, "\n");
            gjetur = 1;
        }
    }
    file.close();
    system("del temp_kerko.txt");
    
    if (gjetur == 0) {
        strcat(rezultati, "  Nuk u gjet asnje file.\n");
    }
}

void infoFile(char* emri, char* rezultati) {
    while (emri[0] == ' ') {
        for (int i = 0; i < (int)strlen(emri); i++) {
            emri[i] = emri[i+1];
        }
    }
    
    struct _stat fileStat;
    if (_stat(emri, &fileStat) != 0) {
        sprintf(rezultati, "GABIM: File-i '%s' nuk u gjet!\n", emri);
        return;
    }
    
    char kohaKrijimit[64];
    char kohaModifikimit[64];
    
    shtypKoha(kohaKrijimit, fileStat.st_ctime);
    shtypKoha(kohaModifikimit, fileStat.st_mtime);
    
    sprintf(rezultati, 
            "========== INFO PER FILE: %s ==========\n"
            "  Madhesia: %lld bytes\n"
            "  Krijuar: %s\n"
            "  Modifikuar: %s\n"
            "========================================\n",
            emri, 
            (long long)fileStat.st_size,
            kohaKrijimit,
            kohaModifikimit);
}