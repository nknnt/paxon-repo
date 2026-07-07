# Troubleshooting: `paxon: command not found` on Linux

## Issue Description

After installing `paxon`, running the command may result in:

```bash
bash: paxon: command not found
```

This usually means that the installation directory is not included in your `PATH`.

## Solution

### Use it immediately (current terminal only)

```bash
export PATH="$HOME/.local/bin:$PATH"
```

Replace `~/.local/bin` with your actual installation directory if you installed `paxon` elsewhere.

### Make it permanent

Add the following line to your shell configuration file:

**Bash / Zsh**

```bash
export PATH="$HOME/.local/bin:$PATH"
```

**Fish**

```fish
set -gx PATH "$HOME/.local/bin" $PATH
```

Then reload your shell or log out and back in.

For Bash:

```bash
source ~/.bashrc
```

For Zsh:

```bash
source ~/.zshrc
```

Or simply open a new terminal session.
