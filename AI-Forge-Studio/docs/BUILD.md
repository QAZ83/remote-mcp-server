# Build Instructions

This document provides detailed instructions for building AI Forge Studio from source on different platforms.

---

## Table of Contents

- [Prerequisites](#prerequisites)
- [Windows Build](#windows-build)
- [Linux Build](#linux-build)
- [macOS Build](#macos-build)
- [Building with Optional Features](#building-with-optional-features)
- [Troubleshooting](#troubleshooting)

---

## Prerequisites

### Required Tools

- **CMake 3.16+**
- **C++17 compatible compiler**
  - Windows: MSVC 2019+ or MinGW-w64
  - Linux: GCC 9+ or Clang 10+
  - macOS: Xcode 12+ (Apple Clang)
- **Git**

### Required Libraries

- **Qt 6.5+** (Core, Gui, Qml, Quick, QuickControls2, Charts)
- **CUDA Toolkit 12.0+** (for GPU acceleration)
- **NVIDIA Driver 550.0+**

### Optional Libraries

- **Vulkan SDK 1.3+** (for advanced rendering)
- **TensorRT 8.6+** (for AI optimization)
- **Python 3.10+** (for Python bridge)
- **Pybind11** (for C++/Python integration)

---

## Windows Build

### Step 1: Install Visual Studio

Download and install [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/)

Select the following workloads:
- Desktop development with C++
- CMake tools for Windows

### Step 2: Install Qt

**Option A: Qt Online Installer**

1. Download [Qt Online Installer](https://www.qt.io/download-qt-installer)
2. Install Qt 6.5 or newer
3. Select these components:
   - Qt 6.x for MSVC 2019 64-bit
   - Qt Quick Controls 2
   - Qt Charts

**Option B: vcpkg**

```powershell
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Install Qt6
.\vcpkg install qt6-base:x64-windows
.\vcpkg install qt6-declarative:x64-windows
.\vcpkg install qt6-charts:x64-windows
```

### Step 3: Install CUDA

1. Download [CUDA Toolkit 12.x](https://developer.nvidia.com/cuda-downloads)
2. Run the installer
3. Select "Custom Installation"
4. Install CUDA Toolkit and Visual Studio Integration

### Step 4: Install Vulkan SDK (Optional)

1. Download [Vulkan SDK](https://vulkan.lunarg.com/sdk/home#windows)
2. Run the installer
3. Add to system PATH

### Step 5: Build Project

```powershell
# Clone repository
git clone https://github.com/yourusername/AI-Forge-Studio.git
cd AI-Forge-Studio

# Create build directory
mkdir build
cd build

# Configure (adjust paths as needed)
cmake .. -G "Visual Studio 17 2022" -A x64 `
    -DCMAKE_PREFIX_PATH="C:/Qt/6.5.0/msvc2019_64" `
    -DCUDA_TOOLKIT_ROOT_DIR="C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.3"

# Build
cmake --build . --config Release

# Run
.\Release\AIForgeStudio.exe
```

---

## Linux Build

### Step 1: Install Build Tools

**Ubuntu/Debian:**

```bash
sudo apt update
sudo apt install build-essential cmake git
```

**Fedora/RHEL:**

```bash
sudo dnf install gcc gcc-c++ cmake git
```

**Arch Linux:**

```bash
sudo pacman -S base-devel cmake git
```

### Step 2: Install Qt6

**Ubuntu/Debian:**

```bash
sudo apt install qt6-base-dev qt6-declarative-dev qt6-tools-dev \
                 qt6-l10n-tools qml6-module-qtquick-controls \
                 qml6-module-qtcharts qml6-module-qtquick-layouts
```

**Fedora:**

```bash
sudo dnf install qt6-qtbase-devel qt6-qtdeclarative-devel \
                 qt6-qtcharts-devel
```

**Arch Linux:**

```bash
sudo pacman -S qt6-base qt6-declarative qt6-charts
```

### Step 3: Install CUDA

**Ubuntu/Debian:**

```bash
# Add NVIDIA repository
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2204/x86_64/cuda-keyring_1.1-1_all.deb
sudo dpkg -i cuda-keyring_1.1-1_all.deb
sudo apt-get update

# Install CUDA
sudo apt-get install cuda

# Add to PATH
echo 'export PATH=/usr/local/cuda/bin:$PATH' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc
```

### Step 4: Install Vulkan SDK (Optional)

```bash
# Add LunarG repository
wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-jammy.list \
    https://packages.lunarg.com/vulkan/lunarg-vulkan-jammy.list
sudo apt update

# Install Vulkan SDK
sudo apt install vulkan-sdk
```

### Step 5: Install Python Dependencies (Optional)

```bash
# Install Python and pip
sudo apt install python3 python3-pip python3-dev

# Install PyTorch and related libraries
pip3 install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121
pip3 install diffusers transformers accelerate safetensors pybind11
```

### Step 6: Build Project

```bash
# Clone repository
git clone https://github.com/yourusername/AI-Forge-Studio.git
cd AI-Forge-Studio

# Create build directory
mkdir build && cd build

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build (use all CPU cores)
cmake --build . -j$(nproc)

# Install (optional)
sudo cmake --install .

# Run
./AIForgeStudio
```

---

## macOS Build

### Step 1: Install Xcode

```bash
xcode-select --install
```

### Step 2: Install Homebrew

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### Step 3: Install Dependencies

```bash
# Install CMake and Qt
brew install cmake qt@6

# Add Qt to PATH
echo 'export PATH="/usr/local/opt/qt@6/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc
```

### Step 4: Build Project

```bash
# Clone repository
git clone https://github.com/yourusername/AI-Forge-Studio.git
cd AI-Forge-Studio

# Create build directory
mkdir build && cd build

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)

# Build
cmake --build . -j$(sysctl -n hw.ncpu)

# Run
./AIForgeStudio.app/Contents/MacOS/AIForgeStudio
```

**Note:** CUDA is not available on macOS. GPU acceleration features will be limited.

---

## Building with Optional Features

### Enable TensorRT

```bash
# Download TensorRT from NVIDIA
# Extract to /opt/tensorrt (or custom location)

# Build with TensorRT
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DTensorRT_DIR=/opt/tensorrt
```

### Enable Python Bridge

```bash
# Ensure Python and pybind11 are installed
pip3 install pybind11

# Build with Python support
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DPython3_ROOT_DIR=/usr \
    -Dpybind11_DIR=$(python3 -m pybind11 --cmakedir)
```

### Enable Tests

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON
cmake --build .

# Run tests
ctest --output-on-failure
```

### Static Linking (Portable Build)

```bash
# Windows
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF

# Linux
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_EXE_LINKER_FLAGS="-static-libgcc -static-libstdc++"
```

---

## Troubleshooting

### Qt Not Found

**Error:** `Could not find Qt6`

**Solution:**
```bash
# Set Qt path explicitly
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64
```

### CUDA Not Found

**Error:** `CUDA not found`

**Solution:**
```bash
# Set CUDA path
export CUDA_HOME=/usr/local/cuda
export PATH=$CUDA_HOME/bin:$PATH
export LD_LIBRARY_PATH=$CUDA_HOME/lib64:$LD_LIBRARY_PATH
```

### Vulkan Headers Not Found

**Error:** `Vulkan/vulkan.h: No such file or directory`

**Solution:**
```bash
# Install Vulkan SDK
# Ubuntu:
sudo apt install libvulkan-dev vulkan-tools

# Or specify Vulkan SDK path
cmake .. -DVulkan_DIR=/path/to/vulkan/sdk
```

### Missing Python.h

**Error:** `Python.h: No such file or directory`

**Solution:**
```bash
# Ubuntu:
sudo apt install python3-dev

# Fedora:
sudo dnf install python3-devel
```

### Linking Errors

**Error:** `undefined reference to...`

**Solution:**
```bash
# Clean build and try again
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --clean-first
```

### Runtime DLL/SO Errors

**Windows:** Copy Qt DLLs to executable directory:
```powershell
windeployqt AIForgeStudio.exe
```

**Linux:** Add Qt lib directory to `LD_LIBRARY_PATH`:
```bash
export LD_LIBRARY_PATH=/path/to/Qt/6.x.x/gcc_64/lib:$LD_LIBRARY_PATH
```

---

## Performance Optimization

### Release Build

Always use Release mode for production:
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

### Link-Time Optimization (LTO)

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON
```

### Native CPU Optimization

```bash
# GCC/Clang
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="-march=native -O3"

# MSVC
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="/arch:AVX2 /O2"
```

---

## Creating Installer Package

### Windows (NSIS)

```powershell
# Build installer
cmake --build . --target package
```

### Linux (DEB/RPM)

```bash
# Create DEB package
cpack -G DEB

# Create RPM package
cpack -G RPM
```

### macOS (DMG)

```bash
# Create DMG
cpack -G DragNDrop
```

---

For additional help, please see the [README](README.md) or open an issue on GitHub.
