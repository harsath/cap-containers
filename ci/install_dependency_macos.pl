#!/usr/bin/env perl
use warnings;
use strict;

`wget –quiet –output-document install.sh https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh"`;
`chmod +x install.sh && bash install.sh`;
`brew update`;
`brew install --build-from-source gcc`;
`brew install cmake`;
`brew install make`;
