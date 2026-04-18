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

        time_t lastPing = time(NULL);
    
    if (llojiKlientit == 1) {
        cout << "\n========== KLIENTI ADMIN ==========\n";
        cout << "Komandat:\n";
        cout << "  /list                           - Listo te gjithe filet\n";
        cout << "  /read <filename>                - Lexo permbajtjen e nje file-i\n";
        cout << "  /delete <filename>              - Fshij nje file\n";
        cout << "  /search <keyword>               - Kerko file nga emri\n";
        cout << "  /info <filename>                - Info per file (madh. data)\n";
        cout << "  /upload <filename>|<content>    - Dergo file ne server\n";
        cout << "  /download <filename>            - Shkarko file nga serveri\n";
        cout << "  /hello                          - Test lidhjeje\n";
        cout << "  /exit                           - Dil\n";
        cout << "===================================\n\n";
        cout << "Shembull upload: /upload test.txt|Pershendetje bote!\n";
        cout << "Shembull download: /download test.txt\n\n";
    } else {
        cout << "\n========== KLIENTI NORMAL ==========\n";
        cout << "Komandat:\n";
        cout << "  /list              - Listo te gjithe filet\n";
        cout << "  /read <filename>   - Lexo permbajtjen e nje file-i\n";
        cout << "  /hello             - Test lidhjeje\n";
        cout << "  /exit              - Dil\n";
        cout << "====================================\n\n";
        cout << "SHENIM: Klientet normal kane vonese 2 sekonda per pergjigje\n\n";
    }
    
    char buffer[200000];
    sockaddr_in adresaPergjigjes;
    int gjatesiaPergjigjes = sizeof(adresaPergjigjes);

        while (true) {
        if (llojiKlientit == 1) {
            cout << "admin> ";
        } else {
            cout << "normal> ";
        }
        
        char komanda[5000];
        cin.getline(komanda, 5000);
        
        if (strcmp(komanda, "/exit") == 0) {
            break;
        }
        
        if (strlen(komanda) == 0) {
            continue;
        }
        
        // Dergo ping nese ka kaluar 20 sekonda pa aktivitet
        time_t tani = time(NULL);
        if (tani - lastPing >= 20) {
            sendto(sock, "/ping", 5, 0, (sockaddr*)&adresaServerit, sizeof(adresaServerit));
            lastPing = tani;
        }
        
        sendto(sock, komanda, strlen(komanda), 0,
               (sockaddr*)&adresaServerit, sizeof(adresaServerit));
        
        int numri = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                             (sockaddr*)&adresaPergjigjes, &gjatesiaPergjigjes);
        
        if (numri > 0) {
            buffer[numri] = '\0';
            cout << buffer << "\n";
        } else {
            cout << "Gabim: Nuk u mor pergjigje nga serveri (timeout)! Kontrollo lidhjen.\n";
        }
    }

        closesocket(sock);
    WSACleanup();
    return 0;
}