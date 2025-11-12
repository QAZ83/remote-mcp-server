/**
 * @file hardware_monitor.cpp
 * @brief Implementation of hardware monitoring system
 *
 * This implementation uses NVML for GPU monitoring and platform-specific APIs
 * for CPU/RAM monitoring. For production use, link against libnvidia-ml.
 */

#include "hardware_monitor.h"
#include "logger.h"
#include <thread>
#include <atomic>
#include <cstring>

// Platform-specific includes
#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#else
    #include <unistd.h>
    #include <sys/sysinfo.h>
    #include <fstream>
#endif

// NVML headers (would be included in production build)
// For this template, we'll simulate NVML functionality
// In production: #include <nvml.h>

namespace AIForge {

// NVML function stubs (replace with actual NVML calls in production)
struct NVMLDevice {
    std::string name;
    unsigned int index;
};

HardwareMonitor::HardwareMonitor()
    : m_initialized(false)
    , m_gpuCount(0)
    , m_nvmlDevices(nullptr)
    , m_monitoring(false)
    , m_monitorThread(nullptr)
{
    Logger::getInstance().log(Logger::LogLevel::INFO, "HardwareMonitor",
                              "Hardware monitor created");
}

HardwareMonitor::~HardwareMonitor() {
    shutdown();
}

bool HardwareMonitor::initialize() {
    if (m_initialized) {
        Logger::getInstance().log(Logger::LogLevel::WARNING, "HardwareMonitor",
                                  "Already initialized");
        return true;
    }

    Logger::getInstance().log(Logger::LogLevel::INFO, "HardwareMonitor",
                              "Initializing hardware monitor...");

    // Initialize NVML
    // In production: nvmlReturn_t result = nvmlInit();
    // For template, simulate successful initialization
    bool nvmlInitSuccess = true;

    if (!nvmlInitSuccess) {
        Logger::getInstance().log(Logger::LogLevel::ERROR, "HardwareMonitor",
                                  "Failed to initialize NVML");
        return false;
    }

    // Get GPU count
    // In production: nvmlDeviceGetCount(&m_gpuCount);
    // For template, simulate detection
    m_gpuCount = 1; // Simulate 1 GPU detected

    Logger::getInstance().log(Logger::LogLevel::INFO, "HardwareMonitor",
                              "Detected " + std::to_string(m_gpuCount) + " GPU(s)");

    // Allocate device handles
    if (m_gpuCount > 0) {
        m_nvmlDevices = new NVMLDevice[m_gpuCount];

        // Initialize device handles
        for (unsigned int i = 0; i < m_gpuCount; i++) {
            // In production: nvmlDeviceGetHandleByIndex(i, &handle);
            NVMLDevice* devices = static_cast<NVMLDevice*>(m_nvmlDevices);
            devices[i].name = "NVIDIA GeForce RTX 5090";
            devices[i].index = i;
        }
    }

    m_initialized = true;
    Logger::getInstance().log(Logger::LogLevel::INFO, "HardwareMonitor",
                              "Hardware monitor initialized successfully");
    return true;
}

void HardwareMonitor::shutdown() {
    if (!m_initialized) {
        return;
    }

    stopMonitoring();

    // Cleanup device handles
    if (m_nvmlDevices) {
        delete[] static_cast<NVMLDevice*>(m_nvmlDevices);
        m_nvmlDevices = nullptr;
    }

    // Shutdown NVML
    // In production: nvmlShutdown();

    m_initialized = false;
    Logger::getInstance().log(Logger::LogLevel::INFO, "HardwareMonitor",
                              "Hardware monitor shutdown complete");
}

GPUMetrics HardwareMonitor::collectGPUMetrics(unsigned int deviceIndex) {
    GPUMetrics metrics = {};

    if (!m_initialized || deviceIndex >= m_gpuCount) {
        return metrics;
    }

    NVMLDevice* devices = static_cast<NVMLDevice*>(m_nvmlDevices);
    metrics.deviceId = deviceIndex;
    metrics.name = devices[deviceIndex].name;

    // In production, use actual NVML calls:
    // nvmlDeviceGetUtilizationRates(device, &utilization);
    // nvmlDeviceGetMemoryInfo(device, &memInfo);
    // nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &temp);
    // nvmlDeviceGetPowerUsage(device, &power);
    // nvmlDeviceGetClockInfo(device, NVML_CLOCK_GRAPHICS, &clock);
    // nvmlDeviceGetFanSpeed(device, &fanSpeed);

    // Simulate realistic metrics for RTX 50-Series
    metrics.gpuUtilization = 45.0f + (rand() % 30);
    metrics.memoryUtilization = 60.0f + (rand() % 20);
    metrics.memoryUsed = 8192 + (rand() % 4096);
    metrics.memoryTotal = 24576; // 24GB for RTX 5090
    metrics.temperature = 55.0f + (rand() % 20);
    metrics.powerUsage = 350.0f + (rand() % 100);
    metrics.clockSpeed = 2400 + (rand() % 300);
    metrics.memoryClock = 10500 + (rand() % 500);
    metrics.fanSpeed = 40 + (rand() % 30);

    return metrics;
}

float HardwareMonitor::collectCPUMetrics() {
    // Platform-specific CPU utilization collection
    float cpuUsage = 0.0f;

#ifdef _WIN32
    // Windows implementation using PDH (Performance Data Helper)
    // For simplicity, simulate
    cpuUsage = 30.0f + (rand() % 40);
#else
    // Linux implementation reading /proc/stat
    static unsigned long long lastTotalUser = 0, lastTotalUserLow = 0;
    static unsigned long long lastTotalSys = 0, lastTotalIdle = 0;

    std::ifstream procStat("/proc/stat");
    if (procStat.is_open()) {
        std::string cpu;
        unsigned long long totalUser, totalUserLow, totalSys, totalIdle;

        procStat >> cpu >> totalUser >> totalUserLow >> totalSys >> totalIdle;
        procStat.close();

        if (lastTotalUser != 0) {
            unsigned long long total = (totalUser - lastTotalUser) +
                                      (totalUserLow - lastTotalUserLow) +
                                      (totalSys - lastTotalSys);
            unsigned long long idle = totalIdle - lastTotalIdle;

            cpuUsage = (total > 0) ? (100.0f * total / (total + idle)) : 0.0f;
        }

        lastTotalUser = totalUser;
        lastTotalUserLow = totalUserLow;
        lastTotalSys = totalSys;
        lastTotalIdle = totalIdle;
    } else {
        // Fallback simulation
        cpuUsage = 30.0f + (rand() % 40);
    }
#endif

    return cpuUsage;
}

void HardwareMonitor::collectRAMMetrics(size_t& used, size_t& total) {
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    total = memInfo.ullTotalPhys / (1024 * 1024); // Convert to MB
    used = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024 * 1024);
#else
    struct sysinfo memInfo;
    sysinfo(&memInfo);

