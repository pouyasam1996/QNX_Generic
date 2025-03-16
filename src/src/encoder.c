// Created by Pouya Samandi on 2025-03-15.
#include "encoder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char *filename;
    int is_initialized;
    int frame_count;
    FILE* output_file; // File handle for saving raw frames
} encoder_t;

int encoder_init(encoder **enc, const char* output_path) {
    if (enc == NULL || output_path == NULL) return -1;
    encoder_t *new_encoder = (encoder_t *)malloc(sizeof(encoder_t));
    if (new_encoder == NULL) return -1;
    new_encoder->filename = strdup(output_path); // Copy the filename
    new_encoder->is_initialized = 1;
    new_encoder->frame_count = 0;
    new_encoder->output_file = NULL; // Initialize file handle as NULL
    *enc = (encoder *)new_encoder;
    return 0;
}

int encoder_uninit(encoder *enc) {
    if (enc == NULL) return -1;
    encoder_t *e = (encoder_t *)enc;
    if (e->filename) free(e->filename);
    if (e->output_file) fclose(e->output_file); // Close file if open
    e->is_initialized = 0;
    free(enc);
    return 0;
}

int encoder_encode_frame(encoder *enc, unsigned char *data, int width, int height) {
    if (enc == NULL || data == NULL) return -1;
    encoder_t *e = (encoder_t *)enc;
    if (e->is_initialized == 0) return -1;

    e->frame_count++;
    printf("Encoding frame %d...\n", e->frame_count);

    // Write raw frame data to file only if output_file is open
    if (e->output_file) {
        size_t frame_size = width * height * 3; // Assuming RGB888 format
        if (fwrite(data, 1, frame_size, e->output_file) != frame_size) {
            printf("Error writing frame %d to %s\n", e->frame_count, e->filename);
            return -1;
        }
    }

    return 0;
}

int encoder_finalize_recording(encoder *enc) {
    if (enc == NULL) return -1;
    encoder_t *e = (encoder_t *)enc;
    if (e->is_initialized == 0) return -1;
    if (e->output_file) {
        fclose(e->output_file);
        e->output_file = NULL;
    }
    printf("Recording finalized for file: %s\n", e->filename);
    e->is_initialized = 0;
    return 0;
}