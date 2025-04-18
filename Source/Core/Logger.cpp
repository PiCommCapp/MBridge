#include "Logger.h"
#include <ctime>

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

Logger::Logger()
    : m_minLevel(Level::Info)
    , m_initialized(false)
{
}

Logger::~Logger()
{
    if (m_fileStream != nullptr)
    {
        m_fileStream->flush();
    }
}

bool Logger::initialize(const juce::String& logFilePath, Level minLevel)
{
    const juce::ScopedLock lock(m_logMutex);

    if (m_initialized)
    {
        return true; // Already initialized
    }

    m_minLevel = minLevel;

    // Create log directory if it doesn't exist
    juce::File logFile(logFilePath);
    juce::File logDir = logFile.getParentDirectory();

    if (!logDir.exists())
    {
        logDir.createDirectory();
    }

    // Create or open the log file
    m_fileStream.reset(new juce::FileOutputStream(logFile));

    if (m_fileStream == nullptr || !m_fileStream->openedOk())
    {
        juce::String errorMsg = "Failed to open log file: " + logFilePath;
        juce::Logger::writeToLog(errorMsg);
        return false;
    }

    m_initialized = true;

    // Log initialization message
    info("Logger initialized. Log level: " + levelToString(m_minLevel));
    return true;
}

void Logger::debug(const juce::String& message)
{
    log(Level::Debug, message);
}

void Logger::info(const juce::String& message)
{
    log(Level::Info, message);
}

void Logger::warning(const juce::String& message)
{
    log(Level::Warning, message);
}

void Logger::error(const juce::String& message)
{
    log(Level::Error, message);
}

void Logger::critical(const juce::String& message)
{
    log(Level::Critical, message);
}

void Logger::setMinLevel(Level level)
{
    const juce::ScopedLock lock(m_logMutex);
    m_minLevel = level;
}

Logger::Level Logger::getMinLevel() const
{
    return m_minLevel;
}

void Logger::log(Level level, const juce::String& message)
{
    if (level < m_minLevel)
    {
        return; // Message level below minimum level
    }

    const juce::ScopedLock lock(m_logMutex);

    // Get current time
    std::time_t now = std::time(nullptr);
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    // Format the log message
    juce::String logMessage = juce::String(timeStr) + " [" + levelToString(level) + "] " + message;

    // Write to console
    juce::Logger::writeToLog(logMessage);

    // Write to file if initialized
    if (m_initialized && m_fileStream != nullptr && m_fileStream->openedOk())
    {
        m_fileStream->writeText(logMessage + juce::newLine, false, false, nullptr);
        m_fileStream->flush();
    }
}

juce::String Logger::levelToString(Level level) const
{
    switch (level)
    {
        case Level::Debug:    return "DEBUG";
        case Level::Info:     return "INFO";
        case Level::Warning:  return "WARNING";
        case Level::Error:    return "ERROR";
        case Level::Critical: return "CRITICAL";
        default:              return "UNKNOWN";
    }
}
