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

    if (eshteStats && eshteJson) {
        // Formati JSON
        strcat(pergjigjja, "{\n");
        sprintf(temp, "  \"lidhje_aktive\": %d,\n", numriKlienteve);
        strcat(pergjigjja, temp);
        sprintf(temp, "  \"max_kliente\": %d,\n", MAX_KLIENTE);
        strcat(pergjigjja, temp);
        sprintf(temp, "  \"total_mesazhe\": %d,\n", totalMesazheve);
        strcat(pergjigjja, temp);
        
        strcat(pergjigjja, "  \"klientet\": [\n");
        for (int i = 0; i < numriKlienteve; i++) {
            sprintf(temp, "    {\"ip\": \"%s\", \"port\": %d, \"tipi\": \"%s\", \"emri\": \"%s\", \"kerkesa\": %d}%s\n",
                    listaKlienteve[i].ip,
                    listaKlienteve[i].porti,
                    listaKlienteve[i].eshteAdmin ? "ADMIN" : "NORMAL",
                    listaKlienteve[i].emri,
                    listaKlienteve[i].numriKerkesave,
                    (i < numriKlienteve - 1) ? "," : "");
            strcat(pergjigjja, temp);
        }
        
        strcat(pergjigjja, "  ],\n");
        strcat(pergjigjja, "  \"mesazhet_e_fundit\": [\n");
        
        int startIdx = (numriMesazheve > 10) ? numriMesazheve - 10 : 0;
        for (int i = startIdx; i < numriMesazheve; i++) {
            char timeBuf[64];
            struct tm* timeinfo = localtime(&historikuMesazheve[i].koha);
            strftime(timeBuf, sizeof(timeBuf), "%H:%M:%S", timeinfo);
            
            
            char komandaEscaped[5000];
            strcpy(komandaEscaped, historikuMesazheve[i].komanda);
            for (int j = 0; j < (int)strlen(komandaEscaped); j++) {
                if (komandaEscaped[j] == '"') {
                  
                    for (int k = strlen(komandaEscaped); k > j; k--) {
                        komandaEscaped[k] = komandaEscaped[k-1];
                    }
                    komandaEscaped[j] = '\\';
                    j++;
                }
            }
