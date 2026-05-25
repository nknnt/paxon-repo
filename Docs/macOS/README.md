## Paxon macOS Build

Paxon macOS build has been released.

When installed via the package, binaries are automatically placed in `/usr/local/bin`, making them globally accessible from all supported shells.

The macOS build uses native MachO executable handling and supports direct binary installation through `curl`.

> [!WARNING]
> **Architecture Support Note:**
> While the **Paxon package manager itself** fully supports both Intel (x86_64) and Apple Silicon (ARM64) architectures, **most of the packages** provided through the Paxon repository currently only support ARM-based Macs (Apple Silicon M1/M2/M3/M4). 

### Features

- Native macOS binary support
- Universal Binary support for Paxon itself (x86_64 & ARM)
- Automatic executable permission setup
- Global installation path (`/usr/local/bin`)
