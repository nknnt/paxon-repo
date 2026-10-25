# Integrity and Security (paxon 1.6.0 or later)

Every file downloaded from this repository is checked against the **SHA-256** listed for it in the package index (`packages.json`), over **HTTPS only**.

- A file whose hash does not match is never installed.
- Files are downloaded to a temporary file and moved into place only after verification, so a failed or interrupted install never leaves a half-written file.
- Package names are validated, so a package cannot write outside the install directory.

## Packages without a SHA-256

Some old or rarely used packages have no SHA-256 in the index. paxon **refuses to install them by default** and explains the risk. You can approve them yourself:

```bash
# Allow specific packages (recommended)
paxon config set PAXON_UNVERIFIED_ALLOW name1,name2

# Allow every package without a SHA-256 (not recommended)
paxon config set PAXON_ALLOW_UNVERIFIED true

# Allow a single run only
paxon -S <package-name> --allow-unverified
```

Notes:
- A wrong or malformed hash is **always** an error, even when unverified downloads are allowed.
- paxon itself and `-Syu` (update everything) never install anything without a hash.
- Running as root ignores the user config, and `--allow-unverified` is refused for root.
- Installed packages that skipped verification are marked `[UNVERIFIED]` in `paxon -Q`.

## Rollback and pinning

paxon keeps the previous version of each package. To go back one version:

```bash
paxon --rollback <package-name>
paxon unpin <package-name>   # allow updates again
```

A rolled-back package is pinned so that `paxon -Syu` does not upgrade it again.
