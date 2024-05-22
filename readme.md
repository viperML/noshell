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

Noshell will be configured as the default shell to **ALL** users. You might want to
disable noshell for some user as a fallback, like `users.users.root.shell = pkgs.bashInteractive;`.

## Why?

Noshell enables some patterns in the NixOS ecosystem, such as:

- Using a shell with a configuration wrapper. As the wrapper is a different program
  that is not present in `/etc/shells`. See [https://github.com/viperML/wrapper-manager](https://github.com/viperML/wrapper-manager).
- User-level configuration of the login shell with home-manager (`xdg.configFile."shell".source = lib.getExe pkgs.nushell;`)

It can be useful too outside of NixOS, for example to use a custom build of some shell.

## A note on login shells

As far as I know, there is nothing "written in stone" about what a login shell must do, but other
tools in Linux do expect some functionality.

Some of these assumptions are:
- The login shell is expected to `. /etc/profile` by its own. This file is a POSIX sh script that sets up environment variables needed by the session. `pam_env` also
  sets environment variables but that is handled earlier in the login process.
- `login` may call your shell with `-progname`, that is with a dash as the first character, to indicate that this is a login shell.
- Other programs may call your shell with the argument `-l` to indicate that it is a login shell.
- Some programs might assume `$SHELL` is something that you can pass arguments to, whatever the arguments are, with `-c`.

Please report any functionality expected from a login shell in the issues board: [https://github.com/viperML/noshell/issues](https://github.com/viperML/noshell/issues).

