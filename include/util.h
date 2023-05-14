#ifndef UTIL_H
#define UTIL_H
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

size_t file_size(FILE *fp);
void read_file(FILE *fp, void *buf, size_t size);
void write_file(FILE *fp, void *buf, size_t size);

// Funciones adicionales
char *remove_enc(const char *in_file);
void print_progress(double percentage, float total_bytes, float send_bytes);
size_t fsize(FILE *fp);
bool check_file(char *path);

// Colores
void red();
void yellow();
void green();
void reset();

#endif   // UTIL_H
