# Standard Error Codes

This document defines the standard error code format used by NI(ne) projects.

## Format

```
0x[L][CATEGORY][ID]
```

Example:

```
0x7PKG0001
```

| Part | Description | Example |
|------|-------------|---------|
| `0x` | Hexadecimal prefix | `0x` |
| `L` | Severity level | `7` |
| `CATEGORY` | Error category | `PKG` |
| `ID` | Error identifier | `0001` |

---

## Severity Levels

| Code | Level | Description |
|------|-------|-------------|
| `0x3` | Warning | Non-critical issue. Execution may continue. |
| `0x5` | Error | General error that requires attention. |
| `0x7` | Critical | Critical failure. Some features cannot continue. |
| `0x9` | Fatal | Unrecoverable error. Immediate termination is recommended. |

### Examples

| Code | Example |
|------|---------|
| `0x3` | Cache used, optional feature failed |
| `0x5` | Invalid `packages.json`, missing configuration file |
| `0x7` | Network unavailable, required component initialization failed |
| `0x9` | Missing DLL, security violation, data corruption |

---

## Categories

| Code | Description |
|------|-------------|
| `PKG` | Package |
| `CFG` | Configuration |
| `NET` | Network |
| `FS` | File System |
| `SEC` | Security |
| `SYS` | System / Internal |
| `DB` | Database |
| `MEM` | Memory |
| `IO` | Input / Output |
| `AUTH` | Authentication |
| `API` | External API |
| `CLI` | Command Line Interface |

Categories may be added, removed, or revised as the specification evolves.

---

## Design Principles

- Error codes are stable and should not change once released.
- Each error code uniquely identifies a single failure condition.
- Applications should rely on error codes rather than message text for programmatic handling.
