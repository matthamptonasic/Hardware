/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/hammy025/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   fifo_ff_sync_r.v
#   Creator       :   Matt Hampton (hammy025@gmail.com)
#   Owner         :   Matt Hampton (hammy025@gmail.com)
#   Creation Date :   03/21/16
#   Description   :   Synchronous flip-flop based fifo with 
#                     registered outputs.
#
###############################################################################
*/
module fifo_ff_sync_r (
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
  logic [ADDR:0]      occup_c;
  logic               empty_c;
  logic               full_c;

  logic [ADDR-1:0]    rd_ptr;
  logic [ADDR-1:0]    rd_ptr_c;
  logic               read_c;
   
  logic [ADDR-1:0]    wr_ptr;
  logic [ADDR-1:0]    wr_ptr_c;
  logic               write_c;

  integer             ii;

// TBD : Add SVA's

  always @(*) begin
    write_c = wr_en && !full;
    read_c  = rd_en && !empty;
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
      occup_c = (ADDR+1)'(occup + 1);
    end
    else if(~write_c & read_c) begin
      occup_c = (ADDR+1)'(occup - 1);
    end
    else begin
      occup_c = occup;
    end
  end

  always @(*) begin
    empty_c = (occup_c == '0);
    full_c  = (occup_c == DEPTH);
  end
   
  always @(posedge clk) begin
    if(rst) begin
      empty    <= '1;
      full     <= '0;
      wr_ptr   <= '0;
      rd_ptr   <= '0;
      occup    <= '0;
    end
    else begin
      empty    <= empty_c;
      full     <= full_c;
      wr_ptr   <= wr_ptr_c;
      rd_ptr   <= rd_ptr_c;
      occup    <= occup_c;
    end
  end
   
  always @(posedge clk) begin
    if(rst) begin
      for(ii=0; ii<DEPTH; ii++) begin
        mem[ii] <= '0;
      end
      rd_data <= '0;
    end
    else begin
      if(write_c) begin
        mem[wr_ptr] <= #1 wr_data;
      end
      if(read_c && empty) begin
        rd_data <= 'x;
      end
      else begin
        rd_data <= mem[rd_ptr];
      end
    end
  end

endmodule

