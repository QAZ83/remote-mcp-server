/**
 * @file logger.cpp
 * @brief Implementation of the logging system
 */

#include "logger.h"
#include <iostream>
#include <iomanip>
#include <ctime>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace AIForge {

Logger::Logger()
    : m_logFilePath("ai_forge_studio.log")
    , m_minLevel(LogLevel::INFO)
    , m_consoleOutput(true)
    , m_fileOutput(true)
{
    // Open log file
    if (m_fileOutput) {
        m_logFile.open(m_logFilePath, std::ios::app);
        if (!m_logFile.is_open()) {
            std::cerr << "Failed to open log file: " << m_logFilePath << std::endl;
            m_fileOutput = false;
        }
    }
}

Logger::~Logger() {
    flush();
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

std::string Logger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << now_ms.count();

    return ss.str();
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:    return "DEBUG";
        case LogLevel::INFO:     return "INFO";
        case LogLevel::WARNING:  return "WARNING";
        case LogLevel::ERROR:    return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}

std::string Logger::formatLogEntry(LogLevel level, const std::string& module,
                                   const std::string& message) {
    std::stringstream ss;
    ss << "[" << getCurrentTimestamp() << "] "
       << "[" << std::setw(8) << std::left << levelToString(level) << "] "
       << "[" << std::setw(20) << std::left << module << "] "
       << message;
    return ss.str();
}

void Logger::writeLog(const std::string& formattedLog) {
    // Console output
    if (m_consoleOutput) {
#ifdef _WIN32
        // On Windows, use colored output
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        WORD saved_attributes = consoleInfo.wAttributes;

        // Set color based on log level (extract from formatted log)
        if (formattedLog.find("[ERROR") != std::string::npos ||
            formattedLog.find("[CRITICAL") != std::string::npos) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        } else if (formattedLog.find("[WARNING") != std::string::npos) {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        }

        std::cout << formattedLog << std::endl;
        SetConsoleTextAttribute(hConsole, saved_attributes);
#else
        // On Linux/Unix, use ANSI color codes
        if (formattedLog.find("[ERROR") != std::string::npos ||
            formattedLog.find("[CRITICAL") != std::string::npos) {
            std::cout << "\033[1;31m" << formattedLog << "\033[0m" << std::endl;
        } else if (formattedLog.find("[WARNING") != std::string::npos) {
            std::cout << "\033[1;33m" << formattedLog << "\033[0m" << std::endl;
        } else {
            std::cout << formattedLog << std::endl;
        }
#endif
    }

    // File output
    if (m_fileOutput && m_logFile.is_open()) {
        m_logFile << formattedLog << std::endl;
    }
}

void Logger::log(LogLevel level, const std::string& module, const std::string& message) {
    // Check if level meets minimum requirement
    if (level < m_minLevel) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    std::string formattedLog = formatLogEntry(level, module, message);
    writeLog(formattedLog);
}

void Logger::setFileOutput(bool enable) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (enable && !m_fileOutput) {
        m_logFile.open(m_logFilePath, std::ios::app);
        if (m_logFile.is_open()) {
            m_fileOutput = true;
        }
    } else if (!enable && m_fileOutput) {
        if (m_logFile.is_open()) {
            m_logFile.close();
        }
        m_fileOutput = false;
    }
}

void Logger::setLogFilePath(const std::string& filepath) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_logFile.is_open()) {
        m_logFile.close();
    }

    m_logFilePath = filepath;

    if (m_fileOutput) {
        m_logFile.open(m_logFilePath, std::ios::app);
        if (!m_logFile.is_open()) {
            std::cerr << "Failed to open new log file: " << m_logFilePath << std::endl;
            m_fileOutput = false;
        }
    }
}

void Logger::flush() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_logFile.is_open()) {
        m_logFile.flush();
    }
    std::cout.flush();
}

} // namespace AIForge
