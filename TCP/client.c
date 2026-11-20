#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define BUFFER_SIZE 256

int main(void) {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in server_addr;
    char server_ip[INET_ADDRSTRLEN];
    int port = 0;
    char buffer[BUFFER_SIZE];


    printf("Inserisci l'IP del server: ");
    fflush(stdout);
    if (scanf("%45s", server_ip) != 1) {
        printf("Errore lettura IP.\n");
        return 1;
    }

    printf("Inserisci la porta del server: ");
    fflush(stdout);
    if (scanf("%d", &port) != 1 || port <= 0 || port > 65535) {
        printf("Porta non valida.\n");
        return 1;
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Errore in WSAStartup: %d\n", WSAGetLastError());
        return 1;
    }


    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("Errore creazione socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons((uint16_t)port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) != 1) {
        printf("Indirizzo IP non valido: %s\n", server_ip);
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Errore di connessione al server %s:%d (%d)\n", server_ip, port, WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }


    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0) {
        printf("Errore o chiusura inaspettata durante la ricezione iniziale.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    buffer[bytes_received] = '\0';
    printf("%s\n", buffer);


    while (1) {
        long n1 = 0, n2 = 0, n3 = 0;
        printf("Inserisci tre interi (n1 n2 n3): ");
        fflush(stdout);
        if (scanf("%ld %ld %ld", &n1, &n2, &n3) != 3) {
            printf("Input non valido.\n");
            break;
        }

        char msg_out[BUFFER_SIZE];
        snprintf(msg_out, sizeof(msg_out), "%ld %ld %ld", n1, n2, n3);

        if (send(sock, msg_out, (int)strlen(msg_out), 0) == SOCKET_ERROR) {
            printf("Errore nell'invio dei dati: %d\n", WSAGetLastError());
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            printf("Connessione chiusa dal server.\n");
            break;
        }
        buffer[bytes_received] = '\0';

        printf("%s\n", buffer);

        if (strcmp(buffer, "QUIT") == 0) {
            break;
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
