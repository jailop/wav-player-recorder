/**
 * WAV file format implementation
 *
 * (c) 2024 Jaime Lopez <jailop AT protonmail DOT com>
 */

#ifndef _WAV_H
#define _WAV_H

#include <stdint.h>

struct wav_header {
    uint8_t file_type[4];
    uint32_t file_size;
    uint8_t file_header[4];
    uint8_t format_marker[4];
    uint32_t format_lenght;
    uint16_t format_type;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t sample_size;
    uint16_t sample_mode;
    uint16_t sample_bits;
    uint8_t data_marker[4];
    uint32_t data_size;
};

struct wav {
    struct wav_header header;
    uint8_t *data;
    uint32_t len;
    uint32_t cap;
};

void print_header(struct wav_header *header);
int wav_init(struct wav *w);
int wav_append(struct wav *w, uint8_t segment[], int size);
int wav_save(struct wav *w, char *filename);
int wav_deinit(struct wav *w);


#endif // _WAV_H
