#include "server_funksionet.h"

void handleHttpRequest(int klientiHttp) {
    char httpBuffer[1024];
    recv(klientiHttp, httpBuffer, sizeof(httpBuffer)-1, 0);
    
    int eshteStats = 0;
    int eshteJson = 0;
    
    if (strstr(httpBuffer, "GET /stats") != NULL) {
        eshteStats = 1;
        if (strstr(httpBuffer, "GET /stats/json") != NULL) {
            eshteJson = 1;
        }
    }
    
    char pergjigjja[100000];
    pergjigjja[0] = '\0';
    char temp[10000];