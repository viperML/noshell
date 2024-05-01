with import <nixpkgs> { };
mkShell {
  packages = [
    clang-tools
    cmake
  ];
  hardeningDisable = ["all"];
}
