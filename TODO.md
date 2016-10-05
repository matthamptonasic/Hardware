This is the top-level status of things that I need to get done.
Mainly a checklist for the initial environment setup.

- Abandoning using SV OOP. There are just too many things that are not working.
  I am spending 80% of my time trying to debug and coerce things to work.
  - Moving to using VPI and C++.

- Simulation flow.
  - Phases like UVM.
  - Threading.
    - How to keep the test function going while the simulation is running.
      - The driver/monitor/models are called every clock from the simulator.
      - The test is on a different thread and is only synchronized to the 
        simulation when it calls certain methods like tbWait.
      - The wait functions somehow pause the test thread (keeps from returning)
        while the simulator runs on it's thread.
        Some type of semiphore is required to hold the wait/test thread
        until a specific event happens in the simulator (i.e. a callback
        scheduled for some future time, or a clock counter hits a value).
- Messaging (LOG, DEBUG, WARNING, ERROR, logging level, etc).
- Test
  The appropriate test class/method is called based on the user parameters.
- Test container.
  - Will hold/parse all of the tests. So the user can simply pass arguments
    to the run command and the test container will select the appropriate test
    and set the test arguments/settings appropriately.

- Driver/Monitor
  - These are tricky since Icarus Verilog does not support interfaces.
  - At some level our class-based transactions will have to drive signals
    based on a hard-coded net hierarchy (and similarly monitor).
  - I am guessing we will have to have some sort of interpreter that recieves
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

