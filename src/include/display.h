#ifndef DISPLAY_H
#define DISPLAY_H
// High-Level Explanation:
// This module manages the display of video frames on QNX systems using the QNX Screen API.
// It creates a window, renders frames, overlays a saving status, and detects keypresses for user interaction ('s' to toggle saving, 'q' to quit).
// The code is part of a QNX-based video pipeline, replacing OpenCV display functionality.
// Important functions initialize the display, render frames, capture keypresses, and clean up resources.
// Key variables include the screen context, window, buffer, and frame dimensions.

// Important Functions:
// - display_init: Initializes the display with a callback for frame processing.
// - display_uninit: Releases display resources.
// - display_display_data: Renders a frame with a saving status overlay.
// - display_get_keypress: Captures user keypresses ('s' or 'q').

// Important Variables:
// - screen_ctx: QNX Screen context for managing the display.
// - screen_win: QNX Screen window for rendering frames.
// - screen_buf: Buffer for storing frame data.
// - width/height: Dimensions of the displayed frame.

// Inputs and Outputs:
// - Inputs: display_callback (void (*)), data (unsigned char*), width (int), height (int), is_saving (int).
// - Outputs: Return codes (int), keypress (int).
typedef struct display display;

// Initialize the display
int display_init(display **disp, void (*display_callback)(void));

// Clean up display resources
int display_uninit(display *disp);

// Display the frame data with saving status
int display_display_data(display *disp, unsigned char *data, int width, int height, int is_saving);

// Get the next keypress (for 's' to toggle saving, 'q' to quit)
int display_get_keypress(void);

#endif