    total = memInfo.totalram / (1024 * 1024); // Convert to MB
    used = (memInfo.totalram - memInfo.freeram) / (1024 * 1024);
#endif
}

SystemMetrics HardwareMonitor::collectMetrics() {
    SystemMetrics metrics;
    metrics.timestamp = std::chrono::system_clock::now();

    if (!m_initialized) {
        Logger::getInstance().log(Logger::LogLevel::WARNING, "HardwareMonitor",
                                  "Cannot collect metrics: not initialized");
        return metrics;
    }

    // Collect CPU metrics
    metrics.cpuUtilization = collectCPUMetrics();

    // Collect RAM metrics
    collectRAMMetrics(metrics.ramUsed, metrics.ramTotal);

    // Collect GPU metrics for all devices
    metrics.gpus.reserve(m_gpuCount);
    for (unsigned int i = 0; i < m_gpuCount; i++) {
        metrics.gpus.push_back(collectGPUMetrics(i));
    }

    return metrics;
}

void HardwareMonitor::startMonitoring(std::function<void(const SystemMetrics&)> callback,
                                     unsigned int intervalMs) {
    if (m_monitoring) {
        Logger::getInstance().log(Logger::LogLevel::WARNING, "HardwareMonitor",
                                  "Monitoring already active");
        return;
    }

    m_monitoring = true;

    m_monitorThread = std::make_unique<std::thread>([this, callback, intervalMs]() {
        Logger::getInstance().log(Logger::LogLevel::INFO, "HardwareMonitor",
                                  "Monitoring thread started");

        while (m_monitoring) {
            SystemMetrics metrics = collectMetrics();
            callback(metrics);
            std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        }

        Logger::getInstance().log(Logger::LogLevel::INFO, "HardwareMonitor",
                                  "Monitoring thread stopped");
    });
}

void HardwareMonitor::stopMonitoring() {
    if (!m_monitoring) {
        return;
    }

    m_monitoring = false;

    if (m_monitorThread && m_monitorThread->joinable()) {
        m_monitorThread->join();
    }

    m_monitorThread.reset();
}

} // namespace AIForge
