/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   TestBase.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/15/16
#   Description   :   
#
###############################################################################
*/

#include "TestBase.h"
#include "TestController.h"

// *==*==*==*==*==*==*==*==*==*==*==*==*
// ===**      TestBase Class       **===
// *==*==*==*==*==*==*==*==*==*==*==*==*

// ====================================
// ===**  Private Static Members  **===
// ====================================

// =============================
// ===** Public Properties **===
// =============================


// =============================
// ===**    Constructor    **===
// =============================
TestBase::TestBase(string iTestName)
{
  m_name = iTestName;
  TestController::Access().RegisterTest(this);
}

// =============================
// ===**      Inits        **===
// =============================


// =============================
// ===**  Public Methods   **===
// =============================


// =============================
// ===**  Private Methods  **===
// =============================


// =============================
// ===**     Operators     **===
// =============================
