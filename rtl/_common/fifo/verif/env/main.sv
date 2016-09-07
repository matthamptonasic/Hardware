/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   main.sv 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   08/31/16
#   Description   :   
#
###############################################################################
*/
`timescale 1ns/10ps
program main();

  `include "/home/matt/code/Hardware/verif/object/object.sv"
  `include "/home/matt/code/Hardware/verif/test/test.sv"

  object obj;
  test tst;

  initial begin
    //$display("result is '%d'", result);
    //result = result.first;
    //$display("result is '%d'", result);
    $display("tick define is %d", 1);
    obj = new("obj");
    $display("obj name is '%s', type is '%s'", obj.Name, obj.Type);
    tst = new("tst");
    $display("tst name is '%s', type is '%s', full type is '%s'", tst.Name, tst.Type, tst.Type_Full);
    $display("result is %d", tst.result);
    tst.result = `TEST_RESULT_PASS;
    $display("result is %d", tst.result);
  end

  task run();
    $display("main::run: obj name is '%s', type is '%s'", obj.Name, obj.Type);
  endtask


endprogram

