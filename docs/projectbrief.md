# Project Brief: Multi-Channel Audio Metering and RTA Application

**1. Project Title:** Multi-Channel Audio Monitor (MCAM)

**2. Project Goal:** To create a cross-platform native application for monitoring up to four selectable audio channels from a potentially larger set of available input channels. The application will feature VU/PPM meters and Real-Time Analyzers (RTAs) for each selected channel. It will also include a REST API for external control of the selected audio sources.

**3. Target Platforms:**
    * Windows
    * macOS
    * Linux

**4. Core Functionality:**
    * **Audio Input:** Accept multi-channel audio input from standard system audio interfaces (e.g., devices exposed by Dante Virtual Soundcard, USB audio interfaces). The application should be capable of recognizing up to 32 input channels. Additionally, the application should support audio loopback functionality, allowing users to monitor the computer's audio outputs in addition to inputs.
    * **Channel Selection:** Allow the user to independently select one of the available input channels for each of the four monitoring slots via dropdown menus.
    * **Metering:** Display a selectable meter type (VU or PPM) for each of the four selected audio channels. Metering should adhere to standard ballistics for each type.
    * **Real-Time Analysis (RTA):** Display a Real-Time Analyzer (frequency spectrum) for each of the four selected audio channels, stacked vertically on the screen.
    * **REST API:** Provide a simple REST API to allow external applications (e.g., Bitfocus Companion) to query and set the currently selected audio input channel for each of the four monitoring slots.

**5. Proposed Architecture:**

* **Language:** C++
* **GUI Framework & Audio Processing:** JUCE Framework (v7 or later)
    * **Reasoning:** JUCE provides excellent cross-platform support for audio I/O, DSP, and GUI development within a single, coherent framework. Its active community and comprehensive documentation are beneficial. JUCE's licensing allows for the creation of FOSS applications under specific conditions (GPL or commercial licensing options exist). We will ensure all JUCE modules used are compatible with a FOSS license.
* **Audio I/O:** JUCE's `AudioDeviceManager` and `AudioIODeviceCallback` classes will be used to interface with the system's audio input devices, including those exposed by Dante Virtual Soundcard.
* **DSP:** JUCE's built-in `dsp` module will be used for:
    * RMS calculation for VU meters.
    * Peak detection for PPM meters.
    * FFT for Real-Time Analysis.
    * Windowing functions for FFT.
* **GUI:** JUCE's component-based GUI system will be used to create:
    * Dropdown menus for channel selection.
    * Dropdown menus for meter type selection.
    * Custom components for VU/PPM meter visualizations.
    * Custom components for RTA visualizations.
* **REST API:** JUCE's networking classes (`ServerSocket`, `Socket`, `URL`) will be used to implement a basic HTTP server for the REST API. A separate thread will handle API requests to avoid blocking the audio processing and GUI threads. Data exchange will likely be in JSON format (using a suitable FOSS JSON library integrated with JUCE if needed, or JUCE's `JSON` class).

**6. Audio Standards:**

* **VU Meter:** Adherence to standard VU meter ballistics (attack and release times). Calibration to a reference level (e.g., -20 dBFS = 0 VU).
* **PPM Meter:** Support for a standard PPM scale (e.g., EBU PPM). Fast attack time and slower release time with peak hold indication.
* **Real-Time Analyzer (RTA):** Display of frequency spectrum, likely on a logarithmic frequency scale. Options for different windowing functions (e.g., Hann). Potentially include options for averaging or peak hold on the RTA display.

**7. Free and Open Source Software (FOSS) Considerations:**

* **JUCE Licensing:** The application will be developed in a way that adheres to a FOSS license compatible with JUCE (likely GPLv2 or GPLv3, depending on the JUCE modules used and project requirements). We will carefully select JUCE modules that are permissively licensed.
* **External Libraries:** Any external libraries used (e.g., for JSON parsing if JUCE's is insufficient) will be chosen based on their FOSS compatibility.
* **No Proprietary Code:** All code developed for this project will be licensed under a suitable FOSS license.

**8. AES67 Operability:**

* Direct AES67 discovery and connection will be avoided in the initial phase to maintain simplicity.
* **Installation Option:** The application could provide a link or a menu option that directs the user to instructions and/or software (e.g., specific sound card drivers or third-party software like Dante Virtual Soundcard) that enable AES67 streams to be presented as standard audio input devices on the operating system. The application will then interface with these devices through the OS audio APIs via JUCE.

**9. User Interface Details:**

* **Main Window:** A single window containing the four monitoring slots arranged vertically or horizontally.
* **Channel Selection:** For each of the four slots, a dropdown menu will list the available audio input channels (e.g., "Channel 1", "Channel 2", ..., "Channel 32") and output loopback channels (e.g., "Output 1", "Output 2", etc.).
* **Meter Type Selection:** For each of the four slots, a dropdown menu will allow the user to choose between "VU" and "PPM" meter types.
* **Meter Display:** Clear and visually distinct representations of the selected meter type for each channel.
* **RTA Display:** Stacked frequency spectrum graphs for each of the four selected channels, providing a real-time visual representation of the frequency content.

**10. REST API Endpoints (Initial):**

* `/api/v1/channels`:
    * `GET`: Returns a JSON object containing the current channel routing for each of the four monitoring slots (e.g., `{ "slot1": 3, "slot2": 15, "slot3": 2, "slot4": 28 }`).
    * `PUT`: Accepts a JSON object to set the channel routing (e.g., `{ "slot1": 5, "slot3": 10 }`). The application will update the routing accordingly.

**11. Future Considerations (Beyond Initial Scope):**

* More advanced RTA options (e.g., averaging modes, peak hold).
* Logging capabilities.
* Presets for channel routing and meter settings.
* Direct AES67 discovery and connection.

This project brief outlines the core requirements and proposed architecture for the Multi-Channel Audio Monitor application, emphasizing the use of FOSS technologies and a simplified approach to audio input via standard system audio interfaces.
