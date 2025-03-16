#ifndef ENCODER_H
#define ENCODER_H

typedef struct encoder encoder;

// Initialize the encoder with an output file path
int encoder_init(encoder **enc, const char* output_path);

// Clean up encoder resources
int encoder_uninit(encoder *enc);

// Encode a frame (write raw data to file if saving)
int encoder_encode_frame(encoder *enc, unsigned char *data, int width, int height);

// Finalize the recording (close the file)
int encoder_finalize_recording(encoder *enc);

#endif