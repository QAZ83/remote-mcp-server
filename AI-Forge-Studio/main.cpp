/**
 * @file main.cpp
 * @brief Main entry point for AI Forge Studio
 *
 * Initializes the Qt application, sets up the QML engine, and integrates
 * C++ backend modules with the QML frontend.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>
#include <QFont>
#include <QFontDatabase>
#include <iostream>
#include <memory>

// Core modules
#include "core/logger.h"
#include "core/hardware_monitor.h"
#include "core/ai_engine.h"
#include "core/render_engine.h"

#ifdef PYTHON_AVAILABLE
#include "python_bridge/bridge.h"
#endif

using namespace AIForge;

/**
 * @class BackendController
 * @brief QObject wrapper for C++ backend, exposed to QML
 *
 * This class provides the interface between QML and C++ backend modules,
 * exposing properties and methods that can be called from QML.
 */
class BackendController : public QObject {
    Q_OBJECT

    // Properties exposed to QML
    Q_PROPERTY(float gpuUtilization READ getGpuUtilization NOTIFY metricsUpdated)
    Q_PROPERTY(float gpuTemperature READ getGpuTemperature NOTIFY metricsUpdated)
    Q_PROPERTY(int gpuMemoryUsed READ getGpuMemoryUsed NOTIFY metricsUpdated)
    Q_PROPERTY(int gpuMemoryTotal READ getGpuMemoryTotal NOTIFY metricsUpdated)
    Q_PROPERTY(float cpuUtilization READ getCpuUtilization NOTIFY metricsUpdated)
    Q_PROPERTY(bool isInitialized READ isInitialized NOTIFY initializationChanged)

public:
    explicit BackendController(QObject* parent = nullptr)
        : QObject(parent)
        , m_initialized(false)
    {
        // Initialize backend modules
        initializeBackend();
    }

    ~BackendController() {
        shutdown();
    }

    // Property getters
    float getGpuUtilization() const { return m_currentMetrics.gpuUtilization; }
    float getGpuTemperature() const { return m_currentMetrics.gpuTemperature; }
    int getGpuMemoryUsed() const { return m_currentMetrics.gpuMemoryUsed; }
    int getGpuMemoryTotal() const { return m_currentMetrics.gpuMemoryTotal; }
    float getCpuUtilization() const { return m_currentMetrics.cpuUtilization; }
    bool isInitialized() const { return m_initialized; }

public slots:
    /**
     * @brief Initialize all backend systems
     */
    void initializeBackend() {
        LOG_INFO("BackendController", "Initializing backend systems...");

        // Initialize hardware monitor
        m_hardwareMonitor = std::make_unique<HardwareMonitor>();
        if (!m_hardwareMonitor->initialize()) {
            LOG_ERROR("BackendController", "Failed to initialize hardware monitor");
            return;
        }

        // Start monitoring with callback
        m_hardwareMonitor->startMonitoring(
            [this](const SystemMetrics& metrics) {
                updateMetrics(metrics);
            },
            1000  // Update every 1 second
        );

        // Initialize AI engine
        m_aiEngine = std::make_unique<AIEngine>();
        if (!m_aiEngine->initialize(0)) {  // Device 0
            LOG_ERROR("BackendController", "Failed to initialize AI engine");
        }

        // Initialize render engine (would need window handle in production)
        m_renderEngine = std::make_unique<RenderEngine>();
        // RenderConfig renderConfig;
        // m_renderEngine->initialize(renderConfig, windowHandle);

#ifdef PYTHON_AVAILABLE
        // Initialize Python bridge
        m_pythonBridge = std::make_unique<PythonBridge>();
        if (!m_pythonBridge->initialize("./python_bridge", "cuda")) {
            LOG_WARNING("BackendController", "Failed to initialize Python bridge");
        }
#endif

        m_initialized = true;
        emit initializationChanged();

        LOG_INFO("BackendController", "Backend initialization complete");
    }

    /**
     * @brief Shutdown all backend systems
     */
    void shutdown() {
        if (!m_initialized) {
            return;
        }

        LOG_INFO("BackendController", "Shutting down backend systems...");

        if (m_hardwareMonitor) {
            m_hardwareMonitor->shutdown();
        }

        if (m_aiEngine) {
            m_aiEngine->shutdown();
        }

        if (m_renderEngine) {
            m_renderEngine->shutdown();
        }

#ifdef PYTHON_AVAILABLE
        if (m_pythonBridge) {
            m_pythonBridge->shutdown();
        }
#endif

        m_initialized = false;
        LOG_INFO("BackendController", "Backend shutdown complete");
    }

    /**
     * @brief Load an AI model
     * @param filepath Path to model file
     * @param name Display name
     * @return Model ID if successful, empty string otherwise
     */
    Q_INVOKABLE QString loadModel(const QString& filepath, const QString& name) {
        if (!m_aiEngine || !m_aiEngine->isInitialized()) {
            LOG_ERROR("BackendController", "AI engine not initialized");
            return "";
        }

        LOG_INFO("BackendController", "Loading model: " + filepath.toStdString());

        std::string modelId = m_aiEngine->loadModel(
            filepath.toStdString(),
            name.toStdString()
        );

        if (!modelId.empty()) {
            emit modelLoaded(QString::fromStdString(modelId), name);
        }

        return QString::fromStdString(modelId);
    }

