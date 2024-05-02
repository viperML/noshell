<h1 align="center">noshell</h1>

<h6 align="center">user-configurable login shell</h1>

noshell is a simple shim that executes $XDG_CONFIG_HOME/shell as your login shell.

If you configure noshell system-wide, users can configure their login shell with
a simple `ln -sT /path/to/zsh ~/.config/shell`, instead of requiring
superuser permissions to modify `/etc/passwd` and `/etc/shells`.

## Usage

```nix
# pass your flake inputs however you want
{ config, inputs, ... }: let
  noshell = inputs.noshell.packages.x86_64-linux.default;
in {
  environment.shells = [ noshell ];
  environment.systemPackages = [ noshell ];

  users.users.foo = {
    # ....
    shell = noshell;
  };
}
```
