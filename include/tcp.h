#ifndef TCP_H
#define TCP_H
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funciones de servidor
// =====================

struct tcp_server_t {
    struct sockaddr_in server_addr;
    int listen_sock;
};

// Crea el servidor TCP
void tcp_server_create(struct tcp_server_t *server, int port);

// Acepta una conexión TCP (crea un socket para comunicarse con el primer
// cliente en la fila)
int tcp_server_accept(struct tcp_server_t *server,
                      struct sockaddr_in *client_addr,
                      socklen_t *client_addr_len);

// Funciones de cliente
// ====================

struct tcp_client_t {
    struct sockaddr_in server_addr;
    int sock;
};

// Crea una conexión TCP con el servidor
void tcp_client_connect(struct tcp_client_t *client, const char *host,
                        int port);

// Funciones comunes
// =================

// Envía y recibe datos
void tcp_send(int sock, const void *data, size_t size);
void tcp_recv(int sock, void *data, size_t size);
void tcp_send_files(int sock, FILE *fp, size_t size);
void tcp_recv_files(int sock, FILE *data, size_t size);

// Cierra la conexión
void tcp_close(int sock);

#endif   // TCP_H
