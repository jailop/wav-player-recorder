#include <stdio.h>
#include <stdlib.h>
#include <pulse/simple.h>
#include "wav.h"

void play_audio(struct wav_header *header, char data[], int size) {
    pa_sample_spec ss;
    ss.format = PA_SAMPLE_S16LE;
    ss.channels = header->channels;
    ss.rate = header->sample_rate;
    pa_simple *s = pa_simple_new(
        NULL,
        "WAV player",
        PA_STREAM_PLAYBACK,
        NULL,
        "wav",
        &ss,
        NULL,
        NULL,
        NULL
    );
    if (s == NULL) {
        fprintf(stderr, "Openning pulse audio failed\n");
        exit(EXIT_FAILURE);
    }
    int err;
    // int chunck = header->sample_rate * 2 * 16;
    int chunck = header->sample_size;
    for (int i = 0; i < size / chunck; i++) {
        printf("\r%4.1f%c", (i + 1) * 100.0 / (size / chunck), '%'); 
        fflush(stdout);
        pa_simple_write(s, data + i * chunck, chunck, &err);
    }
    pa_simple_free(s);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(
            "Usage:"
            "    %s WAVFILENAME\n",
            argv[0]
        );
        exit(EXIT_FAILURE);
    }
    struct wav_header header;
    FILE *fin = fopen(argv[1], "rb");
    if (fin == NULL) {
        fprintf(stderr, "File cannot be openned: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    size_t res = fread(&header, 1, sizeof(header), fin);
    if (res < sizeof(header)) {
        fprintf(stderr, "Incomplete or invalid WAV header\n");
        exit(EXIT_FAILURE);
    }
    print_header(&header);
    char *data = (char *) malloc(sizeof(char) * header.data_size);
    res = fread(data, 1, header.data_size, fin);
    if (res < header.data_size) {
        fprintf(stderr, "Incomplete or invalid data section\n");
        exit(EXIT_FAILURE);
    }
    play_audio(&header, data, header.data_size);
    fclose(fin);
    return EXIT_SUCCESS;
}
