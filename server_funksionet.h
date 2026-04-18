// server_funksionet.h 

#define SERVER_FUNKSIONET_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <ctime>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

// Konstatat
extern int PORTI_UDP;
extern int PORTI_HTTP;
extern int MAX_KLIENTE;
extern int TIMEOUT_SEKONDA;
extern int TIMEOUT_RRETH_PRITJE; 

// Struktura per klient
struct Klienti {
    char ip[20];
    int porti;
    time_t kohaFundit;
    int eshteAdmin;
};

struct Mesazhi {
    char ip[20];
    int port;
    char komanda[5000];
    time_t koha;
};

// Variablat globale (deklarata)
extern Klienti listaKlienteve[10];
extern int numriKlienteve;
extern Mesazhi historikuMesazheve[1000];
extern int numriMesazheve;
extern int totalMesazheve;

// Funksionet e perbashketa
int gjejKlientin(char* ip, int porti);
int gjejKlientinMeEmrin(char* emri);
int gjejKlientinNeTimeoutMeEmrin(char* emri);
void listoFilet(char* rezultati);
void lexoFile(char* emri, char* rezultati);
void kontrolloTimeout();
void ruajMesazhin(const char* ip, int port, const char* komanda);
void shtypKoha(char* buffer, time_t koha);

void handleHttpRequest(int klientiHttp);