/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/hammy025/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   env.sv 
#   Creator       :   Matt Hampton (hammy025@gmail.com)
#   Owner         :   Matt Hampton (hammy025@gmail.com)
#   Creation Date :   04/06/16
#   Description   :   
#
###############################################################################
*/

class env;

  function new();
    $display("Creating env object.");
  endfunction

  task env_init();
    string vcd_file;
    // call dump setup
    $display("env::env_init()");
    if($value$plusargs("vcd_file=%s", vcd_file)) begin
      $dumpfile(vcd_file);
      if($test$plusargs("dump_on")) begin
        $dumpvars;
      end
    end
  endtask

  task dut_init();
    $display("env::dut_init()");
  endtask

endclass

