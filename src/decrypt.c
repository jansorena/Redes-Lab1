#include <stdio.h>
#include <string.h>
#include "decrypt.h"
#include "crypto.h"
#include "util.h"

void f_decrypt(const char* key_file, const char* in_file) {
    char* out_file = remove_enc(in_file);

    if (sodium_init() != 0) {
        perror("could not initialize cryptography\n");
        exit(1);
    }

    unsigned char key[crypto_secretbox_KEYBYTES];
    open_key(key_file, key);

    FILE* f_in;
    if ((f_in = fopen(in_file, "rb")) == NULL) {
        perror("could not open input file\n");
        exit(1);
    }

    size_t input_len = file_size(f_in);

    struct encrypted_t enc;
    enc.msg = malloc(input_len - sizeof enc.nonce);

    const size_t msg_len = input_len - sizeof enc.nonce;
    const size_t data_len = msg_len - crypto_secretbox_MACBYTES;

    enc.len = msg_len;

    read_file(f_in, enc.nonce, sizeof enc.nonce);
    read_file(f_in, enc.msg, msg_len);

    unsigned char* out = malloc(data_len);
    if (decrypt(key, out, &enc) != 0) {
        perror("could not open decrypt file\n");
        exit(1);
    }

    FILE* f_out;
    if ((f_out = fopen(out_file, "wb")) == NULL) {
        perror("could not open output file\n");
        exit(1);
    }

    write_file(f_out, out, data_len);
    fclose(f_out);

    free(out_file);
    printf("Desencriptado exitoso\n\n");
}
