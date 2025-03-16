cat << 'EOF' > docs/README.md
# QNX Video Project

## Overview
This project implements a video capture and processing pipeline using QNX APIs, designed for QNX embedded systems. It captures frames from a camera using the QNX Camera Framework, displays them using the QNX Screen API, and allows saving the video to a file. **Note: This code has not been tested or compiled yet and serves as a generic model developed for future developments, providing a foundation for further refinement and testing on QNX targets.**

### Features
- Capture live video from a camera using QNX Camera APIs.
- Display video frames using QNX Screen API with a "Saving Video" or "Not Saving" status overlay.
- Toggle video saving with the 's' key (press to start, press again to stop).
- Exit the program with the 'q' key.
- Multi-threaded encoding for efficient video processing.

## Prerequisites
- **Operating System**: QNX (target system).
- **QNX SDP**: QNX Software Development Platform (version 7.1 or later) for QNX Camera and Screen APIs.
- **CMake**: For building the project.
- **Compiler**: QNX compiler (`qcc`).
- **Dependencies**:
  - QNX Camera Framework (`libcamera`).
  - QNX Screen API (`libscreen`).
  - POSIX threads (`pthread`) for multi-threading.

## Installation
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/pouyasam1996/QNX_video.git
   cd QNX_video
