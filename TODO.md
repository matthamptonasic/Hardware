This is the top-level status of things that I need to get done.
Mainly a checklist for the initial environment setup.

- Abandoning using SV OOP. There's just too many things that aren't working.
  I'm spending 80% of my time trying to debug and coerce things to work.
  - Moving to using VPI and C++.

- Messaging (LOG, DEBUG, WARNING, ERROR, logging level, etc).
- Test
  - The test.run task is what's executed based on the user input.
  - Each test should be a class that inherits from the Test class.
  - The run task is overridden and performs the test.
  - The test should have some way of being registered in a container class
    so that it can be found in a list based on the user's input.
  - Not yet sure how a class (test) will drive the program and driver.
- Test container.
  - Will hold/parse all of the tests. So the user can simply pass arguments
    to the run command and the test container will select the appropriate test
    and set the test arguments/settings appropriately.

- Driver/Monitor
  - These are tricky since Icarus Verilog does not support interfaces.
  - At some level our class-based transactions will have to drive signals
    based on a hard-coded net hierarchy (and similarly monitor).
  - I'm guessing we'll have to have some sort of interpreter that recieves
    an object and translates the fields of the object into bit wiggles.

- Scoreboard
  - Will come after the driver/monitor.
  - Should be class based.

- Simple packet class.
  - Holds packet meta-data.
  - Payload is not generated unless necessary.
  - Will hold different headers as they are created.
  - Print and compare functions.
- Header types added later.
- Figure out how much randomization iVerilog supports.
  - Fill in the gaps with randomization classes.

