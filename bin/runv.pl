#!/usr/bin/perl
###############################################################################
#   Licensing information found at: 
#     https://github.com/hammy025/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   runv.pl
#   Creator       :   Matt Hampton (hammy025@gmail.com)
#   Owner         :   Matt Hampton (hammy025@gmail.com)
#   Creation Date :   04/03/16
#   Description   :   Script to control verilog compilation and simulation.
#
###############################################################################
use POSIX;
use Getopt::Long;
use File::stat;
use POSIX;

GetOptions( "quiet" => \$quiet,
            "debug=s" => \$debug,
            "dump_dir=s" => \$dump_dir,
            "no_build" => \$no_build,
            "no_dump" => \$no_dump,
            "no_run" => \$no_run);

##=====================  Main Entry  ======================##
unless($no_build) {
  # call build sub
}
unless($no_run) {
  # call simulation sub
}
##=====================  End of Main  =====================##

