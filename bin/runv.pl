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
use File::Find;
use File::Path 'make_path';
use POSIX;
use Env;
Env::import();

my $dump_dir = $DUMP_DIR;
my $gflag = "2012";
my $sim_plus_args = "";

GetOptions( "quiet" => \$quiet,
            "debug=s" => \$debug,
            "dump_dir=s" => \$dump_dir,
            "gflag=s" => \$gflag,
            "f=s" => \$cmd_file,
            "plus_args=s" => \$sim_plus_args,
            "no_build" => \$no_build,
            "no_dump" => \$no_dump,
            "no_run" => \$no_run);

my $dump_path;
my $wave_file_name = "sim.vcd";
my $base_path;
my $full_path;
my $cmd_file_path = "";
my @cmd_file_tests = ("verif", "rtl", "env");
my $cmd_file_test;
my $v_ofile_name = "a.vvp";
my $v_ofile_path;
my $v_build_dir = "/iVerilog";
my $v_build_path;

##=====================  Main Entry  ======================##
if(&set_paths == 0) {
  &myprint("ERROR: Could not set up paths.");
  exit;
}
unless($no_build) {
  if(&v_build() != 1) {
    exit;
  }
}
unless($no_run) {
  if(&v_sim() != 1) {
    exit;
  }
}
##=====================  End of Main  =====================##

sub set_paths
{
  if(&set_base_dir() == 0) {
    return 0;
  }
  elsif(&set_dump_dir() == 0) {
    return 0;
  }
  elsif(&set_cmd_file() == 0) {
    return 0;
  }
  &myprint("===============  AREA SETUP  ===============");
  &myprint("DUMP_DIR      : $DUMP_DIR");
  &myprint("dump_path     : $dump_path");
  &myprint("v_ofile_path  : $v_ofile_path");
  &myprint("base_path     : $base_path");
  &myprint("full_path     : $full_path");
  &myprint("cmd_file_path : $cmd_file_path");
  &myprint("============================================");
  return 1;
}

sub set_base_dir
{
  #The base directory is deduced from one above the verif or rtl
  #directories (and not the same as $PROJECT_ROOT).
  #Ex1 /home/matt/code/Hardware/rtl/_common/fifo/verif/env
  #Ex2 /home/matt/code/Hardware/rtl/_common/fifo/sync
  #Ex3 /home/matt/code/Hardware/rtl/_common/fifo
  #Ex4 /home/matt/code/Hardware/rtl/sgmii_mac_pcs/rtl
  #
  #Ex1 We see verif in the path and it's not $PROJECT_ROOT/verif
  #    so the base is /rtl/_common/fifo
  #Ex2 There is no verif or rtl in the path so we look for those
  #    directories one above where we are.
  #    In this case we see the 'verif' directory one above us.
  #    So the base is /rtl/_common/fifo
  #Ex3 Same as #2, but we see 'verif' in our current directory.
  #    So the base is /rtl/_common/fifo
  #Ex4 We see that 'rtl' is our current directory so we set the base
  #    one above where we are at /rtl/sgmii_mac_pcs
  if($PROJECT_ROOT eq "") {
    return 0;
  }
  my $pwd = `pwd`;
  my $branch = $pwd;
  if($branch =~ m|$PROJECT_ROOT/rtl|) {
    $branch =~ s|$PROJECT_ROOT/rtl(.*)|$1|;
  }
  else {
    return 0;
  }
  #Now we've trimmed off the $PROJECT_ROOT/rtl.
  if($branch =~ m|verif|) {
    $branch =~ s|(.*)/verif.*|$1|;
  }
  elsif($branch =~ m|rtl|) {
    $branch =~ s|(.*)/rtl.*|$1|;
  }
  else {
    #We didn't find verif or rtl in the path. Check down one.
    my @dir = &get_dir_list();
    if(grep(/^verif$/, @dir) || grep(/^rtl$/, @dir)) {
      $base_path = $branch;
    }
    else {
      #No rtl or verif one down. Check one up.
      @dir = &get_dir_list("../");
      if(grep(/^verif$/, @dir) || grep(/^rtl$/, @dir)) {
        $base_path = $branch =~ s|(.*)/\S+|$1|;
      }
      else {
        &myprint("ERROR: Could not find an rtl or verif directory.");
        return 0;
      }
    }
  }
  $base_path = $branch;
  $full_path = ${PROJECT_ROOT}. "/rtl" . ${base_path};
  chomp($full_path);
  return 1;
}

