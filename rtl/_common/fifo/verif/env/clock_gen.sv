/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   clock_gen.sv 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   04/06/16
#   Description   :   
#
###############################################################################
*/
`timescale 1ns/10ps
class clock_gen;

  real rd_clk_period = 1.5;
  real wr_clk_period = 1.0;

  function new();
    begin
      $display("Creating clock generator object.");
    end
  endfunction

  task start();
    $display("Starting clocks.");
      fork begin
        top.dut0.clk_w = '0;
        forever begin
          #(wr_clk_period/2) top.dut0.clk_w = ~top.dut0.clk_w;
        end
      end
      begin
        top.dut0.clk_r = '1;
        forever begin
          #(rd_clk_period/2) top.dut0.clk_r = ~top.dut0.clk_r;
        end
      end
    join
  endtask

endclass

