cmake_minimum_required(VERSION 3.10)
project(QNX_Video LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find QNX libraries
find_library(CAMERA_LIBRARY NAMES camera libcamera)
find_library(SCREEN_LIBRARY NAMES screen libscreen)
if (NOT CAMERA_LIBRARY OR NOT SCREEN_LIBRARY)
    message(FATAL_ERROR "QNX camera or screen library not found. Ensure QNX SDP is installed.")
endif()

# Add source files
set(SOURCES
        src/src/main.c
        src/src/isp.c
        src/src/display.c
        src/src/encoder.c
        src/src/camera_wrapper.c
)

# Add executable
add_executable(QNX_Video ${SOURCES})

# Include directories
target_include_directories(QNX_Video PRIVATE
        src/include
        /opt/qnx710/target/qnx7/usr/include
)

# Link QNX libraries
target_link_libraries(QNX_Video PRIVATE
        ${CAMERA_LIBRARY}
        ${SCREEN_LIBRARY}
        pthread # For multi-threading
)