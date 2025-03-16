#ifndef ISP_H
#define ISP_H
// High-Level Explanation:
// This module implements an Image Signal Processor (ISP) for double-buffering video frames in a QNX-based video pipeline.
// It alternates between two buffers (R0 and R1) to manage frame processing and display, triggered by a callback.
// The code ensures smooth frame handling in a multi-threaded environment, replacing OpenCV-based buffering.
// Important functions initialize the ISP, program buffers, start processing, and retrieve the current buffer.
// Key variables include buffer data, dimensions, and the current buffer state.

// Important Functions:
// - isp_init: Initializes the ISP with a callback for frame processing.
// - isp_uninit: Releases ISP resources.
// - isp_program_R0/R1: Programs the R0 or R1 buffer with frame data.
// - isp_start: Triggers the callback to process frames.
// - isp_get_current_buffer: Retrieves the current buffer for display.

// Important Variables:
// - r0_data/r1_data: Frame data for R0 and R1 buffers.
// - r0_width/r0_height, r1_width/r1_height: Dimensions of each buffer.
// - current_buffer: Indicates the active buffer (0 for R0, 1 for R1).

// Inputs and Outputs:
// - Inputs: callback (void (*)), data (unsigned char*), width (int), height (int).
// - Outputs: data (unsigned char*), width (int*), height (int*), return codes (int).

typedef struct isp isp;

// Initialize the ISP with a callback
int isp_init(isp **isp, void (*callback)(struct isp *));

// Clean up ISP resources
int isp_uninit(isp *isp);

// Program the R0 buffer with frame data
int isp_program_R0(isp *isp, unsigned char *data, int width, int height);

// Program the R1 buffer with frame data
int isp_program_R1(isp *isp, unsigned char *data, int width, int height);

// Start the ISP processing (triggers callback)
int isp_start(isp *isp);

// Get the current buffer for display or processing
unsigned char* isp_get_current_buffer(isp *isp, int *width, int *height);

#endif