#!/usr/bin/env perl
use warnings;
use strict;

print "\n\nIntalling Homebrew\n\n";
`wget –quiet –output-document install.sh https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh"`;
`chmod +x install.sh && bash install.sh`;
print "\n\nHomebrew update\n\n";
`brew update`;
print "\n\nGCC install\n\n";
`brew install gcc`;
print "\n\nCMake install\n\n";
`brew install cmake`;
print "\n\nMake install\n\n";
`brew install make`;
