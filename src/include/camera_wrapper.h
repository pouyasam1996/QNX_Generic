#ifndef CAMERA_WRAPPER_H
#define CAMERA_WRAPPER_H
// High-Level Explanation:
// This module provides a wrapper around the QNX Camera Framework to manage camera input and frame capture on QNX systems.
// It supports capturing frames, toggling video saving to a file, and integrates with the broader application for real-time video processing.
// The code is designed to replace an OpenCV-based implementation, supporting toggle saving with 's' and exit with 'q' in a QNX environment.
// Important functions handle camera initialization, frame capture, saving control, and resource cleanup.
// Key variables include the camera handle, buffers, resolution settings, saving state, and output file path.

// Important Functions:
// - camera_init: Initializes the camera with specified width, height, and output filename.
// - camera_capture_frame: Captures a frame and provides raw pixel data.
// - camera_start_saving: Opens the output file for saving frames.
// - camera_stop_saving: Stops saving and closes the file.
// - camera_is_saving: Checks if saving is active.
// - camera_release: Releases camera resources.

// Important Variables:
// - camera_handle: QNX camera handle for capturing frames.
// - buffers: Array of frame buffers for capturing data.
// - width/height: Resolution settings for the camera.
// - is_saving: Flag indicating if saving is active.
// - output_path: Path for the output video file.
// - output_file: File handle for saving raw frames.

// Inputs and Outputs:
// - Inputs: width (int), height (int), output_path (const char*).
// - Outputs: data (unsigned char**), width (int*), height (int*), return codes (int).

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