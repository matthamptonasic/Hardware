/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   TestDB.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   10/04/16
#   Description   :   
#
###############################################################################
*/

#include "TestDB.h"

// *==*==*==*==*==*==*==*==*==*==*==*==*
// ===**       TestDB Class        **===
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
TestDB::TestDB()
{
  m_testMap = new map<string, TestBase *>();
  m_testToRun = nullptr;
}

// =============================
// ===**      Inits        **===
// =============================


// =============================
// ===**  Public Methods   **===
// =============================
void TestDB::AddTest(TestBase * iTest)
{
  if(iTest == nullptr)
  {
    // TBD - log error.
    return;
  }
  if(m_testMap->find(iTest->getName()) != m_testMap->end())
  {
    // This test is already in the map.
    // TBD - log warning.
    return;
  }
  (*m_testMap)[iTest->getName()] = iTest;
}
TestBase * TestDB::GetTest(string iTestName)
{
  if(m_testMap->find(iTestName) == m_testMap->end())
  {
    // Test not found in map.
    // TBD - log error.
    return nullptr;
  }
  return (*m_testMap)[iTestName];
}
void TestDB::SetTestToRun(string iTestName)
{
  if(m_testMap->find(iTestName) == m_testMap->end())
  {
    // Test not found in map.
    // TBD - log error.
    return;
  }
  m_testToRun = (*m_testMap)[iTestName];
}
TestBase * TestDB::GetTestToRun()
{
  if(m_testToRun == nullptr)
  {
    // TBD - Log error.
  }
  return m_testToRun;
}

// =============================
// ===**  Private Methods  **===
// =============================


// =============================
// ===**     Operators     **===
// =============================
