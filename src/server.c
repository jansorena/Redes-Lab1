#include "tcp.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define SIZE 1024

int main(void) {
    struct tcp_server_t server;
    tcp_server_create(&server, 10000);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int sock = tcp_server_accept(&server, &client_addr, &client_addr_len);

        printf("Cliente conectado con direccion IP: %s\n",inet_ntoa(client_addr.sin_addr));

        tcp_send(sock, "Conectado correctamente con el servidor\n",1024);

        char buffer_filename[SIZE];
        tcp_recv(sock, buffer_filename, SIZE);
        printf("%s\n",buffer_filename);

        int buffer_filesize;
        tcp_recv(sock, &buffer_filesize, SIZE);
        
        buffer_filesize = ntohl(buffer_filesize);

        printf("Archivo: %s [%d bytes]\n",buffer_filename,buffer_filesize);

        FILE *fp;
        if ((fp = fopen(buffer_filename, "wb")) == NULL) {
            perror("No se pudo crear el archivo");
            exit(1);
        }

        tcp_recv_files(sock,fp,buffer_filesize);
        fclose(fp);

        tcp_close(sock);
        printf("Escuchando conexiones entrantes ...\n");
    }

    return 0;
}
