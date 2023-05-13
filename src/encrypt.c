#include <stdio.h>
#include <string.h>
#include "crypto.h"
#include "util.h"
#include "encrypt.h"

void f_encrypt(const char* key_file, const char* in_file) {
    char* out_file = malloc(strlen(in_file) + 5);
    strcpy(out_file, in_file);
    strcat(out_file, ".enc");

    if (sodium_init() != 0) {
        red();
        perror("could not initialize cryptography\n");
        reset();
        exit(1);
    }

    unsigned char key[crypto_secretbox_KEYBYTES];
    open_key(key_file, key);

    FILE* f_in;
    if ((f_in = fopen(in_file, "rb")) == NULL) {
        red();
        perror("could not open input file\n");
        reset();
        exit(1);
    }

    size_t input_len = file_size(f_in);
    unsigned char* input_data = malloc(input_len);

    read_file(f_in, input_data, input_len);
    fclose(f_in);

    struct encrypted_t enc;
    encrypt(key, &enc, input_data, input_len);

    FILE* f_out;
    if ((f_out = fopen(out_file, "wb")) == NULL) {
        red();
        perror("could not open output file\n");
        reset();
        exit(1);
    }

    write_file(f_out, enc.nonce, sizeof enc.nonce);
    write_file(f_out, enc.msg, enc.len);
    fclose(f_out);
    free(out_file);
    green();
    printf("Encriptado exitoso\n");
    reset();
}
