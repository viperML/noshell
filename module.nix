{
  config,
  lib,
  pkgs,
  ...
}: let
  pkg = pkgs.callPackage ./package.nix {};
  cfg = config.programs.noshell;
in {
  options = {
    programs.noshell = {
      enable = lib.mkEnableOption "noshell, user-configurable login shell";
      package = lib.mkOption {
        description = "noshell package";
        default = pkg;
        type = lib.types.package;
      };
    };
  };

  config = lib.mkIf cfg.enable {
    environment = {
      shells = [cfg.package];
      systemPackages = [cfg.package];
    };
    users.defaultUserShell = cfg.package;
  };
}
