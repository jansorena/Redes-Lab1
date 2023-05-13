#include "tcp.h"
#include "md5.h"
#include "util.h"
#include "decrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define SIZE 1024
#define MD5_LEN 32

int main(void){
    // Struct para guardar el server y creacion de el
    struct tcp_server_t server;
    tcp_server_create(&server, 10000);

    while(1){
        // Struct para guardar el cliente y tamaño de el
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        // Socket para esperar las conexiones
        int sock = tcp_server_accept(&server, &client_addr, &client_addr_len);

        printf("Cliente conectado con direccion IP: %s\n\n", inet_ntoa(client_addr.sin_addr));

        // Enviar respuesta de conexion
        tcp_send(sock,"Conectado correctamente con el servidor\n", SIZE);

        // Recibir nombre archivo del cliente
        char buffer_filename[SIZE];
        tcp_recv(sock, &buffer_filename, SIZE);

        // Recibir tamaño archivo del cliente
        unsigned long buffer_filesize;
        tcp_recv(sock, &buffer_filesize, SIZE);
        buffer_filesize = ntohl(buffer_filesize);

        // Recibir hash (md5sum) del cliente
        char md5[MD5_LEN+1];
        tcp_recv(sock, &md5, MD5_LEN+1);

        // Imprimir informacion del archivo a recibir
        printf("Archivo: %s    [%.2f Kb]\n", buffer_filename, (float)buffer_filesize/1024);
        printf("MD5 sum: %s\n\n",md5);
        
        char buffer_filename_2[SIZE] = "../";
        strcat(buffer_filename_2,buffer_filename);

        // Crear archivo a recibir en el puntero fp
        FILE *fp;
        if((fp = fopen(buffer_filename_2, "wb")) == NULL){
            red();
            perror("No se pudo crear el archivo");
            reset();
            exit(1);
        }

        // Recibir el archivo encriptado y cerrarlo
        tcp_recv_files(sock,fp,buffer_filesize);
        fclose(fp);

        // Desencriptacion del archivo
        char path_key[SIZE]; 
        printf("\nIngrese la direccion de la llave: ");
        scanf("%s", path_key);
        while(!check_file(path_key)){
            red();
            printf("Llave no encontrada. Intente nuevamente: ");
            reset();
            scanf("%s", path_key);
        }
        printf("\nDesencriptando el archivo ...\n");
        f_decrypt(path_key, buffer_filename_2);
        
        // Comparacion de ambos hash (md5sum)
        printf("Comparando MD5 sum ...\n");
        char md5_cal[MD5_LEN+1];
        char *file_decrypt = remove_enc(buffer_filename_2);
        if(!CalcFileMD5(file_decrypt, md5_cal)){
            red();
            printf("No se pudo calcular MD5 sum del archivo");
            reset();
            exit(1);
        }
        if(!strcmp(md5, md5_cal)){
            green();
            printf("Comparacion exitosa: %s\n\n",md5_cal);
            reset();
        } 
        else{
            red();
            printf("CUIDADO! MD5 sum es distinto\n\n");
            reset();
        }
        // Borrar archivo encriptado

        remove(buffer_filename_2);


        // Cierre del socket y continua esperando conexiones
        tcp_close(sock);
        free(file_decrypt);


        yellow();
        printf("\nEscuchando conexiones entrantes ...\n");
        reset();
    }
    
    return 0;
}
