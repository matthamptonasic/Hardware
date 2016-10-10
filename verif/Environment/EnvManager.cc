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

#include "TestController.h"
#include "Logger.h"

#include "EnvManager.h"

using namespace std;

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
  tmp->SetDout(tmp);
  
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
