<h1 align="center">noshell</h1>

<h5 align="center">user-configurable login shell</h1>

----

Noshell is a simple shim that executes `$XDG_CONFIG_HOME/shell` as your login shell.
If you configure noshell system-wide, users can configure their login shell with
a simple `ln -sT /path/to/zsh ~/.config/shell`, instead of requiring
superuser permissions to modify `/etc/shells`.

By default, noshell checks the following locations in the order:

- `$XDG_CONFIG_HOME/shell`
- `$HOME/.config/shell`
- `pkgs.bashInteractive` as the fallback. Can be configured at build-time

## Usage

Add this flake's `nixosModules.default` to your NixOS imports, and set:
```
programs.noshell.enable = true
```

Noshell will be configured as the default shell to **ALL** users.

## Why?

Noshell enables some patterns in the NixOS ecosystem, such as:

- Using a shell with a configuration wrapper. As the wrapper is a different program
  that is not present in `/etc/shells`. See https://github.com/viperML/wrapper-manager .
- User-level configuration of the login shell with home-manager (`xdg.configFile."shell".source = lib.getExe pkgs.nushell;`)

It can be useful too outside of NixOS, for example to use a custom build of some shell.
