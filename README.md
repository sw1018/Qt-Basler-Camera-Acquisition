# Qt Basler Camera Acquisition

A desktop image-acquisition application built with **C++**, **Qt**, **Basler pylon SDK**, and **OpenCV**. The program provides real-time preview, camera-parameter control, and categorized image saving for Basler industrial cameras.

该项目是一个基于 **C++ / Qt / Basler pylon SDK / OpenCV** 开发的工业相机采集上位机，用于机器视觉与水下三维重构实验平台中的图像获取、相机参数控制和实验数据采集。

## Features

- Detect and connect to Basler industrial cameras
- Real-time image acquisition and Qt GUI preview
- Set and read camera exposure time
- Set and read acquisition frame rate
- Configure GigE packet size and inter-packet delay
- Save grayscale and laser images separately
- Automatically create output directories and assign image indices
- Convert and save image data with OpenCV
- Run image acquisition in an independent thread to keep the GUI responsive
- Connect to the first available camera by default without storing device serial numbers in source code

## Tech Stack

- **Language:** C++14
- **GUI:** Qt Widgets
- **Camera SDK:** Basler pylon SDK
- **Computer Vision:** OpenCV
- **Concurrency:** `std::thread`
- **Build System:** qmake
- **Target Platform:** Linux

## Project Structure

```text
Qt-Basler-Camera-Acquisition/
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
├── baslercamera.cpp
├── baslercamera.h
├── BaslerCameraAcquisition.pro
├── .gitignore
└── README.md
```

The main modules are organized as follows:

- `mainwindow.*` — GUI interaction, camera-control commands, and image-saving workflow
- `baslercamera.*` — Basler camera initialization, image acquisition, parameter control, and acquisition thread
- `mainwindow.ui` — Qt Designer interface definition
- `BaslerCameraAcquisition.pro` — qmake project configuration and third-party library settings

## Requirements

Before building the project, install:

- Qt with Qt Widgets support
- Basler pylon SDK
- OpenCV
- A C++14-compatible compiler

The repository does **not** include the Basler pylon SDK, OpenCV binaries, or other third-party libraries.

## Build

The qmake project supports custom OpenCV and pylon installation paths through environment variables.

For example:

```bash
export PYLON_ROOT=/opt/pylon
export OPENCV_ROOT=/usr/local/opencv455
```

Then build from the terminal:

```bash
qmake BaslerCameraAcquisition.pro
make -j$(nproc)
./BaslerCameraAcquisition
```

Alternatively, open `BaslerCameraAcquisition.pro` directly in Qt Creator and configure the corresponding build kit.

If OpenCV is installed under `/usr/local` and pylon is installed under `/opt/pylon`, the default paths in the project file can be used without setting these environment variables.

## Camera Configuration

The public version does not contain real camera serial numbers. By default, the application connects to the first Basler camera detected by the pylon SDK.

For a multi-camera system, a specific camera can be selected by assigning its serial number to `serial_number` before opening the device.

The default exposure time, frame rate, GigE packet size, and inter-packet delay originate from the original experimental setup. These values should be adjusted according to the camera model, network interface, illumination, and acquisition task.

## Image Output

At runtime, the application automatically creates the following directories beside the executable:

```text
output/
├── gray/
└── laser/
```

- `gray/` stores grayscale acquisition results.
- `laser/` stores laser-image acquisition results.

The output directory is excluded by `.gitignore` to prevent experimental image data from being committed accidentally.

## Engineering Highlights

This project demonstrates practical experience in:

- C++ / Qt desktop application development
- Industrial camera SDK integration
- Real-time image acquisition and visualization
- Camera parameter configuration
- OpenCV-based image conversion and storage
- Multithreaded acquisition workflow
- Integration of software with physical experimental equipment

## Background

The application was developed as part of an experimental vision system for image acquisition and data collection. It was later reorganized into an independent public repository by removing machine-specific paths, device identifiers, generated files, and unnecessary third-party dependencies.
