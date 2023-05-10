# Código de base para el laboratorio evaluado 1

En esta carpeta van a encontrar un esqueleto de proyecto para el laboratorio evaluado 1. El proyecto está escrito en C y usa CMake para la compilación. El código una vez compilado genera dos ejecutables:

- `server`: Servidor que escucha en el puerto 10000 y espera conexiones de clientes.
- `client`: Cliente que se conecta al servidor y envía un mensaje.

Los archivos del servidor y cliente (`src/client.c` y `src/server.c`) tienen algo de código de ejemplo para que puedan ver cómo se podrían conectar para enviar y recibir mensajes. Ambos archivos ocupan funciones definidas en `src/tcp.c` para conectarse y enviar datos. Este archivo contiene algunas funciones definidas pero sin código, el que deberán rellenar para completar el proyecto, además de implementar la funcionalidad de transferencia de archivos en el código de servidor y cliente.

## Instalación y compilación

Para compilar este proyecto, basta con seguir los pasos habituales para un proyecto con CMake:

```console
$ mkdir build
$ cd build
$ cmake ..
$ make
```

A medida que le vayan haciendo cambios al código, pueden volver a compilar el proyecto ejecutando sólo `make`. Si necesitan modificar `CMakeLists.txt` (ya sea para agregar un archivo o cambiar el lenguaje), deben volver a ejecutar `cmake ..` para que se actualicen los archivos de configuración de CMake.