sub set_dump_dir
{
  if($dump_dir eq "") {
    &myprint("ERROR: No dump directory set (or DUMP_DIR env var).");
    return 0;
  }
  $dump_path = $dump_dir . $base_path;
  chomp($dump_path);
  $v_build_path = $dump_path . $v_build_dir;
  if(not -d $v_build_path) {
    make_path $v_build_path or die "Failed to create build directory.";
  }
  $v_ofile_path = $v_build_path . "/" . $v_ofile_name;
  return 1;
}

sub set_cmd_file
{
  #User may specify $cmd_file (relative to pwd).
  if(-f $cmd_file) {
    my $pwd = `pwd`;
    chomp($pwd);
    $cmd_file_path = $pwd . "/" . $cmd_file;
    return 1;
  }
  elsif($cmd_file ne "") {
    &myprint("ERROR: File passed to -f ($cmd_file) does not exist.");
    &myprint("Searching for standard command files.");
  }

  #If the cmd file is blank (not user specified), then we search for
  #1. verif.vf
  #2. rtl.vf
  #3. env.vf
  foreach $test (@cmd_file_tests) {
    $cmd_file_test = $test;
    find({wanted => \&cmd_file_wanted, no_chdir => 1}, $full_path);
    if($cmd_file_path ne "") {
      last;
    }
  }


  return 1;
}

sub cmd_file_wanted
{
  if($cmd_file_path eq "") {
    if($File::Find::name =~ m|.*/$cmd_file_test\.vf|) {
      $cmd_file_path = $File::Find::name;
    }
  }
}

sub v_build
{
  my $ivl_cmd = "iverilog";
  if($gflag ne "") {
    $ivl_cmd .= " -g" . $gflag;
  }
  if($cmd_file_path ne "") {
    $ivl_cmd .= " -f" . $cmd_file_path;
  }
  #if(-e $v_ofile_path) {
  #remove($v_ofile_path);
  #}
  $ivl_cmd .= " -o" . $v_ofile_path;


  &myprint($ivl_cmd);


  my $ivl_rslt = `$ivl_cmd`;
  &myprint($ivl_rslt);
  return 1;
}

sub v_sim
{
  my $vvp_cmd = "vvp $v_ofile_path";
  
  # Add plus args.
  $sim_plus_args .= " +vcd_file=" . $dump_path . "/" . $wave_file_name;
  if(!$no_dump) {
    $sim_plus_args .= " +dump_on";
  }
  $vvp_cmd .= " $sim_plus_args";

  my $vvp_rslt = `$vvp_cmd`;
  &myprint("=== Simulation Output ===");
  &myprint($vvp_rslt);
  &myprint("====== End  Output ======");
  return 1;
}

sub get_dir_list
{
  my @dirs;
  my $dir = @_ == 0 ? "./" : $_[0];
  my @ls_rslts = `ls -l $dir`;
  foreach $ls_rslt (@ls_rslts) {
    #if it's a directory (starts with 'd')
    if($ls_rslt =~ m/^d.*/) {
      #Strip off the file stats.
      $ls_rslt =~ s/^.*\s(\S+)$/$1/;
      #If it's not one of these directories, it's a test directory.
      #Push it into @outer_fldrs with the absolute path.
      unless(($ls_rslt =~ m/ucli_files/) || ($ls_rslt =~ m/satris_work/)) {
        chomp($ls_rslt);
        push(@dirs, $ls_rslt);
      }
    }
  }
  return @dirs;
}

sub myprint
{
  if(!$quiet) {
    unless(@_ == 0) {
      if(@_ == 1) {
        my $tmp = $_[0];
        chomp($tmp);
        print "$tmp\n";
      }
      else {
        foreach $str (@_) {
          chomp($str);
          print $str . "\n";
        }
      }
    }
  }
}

sub dbg
{
  if(!$quiet && ($debug != 0)) {
    unless(@_ == 0) {
      if(@_ == 1) {
        print "$_[0]\n";
      }
      else {
        foreach $str (@_) {
          chomp($str);
          print $str . "\n";
        }
      }
    }
  }
}

