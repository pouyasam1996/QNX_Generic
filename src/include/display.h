#ifndef DISPLAY_H
#define DISPLAY_H

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