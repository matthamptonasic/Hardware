/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   EnvManager.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   10/06/16
#   Description   :   This class is the top level coordinator for the entire
#                     environment. It does all initialization at simulation
#                     start and runs the test and end of test actions.
#
###############################################################################
*/
#ifndef ENVMANAGER_H
#define ENVMANAGER_H

#include <string>
#include <vector>

#include "Common.h"

using namespace std;

class EnvManager
{
  // Enums
  public:

  // Constants
  private:
    // ====================================
    // ===**  Command Line Arguments  **===
    // ====================================
    // ex: +c_args="logFile=/home/matt/dump/Hardware/_common/fifo/run.log"
    const string c_logFile = "logFile"; // path + filename of logfile (all ostream output).
    const string c_logLvl = "logLvl";   // logLvl=250 or logLvl=HIGH (match the major verbosity levels).

  // Nested Classes
  public:

  // Private Members
  private:

  // Public Properties
  public:
    string LogFileName_get() const { return c_logFile; };

  // Singleton Pattern
  public:
    static EnvManager & Access()
    {
      static EnvManager ls_inst;     // Guaranteed to be destroyed.
      return ls_inst;                // Instantiated on 1st use.
    }
  private:
    EnvManager();
  public:
    EnvManager(EnvManager const &) = delete;     // These are public for clarity of debug.
    void operator=(EnvManager const &) = delete; // Can be made private if intellisense is used.

  // Inits
  private:
    void init();
    bool init_CmdArgs();
    bool init_Logger();

  // Public Methods
  public:

  // Private Methods
  private:

};

#endif /* ENVMANAGER_H */
