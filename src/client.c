#include "tcp.h"
#include "encrypt.h"
#include "md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>

#define SIZE 1024
#define MD5_LEN 32

int main(void){
    char path_file[SIZE], path_key[SIZE], md5[MD5_LEN + 1]; 

    printf("Ingrese la direccion del archivo: ");
    scanf("%s",path_file);
    while(!check_file(path_file)){
        printf("Archivo no encontrado. Intente nuevamente: ");
        scanf("%s",path_file);
    }
    
    printf("\nCalculando MD5 sum ...\n");
    if (!CalcFileMD5(path_file, md5)) {
        printf("No se pudo calcular MD5 sum del archivo");
        exit(1);
    }
    printf("Calculo MD5 sum exitoso!\n\n");

    printf("Ingrese la direccion de la llave: ");
    scanf("%s",path_key);
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
    scanf("%s",server_dir);
    tcp_client_connect(&server, server_dir, 10000);

    char buffer[SIZE];
    tcp_recv(server.sock, buffer, SIZE);
    printf("\n%s\n",buffer);

    char *buffer_filename = basename(path_file);
    tcp_send(server.sock,buffer_filename,strlen(buffer_filename) + 1);

    int buffer_filesize = htonl(fsize(fp));
    tcp_send(server.sock,&buffer_filesize,sizeof(buffer_filesize));

    tcp_send(server.sock,md5,MD5_LEN + 1);

    tcp_send_files(server.sock,fp,fsize(fp));
    fclose(fp);
    
    tcp_close(server.sock);
    return 0;
}
