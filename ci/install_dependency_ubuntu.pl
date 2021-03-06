#!/bin/perl
use warnings;
use strict;

qx/sudo apt update -y/;
qx/sudo add-apt-repository ppa:ubuntu-toolchain-r\/test/;
my @install_apt = (
	"gcc-9 g++-9",
	"ninja-build",
	"cmake"
);
print "\n\n[INSTALL] Apt dependencies\n\n";
for(@install_apt){ system("sudo apt install $_ -y"); }
