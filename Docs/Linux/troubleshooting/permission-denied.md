# Troubleshooting: `Permission denied` on Linux when running `paxon`

## Issue Description
When running the `paxon` command on Linux (alacritty, GNOME terminal, kitty, etc.), the application fails with a permission error such as:

```bash
bash: permission denied: paxon
```

## Solutions
+ Run with sudo (Recommended)
+ Add execute permission
`chmod +x paxon`
