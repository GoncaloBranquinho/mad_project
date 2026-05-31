{
  description = "OR-Tools + SCIP development shell";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
  };

  outputs = { self, nixpkgs }:
  let
    system = "x86_64-linux";
    pkgs = import nixpkgs {
      inherit system;
    };
  in {
    devShells.${system}.default = pkgs.mkShell {
      packages = with pkgs; [
        cmake
        ninja
        gcc

        or-tools
        scip

        gmp
        mpfr
        zlib
      ];
    };
  };
}
