#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wav.h"

#define BUFSIZE 1024

static char *str_marker(uint8_t *pos, char buffer[]) {
    for (int i = 0; i < 4; i++)
        buffer[i] = *(pos + 1);
    buffer[4] = '\0';
    return buffer;
}

void print_header(struct wav_header *header) {
    char buffer[5];
    printf("File type    : %s\n", str_marker(header->file_type, buffer));
    printf("File size    : %d\n", header->file_size);
    printf("File header  : %s\n", str_marker(header->file_header, buffer));
    printf("Format marker: %s\n", str_marker(header->format_marker, buffer));
    printf("Format length: %d\n", header->format_lenght);
    printf("Format type  : %d\n", header->format_type);
    printf("Channels     : %d\n", header->channels);
    printf("Sample rate  : %d\n", header->sample_rate);
    printf("Sample size  : %d\n", header->sample_size);
    printf("Sample mode  : %d\n", header->sample_mode);
    printf("Sample bits  : %d\n", header->sample_bits);
    printf("Data marker  : %s\n", str_marker(header->data_marker, buffer));
    printf("Data size    : %d\n", header->data_size);
}

int wav_init(struct wav *w) {
    memcpy(w->header.file_type, "RIFF", 4);
    memcpy(w->header.file_header, "WAVE", 4);
    memcpy(w->header.format_marker, "fmt ", 4);
    memcpy(w->header.data_marker, "data", 4);
    w->header.format_lenght = 16;
    w->header.format_type = 1;
    w->header.channels = 2;
    w->header.sample_rate = 44100;
    w->header.sample_size = 176400;
    w->header.sample_mode = 4;
    w->header.sample_bits = 16;
    w->header.data_size = 0;
    w->data = NULL;
    w->len = 0;
    w->cap = 0;
    return 0;
}

int wav_append(struct wav *w, uint8_t segment[], int size) {
    if (w->data == NULL) {
        w->cap = BUFSIZE;
        while (size >= w->cap)
            w->cap *= 2;
        w->data = (uint8_t *) malloc(w->cap);
    }
    else {
        while (w->len + size >= w->cap)
            w->cap *= 2;
        uint8_t *aux = realloc(w->data, w->cap);
        if (aux == NULL) {
            fprintf(stderr, "Fatal error reallocating data buffer\n");
            exit(EXIT_FAILURE);
        }
        w->data = aux;
    }
    memcpy(w->data + w->len, segment, size);
    w->len += size;
    return 0;
}

int wav_save(struct wav *w, char *filename) {
    int res;
    w->header.data_size = w->len;
    w->header.file_size = sizeof(struct wav_header) + w->len - 8;
    FILE *fout = fopen(filename, "wb");
    if (fout == NULL) {
        fprintf(stderr, "Output file cannot be openned: %s\n", filename);
        return -1;
    }
    res = fwrite(&(w->header), 1, sizeof(struct wav), fout);
    if (res != sizeof(struct wav)) {
        fprintf(stderr, "Error writing header\n");
        fclose(fout);
        return -1;
    }
    res = fwrite(w->data, 1, w->len, fout);
    if (res != w->len) {
        fprintf(stderr, "Error writing data section\n");
        fclose(fout);
        return -1;
    }
    fclose(fout);
    return 0; 
}

int wav_deinit(struct wav *w) {
    if (w->data != NULL) {
        free(w->data);
    }
    return 0;
}
