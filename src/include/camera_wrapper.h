#ifndef CAMERA_WRAPPER_H
#define CAMERA_WRAPPER_H

typedef struct CameraQNX CameraWrapper;

// Initialize the camera with width, height, and output file path for saving
CameraWrapper* camera_init(int width, int height, const char* output_path);

// Capture a frame (returns pointer to raw pixel data and dimensions)
int camera_capture_frame(CameraWrapper* camera, unsigned char** data, int* width, int* height);

// Start saving video to the output path
int camera_start_saving(CameraWrapper* camera);

// Stop saving video
int camera_stop_saving(CameraWrapper* camera);

// Check if the camera is currently saving
int camera_is_saving(CameraWrapper* camera);

// Release resources
void camera_release(CameraWrapper* camera);

#endif // CAMERA_WRAPPER_H