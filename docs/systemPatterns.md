# System Patterns: Multi-Channel Audio Monitor

## Architecture Patterns

### Component Architecture
- **Main Application**: Central orchestration
- **Audio Engine**: Handles device I/O and processing
- **Channel Selection**: Manager for routing audio channels
- **Metering Components**: Modular VU and PPM implementations
- **RTA Components**: Frequency analysis visualization
- **REST API Server**: External control interface

### Thread Model
- **Audio Thread**: High-priority, real-time thread for audio processing
- **UI Thread**: Main application thread for interface rendering
- **Network Thread**: For handling REST API requests
- **Thread Synchronization**: Lock-free where possible for audio path

### Data Flow
```
Input Devices → Channel Router → [Processing Chain] → Visualization Components
                                    ↓
                                 Meters & RTAs
```

## Code Organization

### Module Structure
- **Core**: Central application and initialization
- **Audio**: Device management and audio processing
- **GUI**: User interface components
- **Metering**: VU and PPM implementations
- **Analysis**: FFT and RTA functionality
- **Network**: REST API implementation

### File Naming Conventions
- CamelCase for class names
- Prefixes for component types (e.g., Meter*, RTA*)
- Consistent header/implementation file pairs

## Reusable Patterns

### Observer Pattern
- For updating UI based on audio processing
- Used for meter and RTA updates without blocking audio thread

### Factory Pattern
- For creating appropriate meter types
- For instantiating platform-specific audio components

### Command Pattern
- For REST API commands
- For undo/redo functionality if implemented

## Testing Strategy
- Unit tests for core DSP algorithms
- Component tests for meters and RTAs
- Integration tests for audio routing
- Cross-platform tests for consistent behavior 