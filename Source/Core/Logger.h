#pragma once

#include "../JuceHeader.h"
#include <memory>
#include <fstream>

/**
 * @class Logger
 * @brief A singleton logger class for the application
 *
 * This logger provides methods for logging at different severity levels
 * and can output to both console and file.
 */
class Logger
{
public:
    /**
     * Log levels in order of severity
     */
    enum class Level
    {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    /**
     * Get the single instance of the logger
     * @return Reference to the logger instance
     */
    static Logger& getInstance();

    /**
     * Initialize the logger with a log file
     * @param logFilePath Path to the log file
     * @param minLevel Minimum level to log
     * @return true if initialization succeeded
     */
    bool initialize(const juce::String& logFilePath, Level minLevel = Level::Info);

    /**
     * Log a debug message
     * @param message The message to log
     */
    void debug(const juce::String& message);

    /**
     * Log an info message
     * @param message The message to log
     */
    void info(const juce::String& message);

    /**
     * Log a warning message
     * @param message The message to log
     */
    void warning(const juce::String& message);

    /**
     * Log an error message
     * @param message The message to log
     */
    void error(const juce::String& message);

    /**
     * Log a critical message
     * @param message The message to log
     */
    void critical(const juce::String& message);

    /**
     * Set the minimum log level
     * @param level The new minimum log level
     */
    void setMinLevel(Level level);

    /**
     * Get the current minimum log level
     * @return The current minimum log level
     */
    Level getMinLevel() const;

private:
    Logger();
    ~Logger();

    /**
     * Log a message with a specific level
     * @param level The log level
     * @param message The message to log
     */
    void log(Level level, const juce::String& message);

    /**
     * Convert a log level to a string
     * @param level The log level
     * @return String representation of the log level
     */
    juce::String levelToString(Level level) const;

    std::unique_ptr<juce::FileOutputStream> m_fileStream;
    juce::CriticalSection m_logMutex;
    Level m_minLevel;
    bool m_initialized;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Logger)
};

// Convenience macros for logging
#define LOG_DEBUG(message) Logger::getInstance().debug(message)
#define LOG_INFO(message) Logger::getInstance().info(message)
#define LOG_WARNING(message) Logger::getInstance().warning(message)
#define LOG_ERROR(message) Logger::getInstance().error(message)
#define LOG_CRITICAL(message) Logger::getInstance().critical(message)
