/**
 * @file render_engine.h
 * @brief Advanced rendering engine with Vulkan/DirectX 12 support
 *
 * This module provides high-performance graphics rendering with support for:
 * - 4K+ real-time preview
 * - NVIDIA DLSS 3.5 integration
 * - Ray tracing effects
 * - HDR rendering
 * - Multi-threaded command buffer generation
 *
 * Designed specifically for NVIDIA RTX 50-Series GPUs to showcase
 * AI-generated imagery with maximum visual fidelity.
 */

#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace AIForge {

/**
 * @enum RenderAPI
 * @brief Supported graphics APIs
 */
enum class RenderAPI {
    VULKAN,
    DIRECTX12,
    AUTO  // Auto-select based on platform
};

/**
 * @enum RenderQuality
 * @brief Render quality presets
 */
enum class RenderQuality {
    LOW,        // 1080p, basic features
    MEDIUM,     // 1440p, some RT effects
    HIGH,       // 4K, full RT
    ULTRA       // 4K+, all effects, DLSS
};

/**
 * @struct RenderConfig
 * @brief Configuration for render engine initialization
 */
struct RenderConfig {
    RenderAPI api = RenderAPI::VULKAN;
    int displayWidth = 1920;
    int displayHeight = 1080;
    bool enableVSync = true;
    bool enableHDR = true;
    bool enableRayTracing = true;
    bool enableDLSS = true;
    RenderQuality quality = RenderQuality::HIGH;
    int maxFramesInFlight = 2;
    bool fullscreen = false;
};

/**
 * @struct RenderStats
 * @brief Frame rendering statistics
 */
struct RenderStats {
    float frameTime;        // ms
    float fps;
    int drawCalls;
    int triangles;
    size_t vramUsed;        // MB
    bool dlssActive;
    bool rayTracingActive;
};

/**
 * @struct ImageRenderData
 * @brief Data for rendering an image to screen
 */
struct ImageRenderData {
    unsigned char* data;
    int width;
    int height;
    int channels;
    bool needsUpdate;
    void* gpuTexture;       // Opaque GPU texture handle
};

/**
 * @class RenderEngine
 * @brief Main rendering engine managing Vulkan/DX12 pipeline
 *
 * Handles device initialization, swapchain management, command buffer
 * recording, and presentation. Integrates NVIDIA-specific features
 * like DLSS and ray tracing.
 */
class RenderEngine {
public:
    RenderEngine();
    ~RenderEngine();

    // Disable copy and move
    RenderEngine(const RenderEngine&) = delete;
    RenderEngine& operator=(const RenderEngine&) = delete;
    RenderEngine(RenderEngine&&) = delete;
    RenderEngine& operator=(RenderEngine&&) = delete;

    /**
     * @brief Initialize render engine with configuration
     * @param config Render configuration
     * @param windowHandle Platform-specific window handle
     * @return true if initialization successful
     */
    bool initialize(const RenderConfig& config, void* windowHandle);

    /**
     * @brief Shutdown and cleanup all resources
     */
    void shutdown();

    /**
     * @brief Begin a new frame
     * @return true if ready to render
     */
    bool beginFrame();

    /**
     * @brief End frame and present to screen
     * @return true if presentation successful
     */
    bool endFrame();

    /**
     * @brief Render an image to the display
     * @param imageData Image data to render
     * @param x X position on screen
     * @param y Y position on screen
     * @param scale Scaling factor
     */
    void renderImage(const ImageRenderData& imageData,
                    float x, float y, float scale = 1.0f);

    /**
     * @brief Upload image data to GPU
     * @param imageData Image data to upload
     * @return GPU texture handle
     */
    void* uploadImageToGPU(const unsigned char* data, int width,
                          int height, int channels);

    /**
     * @brief Free GPU texture
     * @param texture GPU texture handle to free
     */
    void freeGPUTexture(void* texture);

    /**
     * @brief Clear the screen with color
     * @param r Red component (0-1)
     * @param g Green component (0-1)
     * @param b Blue component (0-1)
     * @param a Alpha component (0-1)
     */
    void clearScreen(float r, float g, float b, float a = 1.0f);

    /**
     * @brief Render UI overlay (for debug info, etc.)
     * @param text Text to render
     * @param x X position
     * @param y Y position
     */
    void renderText(const std::string& text, float x, float y);

    /**
     * @brief Get current render statistics
     * @return RenderStats structure
     */
    RenderStats getStats() const { return m_stats; }

    /**
     * @brief Check if DLSS is available
     * @return true if DLSS supported
     */
    bool isDLSSAvailable() const { return m_dlssAvailable; }

    /**
     * @brief Enable or disable DLSS
     * @param enable true to enable DLSS
     * @return true if state changed successfully
     */
    bool setDLSS(bool enable);

    /**
     * @brief Check if ray tracing is available
     * @return true if RT supported
     */
    bool isRayTracingAvailable() const { return m_rayTracingAvailable; }

    /**
     * @brief Enable or disable ray tracing
     * @param enable true to enable RT
     * @return true if state changed successfully
     */
    bool setRayTracing(bool enable);

    /**
     * @brief Resize render targets
     * @param width New width
     * @param height New height
     */
    void resize(int width, int height);

    /**
     * @brief Check if engine is initialized
     * @return true if initialized
     */
    bool isInitialized() const { return m_initialized; }

    /**
     * @brief Wait for GPU to finish all operations
     */
    void waitIdle();

    /**
     * @brief Take screenshot of current frame
     * @param filepath Path to save screenshot
     * @return true if successful
     */
    bool captureScreenshot(const std::string& filepath);

private:
    bool m_initialized;
    RenderConfig m_config;
    RenderStats m_stats;

    // Vulkan/DX12 resources (opaque pointers)
    void* m_instance;           // VkInstance or ID3D12Device
    void* m_device;             // VkDevice or ID3D12Device
    void* m_queue;              // VkQueue or ID3D12CommandQueue
    void* m_swapchain;          // VkSwapchainKHR or IDXGISwapChain
    void* m_commandBuffer;      // VkCommandBuffer or ID3D12CommandList
    void* m_renderPass;         // VkRenderPass or render target
    std::vector<void*> m_framebuffers;

    // Feature availability
    bool m_dlssAvailable;
    bool m_dlssEnabled;
    bool m_rayTracingAvailable;
    bool m_rayTracingEnabled;

    // Frame tracking
    uint32_t m_currentFrame;
    uint32_t m_imageIndex;
    std::chrono::high_resolution_clock::time_point m_lastFrameTime;

    /**
     * @brief Initialize Vulkan
     * @param windowHandle Platform window handle
     * @return true if successful
     */
    bool initializeVulkan(void* windowHandle);

    /**
     * @brief Initialize DirectX 12
     * @param windowHandle Platform window handle
     * @return true if successful
     */
    bool initializeDirectX12(void* windowHandle);

    /**
     * @brief Create swapchain
     * @return true if successful
     */
    bool createSwapchain();

    /**
     * @brief Create render pass and framebuffers
     * @return true if successful
     */
    bool createRenderPass();

    /**
     * @brief Initialize DLSS
     * @return true if available and initialized
     */
    bool initializeDLSS();

    /**
     * @brief Check ray tracing support
     * @return true if supported
     */
    bool checkRayTracingSupport();

    /**
     * @brief Update render statistics
     */
    void updateStats();
};

} // namespace AIForge

#endif // RENDER_ENGINE_H
