/**
 * @file ai_engine.h
 * @brief AI inference engine with CUDA and TensorRT acceleration
 *
 * This module manages AI model loading, inference, and optimization using
 * NVIDIA's CUDA and TensorRT frameworks. Supports multiple model formats
 * including ONNX, TensorRT engines, and PyTorch models.
 *
 * Features:
 * - Multi-model management (Text-to-Image, LLM, Diffusion models)
 * - TensorRT optimization and engine serialization
 * - CUDA stream management for async inference
 * - Dynamic batching and memory management
 * - Support for .safetensors and .gguf formats
 * - FP16/INT8 quantization support
 */

#ifndef AI_ENGINE_H
#define AI_ENGINE_H

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include <future>

namespace AIForge {

/**
 * @enum ModelType
 * @brief Supported AI model types
 */
enum class ModelType {
    TEXT_TO_IMAGE,      // Stable Diffusion, DALL-E style models
    IMAGE_TO_IMAGE,     // Style transfer, enhancement
    TEXT_GENERATION,    // LLM for text generation
    IMAGE_UPSCALING,    // Super-resolution models
    UNKNOWN
};

/**
 * @enum ModelFormat
 * @brief Supported model file formats
 */
enum class ModelFormat {
    ONNX,               // Open Neural Network Exchange
    TENSORRT,           // TensorRT engine file
    PYTORCH,            // PyTorch .pt/.pth files
    SAFETENSORS,        // HuggingFace safe format
    GGUF,               // GPT-Generated Unified Format
    UNKNOWN
};

/**
 * @enum PrecisionMode
 * @brief Inference precision modes
 */
enum class PrecisionMode {
    FP32,               // 32-bit floating point
    FP16,               // 16-bit floating point (faster)
    INT8,               // 8-bit integer (fastest, requires calibration)
    AUTO                // Automatic selection based on hardware
};

/**
 * @struct ModelInfo
 * @brief Metadata for loaded AI models
 */
struct ModelInfo {
    std::string id;             // Unique identifier
    std::string name;           // Display name
    std::string filepath;       // Path to model file
    ModelType type;
    ModelFormat format;
    size_t memoryUsage;         // VRAM usage in MB
    std::vector<int> inputShape;
    std::vector<int> outputShape;
    bool isLoaded;
    bool isOptimized;           // TensorRT optimized
};

/**
 * @struct InferenceConfig
 * @brief Configuration for inference operations
 */
struct InferenceConfig {
    std::string modelId;
    PrecisionMode precision = PrecisionMode::FP16;
    int batchSize = 1;
    int maxTokens = 512;        // For text generation
    float temperature = 1.0f;   // For sampling
    int numInferenceSteps = 50; // For diffusion models
    float guidanceScale = 7.5f; // For guided diffusion
    unsigned int seed = 0;      // Random seed (0 = random)
    bool useVRAMOffload = false; // Offload to system RAM if needed
};

/**
 * @struct InferenceResult
 * @brief Result of an inference operation
 */
struct InferenceResult {
    bool success;
    std::string errorMessage;
    std::vector<float> outputData;      // Raw output tensor
    std::vector<unsigned char> imageData; // For image outputs
    int imageWidth = 0;
    int imageHeight = 0;
    int imageChannels = 0;
    float inferenceTime = 0.0f;         // In milliseconds
    size_t memoryUsed = 0;              // Peak VRAM used in MB
};

/**
 * @class AIEngine
 * @brief Main AI inference engine class
 *
 * Manages CUDA contexts, TensorRT engines, and model lifecycle.
 * Provides both synchronous and asynchronous inference capabilities.
 */
class AIEngine {
public:
    AIEngine();
    ~AIEngine();

    // Disable copy and move
    AIEngine(const AIEngine&) = delete;
    AIEngine& operator=(const AIEngine&) = delete;
    AIEngine(AIEngine&&) = delete;
    AIEngine& operator=(AIEngine&&) = delete;

    /**
     * @brief Initialize CUDA and TensorRT
     * @param deviceId GPU device ID to use (default: 0)
     * @return true if initialization successful
     */
    bool initialize(int deviceId = 0);

    /**
     * @brief Shutdown and cleanup resources
     */
    void shutdown();

