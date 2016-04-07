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

  logic rst_r, rst_w;
  logic init_done = '0;
  env env0;

  //object obj;
  //test tst;

  initial begin
    env0 = new();
    fork begin
      $display("Starting env init at %t", $time);
      env0.env_init();
      $display("Finished env init at %t", $time);
    end
    begin
      $display("Starting gen clocks at %t", $time);
      clk_gen0.start();
      $display("Finished gen clocks at %t", $time);
    end
    join_none
    //obj = new("obj");
    //$display("obj name is '%s', type is '%s'", obj.Name, obj.Type);
    //tst = new("tst");
    //$display("tst name is '%s', type is '%s', full type is '%s'", tst.Name, tst.Type, tst.Type_Full);
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

  // Clock Generator
  clock_gen clk_gen0();

  // DUT Instantiation
  dut dut0(.*);

endmodule

