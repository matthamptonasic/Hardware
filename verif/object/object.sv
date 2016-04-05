/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/hammy025/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   object.sv 
#   Creator       :   Matt Hampton (hammy025@gmail.com)
#   Owner         :   Matt Hampton (hammy025@gmail.com)
#   Creation Date :   04/03/16
#   Description   :   Base object class that everything inherits from.
#
###############################################################################
*/

class object;
  string Name;
  string Type;
  string Type_Full;

  function new(string iName="object");
  begin
    Name = iName;
    Type = "object";
    Type_Full = "object";
  end
  endfunction
endclass

