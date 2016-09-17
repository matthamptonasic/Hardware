#!/usr/bin/perl
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   runv.pl
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   04/03/16
#   Description   :   Script to control verilog compilation and simulation.
#
###############################################################################
use POSIX;
use Getopt::Long;
use File::Copy;
use File::Find;
use File::Path 'make_path', 'remove_tree';
use File::stat;
use POSIX;
use Env;
Env::import();

my $dump_dir = $DUMP_DIR;
my $gflag = "2012";
my $sim_plus_args = "";
my $wave_file_name = "sim.vcd";

GetOptions( "quiet" => \$quiet,
            "debug=s" => \$debug,
            "ivl_debug" => \$ivl_debug,
            "dump_dir=s" => \$dump_dir,
            "wave_file=s" => \$wave_file_name,
            "gflag=s" => \$gflag,
            "f=s" => \$cmd_file,
            "plus_args=s" => \$sim_plus_args,
            "clean" => \$clean,
            "no_build" => \$no_build,
            "no_build_c" => \$no_build_c,
            "no_build_v" => \$no_build_v,
            "no_dump" => \$no_dump,
            "no_run" => \$no_run);

my $dump_path;
my $base_path;
my $full_path;
my $cmd_file_path = "";
my @cmd_file_tests = ("verif", "rtl", "env");
my $cmd_file_test;
my $v_ofile_name = "a.vvp";
my $vpi_file_name = "vpi_entry.vpi";
my $v_ofile_path;
my $c_obj_file_path;
my $vpi_file_path;
my $v_build_dir = "/iVerilog";
my $c_build_dir = "/cpp";
my $v_build_path;
my $c_build_path;
my $verif_make_path = "$PROJECT_ROOT/verif/";

##=====================  Main Entry  ======================##
if(&set_paths == 0) {
  &myprint("ERROR: Could not set up paths.");
  exit;
}
if($clean) {
  my @tmp = glob "'$dump_path/*'";

  my $rem_cnt = remove_tree(@tmp);
  &myprint("Cleaned $rem_cnt files/folders from the dump directory.");
}

if(&build_dump_dir == 0) {
  &myprint("ERROR: Could not build the dump directory.");
  exit;
}
unless($no_build) {
  if(&c_build() != 1) {
    exit;
  }
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
  &myprint("DUMP_DIR        : $DUMP_DIR");
  &myprint("dump_path       : $dump_path");
  &myprint("v_ofile_path    : $v_ofile_path");
  &myprint("base_path       : $base_path");
  &myprint("full_path       : $full_path");
  &myprint("cmd_file_path   : $cmd_file_path");
  &myprint("vpi_file_path   : $vpi_file_path");
  &myprint("c_vpi_file_path : $c_vpi_file_path");
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
  $c_build_path = $dump_path . $c_build_dir;
  $v_ofile_path = $v_build_path . "/" . $v_ofile_name;
  $vpi_file_path = $c_build_path . "/" . $vpi_file_name;
  return 1;
}

sub build_dump_dir
{
  if(not -d $v_build_path) {
    make_path $v_build_path or die "Failed to create v_build directory.";
  }
  if(not -d $c_build_path) {
    make_path $c_build_path or die "Failed to create c_build directory.";
  }
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

# Steps to building the VPI, RTL, and simulating are:
# 1. Create the c++ object files.
# 2. Pass the final linked object file to iverilog_vpi.
#    "iverilog_vpi entry.o"
# 3. The output of 2 (*.vpi) is moved to the output directory.
#    (There is no built-in option to output these to a configurable location)
#    "move($vpi_file_name, $vpi_file_path);"
# 4. The v_build happens normally, compiling the verilog only files.
# 5. The v_sim step adds the -m option which specifies the .vpi file.
sub c_build
{
  if($no_build_c)
  {
    return 1;
  }
  &myprint("Building C++ object files.");
  # Build the C++ object files in the build directory.
  # Start with the top level verif library files.
  my $make_cmd = "make -C $verif_make_path ODIR=$c_build_path";
  $rslt = `$make_cmd`;
  &myprint("rslt = $rslt");
  if((${^CHILD_ERROR_NATIVE} >> 8) != 0) 
  {
    &myprint("Error compiling C++ environment.");
    return 0;
  }

  # Build the local environment.
  # TBD

  # Now link all of those files using the iverilog-vpi command to get the .vpi file.
  my @vpi_entry_file = glob "'$c_build_path/vpi_entry.o'";
  my @object_files = (@vpi_entry_file, grep { ! /.*vpi_entry.o/} glob "'$c_build_path/*.o'");
  &myprint("Object files to link:");
  &myprint(@object_files);

  my $vpi_cmd = "iverilog-vpi @object_files";
  &myprint("vpi command:");
  &myprint($vpi_cmd);
  &nl();

  my $ivl_vpi_rslt = `$vpi_cmd`;
  move($vpi_file_name, $vpi_file_path);
  &myprint($ivl_vpi_rslt);
  return 1;
}

sub v_build
{
  if($no_build_v)
  {
    return 1;
  }
  my $ivl_cmd = "iverilog";
  if($ivl_debug) {
    $ivl_cmd .= "_dev";
  }
  if($gflag ne "") {
    $ivl_cmd .= " -g" . $gflag;
  }
  if($cmd_file_path ne "") {
    $ivl_cmd .= " -f" . $cmd_file_path;
  }
  $ivl_cmd .= " -o" . $v_ofile_path;

  &nl();
  &myprint("ivl command:");
  &myprint($ivl_cmd);
  &nl();

  my $ivl_rslt = `$ivl_cmd`;
  &myprint($ivl_rslt);
  my $filename = $dump_path . "/" . 'v_build.log';
  open(my $fh, '>', $filename) or die "Could not open file '$filename' $!";
  print $fh $ivl_rslt;

  close $fh;
  return 1;
}

sub v_sim
{
  my $vvp_cmd = "vvp";
  if($ivl_debug) {
    $vvp_cmd .= "_dev";
  }
  $vvp_cmd .= " -m$vpi_file_path";
  $vvp_cmd .= " $v_ofile_path";
  
  # Add plus args.
  $sim_plus_args .= " +vcd_file=" . $dump_path . "/" . $wave_file_name;
  if(!$no_dump) {
    $sim_plus_args .= " +dump_on";
  }
  $vvp_cmd .= " $sim_plus_args";

  &myprint("vvp command:");
  &myprint($vvp_cmd);
  &nl();
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

sub nl
{
  if(!$quiet) {
    if(@_ == 0) {
      print "\n";
    }
    else {
      for($ii = 0; $ii < $_[0]; $ii = $ii + 1) {
        print "\n";
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

