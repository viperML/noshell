with import <nixpkgs> { };
mkShell {
  packages = [
    cmake
    clang-tools
    gdb
  ];
  hardeningDisable = ["all"];
}
