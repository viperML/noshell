<h1 align="center">noshell</h1>

<h5 align="center">user-configurable login shell</h1>

----

noshell is a simple shim that executes `$XDG_CONFIG_HOME/shell` as your login shell.
If you configure noshell system-wide, users can configure their login shell with
a simple `ln -sT /path/to/zsh ~/.config/shell`, instead of requiring
superuser permissions to modify `/etc/passwd` and `/etc/shells`.

## Usage

Add this flake's `nixosModules.default` to your NixOS imports. The shell is configured
by default to **ALL** users.
