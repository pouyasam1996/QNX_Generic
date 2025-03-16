#ifndef ISP_H
#define ISP_H

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