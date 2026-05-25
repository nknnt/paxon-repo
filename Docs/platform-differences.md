## Platform Differences

The Windows, Linux, and macOS builds of Paxon utilize different package repositories and native runtime implementations. As a result, the runtime behavior, directory structures, and development environments vary between platforms.

### 1. Platform Comparison Table

| Feature | Windows | Linux | macOS |
| :--- | :--- | :--- | :--- |
| **Installation Path** | `/commands` | `~/.local/bin/` | `/usr/local/bin/` |
| **Package Extension** | `.exe` | None (Native Binary) | None (MachO Binary) |
| **Configuration Path**| `.config/paxon.rpp` | `~/.config/paxon/` | `.config/paxon.rpp` |
| **Main Architecture** | x86_64 | x86_64 (Intel/AMD) | ARM64 (Apple Silicon) |

---

### 2. Core Architectural Differences

#### 🪟 Windows Runtime
* Uses standard Windows executable (`.exe`) handling.
* Binaries are isolated within the local `/commands` directory.

#### 🐧 Linux Runtime
* Tailored for native Linux environments using standard POSIX APIs (`fork`/`execvp`).
* Integrates seamlessly with the user environment via `~/.local/bin`, ensuring cross shell availability if the path is exported.

#### 🍏 macOS Runtime
* Leverages native MachO executable handling and `_NSGetExecutablePath` for path resolution.
* Installed globally under `/usr/local/bin` for system wide access.

---

> [!CAUTION]
> ### ⚠️ Cross Platform Package Warning
> Please note that **packages are not cross compatible between platforms**. For example, a package built and hosted for the macOS (ARM64) repository will not execute on a Linux (x86_64) system, and vice versa. Always ensure you are using the correct Paxon binary matching your native Operating System.
