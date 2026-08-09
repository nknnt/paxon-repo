## Paxon Linux Build

Paxon Linux build has been released.
When installed via the package, it is automatically placed in `~/.local/bin`, making it available across all shells.

> [!WARNING]
> **Compatibility Notes:**
> * **Architecture:** Most packages provided by Paxon currently **do not support ARM based CPUs** (e.g., Apple Silicon M1/M2/M3 virtualization, Raspberry Pi, AWS Graviton). They are optimized primarily for Intel/AMD (x86_64) architectures. 
>   * *Note for Apple Silicon users:* If you are on a Mac with Apple Silicon, please use the **macOS build of Paxon** instead of the Linux build inside a virtual machine or container.
> * **OS Version:** If you are using a very old Linux distribution, the program may fail to run due to library version conflicts (such as `GLIBC`). It is highly recommended to use it on a modern, up to date Linux environment.

See [platform differences](/Docs/platform-differences.md) for more details.
