#ifndef ENCODER_H
#define ENCODER_H
// High-Level Explanation:
// This module manages video encoding on QNX systems by writing raw frame data to a file.
// It operates in a separate thread, encoding frames when saving is active, and finalizes the recording process.
// The code replaces an OpenCV-based encoder, supporting toggle saving functionality in a QNX video pipeline.
// Important functions initialize the encoder, encode frames, and finalize recording.
// Key variables include the output filename, frame count, and file handle.

// Important Functions:
// - encoder_init: Initializes the encoder with an output file path.
// - encoder_uninit: Cleans up encoder resources.
// - encoder_encode_frame: Writes a frame to the output file if saving is active.
// - encoder_finalize_recording: Closes the output file and finalizes recording.

// Important Variables:
// - filename: Path for the output video file.
// - frame_count: Tracks the number of encoded frames.
// - output_file: File handle for writing raw frame data.

// Inputs and Outputs:
// - Inputs: output_path (const char*), data (unsigned char*), width (int), height (int).
// - Outputs: Return codes (int).

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