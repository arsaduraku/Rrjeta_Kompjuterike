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