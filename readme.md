<h1 align="center">noshell</h1>

<h5 align="center">user-configurable login shell</h1>

----

Noshell is a simple shim that executes `$XDG_CONFIG_HOME/shell` as your login shell.
If you configure noshell system-wide, users can configure their login shell with
a simple `ln -sT /path/to/zsh ~/.config/shell`, instead of requiring
superuser permissions to modify `/etc/passwd` and `/etc/shells`.

By default, noshell checks the following locations in the order:

- `$XDG_CONFIG_HOME/shell`
- `$HOME/.config/shell`
- `/bin/sh` as the fallback

## Usage

Add this flake's `nixosModules.default` to your NixOS imports, and set:
```
programs.noshell.enable = true
```

Noshell will be configured as the default shell to **ALL** users.
