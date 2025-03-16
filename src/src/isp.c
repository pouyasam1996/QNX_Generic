// Created by Pouya Samandi on 2025-03-15.
#include "isp.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    void (*callback)(struct isp *); // Updated to pass isp pointer
    unsigned char *r0_data;
    unsigned char *r1_data;
    int r0_width, r0_height;
    int r1_width, int r1_height;
    int current_buffer; // 0 for R0, 1 for R1
} isp_t;

int isp_init(isp **isp, void (*callback)(struct isp *)) {
    isp_t *new_isp = (isp_t *)malloc(sizeof(isp_t));
    if (!new_isp) return -1;
    new_isp->callback = callback;
    new_isp->r0_data = NULL;
    new_isp->r1_data = NULL;
    new_isp->current_buffer = 0;
    *isp = (struct isp *)new_isp; // Cast to opaque type
    return 0;
}

int isp_uninit(isp *isp) {
    if (!isp) return -1;
    // No need to free r0_data or r1_data as they are pointers to external buffers
    free(isp);
    return 0;
}

int isp_program_R0(isp *isp, unsigned char *data, int width, int height) {
    if (!isp) return -1;
    isp_t *isp_ptr = (isp_t *)isp;
    isp_ptr->r0_data = data;
    isp_ptr->r0_width = width;
    isp_ptr->r0_height = height;
    return 0;
}

int isp_program_R1(isp *isp, unsigned char *data, int width, int height) {
    if (!isp) return -1;
    isp_t *isp_ptr = (isp_t *)isp;
    isp_ptr->r1_data = data;
    isp_ptr->r1_width = width;
    isp_ptr->r1_height = height;
    return 0;
}

int isp_start(isp *isp) {
    if (!isp || !((isp_t *)isp)->callback) return -1;
    ((isp_t *)isp)->callback(isp); // Pass isp pointer to callback
    return 0;
}

unsigned char* isp_get_current_buffer(isp *isp, int *width, int *height) {
    isp_t *isp_ptr = (isp_t *)isp;
    if (isp_ptr->current_buffer == 0) {
        *width = isp_ptr->r0_width;
        *height = isp_ptr->r0_height;
        isp_ptr->current_buffer = 1;
        return isp_ptr->r0_data;
    } else {
        *width = isp_ptr->r1_width;
        *height = isp_ptr->r1_height;
        isp_ptr->current_buffer = 0;
        return isp_ptr->r1_data;
    }
}