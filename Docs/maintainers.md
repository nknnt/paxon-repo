# For Maintainers

Every entry in `packages.json` (including netdust packages and their `sub_packages`) should have a `sha256` field and an `https://` URL.<br>
Entries without a `sha256` can only be installed by users who explicitly approve them as described in [Integrity and Security](SECURITY.md).
