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

#include "Logger.h"
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
    LOG_ERR_ENV << "Test pointer was null." << endl;
    return;
  }
  if(m_testMap->find(iTest->getName()) != m_testMap->end())
  {
    LOG_WRN_ENV << "Test '" << iTest->getName() << " was registered more than once." << endl;
    return;
  }
  (*m_testMap)[iTest->getName()] = iTest;
}
TestBase * TestDB::GetTest(string iTestName)
{
  if(m_testMap->find(iTestName) == m_testMap->end())
  {
    LOG_ERR_ENV << "Test '" << iTestName << "' was not found in the test list." << endl;
    return nullptr;
  }
  return (*m_testMap)[iTestName];
}
void TestDB::SetTestToRun(string iTestName)
{
  if(m_testMap->find(iTestName) == m_testMap->end())
  {
    LOG_ERR_ENV << "Test '" << iTestName << "' was not found in the test list." << endl;
    return;
  }
  m_testToRun = (*m_testMap)[iTestName];
}
TestBase * TestDB::GetTestToRun()
{
  if(m_testToRun == nullptr)
  {
    LOG_ERR_ENV << "There is no test set to run." << endl;
  }
  return m_testToRun;
}

// =============================
// ===**  Private Methods  **===
// =============================


// =============================
// ===**     Operators     **===
// =============================
