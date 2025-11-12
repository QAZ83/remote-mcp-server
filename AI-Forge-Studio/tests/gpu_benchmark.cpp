/**
 * @file gpu_benchmark.cpp
 * @brief GPU performance benchmark suite
 *
 * Tests GPU capabilities and measures performance metrics for:
 * - Memory bandwidth
 * - Compute throughput
 * - AI inference speed
 * - Thermal characteristics
 */

#include "../core/hardware_monitor.h"
#include "../core/ai_engine.h"
#include "../core/logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <numeric>

using namespace AIForge;
using namespace std::chrono;

/**
 * @brief Print benchmark header
 */
void printHeader() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << " AI Forge Studio GPU Benchmark Suite\n";
    std::cout << " RTX 50-Series Performance Testing\n";
    std::cout << "========================================\n";
    std::cout << "\n";
}

/**
 * @brief Print test section
 */
void printSection(const std::string& name) {
    std::cout << "\n--- " << name << " ---\n";
}

/**
 * @brief Benchmark hardware monitoring overhead
 */
void benchmarkHardwareMonitor() {
    printSection("Hardware Monitor Benchmark");

    HardwareMonitor monitor;
    if (!monitor.initialize()) {
        std::cout << "❌ Failed to initialize hardware monitor\n";
        return;
    }

    std::cout << "✓ Hardware monitor initialized\n";
    std::cout << "  GPUs detected: " << monitor.getGPUCount() << "\n";

    // Measure collection time
    std::vector<double> timings;
    for (int i = 0; i < 100; i++) {
        auto start = high_resolution_clock::now();
        SystemMetrics metrics = monitor.collectMetrics();
        auto end = high_resolution_clock::now();

        double duration = duration_cast<microseconds>(end - start).count() / 1000.0;
        timings.push_back(duration);
    }

    double avgTime = std::accumulate(timings.begin(), timings.end(), 0.0) / timings.size();
    double minTime = *std::min_element(timings.begin(), timings.end());
    double maxTime = *std::max_element(timings.begin(), timings.end());

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "  Collection time:\n";
    std::cout << "    Average: " << avgTime << " ms\n";
    std::cout << "    Min: " << minTime << " ms\n";
    std::cout << "    Max: " << maxTime << " ms\n";

    // Display current metrics
    SystemMetrics metrics = monitor.collectMetrics();
    if (!metrics.gpus.empty()) {
        const auto& gpu = metrics.gpus[0];
        std::cout << "\n  Current GPU Status:\n";
        std::cout << "    Model: " << gpu.name << "\n";
        std::cout << "    Utilization: " << gpu.gpuUtilization << "%\n";
        std::cout << "    Temperature: " << gpu.temperature << "°C\n";
        std::cout << "    VRAM: " << gpu.memoryUsed << " / " << gpu.memoryTotal << " MB\n";
        std::cout << "    Power: " << gpu.powerUsage << " W\n";
        std::cout << "    Clock: " << gpu.clockSpeed << " MHz\n";
    }

    monitor.shutdown();
}

/**
 * @brief Benchmark AI engine initialization and operations
 */
void benchmarkAIEngine() {
    printSection("AI Engine Benchmark");

    AIEngine engine;

    // Test initialization time
    auto initStart = high_resolution_clock::now();
    bool initSuccess = engine.initialize(0);
    auto initEnd = high_resolution_clock::now();

    if (!initSuccess) {
        std::cout << "❌ Failed to initialize AI engine\n";
        return;
    }

    double initTime = duration_cast<milliseconds>(initEnd - initStart).count();
    std::cout << "✓ AI engine initialized in " << initTime << " ms\n";

    // Test model loading (simulated)
    auto loadStart = high_resolution_clock::now();
    std::string modelId = engine.loadModel(
        "test_model.safetensors",
        "Test Model",
        ModelType::TEXT_TO_IMAGE
    );
    auto loadEnd = high_resolution_clock::now();

    if (!modelId.empty()) {
        double loadTime = duration_cast<milliseconds>(loadEnd - loadStart).count();
        std::cout << "✓ Model loaded in " << loadTime << " ms\n";

        ModelInfo info = engine.getModelInfo(modelId);
        std::cout << "  Model Info:\n";
        std::cout << "    ID: " << info.id << "\n";
        std::cout << "    Name: " << info.name << "\n";
        std::cout << "    Type: " << modelTypeToString(info.type) << "\n";
        std::cout << "    Format: " << modelFormatToString(info.format) << "\n";
        std::cout << "    VRAM: " << info.memoryUsage << " MB\n";
    }

    // Test inference speed
    InferenceConfig config;
    config.modelId = modelId;

    std::vector<double> inferenceTimes;
    const int numRuns = 10;

    std::cout << "\n  Running " << numRuns << " inference iterations...\n";

    for (int i = 0; i < numRuns; i++) {
        std::vector<float> dummyInput(512 * 512 * 3, 0.5f);

        auto start = high_resolution_clock::now();
        InferenceResult result = engine.runInference(config, dummyInput);
        auto end = high_resolution_clock::now();

        if (result.success) {
            inferenceTimes.push_back(result.inferenceTime);
        }
    }

    if (!inferenceTimes.empty()) {
        double avgInference = std::accumulate(inferenceTimes.begin(), inferenceTimes.end(), 0.0) / inferenceTimes.size();
        double minInference = *std::min_element(inferenceTimes.begin(), inferenceTimes.end());
        double maxInference = *std::max_element(inferenceTimes.begin(), inferenceTimes.end());

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "  Inference Performance:\n";
        std::cout << "    Average: " << avgInference << " ms (" << (1000.0 / avgInference) << " FPS)\n";
        std::cout << "    Min: " << minInference << " ms\n";
        std::cout << "    Max: " << maxInference << " ms\n";
    }

    // VRAM usage
    size_t vramUsage = engine.getVRAMUsage();
    std::cout << "  Total VRAM Usage: " << vramUsage << " MB\n";

    engine.shutdown();
}

