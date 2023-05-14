#include "util.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

size_t file_size(FILE *fp) {
    fseek(fp, 0L, SEEK_END);
    size_t ret = ftell(fp);
    rewind(fp);

    return ret;
}

void read_file(FILE *fp, void *buf, size_t size) {
    size_t read_bytes = fread(buf, 1, size, fp);

    if (read_bytes != size) {
        red();
        fprintf(stderr, "fread failed\n");
        reset();
        exit(1);
    }
}

void write_file(FILE *fp, void *buf, size_t size) {
    size_t written_bytes = fwrite(buf, 1, size, fp);

    if (written_bytes != size) {
        red();
        fprintf(stderr, "fwrite failed\n");
        reset();
        exit(1);
    }
}

char *remove_enc(const char *in_file) {
    char *out_file = malloc(strlen(in_file) - 4 + 1);
    strncpy(out_file, in_file, strlen(in_file) - 4);
    out_file[strlen(in_file) - 4] = '\0';
    return out_file;
}

void print_progress(double percentage, float total_bytes, float send_bytes) {
    int i;
    printf("%.0f/%.0f Kb [", send_bytes, total_bytes);
    for (i = 0; i < 50; i++) {
        if (i < percentage * 50) {
            green();
            printf("#");
            reset();
        } else {
            printf(" ");
        }
    }
    printf("] %.2f%%\r", percentage * 100);
    fflush(stdout);
}

unsigned long fsize(FILE *fp) {
    unsigned long prev = ftell(fp);
    fseek(fp, 0L, SEEK_END);
    unsigned long sz = ftell(fp);
    fseek(fp, prev, SEEK_SET);
    return sz;
}

bool check_file(char *path) {
    FILE *fp = fopen(path, "r");
    if (fp != NULL) {
        fclose(fp);
        return true;
    } else
        return false;
}

void red() { printf("\033[1;31m"); }

void yellow() { printf("\033[1;33m"); }

void green() { printf("\033[0;32m"); }

void reset() { printf("\033[0m"); }