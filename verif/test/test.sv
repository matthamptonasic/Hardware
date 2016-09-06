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

class test extends object;

  int errors = '0;

  function new(string iName="test");
  begin
    super.new(iName);
    Name = iName;
    Type = "test";
    Type_Full = {Type_Full, ".test"};
  end
  endfunction

  virtual function void run();
  endfunction
endclass

