import <nixpkgs> {
  overlays = [ (final: prev: { noshell = final.callPackage ./package.nix { }; }) ];
}
