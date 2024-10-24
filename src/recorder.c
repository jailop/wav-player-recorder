/**
 * A simple audio record program
 *
 * (c) 2024 - Jaime Lopez <jailop AT protonmail DOT com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pulse/simple.h>
#include "wav.h"

#define SAMPLE_RATE 44100
#define BUFSIZE 1024
#define CHANNELS 2

int flag = 1;

void signal_handler(int sig) {
    flag = 0;
}

void help(char *progname) {
    fprintf(
        stderr,
        "Usage:"
        "    %s OUTPUTWAVFILE",
        progname
    );
}

int main(int argc, char *argv[]) {
    // Checking the number of arguments.
    // It is expected that a filename is provided.
    // That file is in which data will be output using WAV format
    if (argc != 2) {
        help(argv[0]);
        exit(EXIT_FAILURE);
    }
    char *output_file = argv[1];

    // General variables
    int err;                   // Return value
    uint8_t buffer[BUFSIZE];   // To interchange data

    // Registering the signal handler. It changes
    // the value of `flag` from 1 to 0. In that
    // way, the main loop can finish.
    signal(SIGINT, signal_handler);
  
    // Openning the mic 
    const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16NE,
        .channels = CHANNELS,
        .rate = SAMPLE_RATE
    };
    pa_simple *s = pa_simple_new(
        NULL,
        "AudioRecorder",
        PA_STREAM_RECORD,
        NULL,
        "record",
        &ss,
        NULL,
        NULL,
        &err
    );
    if (s == NULL) {
        fprintf(stderr, "Failed openning connection to server: %d\n", err);
        exit(EXIT_FAILURE);
    }

    struct wav w;
    wav_init(&w);

    printf("Recording... press Ctrl+C to finish\n");
    
    while (flag == 1) {
        if (pa_simple_read(s, buffer, BUFSIZE, &err) < 0) {
            fprintf(stderr, "Failed reading audio data\n");
            break;
        }
        wav_append(&w, buffer, BUFSIZE);
    }

    wav_save(&w, output_file);
    wav_deinit(&w);
    // Closing and cleaning up 
    pa_simple_free(s);

    return  0;
}
