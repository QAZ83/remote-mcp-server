# AI Forge Studio

## RTX 50-Series Edition

![Version](https://img.shields.io/badge/version-1.0.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey)

**AI Forge Studio** is a professional desktop application designed specifically for NVIDIA RTX 50-Series GPU users. It combines cutting-edge artificial intelligence with advanced graphics processing to deliver high-quality image generation, real-time hardware monitoring, and AI model management.

---

## 🚀 Features

### Core Capabilities

- **Real-Time Hardware Monitoring**
  - GPU utilization, temperature, and power consumption
  - VRAM usage tracking
  - Multi-GPU support
  - CPU and RAM monitoring
  - Live performance graphs

- **AI Model Management**
  - Support for multiple model formats (SafeTensors, GGUF, ONNX, PyTorch)
  - Text-to-Image generation (Stable Diffusion, SDXL)
  - Image upscaling (Real-ESRGAN)
  - LLM integration
  - One-click model loading and optimization
  - HuggingFace model import

- **Advanced Rendering Engine**
  - 4K+ real-time preview
  - NVIDIA DLSS 3.5 integration
  - Hardware ray tracing support
  - HDR rendering
  - Vulkan/DirectX 12 backend

- **Optimization Features**
  - TensorRT model optimization
  - FP16/INT8 precision modes
  - VRAM ↔ RAM off-loading
  - Automatic memory management
  - Multi-GPU link support

### User Interface

- **Glass-morphism Design** - Modern UI with blur effects and Neon Cyan/Purple accents
- **Dark Mode** - Easy on the eyes for extended sessions
- **Responsive Layout** - Adapts to different screen sizes
- **Real-Time Updates** - Live metrics and progress indicators
- **Intuitive Navigation** - Easy-to-use multi-panel interface

---

## 📋 Requirements

### Hardware Requirements

**Minimum:**
- NVIDIA RTX 4060 or higher
- 16 GB RAM
- 50 GB free storage
- 1920x1080 display

**Recommended:**
- NVIDIA RTX 5080/5090
- 32 GB+ RAM
- 100 GB+ free SSD storage
- 4K display

### Software Requirements

**Windows:**
- Windows 10/11 (64-bit)
- NVIDIA Driver 550.0 or newer
- Visual Studio 2019/2022 (for building)

**Linux:**
- Ubuntu 22.04 LTS or newer (or equivalent)
- NVIDIA Driver 550.0 or newer
- GCC 9.0+ or Clang 10.0+

**Common:**
- Qt 6.5+
- CUDA Toolkit 12.0+
- Python 3.10+ (optional, for Python bridge)
- Vulkan SDK 1.3+ (optional, for rendering)

---

## 🛠️ Building from Source

### 1. Install Dependencies

#### Windows

```powershell
# Install Qt (via online installer or vcpkg)
vcpkg install qt6

# Install CUDA Toolkit
# Download from: https://developer.nvidia.com/cuda-downloads

# Install Vulkan SDK
# Download from: https://vulkan.lunarg.com/
```

#### Linux (Ubuntu/Debian)

```bash
# Install Qt6
sudo apt install qt6-base-dev qt6-declarative-dev qt6-tools-dev \
                 qml6-module-qtquick-controls qml6-module-qtcharts

# Install CUDA Toolkit
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2204/x86_64/cuda-ubuntu2204.pin
sudo mv cuda-ubuntu2204.pin /etc/apt/preferences.d/cuda-repository-pin-600
wget https://developer.download.nvidia.com/compute/cuda/12.3.0/local_installers/cuda-repo-ubuntu2204-12-3-local_12.3.0-545.23.06-1_amd64.deb
sudo dpkg -i cuda-repo-ubuntu2204-12-3-local_12.3.0-545.23.06-1_amd64.deb
sudo cp /var/cuda-repo-ubuntu2204-12-3-local/cuda-*-keyring.gpg /usr/share/keyrings/
sudo apt-get update
sudo apt-get -y install cuda

# Install Vulkan SDK
wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-jammy.list https://packages.lunarg.com/vulkan/lunarg-vulkan-jammy.list
sudo apt update
sudo apt install vulkan-sdk

# Install Python dependencies (optional)
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121
pip install diffusers transformers accelerate safetensors
```

### 2. Clone and Build

```bash
# Clone repository
git clone https://github.com/yourusername/AI-Forge-Studio.git
cd AI-Forge-Studio

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release -j$(nproc)

# Run
./AIForgeStudio
```

### 3. Optional: Build with TensorRT

```bash
# Download TensorRT from NVIDIA website
# Extract and set environment variable
export TensorRT_DIR=/path/to/TensorRT

# Rebuild with TensorRT support
cmake .. -DCMAKE_BUILD_TYPE=Release -DTensorRT_DIR=$TensorRT_DIR
cmake --build . --config Release -j$(nproc)
```

---

## 📖 Usage Guide

### Getting Started

1. **Launch the Application**
   ```bash
   ./AIForgeStudio
   ```

2. **Check Dashboard**
   - View real-time GPU/CPU metrics
   - Monitor VRAM usage
   - Check system status

3. **Load a Model**
   - Navigate to "Models" tab
   - Click "Load Model" or "Import from HF"
   - Select your AI model file or HuggingFace model ID
   - Wait for loading to complete

4. **Generate Images**
   - Go to "Render" tab
   - Select loaded model
   - Enter your prompt
   - Adjust settings (steps, guidance, resolution)
   - Click "Generate Image"

### Model Management

**Supported Formats:**
- `.safetensors` - HuggingFace safe format (recommended)
- `.gguf` - GPT-Generated Unified Format
- `.onnx` - Open Neural Network Exchange
- `.pt`/`.pth` - PyTorch models

**Optimization:**
- Click "Optimize" on any loaded model
- Select precision mode (FP16 recommended for RTX 50-series)
- Wait for TensorRT optimization (one-time process)
- Enjoy faster inference!

### Tips for Best Performance

1. **Enable TensorRT Optimization** - 2-3x faster inference
2. **Use FP16 Precision** - Optimal for RTX 50-series
3. **Enable DLSS** - Better preview quality with less VRAM
4. **Monitor VRAM** - Unload unused models to free memory
5. **Adjust Batch Size** - Lower for high-resolution images

---

## 🏗️ Architecture

### Project Structure

```
AI-Forge-Studio/
├── core/                      # C++ backend modules
│   ├── hardware_monitor.cpp   # GPU/CPU monitoring (NVML)
│   ├── ai_engine.cpp          # AI inference (CUDA/TensorRT)
│   ├── render_engine.cpp      # Graphics rendering (Vulkan)
│   └── logger.cpp             # Logging system
│
├── python_bridge/             # Python integration
│   ├── bridge.cpp             # C++ ↔ Python bridge (Pybind11)
│   └── model_runner.py        # Python AI model runner
│
├── ui/                        # QML user interface
│   ├── main.qml               # Main window
│   ├── dashboard.qml          # Hardware monitoring dashboard
│   ├── model_manager.qml      # Model management interface
│   ├── render_view.qml        # Image generation view
│   └── settings.qml           # Application settings
│
├── tests/                     # Test suite
├── docs/                      # Documentation
├── models/                    # Model storage directory
├── output/                    # Generated images output
└── main.cpp                   # Application entry point
```

### Technology Stack

- **Core:** C++17
- **UI Framework:** Qt 6 (QML + QtQuick)
- **Graphics API:** Vulkan 1.3 / DirectX 12
- **AI Framework:** CUDA 12 + TensorRT 8
- **Python Integration:** Pybind11
- **Hardware Monitoring:** NVIDIA NVML
- **Build System:** CMake 3.16+

### Component Interaction

```
┌─────────────────────────────────────────────────────────┐
│                    QML Frontend                         │
│  (Dashboard, Model Manager, Render View, Settings)      │
└────────────────┬────────────────────────────────────────┘
                 │ Qt Signals/Slots
┌────────────────▼────────────────────────────────────────┐
│              BackendController (C++)                    │
│  - Exposes C++ API to QML                              │
│  - Manages component lifecycle                          │
└─────┬──────────┬──────────┬──────────┬─────────────────┘
      │          │          │          │
┌─────▼────┐ ┌──▼─────┐ ┌──▼─────┐ ┌──▼──────────┐
│ Hardware │ │   AI   │ │ Render │ │   Python    │
│ Monitor  │ │ Engine │ │ Engine │ │   Bridge    │
│  (NVML)  │ │(CUDA/  │ │(Vulkan)│ │ (Pybind11)  │
│          │ │TensorRT│ │        │ │             │
└──────────┘ └────────┘ └────────┘ └──────┬──────┘
                                           │
                                    ┌──────▼──────┐
                                    │   Python    │
                                    │ Model Runner│
                                    │ (PyTorch)   │
                                    └─────────────┘
```

---

## 🧪 Testing

### Running Tests

```bash
# Build with tests enabled
cmake .. -DBUILD_TESTS=ON
cmake --build . --config Release

# Run all tests
ctest --output-on-failure

# Run specific test
./tests/gpu_benchmark
```

### Benchmark Suite

The included benchmark suite tests:
- GPU inference performance
- Memory allocation efficiency
- Model loading times
- Rendering frame rates

---

## 🐛 Troubleshooting

### Common Issues

**Issue: "CUDA not found"**
- Ensure CUDA Toolkit is installed
- Set `CUDA_PATH` environment variable
- Verify NVIDIA driver version

**Issue: "Failed to initialize NVML"**
- Update NVIDIA drivers to latest version
- Check if GPU is properly detected by OS
- Try running with administrator/sudo privileges

**Issue: "QML module not found"**
- Install Qt6 QuickControls and Charts modules
- Set `QML_IMPORT_PATH` environment variable
- Verify Qt installation

**Issue: "Python bridge failed"**
- Install required Python packages: `torch`, `diffusers`, `transformers`
- Ensure Python 3.10+ is in PATH
- Check `python_bridge` directory is accessible

### Debug Mode

```bash
# Build in debug mode
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

# Run with verbose logging
./AIForgeStudio --log-level=DEBUG
```

---

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📧 Contact

Project Link: [https://github.com/yourusername/AI-Forge-Studio](https://github.com/yourusername/AI-Forge-Studio)

---

## 🙏 Acknowledgments

- NVIDIA for CUDA, TensorRT, and DLSS technologies
- Qt Project for the excellent UI framework
- HuggingFace for AI model hosting
- The open-source community for various libraries and tools

---

**Built with ❤️ for RTX 50-Series GPUs**
