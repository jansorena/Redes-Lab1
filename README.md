# Laboratorio Evaluado 1

## Descripción

Este proyecto es una implementación en lenguaje C de un servidor y un cliente que se comunican a través de sockets TCP para transferir archivos. El servidor escucha en el puerto 10000 y espera conexiones de clientes. Una vez establecida la conexión, el cliente envía un archivo al servidor, quien guarda el archivo.

El proyecto esta divido en los siguientes archivos:

- `server.c`: Implementa el servidor que escucha en el puerto 10000 y espera conexiones de clientes.
- `client.c`: Implementa el cliente que se conecta al servidor y envía un archivo.
- `tcp.c`: Implementa las funciones de creación del socket, bindear al puerto respectivo, escucha del servidor, conexión entre cliente y servidor, ademas del envío y recepción de datos a través de sockets TCP.
- `md5.c`: Implementa la función de hash MD5 para calcular el hash de un archivo.
- `keygen.c`: Genera una llave secreta para el cifrado y descifrado.
- `encrypt.c`: Implementa la función de cifrado de un archivo con un algoritmo simétrico.
- `decrypt.c`: Implementa la función de descifrado de un archivo cifrado con un algoritmo simétrico.
- `crypto.c`: Implementa las funciones de cifrado y descifrado en secuencia de un archivo a partir de una llave.
- `util.c`: Implementa funciones útiles como obtener el tamaño del fichero, lectura y escritura de archivos e impresión de la barra de progreso.

## Instalación y compilación

Para compilar este proyecto, se requiere tener instalado `CMake`, `GCC` y la biblioteca `libsodium`. Los pasos para compilar son los siguientes:

1. Clonar el repositorio:
   
   ```bash
   git clone https://github.com/jansorena/Redes-Lab1.git
   ```

2. Crear un directorio `build`:
   
   ```bash
   mkdir build
   cd build
   ```

3. Ejecutar CMake para generar los archivos Makefile:
   
   ```bash
   cmake ..
   ```

4. Compilar el proyecto con el comando `make`:
   
   ```bash
   make
   ```

## Uso

### Llave

Previo a la ejecución del programa, se requiere una llave para poder realizar un cifrado simétrico del archivo a enviar. Esta se puede generar ejecutando:

```bash
./keygen llave.key
```

y debe ser compartida por un canal seguro entre el cliente y el servidor.

### Servidor

Para iniciar el servidor, ejecutar el archivo `server` compilado en el directorio `build`:

```bash
./server
```

El servidor escuchará en el puerto 10000 y esperará conexiones de clientes. Una vez recibido el archivo, el programa solicitara la ruta de la llave.

### Cliente

Para enviar un archivo al servidor, ejecutar el archivo `client` compilado en el directorio `build`:

```bash
./client
```

Luego, el programa pedirá la ruta del archivo a enviar, la ruta de la llave y la dirección del servidor.

## Demostración
https://youtu.be/C4i9JWzq3xs
