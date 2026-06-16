Build Instructions
==================

### 1. Manual Build
1. Compile the source code using the appropriate compiler or build tool for your language (e.g., `g++` or `CMake` for C++, or the respective builder for other languages).
2. Move the compiled binary to the appropriate directory based on your OS:
   * **Windows:** Move the executable to the automatic (Registry HKCU\Environment) directory.
   * **Linux:** Move the executable to the `~/.local/bin/` directory.
   * **macOS:** Move the executable to the `/usr/local/bin/` directory.

### 2. Installation via Paxon
If installed via **Paxon**, the executable will be automatically placed in the correct directory:
* **Windows:** Automatically placed in the `/commands` directory.
* **Linux:** Automatically placed in the `~/.local/bin/` directory.
* **macOS:** Automatically placed in the `/usr/local/bin/` directory.
