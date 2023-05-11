#include "util.h"
#include <string.h>
#include <stdlib.h>

size_t file_size(FILE *fp) {
    fseek(fp, 0L, SEEK_END);
    size_t ret = ftell(fp);
    rewind(fp);

    return ret;
}

void read_file(FILE *fp, void *buf, size_t size) {
    size_t read_bytes = fread(buf, 1, size, fp);

    if (read_bytes != size) {
        fprintf(stderr, "fread failed\n");
        exit(1);
    }
}

void write_file(FILE *fp, void *buf, size_t size) {
    size_t written_bytes = fwrite(buf, 1, size, fp);

    if (written_bytes != size) {
        fprintf(stderr, "fwrite failed\n");
        exit(1);
    }
}

char* remove_enc(const char* in_file){
    char* out_file = malloc(strlen(in_file) - 4 + 1);
    strncpy(out_file, in_file, strlen(in_file) - 4);
    out_file[strlen(in_file) - 4] = '\0';
    return out_file;
}