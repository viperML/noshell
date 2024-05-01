{
  stdenv,
  cmake,
  lib,
}:
stdenv.mkDerivation {
  name = "noshell";

  src = lib.cleanSource ./.;

  nativeBuildInputs = [ cmake ];

  passthru = {
    shellPath = "/bin/noshell";
  };
}
