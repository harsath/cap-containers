#!/bin/perl
use warnings;
use strict;

if($#ARGV+1 != 1){ print "Usage: ./script.pl <BUILD-DIR-NAME>"; exit(1); }
my $build_dir_name = $ARGV[0];
my $test_dir_name = "tests";
chdir($test_dir_name);
sub run_tests {
	if(-d $build_dir_name){ print "[ERROR]: Build dir already exists, remove first"; exit(1); }
	mkdir($build_dir_name);
	chdir($build_dir_name);
	system(qq/cmake -GNinja .. && ninja/);
}

run_tests();
