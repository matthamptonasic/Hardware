/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   test_manager.sv 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/05/16
#   Description   :   
#
###############################################################################
*/

class test_manager extends object;

  local static test_manager m_instance;

  local function new(string iName="test_manager");
  begin
    super.new(iName);
    Name = iName;
    Type = "test_manager";
    Type_Full = {Type_Full, ".test_manager"};
  end
  endfunction

  static function test_manager Access();
    if(m_instance == null) begin
      m_instance = new();
    end
    return m_instance;
  endfunction

endclass

