/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/hammy025/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   top.sv 
#   Creator       :   Matt Hampton (hammy025@gmail.com)
#   Owner         :   Matt Hampton (hammy025@gmail.com)
#   Creation Date :   04/02/16
#   Description   :   
#
###############################################################################
*/
`timescale 1ns/10ps
module top();

  logic clk_r, clk_w;
  logic rst_r, rst_w;

  initial begin
    $dumpfile("sim.vcd");
    $dumpvars;
    clk_w = '0;
    repeat(10) begin
      $display("tick...");
      #1ns clk_w = ~clk_w;
    end
  end

endmodule

