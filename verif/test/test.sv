/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   test.sv 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   04/04/16
#   Description   :   
#
###############################################################################
*/

`define TEST_RESULT_PASS 0
`define TEST_RESULT_FAIL 1
class test extends object;

  int errors;
  int result;

  function new(string iName="test");
  begin
    super.new(iName);
    Name = iName;
    Type = "test";
    Type_Full = {Type_Full, ".test"};
    this.init();
  end
  endfunction

  local function void init();
  begin
    errors = '0;
    result = `TEST_RESULT_FAIL;
  end
  endfunction

  virtual function void run();
  endfunction
endclass