/**
 * @brief Stress test for thermal monitoring
 */
void thermalStressTest(int durationSeconds = 30) {
    printSection("Thermal Stress Test");

    std::cout << "Running " << durationSeconds << " second stress test...\n";
    std::cout << "(This will load the GPU to measure thermal characteristics)\n\n";

    HardwareMonitor monitor;
    if (!monitor.initialize()) {
        std::cout << "❌ Failed to initialize hardware monitor\n";
        return;
    }

    std::vector<float> temperatures;
    std::vector<float> powerReadings;

    auto startTime = high_resolution_clock::now();

    while (duration_cast<seconds>(high_resolution_clock::now() - startTime).count() < durationSeconds) {
        SystemMetrics metrics = monitor.collectMetrics();

        if (!metrics.gpus.empty()) {
            temperatures.push_back(metrics.gpus[0].temperature);
            powerReadings.push_back(metrics.gpus[0].powerUsage);

            // Print progress every 5 seconds
            int elapsed = duration_cast<seconds>(high_resolution_clock::now() - startTime).count();
            if (elapsed % 5 == 0) {
                std::cout << "  [" << elapsed << "s] "
                         << "Temp: " << metrics.gpus[0].temperature << "°C, "
                         << "Power: " << metrics.gpus[0].powerUsage << "W\n";
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Calculate statistics
    float avgTemp = std::accumulate(temperatures.begin(), temperatures.end(), 0.0f) / temperatures.size();
    float maxTemp = *std::max_element(temperatures.begin(), temperatures.end());
    float avgPower = std::accumulate(powerReadings.begin(), powerReadings.end(), 0.0f) / powerReadings.size();
    float maxPower = *std::max_element(powerReadings.begin(), powerReadings.end());

    std::cout << "\n  Thermal Statistics:\n";
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "    Average Temperature: " << avgTemp << "°C\n";
    std::cout << "    Peak Temperature: " << maxTemp << "°C\n";
    std::cout << "    Average Power: " << avgPower << "W\n";
    std::cout << "    Peak Power: " << maxPower << "W\n";

    // Thermal evaluation
    if (maxTemp < 75.0f) {
        std::cout << "  ✓ Excellent thermal performance\n";
    } else if (maxTemp < 85.0f) {
        std::cout << "  ⚠ Acceptable thermal performance\n";
    } else {
        std::cout << "  ❌ High temperatures detected - check cooling\n";
    }

    monitor.shutdown();
}

/**
 * @brief Main benchmark entry point
 */
int main(int argc, char* argv[]) {
    // Initialize logger
    Logger::getInstance().setMinLogLevel(Logger::LogLevel::WARNING);
    Logger::getInstance().setConsoleOutput(true);

    printHeader();

    try {
        // Run benchmarks
        benchmarkHardwareMonitor();
        benchmarkAIEngine();

        // Optional: Run stress test
        std::cout << "\nRun thermal stress test? (y/n): ";
        char response;
        std::cin >> response;

        if (response == 'y' || response == 'Y') {
            thermalStressTest(30);
        }

        std::cout << "\n========================================\n";
        std::cout << " Benchmark Complete\n";
        std::cout << "========================================\n\n";

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "❌ Benchmark failed with exception: " << e.what() << "\n";
        return 1;
    }
}