    /**
     * @brief Generate image from text prompt
     * @param modelId Model to use
     * @param prompt Text prompt
     * @return true if generation started
     */
    Q_INVOKABLE bool generateImage(const QString& modelId, const QString& prompt) {
        if (!m_aiEngine || !m_aiEngine->isInitialized()) {
            LOG_ERROR("BackendController", "AI engine not initialized");
            return false;
        }

        LOG_INFO("BackendController", "Generating image: " + prompt.toStdString());

        // Create config
        InferenceConfig config;
        config.modelId = modelId.toStdString();

        // Generate image asynchronously
        auto future = m_aiEngine->runInferenceAsync(config, {});

        // In production, handle the async result properly
        // For now, just return true to indicate generation started
        emit generationStarted(modelId);

        return true;
    }

    /**
     * @brief Get list of loaded models
     * @return JSON string with model information
     */
    Q_INVOKABLE QString getLoadedModels() {
        if (!m_aiEngine || !m_aiEngine->isInitialized()) {
            return "[]";
        }

        auto models = m_aiEngine->getLoadedModels();

        // Convert to JSON string (simplified)
        QString json = "[";
        for (size_t i = 0; i < models.size(); i++) {
            const auto& model = models[i];
            if (i > 0) json += ",";
            json += QString("{\"id\":\"%1\",\"name\":\"%2\",\"type\":\"%3\"}")
                .arg(QString::fromStdString(model.id))
                .arg(QString::fromStdString(model.name))
                .arg(static_cast<int>(model.type));
        }
        json += "]";

        return json;
    }

signals:
    void metricsUpdated();
    void initializationChanged();
    void modelLoaded(const QString& modelId, const QString& name);
    void generationStarted(const QString& modelId);
    void generationComplete(const QString& imagePath);

private:
    bool m_initialized;

    // Backend modules
    std::unique_ptr<HardwareMonitor> m_hardwareMonitor;
    std::unique_ptr<AIEngine> m_aiEngine;
    std::unique_ptr<RenderEngine> m_renderEngine;

#ifdef PYTHON_AVAILABLE
    std::unique_ptr<PythonBridge> m_pythonBridge;
#endif

    // Current metrics
    struct {
        float gpuUtilization = 0.0f;
        float gpuTemperature = 0.0f;
        int gpuMemoryUsed = 0;
        int gpuMemoryTotal = 0;
        float cpuUtilization = 0.0f;
    } m_currentMetrics;

    /**
     * @brief Update metrics from hardware monitor
     */
    void updateMetrics(const SystemMetrics& metrics) {
        if (!metrics.gpus.empty()) {
            const auto& gpu = metrics.gpus[0];
            m_currentMetrics.gpuUtilization = gpu.gpuUtilization;
            m_currentMetrics.gpuTemperature = gpu.temperature;
            m_currentMetrics.gpuMemoryUsed = static_cast<int>(gpu.memoryUsed);
            m_currentMetrics.gpuMemoryTotal = static_cast<int>(gpu.memoryTotal);
        }

        m_currentMetrics.cpuUtilization = metrics.cpuUtilization;

        emit metricsUpdated();
    }
};

/**
 * @brief Main entry point
 */
int main(int argc, char *argv[])
{
    // Set application attributes
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Create application
    QGuiApplication app(argc, argv);

    // Set application info
    app.setApplicationName("AI Forge Studio");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("AI Forge Studio");

    // Initialize logger
    Logger::getInstance().setMinLogLevel(Logger::LogLevel::INFO);
    Logger::getInstance().setConsoleOutput(true);
    Logger::getInstance().setFileOutput(true);
    Logger::getInstance().setLogFilePath("ai_forge_studio.log");

    LOG_INFO("Main", "=================================================");
    LOG_INFO("Main", "AI Forge Studio - RTX 50-Series Edition");
    LOG_INFO("Main", "Version 1.0.0");
    LOG_INFO("Main", "=================================================");

    // Set QuickControls style
    QQuickStyle::setStyle("Material");

    // Create QML engine
    QQmlApplicationEngine engine;

    // Create backend controller
    BackendController backendController;

    // Expose backend to QML
    engine.rootContext()->setContextProperty("backend", &backendController);

    // Load main QML
    const QUrl url(QStringLiteral("ui/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            LOG_ERROR("Main", "Failed to load QML");
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        LOG_ERROR("Main", "No root QML objects loaded");
        return -1;
    }

    LOG_INFO("Main", "Application started successfully");
    LOG_INFO("Main", "Qt version: " + QString(qVersion()).toStdString());

    // Run application
    int result = app.exec();

    LOG_INFO("Main", "Application exiting with code: " + std::to_string(result));

    return result;
}

// Include MOC file for Q_OBJECT macro
#include "main.moc"
