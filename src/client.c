#include "tcp.h"
#include "encrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>

#define SIZE 1024

int main(void){
    char path_file[SIZE], path_key[SIZE]; 
    
    printf("Ingrese la direccion del archivo: ");
    scanf("%s",path_file); //faltaria check para buffer overflow?
    while(!check_file(path_file)){
        printf("Archivo no encontrado. Intente nuevamente: ");
        scanf("%s",path_file);
    }
    
    printf("Ingrese la direccion de la llave: ");
    scanf("%s",path_key); //faltaria check para buffer overflow?
    while(!check_file(path_key)){
        printf("Llave no encontrada. Intente nuevamente: ");
        scanf("%s",path_key);
    }

    printf("\nEncriptando el archivo ...\n");
    f_encrypt(path_key,path_file);

    strcat(path_file,".enc");
    
    check_file(path_file);
    FILE *fp = fopen(path_file, "r");

    struct tcp_client_t server;

    char server_dir[SIZE];

    printf("\nIngrese la direccion del servidor: ");
    scanf("%s",server_dir); //faltaria check para buffer overflow?
    tcp_client_connect(&server, server_dir, 10000); // conexion con el server

    char buffer[SIZE];
    tcp_recv(server.sock, buffer, SIZE); // respuesta de la conexion inicial
    printf("\n%s\n",buffer);

    char *buffer_filename = basename(path_file);
    tcp_send(server.sock,buffer_filename,strlen(buffer_filename) + 1);

    int buffer_filesize = htonl(fsize(fp));
    tcp_send(server.sock,&buffer_filesize,sizeof(buffer_filesize));

    tcp_send_files(server.sock,fp,fsize(fp));
    fclose(fp);
    
    tcp_close(server.sock);
    return 0;
}
