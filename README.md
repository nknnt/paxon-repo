# Paxon Package Repository

**English** · [简体中文](README.zh.md) · [日本語](README.ja.md) · [한국어](README.ko.md)

Paxon is a lightweight package manager for **sopsh**, **development environments**, and other components of the **ni ecosystem**.
The official default mirror for paxon is this github repository.

[![Platform](https://img.shields.io/badge/platform-linux%20%7C%20macos%20%7C%20windows-18181b?style=flat&color=FFC0FF)](...)

See the [paxon website](https://nknnt.com/paxon) for documentation and additional information.

-----

## Installation

Install a package using paxon:

```bash
paxon -S <package-name>
```

Install a netdust package using paxon:

```bash
paxon -S netdust@<package-name>
```
> [!NOTE]
> This feature requires **paxon 1.4.4 or later**.

-----

## Private packages

With **paxon 1.7.0 or later** you can publish packages that only you, the people you allow, or holders of a share key can install. Private packages live on [NKNNT Passport](https://passport.nknnt.com), not in this repository, and are named `handle@package`.

Install one:

```bash
paxon login                      # once, opens your browser
paxon -S <handle>@<package>
```

Someone shared a key with you instead of adding your account? Set it in the environment, or just run the install and paxon asks for it (hidden prompt):

```bash
PAXON_SHARE_KEY=nk_... paxon -S <handle>@<package>
```

> [!IMPORTANT]
> Share keys are never accepted as command-line arguments (paxon refuses them) so they don't end up in `ps` or your shell history. A share key only reads that one package; it cannot publish or touch your account.

Publish your own:

```bash
paxon login
paxon create mytool                          # or: paxon publish ... --create
paxon publish mytool ./mytool 1.0.0          # name, file, version
paxon logs mytool                            # who uploaded / downloaded what (owner only)
```

- Uploads show progress. If the connection drops, **run the same command again and it resumes** where it stopped.
- The server computes the SHA-256 and size itself and refuses a file that doesn't match. Versions can't be overwritten.
- Size and package limits depend on your plan; `paxon whoami` shows your plan and usage.
- Pick your handle, manage people and share keys, and upload from the browser at [passport.nknnt.com](https://passport.nknnt.com).

| Command | What it does |
|---|---|
| `paxon login` / `logout` / `whoami` | Sign in with your browser, sign out (also revokes the token on the server), show account and limits |
| `paxon create <name>` | Create an empty private package under your handle |
| `paxon publish <name> <file> <version>` | Upload a version (resumable). `--create` creates the package if needed |
| `paxon logs <name>` | Audit log of one of your packages |
| `paxon -S <handle>@<name>` | Install a private package |

Updating (`-Su`), `-R`, `-Q`, `rollback` and `pin` work for private packages the same way as for public ones. `-Su` only checks private packages while you are logged in.

> [!NOTE]
> `netdust@<module>` still means a netdust package from this repository, never a private one. The handles `netdust` and `meyku` are reserved.

-----

## Security and maintenance

Downloads are verified with SHA-256 over HTTPS. Packages without a hash are refused unless you approve them yourself. Private packages are always verified and can't be installed unverified.

- [Integrity and security](Docs/security.md): hash verification, unverified packages, rollback and pinning
- [For maintainers](Docs/maintainers.md): package index requirements

Login uses your browser (PKCE on a local loopback address). Tokens are kept in the macOS Keychain or the Linux Secret Service (`secret-tool`); if neither exists they go to a `0600` file in paxon's config folder and paxon tells you. Keys and tokens are never printed, not even with `--verbose`.

-----

> [!CAUTION]
> Due to a logic update, this repo no longer works with version 1.4.1 or earlier.
>
> Please update to the latest version using `paxon update`.
