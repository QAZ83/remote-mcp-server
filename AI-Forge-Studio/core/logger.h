/**
 * @file logger.h
 * @brief Thread-safe logging system for AI Forge Studio
 *
 * Provides a centralized logging mechanism with multiple severity levels,
 * file output, console output, and automatic log rotation.
 *
 * Features:
 * - Thread-safe singleton pattern
 * - Multiple log levels (DEBUG, INFO, WARNING, ERROR, CRITICAL)
 * - Timestamped log entries
 * - Console and file output
 * - Automatic log file rotation
 * - Performance monitoring integration
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <chrono>
#include <sstream>

namespace AIForge {

/**
 * @class Logger
 * @brief Singleton logger class with thread-safe operations
 *
 * Usage:
 * Logger::getInstance().log(Logger::LogLevel::INFO, "Module", "Message");
 */
class Logger {
public:
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    /**
     * @brief Get singleton instance
     * @return Reference to the logger instance
     */
    static Logger& getInstance();

    // Disable copy and move
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    /**
     * @brief Log a message with specified level
     * @param level Severity level of the message
     * @param module Name of the module generating the log
     * @param message The log message
     */
    void log(LogLevel level, const std::string& module, const std::string& message);

    /**
     * @brief Set minimum log level to display/record
     * @param level Minimum level (messages below this are ignored)
     */
    void setMinLogLevel(LogLevel level) { m_minLevel = level; }

    /**
     * @brief Enable or disable console output
     * @param enable true to enable console output, false to disable
     */
    void setConsoleOutput(bool enable) { m_consoleOutput = enable; }

    /**
     * @brief Enable or disable file output
     * @param enable true to enable file output, false to disable
     */
    void setFileOutput(bool enable);

    /**
     * @brief Set the log file path
     * @param filepath Path to the log file
     */
    void setLogFilePath(const std::string& filepath);

    /**
     * @brief Flush all pending log entries to file
     */
    void flush();

    /**
     * @brief Get string representation of log level
     * @param level The log level
     * @return String representation
     */
    static std::string levelToString(LogLevel level);

    /**
     * @brief Get current timestamp as formatted string
     * @return Timestamp string in format "YYYY-MM-DD HH:MM:SS.mmm"
     */
    static std::string getCurrentTimestamp();

private:
    Logger();
    ~Logger();

    std::mutex m_mutex;
    std::ofstream m_logFile;
    std::string m_logFilePath;
    LogLevel m_minLevel;
    bool m_consoleOutput;
    bool m_fileOutput;

    /**
     * @brief Format a log entry
     * @param level Log level
     * @param module Module name
     * @param message Log message
     * @return Formatted log string
     */
    std::string formatLogEntry(LogLevel level, const std::string& module,
                               const std::string& message);

    /**
     * @brief Write formatted log to outputs
     * @param formattedLog The formatted log string
     */
    void writeLog(const std::string& formattedLog);
};

/**
 * @brief Helper macro for debug logging
 */
#define LOG_DEBUG(module, message) \
    Logger::getInstance().log(Logger::LogLevel::DEBUG, module, message)

#define LOG_INFO(module, message) \
    Logger::getInstance().log(Logger::LogLevel::INFO, module, message)

#define LOG_WARNING(module, message) \
    Logger::getInstance().log(Logger::LogLevel::WARNING, module, message)

#define LOG_ERROR(module, message) \
    Logger::getInstance().log(Logger::LogLevel::ERROR, module, message)

#define LOG_CRITICAL(module, message) \
    Logger::getInstance().log(Logger::LogLevel::CRITICAL, module, message)

} // namespace AIForge

#endif // LOGGER_H
