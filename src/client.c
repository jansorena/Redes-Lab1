#include "tcp.h"
#include "md5.h"
#include "util.h"
#include "encrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>

#define SIZE 1024
#define MD5_LEN 32

int main(void){
    char path_file[SIZE];
    char path_key[SIZE];
    char md5[MD5_LEN + 1]; 

    // Archivo a enviar
    printf("Ingrese la direccion del archivo: ");
    scanf("%s",path_file);
    while(!check_file(path_file)){
        red();
        printf("Archivo no encontrado. Intente nuevamente: ");
        reset();
        scanf("%s",path_file);
    }
    
    // Calculo del hash (md5sum)
    printf("\nCalculando MD5 sum ...\n");
    if (!CalcFileMD5(path_file, md5)){
        red();
        printf("No se pudo calcular MD5 sum del archivo");
        reset();
        exit(1);
    }else{
        green();
        printf("Calculo MD5 sum exitoso!\n\n");
        reset();
    }

    // Llave para encriptar el archivo
    printf("Ingrese la direccion de la llave: ");
    scanf("%s",path_key);
    while(!check_file(path_key)){
        red();
        printf("Llave no encontrada. Intente nuevamente: ");
        scanf("%s",path_key);
        reset();
    }

    // Encriptacion
    printf("\nEncriptando el archivo ...\n");
    f_encrypt(path_key,path_file);

    // Verificar archivo encriptado y guardarlo en el puntero fp
    strcat(path_file,".enc");
    check_file(path_file);
    FILE *fp = fopen(path_file, "r");

    // Struct para guardar la informacion del server
    struct tcp_client_t server;

    // Direccion del server
    char server_dir[SIZE];
    printf("\nIngrese la direccion del servidor: ");
    scanf("%s",server_dir);

    // Conexion con el server
    tcp_client_connect(&server, server_dir, 10000);

    // Respuesta de conexion del servidor
    char buffer[SIZE];
    tcp_recv(server.sock, buffer, SIZE);
    green();
    printf("\n%s\n",buffer);
    reset();

    // Envio del nombre del archivo
    char *buffer_filename = basename(path_file);
    tcp_send(server.sock,buffer_filename,strlen(buffer_filename) + 1);

    // Envio del tamaño del archivo
    int buffer_filesize = htonl(fsize(fp));
    tcp_send(server.sock,&buffer_filesize,sizeof(buffer_filesize));

    // Envio del hash (md5sum)
    tcp_send(server.sock,md5,MD5_LEN + 1);

    // Envio del archivo encriptado y cierre de la lectura
    tcp_send_files(server.sock,fp,fsize(fp));
    fclose(fp);
    
    // Cierre del socket
    tcp_close(server.sock);

    return 0;
}
