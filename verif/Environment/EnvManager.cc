/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   EnvManager.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   10/06/16
#   Description   :   
#
###############################################################################
*/

#include <iostream>

#include "Logger.h"
#include "Parsing.h"
#include "TestController.h"

#include "EnvManager.h"

using namespace std;
using namespace Text;

// *==*==*==*==*==*==*==*==*==*==*==*==*
// ===**     EnvManager Class      **===
// *==*==*==*==*==*==*==*==*==*==*==*==*


// =============================
// ===** Public Properties **===
// =============================


// =============================
// ===**    Constructor    **===
// =============================
EnvManager::EnvManager()
{
  init();
}

// =============================
// ===**      Inits        **===
// =============================
void EnvManager::init()
{
  // Set up test controller.
  init_CmdArgs();
}

bool EnvManager::init_CmdArgs()
{
  if(!init_Logger())
  {
    return false;
  }

  return true;
}

bool EnvManager::init_Logger()
{
  string l_logFile = TestController::Access().GetCmdArg_string(c_logFile);
  if(l_logFile == "")
  {
    cout << "[ERROR] " << __PRETTY_FUNCTION__ << ": TestController returned empty log file name." << endl;
    return false;
  }
  Logger * tmp = new Logger(l_logFile, &cout);
  tmp->SetAsDout();

  // TBD - check for logging level to set.
  // i.e. logLvl=250 or logLvl=HIGH (use a *match* on the major levels)
  // Also support any scope with:
  // i.e. logScope=TestBase
  // This is where we need to get tricky and allow a hierarchical scoping.
  // So, for instance, the user wants to log everything under the TestBase tree.
  // The inherited classes will all have a different class name.
  // So how do we enable all of their scopes as well?
  // Like UVM, we'll have a registry of class objects. This will allow us to 
  // determine the full hierarchical name of the class.
  // From there we can determine if the scope we're adding includes this class.
  // When this is run, we'll go through any objects that are already registered
  // and enable their scopes.
  // After this is run, the newly registered objects will need to enable their scopes
  // while registering.

  // logLvl
  string l_logLvl = TestController::Access().GetCmdArg_string(c_logLvl);
  if(l_logLvl == "")
  {
    // Try getting a UInt32.
    UInt32 l_logLvl_UInt32 = TestController::Access().GetCmdArg_UInt32(c_logLvl);
    if(l_logLvl_UInt32 != 0)
    {
      DOUT.VerbosityLevel_set(l_logLvl_UInt32);
    }
  }
  else
  {
    if(l_logLvl == "DEBUG")
    {
      DOUT.VerbosityLevel_set(Logger::Scope::Vrb_DEBUG());
    }
    else if(l_logLvl == "FULL")
    {
      DOUT.VerbosityLevel_set(Logger::Scope::Vrb_FULL());
    }
    else if(l_logLvl == "HIGH")
    {
      DOUT.VerbosityLevel_set(Logger::Scope::Vrb_HIGH());
    }
    else if(l_logLvl == "MEDIUM")
    {
      DOUT.VerbosityLevel_set(Logger::Scope::Vrb_MEDIUM());
    }
    else if(l_logLvl == "LOW")
    {
      DOUT.VerbosityLevel_set(Logger::Scope::Vrb_LOW());
    }
    else if(l_logLvl == "NONE")
    {
      DOUT.VerbosityLevel_set(Logger::Scope::Vrb_NONE());
    }
    else
    {
      LOG_WRN_ENV << "logLvl passed (" << l_logLvl << ") is not recognized." << endl;
    }
  }
  LOG_MSG << "Verbosity Level is set to " << DOUT.VerbosityLevel_get() << endl;

  return true;
}

// =============================
// ===**  Public Methods   **===
// =============================


// =============================
// ===**  Private Methods  **===
// =============================


// =============================
// ===**     Operators     **===
// =============================
