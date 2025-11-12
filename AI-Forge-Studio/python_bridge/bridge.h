/**
 * @file bridge.h
 * @brief C++ to Python bridge using Pybind11
 *
 * This module provides a C++ interface to Python AI model runners,
 * enabling seamless integration of PyTorch models with the C++ application.
 *
 * Features:
 * - Initialize Python interpreter
 * - Load and manage Python AI models
 * - Execute inference operations
 * - Transfer data between C++ and Python
 * - Exception handling and error reporting
 */

#ifndef PYTHON_BRIDGE_H
#define PYTHON_BRIDGE_H

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace AIForge {

/**
 * @struct PythonInferenceConfig
 * @brief Configuration for Python model inference
 */
struct PythonInferenceConfig {
    std::string modelId;
    std::string prompt;
    std::string negativePrompt;
    int numInferenceSteps = 50;
    float guidanceScale = 7.5f;
    int width = 512;
    int height = 512;
    int seed = -1;  // -1 for random
    int batchSize = 1;
    std::string precision = "fp16";
};

/**
 * @struct PythonInferenceResult
 * @brief Result from Python inference
 */
struct PythonInferenceResult {
    bool success;
    std::string errorMessage;
    std::vector<unsigned char> imageData;
    int imageWidth = 0;
    int imageHeight = 0;
    float inferenceTime = 0.0f;
    size_t memoryUsed = 0;
};

/**
 * @struct PythonMemoryInfo
 * @brief Python/CUDA memory information
 */
struct PythonMemoryInfo {
    size_t allocated;       // MB
    size_t reserved;        // MB
    size_t maxAllocated;    // MB
    size_t total;           // MB
};

/**
 * @class PythonBridge
 * @brief Bridge between C++ and Python for AI operations
 *
 * This class manages the Python interpreter lifecycle and provides
 * methods to call Python functions for AI model operations.
 */
class PythonBridge {
public:
    PythonBridge();
    ~PythonBridge();

    // Disable copy and move
    PythonBridge(const PythonBridge&) = delete;
    PythonBridge& operator=(const PythonBridge&) = delete;
    PythonBridge(PythonBridge&&) = delete;
    PythonBridge& operator=(PythonBridge&&) = delete;

    /**
     * @brief Initialize Python interpreter and import modules
     * @param pythonPath Path to Python scripts directory
     * @param device Device to use ("cuda" or "cpu")
     * @return true if initialization successful
     */
    bool initialize(const std::string& pythonPath, const std::string& device = "cuda");

    /**
     * @brief Shutdown Python interpreter
     */
    void shutdown();

    /**
     * @brief Load a model in Python
     * @param modelPath Path to model file or HuggingFace ID
     * @param modelId Unique identifier for model
     * @param modelType Type of model ("text_to_image", etc.)
     * @param precision Precision mode ("fp16", "fp32")
     * @return true if model loaded successfully
     */
    bool loadModel(const std::string& modelPath, const std::string& modelId,
                   const std::string& modelType, const std::string& precision = "fp16");

    /**
     * @brief Unload a model from Python
     * @param modelId Model identifier
     * @return true if successful
     */
    bool unloadModel(const std::string& modelId);

    /**
     * @brief Generate image using Python model
     * @param config Inference configuration
     * @return Result with generated image
     */
    PythonInferenceResult generateImage(const PythonInferenceConfig& config);

    /**
     * @brief Get Python/CUDA memory information
     * @return Memory info structure
     */
    PythonMemoryInfo getMemoryInfo();

    /**
     * @brief Check if bridge is initialized
     * @return true if initialized
     */
    bool isInitialized() const { return m_initialized; }

    /**
     * @brief Execute arbitrary Python code
     * @param code Python code to execute
     * @return true if successful
     */
    bool executePython(const std::string& code);

    /**
     * @brief Set progress callback for long operations
     * @param callback Function to call with progress (0.0 to 1.0)
     */
    void setProgressCallback(std::function<void(float)> callback);

private:
    bool m_initialized;
    void* m_pythonModule;       // Opaque pointer to Python module
    std::function<void(float)> m_progressCallback;

    /**
     * @brief Call a Python function and get result
     * @param functionName Name of function to call
     * @param args JSON string of arguments
     * @return JSON string result
     */
    std::string callPythonFunction(const std::string& functionName,
                                   const std::string& args);

    /**
     * @brief Decode base64 image data
     * @param base64Data Base64 encoded string
     * @return Decoded binary data
     */
    std::vector<unsigned char> decodeBase64(const std::string& base64Data);
};

} // namespace AIForge

#endif // PYTHON_BRIDGE_H
