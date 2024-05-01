{
  stdenv,
  cmake,
  lib,
}:
stdenv.mkDerivation {
  name = "noshell";

  src = lib.cleanSource ./.;

  nativeBuildInputs = [ cmake ];
}
