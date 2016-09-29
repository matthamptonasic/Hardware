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

#include "Manip.h"
#include "pli.h"
#include "Parsing.h"
#include "TestController.h"

using namespace std;
using namespace Text;
using namespace Text::Manip;
using namespace Text::Parsing;

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
  init();
}
// =============================
// ===**      Inits        **===
// =============================
void TestController::init()
{
  commandLineArgs_set();
  findCArgs();
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
    Vpi::vpi_printf("cArgs = '%s'\n", m_cArgs.c_str());
}

// =============================
// ===**  Private Methods  **===
// =============================
void TestController::commandLineArgs_set()
{
  m_commandLineArgs = Pli::GetCommandLineArgs();
}
void TestController::findCArgs()
{
  // Loop through the vector of arguments and find one that starts with "+c_args=".
  m_cArgs = "";
  for(UInt32 ii=0; ii<m_commandLineArgs->size(); ii++)
  {
    if(Parsing::StartsWith(m_commandLineArgs->at(ii), c_cArgsPrefix))
    {
      if(m_cArgs != "")
      {
        m_cArgs += " ";
      }
      m_cArgs += Manip::Replace(m_commandLineArgs->at(ii), c_cArgsPrefix, "");
    }
  }
}
void TestController::parseCArgs()
{
  // Trim spaces.
  // Split by spaces.
  // Each string is now its own argument.
  // Trim off any quotes.
  // Parse token and value.
}

// =============================
// ===**     Operators     **===
// =============================
