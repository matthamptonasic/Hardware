# Hardware
The intention of this repository is to create synthesizable designs (generally related to networking technology) in verilog HDL
and the environments to fully test them.
Initially the environments will be written primarily in Verilog with whatever SystemVerilog Icarus Verilog supports.

Simply browse the 'rtl' directory for a list of the designs.
The 'rtl/common/' directory has various common designs used in multiple designs.

The only requisites are Icarus Verilog v10+ and a waveviewer of your choosing.
I use gtkwave myself.

To run any of the environments:
1. Check out the Hardware tree.
2. You'll need to set a couple of environment variables:
  a. $PROJECT_ROOT  - set to your Hardware folder. For instance, mine is:
    setenv PROJECT_ROOT /home/matt/code/Hardware
  b. $DUMP_DIR  - set to wherever you want the compilation/simulation results to be.
    setenv DUMP_DIR /home/matt/dump/Hardware

3. Navigate to the environment you want to run. The general structure is:
   ./module/sub_module/   // Where the RTL/tree is.
   ./module/verif/        // Where the environment top is.
   ./module/verif/env/    // The core of the environment is (top.sv).

4. From any of the verif folders you should be able to run '$PROJECT_ROOT/bin/runv.pl'
   This will find the needed file lists and try to run a basic simulation at the top
   level of the module you're in.

   Of course, you can open the runv.pl to see some of the basic arguments you can pass.

That's it, it should be that simple. Let me know if it's not.

