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

        // /delete
    if (strncmp(komanda, "/delete ", 8) == 0) {
        char emri[200];
        strcpy(emri, komanda + 8);
        fshijFile(emri, pergjigjja);
        *gjatesiaPergjigjes = strlen(pergjigjja);
        return;
    }

        // /search
    if (strncmp(komanda, "/search ", 8) == 0) {
        char fjala[200];
        strcpy(fjala, komanda + 8);
        kerkoFile(fjala, pergjigjja);
        *gjatesiaPergjigjes = strlen(pergjigjja);
        return;
    }
    
    // /info
    if (strncmp(komanda, "/info ", 6) == 0) {
        char emri[200];
        strcpy(emri, komanda + 6);
        infoFile(emri, pergjigjja);
        *gjatesiaPergjigjes = strlen(pergjigjja);
        return;
    }

        // /upload
    if (strncmp(komanda, "/upload ", 8) == 0) {
        char pjesa[100000];
        strcpy(pjesa, komanda + 8);
        
        int pozita = -1;
        for (int i = 0; i < (int)strlen(pjesa); i++) {
            if (pjesa[i] == '|') {
                pozita = i;
                break;
            }
        }
        
        if (pozita == -1) {
            strcpy(pergjigjja, "GABIM: Formati: /upload emri|permbajtja\n");
            *gjatesiaPergjigjes = strlen(pergjigjja);
            return;
        }

                char emri[200];
        char permbajtja[100000];
        
        for (int i = 0; i < pozita; i++) {
            emri[i] = pjesa[i];
        }
        emri[pozita] = '\0';
        
        int gjatesiaPermbajtjes = 0;
        for (int i = pozita + 1; i < (int)strlen(pjesa); i++) {
            permbajtja[gjatesiaPermbajtjes++] = pjesa[i];
        }
        
        uploadFile(emri, permbajtja, gjatesiaPermbajtjes, pergjigjja);
        *gjatesiaPergjigjes = strlen(pergjigjja);
        return;
    }

        // /download
    if (strncmp(komanda, "/download ", 10) == 0) {
        char emri[200];
        strcpy(emri, komanda + 10);
        downloadFile(emri, pergjigjja, gjatesiaPergjigjes);
        return;
    }
    
    // Komande e panjohur
    sprintf(pergjigjja, "KOMANDE E PANJOHUR: '%s'\n", komanda);
    *gjatesiaPergjigjes = strlen(pergjigjja);
}

