## Platform Differences

The Windows, Linux, and macOS builds of Paxon utilize different package repositories and native runtime implementations. As a result, the runtime behavior, directory structures, and development environments vary between platforms.

### 1. Platform Comparison Table

| Feature | Windows | Linux | macOS |
| :--- | :--- | :--- | :--- |
| **Installation Path** | `%USERPROFILE%\paxon\` | `~/.local/bin/` | `/usr/local/bin/` |
| **Package Extension** | `.exe` | None (Native Binary) | None (MachO Binary) |
| **Configuration Path**| `%USERPROFILE%\.config\paxon.json` | `~/.config/paxon/` | `~/.config/paxon.json` |
| **Main Architecture** | x86_64 | x86_64 | ARM64 (Apple Silicon) |
| **PATH Registration** | Automatic (Registry `HKCU\Environment`) | Manual (export in shell profile) | Manual (`/usr/local/bin` is pre-registered) |

---

### 2. Core Architectural Differences

#### 🪟 Windows Runtime
* Uses standard Windows executable (`.exe`) handling.
* On first run, `paxon.exe` automatically copies itself to `%USERPROFILE%\paxon\` and registers that directory in the user PATH via `HKCU\Environment` in the Windows Registry.
* All installed packages are stored under `%USERPROFILE%\paxon\` and are immediately accessible from any terminal (cmd, PowerShell, Windows Terminal) after opening a new window.

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
