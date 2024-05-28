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
  fs = lib.fileset;
  r = ./.;
in
  stdenv.mkDerivation {
    name = "noshell";

    src = fs.toSource {
      root = r;
      fileset = fs.intersection (lib.fileset.fromSource (lib.sources.cleanSource r)) (
        lib.fileset.unions [
          (fs.fileFilter (file: file.hasExt "c") r)
          (fs.fileFilter (file: file.hasExt "txt") r)
          (fs.fileFilter (file: file.hasExt "in") r)
        ]
      );
    };

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
