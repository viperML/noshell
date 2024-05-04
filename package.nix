{
  # Fallback shell to use
  # Can be either a shell with a proper passthru.shellPath, or a path to a file
  bashInteractive,
  defaultShell ? bashInteractive,
  #
  stdenv,
  cmake,
  lib,
}: let
  defaultShell' =
    if lib.isDerivation defaultShell
    then "${defaultShell}${defaultShell.passthru.shellPath}"
    else defaultShell;
in
  stdenv.mkDerivation {
    name = "noshell";

    src = lib.cleanSource ./.;

    nativeBuildInputs = [cmake];

    cmakeFlags = [
      "-DDEFAULT_SHELL=${defaultShell'}"
    ];

    passthru = {
      shellPath = "/bin/noshell";
    };

    meta = {
      mainProgram = "noshell";
      description = "User-configurable login shell";
      homepage = "https://github.com/viperML/noshell";
      license = lib.licenses.eupl12;
    };
  }
