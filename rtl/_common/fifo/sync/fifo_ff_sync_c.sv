/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   fifo_ff_sync_c.v
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   03/21/16
#   Description   :   Synchronous flip-flop based fifo with 
#                     combinational outputs.
#
###############################################################################
*/
module fifo_ff_sync_c (
  output logic              empty, 
  output logic              full,
  output logic [WIDTH-1:0]  rd_data,
  output logic [ADDR:0]     occup,

  input  wire               clk, 
  input  wire               rst, 
  input  wire [WIDTH-1:0]   wr_data,
  input  wire               wr_en, 
  input  wire               rd_en
  );

  // synopsys template
  parameter            WIDTH = 32;
  parameter            DEPTH = 16;
  parameter            ADDR = $clog2(DEPTH);

  logic [WIDTH-1:0]   mem [DEPTH-1:0];
  logic [WIDTH-1:0]   rd_data_c;
  logic [ADDR:0]      occup_c;
  logic [ADDR:0]      occup_r;
  logic               empty_c;
  logic               empty_r;
  logic               full_c;
  logic               full_r;

  logic [ADDR-1:0]    rd_ptr;
  logic [ADDR-1:0]    rd_ptr_c;
  logic               read_c;
   
  logic [ADDR-1:0]    wr_ptr;
  logic [ADDR-1:0]    wr_ptr_c;
  logic               write_c;

  integer             ii;

// TBD : Add SVA's

  always @(*) begin
    write_c = wr_en && !full_r;
    read_c  = rd_en && !empty_r;
  end
   
  always @(*) begin
    wr_ptr_c = ADDR'(wr_ptr + write_c);
    if(wr_ptr_c == DEPTH) begin
      wr_ptr_c = '0;
    end
      
    rd_ptr_c = ADDR'(rd_ptr + read_c);
    if(rd_ptr_c == ADDR'(DEPTH)) begin
      rd_ptr_c = 0;
    end
  end
   
  always @(*) begin
    if(write_c & ~read_c) begin
      occup_c = (ADDR+1)'(occup_r + 1);
    end
    else if(~write_c & read_c) begin
      occup_c = (ADDR+1)'(occup_r - 1);
    end
    else begin
      occup_c = occup_r;
    end
  end

  always @(*) begin
    empty_c = (occup_c == '0);
    full_c  = (occup_c == DEPTH);
  end
   
  always @(posedge clk) begin
    if(rst) begin
      empty_r  <= '1;
      full_r   <= '0;
      wr_ptr   <= '0;
      rd_ptr   <= '0;
      occup_r  <= '0;
    end
    else begin
      empty_r  <= empty_c;
      full_r   <= full_c;
      wr_ptr   <= wr_ptr_c;
      rd_ptr   <= rd_ptr_c;
      occup_r  <= occup_c;
    end
  end
   
  always @(posedge clk) begin
    if(rst) begin
      for(ii=0; ii<DEPTH; ii++) begin
        mem[ii] <= '0;
      end
    end
    else begin
      if(write_c) begin
        mem[wr_ptr] <= #1 wr_data;
      end
    end
  end

  always @(*) begin
    if(rst) begin
      rd_data_c = '0;
    end
    else begin
      if(read_c && empty_r) begin
        rd_data_c = 'x;
      end
      else begin
        rd_data_c = mem[rd_ptr_c];
      end
    end
  end

  assign empty = empty_c;
  assign full = full_c;
  assign rd_data = rd_data_c;
  assign occup = occup_c;

endmodule

