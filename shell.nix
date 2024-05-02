{pkgs ? import <nixpkgs> {}}:
with pkgs;
  mkShell {
    packages = [
      cmake
      clang-tools
      gdb
    ];
    hardeningDisable = ["all"];
  }
