#include "tcp.h"
#include "util.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void tcp_server_create(struct tcp_server_t *server, int port) {
    // Crear socket de escucha (se guarda en server->listen_sock)
    server->listen_sock = socket(
        AF_INET, SOCK_STREAM, 0);   // parametros: ipv4,socket tco, api layer 0
    if (server->listen_sock < 0) {
        red();
        perror("No se pudo crear el socket.\n");
        reset();
        exit(1);
    } else {
        green();
        printf("TCP server socket creado correctamente.\n");
        reset();
    }

    memset(
        &server->server_addr, 0,
        sizeof(server->server_addr));   // inicializar la direccion del server
    server->server_addr.sin_family = AF_INET;     // usar ipv4
    server->server_addr.sin_port = htons(port);   // puerto definido (10000)
    server->server_addr.sin_addr.s_addr =
        htonl(INADDR_ANY);   // acepta conexiones de cualquier IP

    // Bindear socket a puerto
    if (bind(server->listen_sock, (struct sockaddr *) &server->server_addr,
             sizeof(server->server_addr)) < 0) {
        red();
        perror("Error al bindear el socker a puerto.\n");
        reset();
        exit(1);
    }

    // Escuchar conexiones entrantes
    int wait_size = 16;   // maximo numero de clientes esperando
    if (listen(server->listen_sock, wait_size) < 0) {
        red();
        perror("No se puede escuchar conexiones entrantes.\n");
        reset();
        exit(1);
    } else {
        yellow();
        printf("Escuchando conexiones entrantes ...\n");
        reset();
        printf("\n");
    }
}

int tcp_server_accept(struct tcp_server_t *server,
                      struct sockaddr_in *client_addr,
                      socklen_t *client_addr_len) {
    // Aceptar primera conexion entrante (guardar dirección del cliente en
    // client_addr)
    int client_sock = accept(server->listen_sock,
                             (struct sockaddr *) client_addr, client_addr_len);
    // Retorna descriptor de archivo del socket de comunicación con el cliente
    if (client_sock < 0) {
        red();
        perror("No se puede abrir un socket\n");
        reset();
        exit(1);
    } else {
        return client_sock;
    }
}

void tcp_client_connect(struct tcp_client_t *client, const char *host,
                        int port) {
    // Crear socket de cliente (se guarda en client->sock)
    client->sock = socket(PF_INET, SOCK_STREAM, 0);
    if (client->sock < 0) {
        red();
        perror("No se pudo crear el socket.\n");
        reset();
        exit(1);
    }
    // Conectar con host y puerto indicados (se guarda en client->server_addr y
    // se usa en llamada a connect())
    memset(&client->server_addr, 0, sizeof(client->server_addr));
    client->server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, host, &client->server_addr.sin_addr);
    client->server_addr.sin_port = htons(port);

    if (connect(client->sock, (struct sockaddr *) &client->server_addr,
                sizeof(client->server_addr)) < 0) {
        red();
        perror("No se pudo conectar al servidor.\n");
        reset();
        exit(1);
    }
}

void tcp_send(int sock, const void *data, size_t size) {
    if (send(sock, data, size, 0) < 0) {
        red();
        perror("No se puede enviar el mensaje.\n");
        reset();
        exit(1);
    }
}

void tcp_recv(int sock, void *data, size_t size) {
    if (recv(sock, data, size, 0) < 0) {
        red();
        perror("No se puede recibir el mensaje\n");
        reset();
        exit(1);
    }
}

void tcp_send_files(int sock, FILE *fp, size_t size) {
    char buffer[1024];
    memset(buffer, 0, 1024);
    unsigned long bytes_sent = 0;
    unsigned long total_bytes_sent = 0;
    unsigned long counter = 0;

    while (total_bytes_sent < size) {
        bytes_sent = fread(buffer, sizeof(char), 1024, fp);
        if (send(sock, buffer, bytes_sent, 0) < 0) {
            red();
            perror("No se pudo enviar el archivo.\n");
            reset();
            exit(1);
        }
        memset(buffer, 0, 1024);
        total_bytes_sent += bytes_sent;
        counter++;
        if (counter % 100 == 0) {
            double percen = ((double) total_bytes_sent / (double) size);
            print_progress(percen, (float) size / 1024,
                           (float) total_bytes_sent / 1024);
        }
    }
    // Verificacion del 100%
    print_progress((double) total_bytes_sent / (double) size,
                   (float) size / 1024, (float) total_bytes_sent / 1024);
    green();
    printf("\n\nArchivo enviado correctamente.\n\n");
    reset();
}

void tcp_recv_files(int sock, FILE *fp, size_t size) {
    char buffer[1024];
    memset(buffer, 0, 1024);
    unsigned long bytes_received = 0;
    unsigned long total_bytes_received = 0;
    unsigned long counter = 0;

    while (total_bytes_received < size) {
        if ((bytes_received = recv(sock, buffer, 1024, 0)) < 0) {
            red();
            perror("No se pudo recibir el archivo.\n");
            reset();
            exit(1);
        }
        fwrite(buffer, sizeof(char), bytes_received, fp);
        memset(buffer, 0, 1024);
        total_bytes_received += bytes_received;
        counter++;
        if (counter % 100 == 0) {
            double percen = ((double) total_bytes_received / (double) size);
            print_progress(percen, (float) size / 1024,
                           (float) total_bytes_received / 1024);
        }
    }
    // Verificacion del 100%
    print_progress((double) total_bytes_received / (double) size,
                   (float) size / 1024, (float) total_bytes_received / 1024);
    green();
    printf("\n\nArchivo recibido correctamente.\n");
    reset();
}

void tcp_close(int sock) {
    close(sock);
    printf("Conexion cerrada.\n");
}
