// klient.cpp - Version pa thread (me ping manual)

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <ctime>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    sockaddr_in adresaServerit;
    adresaServerit.sin_family = AF_INET;
    adresaServerit.sin_port = htons(5000);
    adresaServerit.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if (sock == INVALID_SOCKET) {
        cout << "Gabim: Nuk mund te krijohet socket-i!\n";
        return 1;
    }

    // Set timeout per recvfrom (5 sekonda)
    int timeout = 5000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));


    time_t lastPing = time(NULL);

        int llojiKlientit;
    cout << "\n";
    cout << "========================================\n";
    cout << "     KLIENTI I RRJETAVE KOMPJUTERIKE    \n";
    cout << "========================================\n\n";
    cout << "Cfare lloj klienti jeni?\n";
    cout << "  1 - ADMIN (qasje e plote)\n";
    cout << "  2 - NORMAL (vetem lexim)\n";
    cout << "\nZgjedhja juaj: ";
    cin >> llojiKlientit;
    cin.ignore();
    
    char emriKlientit[50];
    cout << "Shkruani emrin tuaj (per identifikim, p.sh. Klient1): ";
    cin.getline(emriKlientit, 50);