// server.cpp

#include "server_funksionet.h"

// ==================== KONSTANTAT ====================
int PORTI_UDP = 5000;
int PORTI_HTTP = 8080;
int MAX_KLIENTE = 4;
int TIMEOUT_SEKONDA = 30;

// ==================== LISTAT ====================
Klienti listaKlienteve[10];
int numriKlienteve = 0;

// Radha e pritjes per klientet
static char radhaPritjes[10][50];
static int numriNeRadhe = 0;

// ==================== FUNKSIONET ====================

int gjejKlientin(char* ip, int porti) {
    for (int i = 0; i < numriKlienteve; i++) {
        if (strcmp(listaKlienteve[i].ip, ip) == 0 && listaKlienteve[i].porti == porti) {
            return i;
        }
    }
    return -1;
}

void listoFilet(char* rezultati) {
    strcpy(rezultati, "========== FILE-T NE SERVER ==========\n");
    system("dir /b > temp_lista.txt");
    
    ifstream file("temp_lista.txt");
    char rreshti[200];
    while (file.getline(rreshti, 200)) {
        if (strcmp(rreshti, "temp_lista.txt") != 0 &&
            strcmp(rreshti, "server.exe") != 0 &&
            strcmp(rreshti, "klient.exe") != 0) {
            strcat(rezultati, "  ");
            strcat(rezultati, rreshti);
            strcat(rezultati, "\n");
        }
    }
    file.close();
    system("del temp_lista.txt");
}

void lexoFile(char* emri, char* rezultati) {
    while (emri[0] == ' ') {
        for (int i = 0; i < strlen(emri); i++) {
            emri[i] = emri[i+1];
        }
    }
    
    ifstream file(emri);
    if (!file.is_open()) {
        sprintf(rezultati, "GABIM: File-i '%s' nuk u gjet!\n", emri);
        return;
    }
    
    rezultati[0] = '\0';
    char rreshti[1000];
    while (file.getline(rreshti, 1000)) {
        strcat(rezultati, rreshti);
        strcat(rezultati, "\n");
    }
    file.close();
    
    if (strlen(rezultati) == 0) {
        strcpy(rezultati, "File-i eshte bosh.\n");
    }
}