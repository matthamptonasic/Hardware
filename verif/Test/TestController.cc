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

// =============================
// ===** Public Properties **===
// =============================
vector<string> & TestController::CommandLineArgs_get()
{
  return *m_commandLineArgs;
}

// =============================
// ===**    Constructor    **===
// =============================
TestController::TestController()
{
  Init();
}
// =============================
// ===**      Inits        **===
// =============================
void TestController::Init()
{
  if(m_initDone)
  {
    return;
  }
  commandLineArgs_set();

  m_initDone = true;
}

// =============================
// ===**  Public Methods   **===
// =============================
void TestController::PrintCommandLineArgs()
{
  for(UInt32 ii=0; ii<m_commandLineArgs->size(); ii++)
  {
    // TBD - change to logger.
    Vpi::vpi_printf("Argument[%d] '%s'\n", ii, m_commandLineArgs->at(ii).c_str());
  }
}

// =============================
// ===**  Private Methods  **===
// =============================
void TestController::commandLineArgs_set()
{
  m_commandLineArgs = Pli::GetCommandLineArgs();
}

// =============================
// ===**     Operators     **===
// =============================
