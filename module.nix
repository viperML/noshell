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

    # Default root to bash
    # defaultUserShell sets prio to 1000, we want a lower prio but still
    # let users change this without mkForce
    users.users.root.shell = lib.mkOverride 999 (pkgs.bashInteractive);
  };
}
