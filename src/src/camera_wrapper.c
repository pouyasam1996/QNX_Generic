// Created by Pouya Samandi on 2025-03-15.
#include "camera_wrapper.h"
#include <camera/camera.h> // Adjusted to QNX Camera Framework header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BUFFERS 3

struct CameraQNX {
    camera_handle_t camera_handle; // QNX camera handle
    camera_buffer_t buffers[NUM_BUFFERS]; // Buffers for frames
    int width;
    int height;
    int is_saving;              // Toggle state for saving
    char* output_path;          // Path for saving video
    FILE* output_file;          // File handle for saving raw frames
};

CameraWrapper* camera_init(int width, int height, const char* output_path) {
    CameraWrapper* camera = (CameraWrapper*)malloc(sizeof(struct CameraQNX));
    if (!camera) return NULL;

    // Store output path
    ((struct CameraQNX*)camera)->output_path = strdup(output_path);
    ((struct CameraQNX*)camera)->output_file = NULL; // Initialize file handle as NULL
    ((struct CameraQNX*)camera)->is_saving = 0;     // Start with saving disabled

    // Open QNX camera
    if (camera_open(CAMERA_UNIT_0, CAMERA_MODE_RW, &((struct CameraQNX*)camera)->camera_handle) != CAMERA_EOK) {
        free(((struct CameraQNX*)camera)->output_path);
        free(camera);
        printf("Failed to open camera!\n");
        return NULL;
    }

    // Configure camera settings
    camera_set_videomode(((struct CameraQNX*)camera)->camera_handle, width, height, 30); // 30 FPS (hypothetical)
    ((struct CameraQNX*)camera)->width = width;
    ((struct CameraQNX*)camera)->height = height;

    // Allocate and set buffers
    for (int i = 0; i < NUM_BUFFERS; i++) {
        ((struct CameraQNX*)camera)->buffers[i].data = malloc(width * height * 3); // Assuming RGB format
        ((struct CameraQNX*)camera)->buffers[i].size = width * height * 3;
        if (!((struct CameraQNX*)camera)->buffers[i].data) {
            for (int j = 0; j < i; j++) free(((struct CameraQNX*)camera)->buffers[j].data);
            camera_close(((struct CameraQNX*)camera)->camera_handle);
            free(((struct CameraQNX*)camera)->output_path);
            free(camera);
            return NULL;
        }
    }
    camera_set_buffers(((struct CameraQNX*)camera)->camera_handle, NUM_BUFFERS, ((struct CameraQNX*)camera)->buffers); // Hypothetical

    // Start camera
    if (camera_start(((struct CameraQNX*)camera)->camera_handle) != CAMERA_EOK) {
        for (int i = 0; i < NUM_BUFFERS; i++) free(((struct CameraQNX*)camera)->buffers[i].data);
        camera_close(((struct CameraQNX*)camera)->camera_handle);
        free(((struct CameraQNX*)camera)->output_path);
        free(camera);
        printf("Failed to start camera!\n");
        return NULL;
    }

    return camera;
}

int camera_capture_frame(CameraWrapper* camera, unsigned char** data, int* width, int* height) {
    if (!camera || !((struct CameraQNX*)camera)->camera_handle) return -1;

    // Capture a frame
    int idx = -1;
    if (camera_get_frame(((struct CameraQNX*)camera)->camera_handle, &((struct CameraQNX*)camera)->buffers[0], &idx, CAMERA_TIMEOUT_INFINITE) != CAMERA_EOK) {
        printf("Failed to capture frame!\n");
        return -1;
    }

    *data = (unsigned char*)((struct CameraQNX*)camera)->buffers[idx].data;
    *width = ((struct CameraQNX*)camera)->width;
    *height = ((struct CameraQNX*)camera)->height;

    // Save raw frame data to file only if saving is enabled
    if (((struct CameraQNX*)camera)->is_saving && ((struct CameraQNX*)camera)->output_file) {
        size_t frame_size = ((struct CameraQNX*)camera)->width * ((struct CameraQNX*)camera)->height * 3;
        fwrite(*data, 1, frame_size, ((struct CameraQNX*)camera)->output_file);
    }

    // Release the frame for reuse
    camera_release_frame(((struct CameraQNX*)camera)->camera_handle, idx); // Hypothetical

    return 0;
}

int camera_start_saving(CameraWrapper* camera) {
    if (!camera) return -1;
    struct CameraQNX* cam = (struct CameraQNX*)camera;
    if (cam->is_saving) return -1; // Already saving, do nothing
    cam->output_file = fopen(cam->output_path, "wb");
    if (!cam->output_file) {
        printf("Failed to open output file: %s\n", cam->output_path);
        return -1;
    }
    cam->is_saving = 1;
    return 0;
}

int camera_stop_saving(CameraWrapper* camera) {
    if (!camera) return -1;
    struct CameraQNX* cam = (struct CameraQNX*)camera;
    if (!cam->is_saving) return -1; // Not saving, do nothing
    if (cam->output_file) {
        fclose(cam->output_file);
        cam->output_file = NULL;
    }
    cam->is_saving = 0;
    return 0;
}

int camera_is_saving(CameraWrapper* camera) {
    if (!camera) return 0;
    struct CameraQNX* cam = (struct CameraQNX*)camera;
    return cam->is_saving;
}

void camera_release(CameraWrapper* camera) {
    if (camera) {
        struct CameraQNX* cam = (struct CameraQNX*)camera;
        // Stop camera
        camera_stop(cam->camera_handle); // Hypothetical
        camera_close(cam->camera_handle);

        // Free resources
        for (int i = 0; i < NUM_BUFFERS; i++) {
            free(cam->buffers[i].data);
        }
        if (cam->output_file) fclose(cam->output_file);
        free(cam->output_path);
        free(camera);
    }
}