/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/hammy025/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   test.sv 
#   Creator       :   Matt Hampton (hammy025@gmail.com)
#   Owner         :   Matt Hampton (hammy025@gmail.com)
#   Creation Date :   04/04/16
#   Description   :   
#
###############################################################################
*/

class test extends object;

  function new(string iName="test");
  begin
    super.new(iName);
    Name = iName;
    Type = "test";
    Type_Full = {Type_Full, ".test"};
  end
  endfunction
endclass