    /**
     * @brief Load an AI model from file
     * @param filepath Path to model file
     * @param name Display name for the model
     * @param type Type of model (auto-detected if UNKNOWN)
     * @return Model ID if successful, empty string otherwise
     */
    std::string loadModel(const std::string& filepath, const std::string& name,
                          ModelType type = ModelType::UNKNOWN);

    /**
     * @brief Unload a model from memory
     * @param modelId ID of model to unload
     * @return true if successful
     */
    bool unloadModel(const std::string& modelId);

    /**
     * @brief Optimize model with TensorRT
     * @param modelId ID of model to optimize
     * @param precision Target precision mode
     * @return true if optimization successful
     */
    bool optimizeModel(const std::string& modelId, PrecisionMode precision);

    /**
     * @brief Get information about a loaded model
     * @param modelId ID of model
     * @return ModelInfo structure
     */
    ModelInfo getModelInfo(const std::string& modelId) const;

    /**
     * @brief Get list of all loaded models
     * @return Vector of ModelInfo structures
     */
    std::vector<ModelInfo> getLoadedModels() const;

    /**
     * @brief Run synchronous inference
     * @param config Inference configuration
     * @param inputData Input tensor data
     * @return InferenceResult with output
     */
    InferenceResult runInference(const InferenceConfig& config,
                                const std::vector<float>& inputData);

    /**
     * @brief Run asynchronous inference
     * @param config Inference configuration
     * @param inputData Input tensor data
     * @return Future containing InferenceResult
     */
    std::future<InferenceResult> runInferenceAsync(const InferenceConfig& config,
                                                   const std::vector<float>& inputData);

    /**
     * @brief Generate image from text prompt
     * @param modelId Text-to-image model ID
     * @param prompt Text description
     * @param config Additional inference configuration
     * @return InferenceResult with generated image
     */
    InferenceResult generateImage(const std::string& modelId,
                                 const std::string& prompt,
                                 const InferenceConfig& config);

    /**
     * @brief Upscale an image using AI
     * @param modelId Upscaling model ID
     * @param inputImage Input image data (HWC format)
     * @param width Input image width
     * @param height Input image height
     * @param scaleFactor Upscaling factor (2x, 4x, etc.)
     * @return InferenceResult with upscaled image
     */
    InferenceResult upscaleImage(const std::string& modelId,
                                const std::vector<unsigned char>& inputImage,
                                int width, int height, int scaleFactor);

    /**
     * @brief Check if engine is initialized
     * @return true if initialized
     */
    bool isInitialized() const { return m_initialized; }

    /**
     * @brief Get current VRAM usage
     * @return VRAM used by AI engine in MB
     */
    size_t getVRAMUsage() const;

    /**
     * @brief Set progress callback for long operations
     * @param callback Function called with progress (0.0 to 1.0)
     */
    void setProgressCallback(std::function<void(float)> callback);

private:
    bool m_initialized;
    int m_deviceId;
    void* m_cudaContext;        // Opaque CUDA context
    void* m_tensorrtContext;    // Opaque TensorRT context
    std::map<std::string, std::unique_ptr<class AIModel>> m_models;
    std::function<void(float)> m_progressCallback;

    /**
     * @brief Detect model type from file
     * @param filepath Path to model file
     * @return Detected ModelType
     */
    ModelType detectModelType(const std::string& filepath);

    /**
     * @brief Detect model format from file
     * @param filepath Path to model file
     * @return Detected ModelFormat
     */
    ModelFormat detectModelFormat(const std::string& filepath);

    /**
     * @brief Generate unique model ID
     * @return Unique ID string
     */
    std::string generateModelId();

    /**
     * @brief Allocate CUDA memory
     * @param size Size in bytes
     * @return Pointer to allocated memory
     */
    void* allocateCudaMemory(size_t size);

    /**
     * @brief Free CUDA memory
     * @param ptr Pointer to free
     */
    void freeCudaMemory(void* ptr);
};

/**
 * @brief Helper function to convert ModelType to string
 */
std::string modelTypeToString(ModelType type);

/**
 * @brief Helper function to convert ModelFormat to string
 */
std::string modelFormatToString(ModelFormat format);

/**
 * @brief Helper function to convert PrecisionMode to string
 */
std::string precisionModeToString(PrecisionMode mode);

} // namespace AIForge

#endif // AI_ENGINE_H
