# VS Code Configuration for MCAM Project

This directory contains VS Code configuration files that use non-Microsoft extensions for C++ development. The setup is designed to be compatible with VS Code derivatives that may have issues with Microsoft-licensed extensions.

## Required Extensions

### C/C++ Language Support
- **Clangd** (`llvm-vs-code-extensions.vscode-clangd`): Language server from LLVM
- **CCLS** (`ccls-project.ccls`): Alternative language server

### CMake Support
- **CMake** (`twxs.cmake`): CMake syntax highlighting
- **CMake Format** (`cheshirekow.cmake-format`): CMake formatting

### Code Formatting
- **Clang Format** (`xaver.clang-format`): C++ code formatting

### Other Recommended Extensions
See the full list in `extensions.json`.

## First-Time Setup

1. Install Clangd or CCLS on your system
2. Install recommended extensions
3. JUCE Framework is located at `~/JUCE` and this path is configured in settings.json and tasks.json
4. Run the `CMake: Configure` task
5. Run the `Copy compile_commands.json` task to enable the language server

## Important Files

- `settings.json`: Editor and language server settings
- `extensions.json`: Recommended extensions
- `.clang-format`: C++ code formatting rules
- `tasks.json`: Build and utility tasks
- `launch.json`: Debugging configurations

## Language Server Configuration

The configuration supports both Clangd and CCLS. By default, both are enabled in the settings, but you should install and use only one:

### Using Clangd
1. Install Clangd (`sudo apt install clangd` / `brew install llvm` / download from LLVM)
2. Install the Clangd extension
3. Make sure the CMake configure task generates `compile_commands.json`

### Using CCLS
1. Install CCLS (`sudo apt install ccls` / `brew install ccls` / build from source)
2. Install the CCLS extension
3. Make sure the CMake configure task generates `compile_commands.json`

---

This setup avoids dependencies on Microsoft-specific extensions while providing full C++ development capabilities.
