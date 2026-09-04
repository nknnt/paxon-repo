# Changing the Paxon Mirror

By default, Paxon downloads packages from GitHub:

```json
"PAXON_MIRROR": "github",
```

`github` is the default mirror and requires no extra configuration.

## Using a Custom Mirror

To use a different mirror (for example, a self hosted one), change the mirror type to `custom` and set the target URL:

```json
"PAXON_MIRROR": "custom",
"PAXON_CUSTOM_URL": "https://example.com/repo/",
```

Replace the example URL with your actual mirror site. For example:

```json
"PAXON_MIRROR": "custom",
"PAXON_CUSTOM_URL": "https://mirror.nknnt.com/",
```

Once both fields are set, Paxon will fetch packages from the custom URL instead of GitHub.

## Notes

- `PAXON_CUSTOM_URL` is only used when `PAXON_MIRROR` is set to `"custom"`.
- The custom mirror must replicate the same directory structure as the default repo (e.g. `macos/`, `linux/`, `windows/`, `netdust/`).
- If the custom URL is unreachable, Paxon will not automatically fall back to GitHub : make sure the mirror is available before switching.
