/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   TestController.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/15/16
#   Description   :   
#
###############################################################################
*/

#include "pli.h"
#include "TestController.h"

// *==*==*==*==*==*==*==*==*==*==*==*==*
// ===**   TestController Class    **===
// *==*==*==*==*==*==*==*==*==*==*==*==*

// ====================================
// ===**  Private Static Members  **===
// ====================================
bool             TestController::s_initDone = false;
vector<string> * TestController::s_commandLineArgs = nullptr;

// =============================
// ===** Public Properties **===
// =============================
vector<string> & TestController::CommandLineArgs_get()
{
  Init();
  return *s_commandLineArgs;
}

// =============================
// ===**    Constructor    **===
// =============================

// =============================
// ===**      Inits        **===
// =============================
void TestController::Init()
{
  if(s_initDone)
  {
    return;
  }
  commandLineArgs_set();

  s_initDone = true;
}

// =============================
// ===**  Public Methods   **===
// =============================
void TestController::PrintCommandLineArgs()
{
  Init();
  for(UInt32 ii=0; ii<s_commandLineArgs->size(); ii++)
  {
    // TBD - change to logger.
    Vpi::vpi_printf("Argument[%d] '%s'\n", ii, s_commandLineArgs->at(ii).c_str());
  }
}

// =============================
// ===**  Private Methods  **===
// =============================
void TestController::commandLineArgs_set()
{
  if(!s_initDone)
  {
    s_commandLineArgs = Pli::GetCommandLineArgs();
  }
  return;
}

// =============================
// ===**     Operators     **===
// =============================