// ==================== MAIN ====================
int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    
    cout << "\n==============================================\n";
    cout << "     SERVERI I RRJETAVE KOMPJUTERIKE          \n";
    cout << "==============================================\n";
    cout << "  UDP Port: " << PORTI_UDP << "\n";
    cout << "  HTTP Port: " << PORTI_HTTP << "\n";
    cout << "  Max Kliente: " << MAX_KLIENTE << "\n";
    cout << "  Timeout: " << TIMEOUT_SEKONDA << " sekonda\n";
    cout << "==============================================\n\n";
    
    SOCKET sockUdp = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in adresaUdp;
    adresaUdp.sin_family = AF_INET;
    adresaUdp.sin_port = htons(PORTI_UDP);
    adresaUdp.sin_addr.s_addr = INADDR_ANY;
    bind(sockUdp, (sockaddr*)&adresaUdp, sizeof(adresaUdp));
    cout << "[UDP] Serveri ka filluar ne portin " << PORTI_UDP << endl;
    
    SOCKET sockHttp = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in adresaHttp;
    adresaHttp.sin_family = AF_INET;
    adresaHttp.sin_port = htons(PORTI_HTTP);
    adresaHttp.sin_addr.s_addr = INADDR_ANY;
    bind(sockHttp, (sockaddr*)&adresaHttp, sizeof(adresaHttp));
    listen(sockHttp, 5);
    cout << "[HTTP] Serveri HTTP ne portin " << PORTI_HTTP << endl;
    cout << "[UDP] Ne pritje te klienteve...\n\n";
    
    u_long mode = 1;
    ioctlsocket(sockUdp, FIONBIO, &mode);
    ioctlsocket(sockHttp, FIONBIO, &mode);
    
    char buffer[200000];
    sockaddr_in klientiAdresa;
    int len = sizeof(klientiAdresa);
    time_t lastTimeoutCheck = time(NULL);

}
 while (true) {
        // HTTP
        SOCKET klientiHttp = accept(sockHttp, NULL, NULL);
        if (klientiHttp != INVALID_SOCKET) {
            handleHttpRequest((int)klientiHttp);
        }
        
        // UDP
        memset(buffer, 0, sizeof(buffer));
        int bytes = recvfrom(sockUdp, buffer, sizeof(buffer)-1, 0, (sockaddr*)&klientiAdresa, &len);
        
        if (bytes > 0) {
            buffer[bytes] = '\0';
            
          
            int gjatesia = (int)strlen(buffer);
            while (gjatesia > 0 && (buffer[gjatesia-1] == '\n' || buffer[gjatesia-1] == '\r')) {
                buffer[gjatesia-1] = '\0';
                gjatesia--;
            }
            
            char ip[20];
            strcpy(ip, inet_ntoa(klientiAdresa.sin_addr));
            int port = ntohs(klientiAdresa.sin_port);
            
            cout << "[PRANOVA] '" << buffer << "' nga " << ip << ":" << port << endl;
            ruajMesazhin(ip, port, buffer);
            
            // REGJISTRIMI
            if (strncmp(buffer, "REG:", 4) == 0) {
                int lloji = buffer[4] - '0';
                
                char emriKlientit[50] = "KlientPaEmer";
                if ((int)strlen(buffer) > 6) {
                    int start = 6;
                    int i = 0;
                    while (buffer[start + i] != '\0' && i < 49) {
                        emriKlientit[i] = buffer[start + i];
                        i++;
                    }
                    emriKlientit[i] = '\0';
                }
                 int idxTimeout = gjejKlientinNeTimeoutMeEmrin(emriKlientit);
                if (idxTimeout != -1) {
                    strcpy(listaKlienteve[idxTimeout].ip, ip);
                    listaKlienteve[idxTimeout].porti = port;
                    listaKlienteve[idxTimeout].kohaFundit = time(NULL);
                    listaKlienteve[idxTimeout].neTimeout = 0;
                    listaKlienteve[idxTimeout].numriKerkesave = 0;
                    
                    cout << "[RIKUPERIM] Klienti " << emriKlientit << " u rikthye nga timeout" << endl;
                    
                    char confirm[200];
                    sprintf(confirm, "OK - Rikthimi nga timeout u krye! Mir se vini perseri %s\n", emriKlientit);
                    sendto(sockUdp, confirm, strlen(confirm), 0, (sockaddr*)&klientiAdresa, len);
                    continue;
                }
                    int idx = gjejKlientinMeEmrin(emriKlientit);
                if (idx != -1) {
                    strcpy(listaKlienteve[idx].ip, ip);
                    listaKlienteve[idx].porti = port;
                    listaKlienteve[idx].kohaFundit = time(NULL);
                    
                    cout << "[RIKUPERIM] Klienti " << emriKlientit << " u rikthye" << endl;
                    
                    char confirm[200];
                    sprintf(confirm, "OK - Rikthimi u krye! Mir se vini perseri %s\n", emriKlientit);
                    sendto(sockUdp, confirm, strlen(confirm), 0, (sockaddr*)&klientiAdresa, len);
                    continue;
                }
                   if (numriKlienteve >= MAX_KLIENTE) {
                    if (numriNeRadhe < 10) {
                        strcpy(radhaPritjes[numriNeRadhe], emriKlientit);
                        numriNeRadhe++;
                        
                        char reply[200];
                        sprintf(reply, "SERVERI ESHTE I MBUSHUR! Je ne radhe. Pozita: %d\n", numriNeRadhe);
                        sendto(sockUdp, reply, strlen(reply), 0, (sockaddr*)&klientiAdresa, len);
                        cout << "[PRITJE] Klienti " << emriKlientit << " ne radhe" << endl;
                    } else {
                        char reply[] = "RADHA ESHTE E MBUSHUR! Provo me vone.\n";
                        sendto(sockUdp, reply, strlen(reply), 0, (sockaddr*)&klientiAdresa, len);
                    }
                    continue;
                }
                    Klienti k;
                strcpy(k.ip, ip);
                k.porti = port;
                strcpy(k.emri, emriKlientit);
                k.numriKerkesave = 0;
                k.kohaFundit = time(NULL);
                k.eshteAdmin = (lloji == 1) ? 1 : 0;
                k.neTimeout = 0;
                k.kohaTimeout = 0;
                
                listaKlienteve[numriKlienteve] = k;
                numriKlienteve++;
                
                cout << "[LIDHJE] Klient " << (k.eshteAdmin ? "ADMIN" : "NORMAL") 
                     << ": " << emriKlientit << " nga " << ip << endl;
                
                char confirm[200];
                sprintf(confirm, "OK - Regjistrimi u krye! Mir se vini %s\n", emriKlientit);
                sendto(sockUdp, confirm, strlen(confirm), 0, (sockaddr*)&klientiAdresa, len);
            }
            else if (strcmp(buffer, "/ping") == 0) {
               
                int idx = gjejKlientin(ip, port);
                if (idx != -1) {
                    listaKlienteve[idx].kohaFundit = time(NULL);
                }
              
            }

                
