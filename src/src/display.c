// Created by Pouya Samandi on 2025-03-15.
#include "display.h"
#include <screen/screen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper structure to store display state
typedef struct {
    void (*callback)(void);
    int is_initialized;
    screen_context_t screen_ctx;
    screen_window_t screen_win;
    screen_buffer_t screen_buf;
    int width, height;
} display_t;

// Helper function to draw text (simplified placeholder, replace with actual QNX text rendering if available)
void draw_text(screen_window_t win, const char* text, int x, int y, int color) {
    // Placeholder: QNX Screen API doesn't natively support text rendering
    // For production, use a bitmap font or a library like fontconfig
    printf("Drawing text: %s at (%d, %d) with color 0x%06x\n", text, x, y, color);
}

int display_init(display **disp, void (*display_callback)(void)) {
    display_t *new_display = (display_t *)malloc(sizeof(display_t));
    if (!new_display) return -1;

    new_display->callback = display_callback;
    new_display->is_initialized = 1;

    // Create screen context
    if (screen_create_context(&new_display->screen_ctx, SCREEN_APPLICATION_CONTEXT) != 0) {
        free(new_display);
        return -1;
    }

    // Create a window
    if (screen_create_window(&new_display->screen_win, new_display->screen_ctx) != 0) {
        screen_destroy_context(new_display->screen_ctx);
        free(new_display);
        return -1;
    }

    // Set window properties
    int format = SCREEN_FORMAT_RGB888;
    screen_set_window_property_iv(new_display->screen_win, SCREEN_PROPERTY_FORMAT, &format);
    int usage = SCREEN_USAGE_WRITE | SCREEN_USAGE_NATIVE;
    screen_set_window_property_iv(new_display->screen_win, SCREEN_PROPERTY_USAGE, &usage);

    *disp = (struct display *)new_display;
    return 0;
}

int display_uninit(display *disp) {
    if (!disp) return -1;
    display_t *d = (display_t *)disp;

    if (d->screen_buf) screen_destroy_buffer(d->screen_buf);
    screen_destroy_window(d->screen_win);
    screen_destroy_context(d->screen_ctx);
    d->is_initialized = 0;
    free(d);
    return 0;
}

int display_display_data(display *disp, unsigned char *data, int width, int height, int is_saving) {
    if (!disp || !data) return -1;
    display_t *d = (display_t *)disp;
    if (d->is_initialized == 0) return -1;

    // Update window dimensions if needed
    if (d->width != width || d->height != height) {
        d->width = width;
        d->height = height;
        screen_set_window_property_iv(d->screen_win, SCREEN_PROPERTY_BUFFER_SIZE, (int[]){width, height});
        screen_set_window_property_iv(d->screen_win, SCREEN_PROPERTY_SIZE, (int[]){width, height});

        // Create a buffer for the window
        if (d->screen_buf) screen_destroy_buffer(d->screen_buf);
        screen_create_window_buffers(d->screen_win, 1);
        screen_get_window_property_pv(d->screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void**)&d->screen_buf);
    }

    // Copy data to the screen buffer
    void *ptr;
    screen_get_buffer_property_pv(d->screen_buf, SCREEN_PROPERTY_POINTER, &ptr);
    memcpy(ptr, data, width * height * 3); // Assuming RGB888 format

    // Overlay saving status text
    const char *status_text = is_saving ? "Saving Video" : "Not Saving";
    int color = is_saving ? 0x00FF00 : 0xFF0000; // Green for saving, Red for not saving
    draw_text(d->screen_win, status_text, 10, 20, color); // Placeholder text rendering

    // Post the buffer to the window
    int dirty_rect[4] = {0, 0, width, height};
    screen_post_window(d->screen_win, d->screen_buf, 1, dirty_rect, 0);

    // Call the callback
    if (d->callback) {
        d->callback();
    }

    return 0;
}

int display_get_keypress(void) {
    screen_event_t event;
    if (screen_create_event(&event) != 0) return -1;

    int key = -1;
    if (screen_get_event(NULL, event, 0) == 0) { // Use default context or pass specific context
        int type;
        screen_get_event_property_iv(event, SCREEN_PROPERTY_TYPE, &type);
        if (type == SCREEN_EVENT_KEYBOARD) {
            screen_get_event_property_iv(event, SCREEN_PROPERTY_KEY_CODE, &key);
            // Map QNX key codes to ASCII (simplified)
            if (key == 0x73) key = 's'; // 's' key
            else if (key == 0x71) key = 'q'; // 'q' key
        }
    }

    screen_destroy_event(event);
    return key; // Return -1 if no keypress, or 's'/'q' if detected
}