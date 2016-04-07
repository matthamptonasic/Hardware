/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/hammy025/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   clock_gen.sv 
#   Creator       :   Matt Hampton (hammy025@gmail.com)
#   Owner         :   Matt Hampton (hammy025@gmail.com)
#   Creation Date :   04/06/16
#   Description   :   
#
###############################################################################
*/
`timescale 1ns/10ps
module clock_gen();

  task start();
      $display("Starting clocks.");
      top.dut0.clk_w = '0;
      top.dut0.clk_r = '1;
      repeat(100) begin
        #1ns top.dut0.clk_w = ~top.dut0.clk_w;
             top.dut0.clk_r = ~top.dut0.clk_r;
      end
  endtask

endmodule

