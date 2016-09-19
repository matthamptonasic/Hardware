/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   top.sv 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   04/02/16
#   Description   :   
#
###############################################################################
*/

`timescale 1ns/10ps

module top();

  logic rst_r, rst_w;
  logic init_done = '0;
  env env0;
  clock_gen clks0;

  initial begin
    env0 = new();
    clks0 = new();
    $tb_build(top);
    fork begin
      $display("Starting env init at %t", $time);
      env0.env_init();
      $display("Finished env init at %t", $time);
    end
    begin
      $display("Starting gen clocks at %t", $time);
      clks0.start();
      $display("Finished gen clocks at %t", $time);
    end
    begin
      #10000ns;
      $display("Test finished at %t", $time);
      $finish;
    end
    join
    init_done = '1;
  end

  initial begin
    rst_r = '1;
    rst_w = '1;
    #10ns rst_w = '0;
    #5ns rst_r = '0;
  end

  initial begin
    dut0.sel = 0;
    #25 dut0.sel = 1;
  end

  // Interface members
  wire                                empty;
  wire                                full;
  wire [`FIFO_FF_DUT_WIDTH-1:0]       rd_data;
  wire [$clog2(`FIFO_FF_DUT_DEPTH):0] occup;
  wire [`FIFO_FF_DUT_WIDTH-1:0]       wr_data;
  wire                                wr_en;
  wire                                rd_en;

  // DUT Instantiation
  dut dut0(.*);

endmodule

