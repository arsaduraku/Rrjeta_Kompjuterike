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
void shtypKoha(char* buffer, time_t koha) {
    struct tm* timeinfo = localtime(&koha);
    strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", timeinfo);
}

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
    int kaFile = 0;
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
    if (kaFile == 0) {
        strcat(rezultati, "  (Nuk ka asnje file ne server)\n");
    }
}

void lexoFile(char* emri, char* rezultati) {
    while (emri[0] == ' ') {
        for (int i = 0; i < (int)strlen(emri); i++) {
            emri[i] = emri[i+1];
        }
    }
    
    ifstream file(emri);
    if (!file.is_open()) {
        sprintf(rezultati, "GABIM: File-i '%s' nuk u gjet!\n", emri);
        return;
    }
    
    // Merr madhesine e file-it
    file.seekg(0, ios::end);
    int madhesia = file.tellg();
    file.seekg(0, ios::beg);
    
    // Kufizo madhesine per siguri
    if (madhesia > 50000) {
        sprintf(rezultati, "GABIM: File-i '%s' eshte shume i madh (%d bytes). Max 50000 bytes.\n", emri, madhesia);
        file.close();
        return;
    }
    
    // Lexo permbajtjen
    char* buffer = new char[madhesia + 1];
    file.read(buffer, madhesia);
    buffer[madhesia] = '\0';
    file.close();
    
    strcpy(rezultati, buffer);
    delete[] buffer;
    
    if (strlen(rezultati) == 0) {
        strcpy(rezultati, "(File-i eshte bosh)\n");
    }
}

// ==================== PËRPUNIMI I KOMANDAVE ====================
void perpunoKomanden(char* komanda, int eshteAdmin, char* pergjigjja, int* gjatesiaPergjigjes) {
    // Fillimisht pastro pergjigjjen
    pergjigjja[0] = '\0';
    *gjatesiaPergjigjes = 0;
    
    // Vonese per klientet normal
    if (eshteAdmin == 0) {
        Sleep(2000);
    }
    
    // Komanda /ping
    if (strcmp(komanda, "/ping") == 0) {
        strcpy(pergjigjja, "PONG\n");
        *gjatesiaPergjigjes = strlen(pergjigjja);
        return;
    }
    
    // Komanda /hello
    if (strcmp(komanda, "/hello") == 0) {
        strcpy(pergjigjja, "PERSHENDETJE! Lidhja me serverin funksionon.\n");
        *gjatesiaPergjigjes = strlen(pergjigjja);
        return;
    }
    
    // Komanda /list
    if (strcmp(komanda, "/list") == 0) {
        listoFilet(pergjigjja);
        *gjatesiaPergjigjes = strlen(pergjigjja);
        return;
    }
    
    // Komanda /read
    if (strncmp(komanda, "/read ", 6) == 0) {
        char emri[200];
        strcpy(emri, komanda + 6);
        lexoFile(emri, pergjigjja);
        *gjatesiaPergjigjes = strlen(pergjigjja);
        return;
    }

        // Komandat e adminit - kontrollo te drejtat
    if (eshteAdmin == 0) {
        sprintf(pergjigjja, "GABIM: Nuk keni te drejta per kete komande! Vetem admin.\n");
        *gjatesiaPergjigjes = strlen(pergjigjja);
        return;
    }

