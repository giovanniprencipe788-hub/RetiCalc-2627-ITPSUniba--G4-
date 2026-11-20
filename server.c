#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    WSADATA wsaData;
    SOCKET server_fd = INVALID_SOCKET;
    struct sockaddr_in server_addr;
    int port = DEFAULT_PORT;

    if (argc >= 2) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            printf("Porta non valida. Uso la porta predefinita %d.\n", DEFAULT_PORT);
            port = DEFAULT_PORT;
        }
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Errore in WSAStartup: %d\n", WSAGetLastError());
        return 1;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        printf("Errore creazione socket server: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons((uint16_t)port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Errore bind sulla porta %d: %d\n", port, WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        printf("Errore listen: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Server TCP in ascolto sulla porta %d...\n", port);
    fflush(stdout);


    while (1) {
        struct sockaddr_in client_addr;
        int client_addr_len = sizeof(client_addr);
        SOCKET client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd == INVALID_SOCKET) {
            printf("Errore accept: %d\n", WSAGetLastError());
            continue;
        }


        char client_ip[INET_ADDRSTRLEN] = "sconosciuto";
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        printf("connesso con il client il cui nome IP: %s\n", client_ip);


        const char *msg_conn = "connessione avvenuta";
        send(client_fd, msg_conn, (int)strlen(msg_conn), 0);


        while (1) {
            char buffer[BUFFER_SIZE];
            memset(buffer, 0, BUFFER_SIZE);

            int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_received <= 0) {
                printf("Client %s disconnesso.\n", client_ip);
                break;
            }
            buffer[bytes_received] = '\0';

            long n1 = 0, n2 = 0, n3 = 0;
            if (sscanf(buffer, "%ld %ld %ld", &n1, &n2, &n3) != 3) {
                printf("Dati non validi ricevuti da %s: %s\n", client_ip, buffer);
                break;
            }


            long res = n1 + n2 - n3;
            printf("la somma di %ld+%ld-%ld=%ld\n", n1, n2, n3, res);
            fflush(stdout);


            char risposta[BUFFER_SIZE];
            if (res == 100) {
                snprintf(risposta, sizeof(risposta), "QUIT");
            } else {
                snprintf(risposta, sizeof(risposta), "%ld", res);
            }

            if (send(client_fd, risposta, (int)strlen(risposta), 0) == SOCKET_ERROR) {
                printf("Errore invio risposta a %s: %d\n", client_ip, WSAGetLastError());
                break;
            }

            if (res == 100) {
                break;
            }
        }

        closesocket(client_fd);
    }

    closesocket(server_fd);
    WSACleanup();
    return 0;
}
