#include "../JuceHeader.h"
#include "MainComponent.h"
#include "Logger.h"

// Application properties file name
const char* APP_PROPERTIES_FILE = "MCAMProperties.xml";
// Log file location
const char* LOG_FILE_PATH = "logs/mcam.log";
// Application name and version fallbacks
const char* APP_NAME = "Multi-Channel Audio Monitor";
const char* APP_VERSION = "0.1.0";

//==============================================================================
class MCAMApplication : public juce::JUCEApplication
{
public:
    //==============================================================================
    MCAMApplication() {}

    const juce::String getApplicationName() override
    {
        // Use our hardcoded fallback if the macro definition is empty
        juce::String name(JUCE_APPLICATION_NAME_STRING);
        return name.isEmpty() ? APP_NAME : name;
    }

    const juce::String getApplicationVersion() override
    {
        // Use our hardcoded fallback if the macro definition is empty
        juce::String version(JUCE_APPLICATION_VERSION_STRING);
        return version.isEmpty() ? APP_VERSION : version;
    }

    bool moreThanOneInstanceAllowed() override { return true; }

    //==============================================================================
    void initialise(const juce::String& commandLine) override
    {
        // Initialize logger first
        initializeLogger();

        // Log application startup
        LOG_INFO("Application starting: " + getApplicationName() + " v" + getApplicationVersion());

        // Initialize application properties
        initializeAppProperties();

        // Initialize the main window
        mainWindow.reset(new MainWindow(getApplicationName(), appProperties.get()));

        LOG_INFO("Application initialized successfully");
    }

    void shutdown() override
    {
        LOG_INFO("Application shutting down");

        // Clean up resources
        mainWindow = nullptr;
        appProperties = nullptr;

        LOG_INFO("Application shutdown complete");
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        LOG_INFO("System requested application quit");
        quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override
    {
        // Handle another instance if needed
        LOG_INFO("Another instance started with arguments: " + commandLine);
    }

    juce::ApplicationProperties* getAppProperties() const
    {
        return appProperties.get();
    }

private:
    void initializeLogger()
    {
        // Create logs directory if it doesn't exist
        juce::File logDir(juce::File::getCurrentWorkingDirectory().getChildFile("logs"));
        if (!logDir.exists())
        {
            logDir.createDirectory();
        }

        // Initialize logger
        juce::String logFilePath = logDir.getFullPathName() + "/" + LOG_FILE_PATH;
        Logger::getInstance().initialize(logFilePath, Logger::Level::Debug);
    }

    void initializeAppProperties()
    {
        appProperties.reset(new juce::ApplicationProperties());

        // Setup properties file
        juce::PropertiesFile::Options options;
        options.applicationName = getApplicationName();
        options.filenameSuffix = ".settings";
        options.osxLibrarySubFolder = "Application Support";
        options.folderName = getApplicationName();
        options.storageFormat = juce::PropertiesFile::StorageFormat::storeAsXML;

        appProperties->setStorageParameters(options);

        LOG_INFO("Application properties initialized");
    }

    //==============================================================================
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name, juce::ApplicationProperties* properties)
            : DocumentWindow(name,
                            juce::Desktop::getInstance().getDefaultLookAndFeel()
                                .findColour(juce::ResizableWindow::backgroundColourId),
                            DocumentWindow::allButtons)
        {
            LOG_INFO("Creating main window");

            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);

            // Restore window position and size from properties
            if (properties != nullptr)
            {
                auto* props = properties->getUserSettings();

                // Set default size first
                setSize(800, 600);

                // Then try to restore saved size and position
                if (props->containsKey("mainWindowX") && props->containsKey("mainWindowY"))
                {
                    int x = props->getIntValue("mainWindowX");
                    int y = props->getIntValue("mainWindowY");
                    int width = props->getIntValue("mainWindowWidth", 800);
                    int height = props->getIntValue("mainWindowHeight", 600);

                    setBoundsConstrained({x, y, width, height});
                    LOG_INFO("Restored window position: " + juce::String(x) + "," + juce::String(y) +
                             " size: " + juce::String(width) + "x" + juce::String(height));
                }
                else
                {
                    // Center window on screen
                    centreWithSize(getWidth(), getHeight());
                    LOG_INFO("Centered window with size: " + juce::String(getWidth()) + "x" + juce::String(getHeight()));
                }
            }
            else
            {
                centreWithSize(getWidth(), getHeight());
            }

            setVisible(true);
            LOG_INFO("Main window created and made visible");
        }

        void closeButtonPressed() override
        {
            LOG_INFO("Close button pressed - requesting application quit");

            // Save window position and size before closing
            auto* appProperties = dynamic_cast<MCAMApplication*>(JUCEApplication::getInstance())->getAppProperties();
            if (appProperties != nullptr)
            {
                auto* props = appProperties->getUserSettings();
                props->setValue("mainWindowX", getX());
                props->setValue("mainWindowY", getY());
                props->setValue("mainWindowWidth", getWidth());
                props->setValue("mainWindowHeight", getHeight());
                appProperties->saveIfNeeded();

                LOG_INFO("Saved window position: " + juce::String(getX()) + "," + juce::String(getY()) +
                         " size: " + juce::String(getWidth()) + "x" + juce::String(getHeight()));
            }

            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

    std::unique_ptr<juce::ApplicationProperties> appProperties;
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the application's main function
START_JUCE_APPLICATION(MCAMApplication)
