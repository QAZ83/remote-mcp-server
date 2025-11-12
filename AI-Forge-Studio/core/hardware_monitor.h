/**
 * @file hardware_monitor.h
 * @brief Hardware monitoring system for AI Forge Studio
 *
 * This module provides real-time monitoring of GPU, CPU, RAM, and thermal metrics
 * using NVIDIA NVML (NVIDIA Management Library) for RTX 50-Series GPUs.
 *
 * Features:
 * - GPU utilization and memory usage tracking
 * - Temperature and power consumption monitoring
 * - Multi-GPU support
 * - Real-time data collection with configurable intervals
 */

#ifndef HARDWARE_MONITOR_H
#define HARDWARE_MONITOR_H

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>

namespace AIForge {

/**
 * @struct GPUMetrics
 * @brief Stores comprehensive GPU performance metrics
 */
struct GPUMetrics {
    unsigned int deviceId;
    std::string name;
    float gpuUtilization;      // Percentage (0-100)
    float memoryUtilization;   // Percentage (0-100)
    size_t memoryUsed;         // In MB
    size_t memoryTotal;        // In MB
    float temperature;         // In Celsius
    float powerUsage;          // In Watts
    unsigned int clockSpeed;   // In MHz
    unsigned int memoryClock;  // In MHz
    unsigned int fanSpeed;     // Percentage (0-100)
};

/**
 * @struct SystemMetrics
 * @brief Stores overall system performance metrics
 */
struct SystemMetrics {
    float cpuUtilization;      // Percentage (0-100)
    size_t ramUsed;            // In MB
    size_t ramTotal;           // In MB
    std::vector<GPUMetrics> gpus;
    std::chrono::system_clock::time_point timestamp;
};

/**
 * @class HardwareMonitor
 * @brief Main hardware monitoring class with NVML integration
 *
 * This class manages hardware monitoring operations, including initialization,
 * data collection, and cleanup of NVML resources.
 */
class HardwareMonitor {
public:
    HardwareMonitor();
    ~HardwareMonitor();

    // Disable copy and move constructors
    HardwareMonitor(const HardwareMonitor&) = delete;
    HardwareMonitor& operator=(const HardwareMonitor&) = delete;
    HardwareMonitor(HardwareMonitor&&) = delete;
    HardwareMonitor& operator=(HardwareMonitor&&) = delete;

    /**
     * @brief Initialize NVML and detect available GPUs
     * @return true if initialization successful, false otherwise
     */
    bool initialize();

    /**
     * @brief Shutdown NVML and cleanup resources
     */
    void shutdown();

    /**
     * @brief Collect current system metrics
     * @return SystemMetrics structure containing all current metrics
     */
    SystemMetrics collectMetrics();

    /**
     * @brief Get number of detected NVIDIA GPUs
     * @return Number of GPUs available
     */
    unsigned int getGPUCount() const { return m_gpuCount; }

    /**
     * @brief Check if monitor is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const { return m_initialized; }

    /**
     * @brief Start asynchronous monitoring with callback
     * @param callback Function to call with new metrics
     * @param intervalMs Update interval in milliseconds
     */
    void startMonitoring(std::function<void(const SystemMetrics&)> callback,
                        unsigned int intervalMs = 1000);

    /**
     * @brief Stop asynchronous monitoring
     */
    void stopMonitoring();

private:
    bool m_initialized;
    unsigned int m_gpuCount;
    void* m_nvmlDevices;  // Opaque pointer to NVML device handles
    bool m_monitoring;
    std::unique_ptr<class std::thread> m_monitorThread;

    /**
     * @brief Collect metrics for a specific GPU
     * @param deviceIndex Index of the GPU to query
     * @return GPUMetrics for the specified device
     */
    GPUMetrics collectGPUMetrics(unsigned int deviceIndex);

    /**
     * @brief Collect CPU utilization
     * @return CPU usage percentage
     */
    float collectCPUMetrics();

    /**
     * @brief Collect RAM usage
     * @param used Reference to store used RAM in MB
     * @param total Reference to store total RAM in MB
     */
    void collectRAMMetrics(size_t& used, size_t& total);
};

} // namespace AIForge

#endif // HARDWARE_MONITOR_H
