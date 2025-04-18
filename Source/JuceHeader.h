#pragma once

// Including all JUCE modules
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_events/juce_events.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

// Project version information
#ifndef  JUCE_APPLICATION_NAME_STRING
#define JUCE_APPLICATION_NAME_STRING "MCAM"
#endif

#ifndef  JUCE_APPLICATION_VERSION_STRING
#define JUCE_APPLICATION_VERSION_STRING "0.1.0"
#endif

namespace ProjectInfo
{
    const char* const  projectName    = JUCE_APPLICATION_NAME_STRING;
    const char* const  companyName    = "MCAM Team";
    const char* const  versionString  = JUCE_APPLICATION_VERSION_STRING;
    const int          versionNumber  = 0x010;
}
