/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/hammy025/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   dut.sv 
#   Creator       :   Matt Hampton (hammy025@gmail.com)
#   Owner         :   Matt Hampton (hammy025@gmail.com)
#   Creation Date :   04/05/16
#   Description   :   Instantiates the DUT and handles the selection of which
#                     module(s) is(are) active.
#
###############################################################################
*/

module dut(
  output logic              empty, 
  output logic              full,
  output logic [WIDTH-1:0]  rd_data,
  output logic [ADDR:0]     occup,

  input  wire               rst_r, 
  input  wire               rst_w, 
  input  wire [WIDTH-1:0]   wr_data,
  input  wire               wr_en, 
  input  wire               rd_en
  );

  parameter WIDTH = `FIFO_FF_DUT_WIDTH;
  parameter DEPTH = `FIFO_FF_DUT_DEPTH;
  parameter ADDR = $clog2(DEPTH);

  parameter NB_FIFOS = 2;
  
  logic clk_r;
  logic clk_w;

  int sel;

  always@(posedge clk_r) begin
    if(rst_r) begin
      rd_data <= '0;
    end
    else begin
      rd_data <= '1;
    end
  end

  
  logic              empty_s    [NB_FIFOS];
  logic              full_s     [NB_FIFOS];
  logic [WIDTH-1:0]  rd_data_s  [NB_FIFOS];
  logic [ADDR:0]     occup_s    [NB_FIFOS];

  logic              clk_r_s    [NB_FIFOS];
  logic              clk_w_s    [NB_FIFOS];
  logic              rst_r_s    [NB_FIFOS];
  logic              rst_w_s    [NB_FIFOS];
  logic [WIDTH-1:0]  wr_data_s  [NB_FIFOS];
  logic              wr_en_s    [NB_FIFOS];
  logic              rd_en_s    [NB_FIFOS];

  always @(*) begin
    for(int ii=0; ii<NB_FIFOS; ii++) begin
      clk_r_s[ii] = '0;
      clk_w_s[ii] = '0;
      rst_r_s[ii] = '0;
      rst_w_s[ii] = '0;
      wr_data_s[ii] = '0;
      wr_en_s[ii] = '0;
      rd_en_s[ii] = '0;
    end
    empty = '0;
    full = '0;
    rd_data = '0;
    occup = '0;

    clk_r_s[sel] = clk_r;
    clk_w_s[sel] = clk_w;
    rst_r_s[sel] = rst_r;
    rst_w_s[sel] = rst_w;
    wr_data_s[sel] = wr_data;
    wr_en_s[sel] = wr_en;
    rd_en_s[sel] = rd_en;

    empty = empty_s[sel];
    full = full_s[sel];
    rd_data = rd_data_s[sel];
    occup = occup_s[sel];
  end
 
  fifo_ff_sync_c #( .WIDTH(WIDTH),
                    .DEPTH(DEPTH),
                    .ADDR(ADDR))
    sync_c(
      .empty    (empty_s[0]),
      .full     (full_s[0]),
      .rd_data  (rd_data_s[0]),
      .occup    (occup_s[0]),
      .clk      (clk_w_s[0]),
      .rst      (rst_w_s[0]),
      .wr_data  (wr_data_s[0]),
      .wr_en    (wr_en_s[0]),
      .rd_en    (rd_en_s[0])
    );

  fifo_ff_sync_r #( .WIDTH(WIDTH),
                    .DEPTH(DEPTH),
                    .ADDR(ADDR))
    sync_r(
      .empty    (empty_s[1]),
      .full     (full_s[1]),
      .rd_data  (rd_data_s[1]),
      .occup    (occup_s[1]),
      .clk      (clk_w_s[1]),
      .rst      (rst_w_s[1]),
      .wr_data  (wr_data_s[1]),
      .wr_en    (wr_en_s[1]),
      .rd_en    (rd_en_s[1])
    );

endmodule

