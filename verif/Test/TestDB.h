/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   TestDB.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   10/04/16
#   Description   :   A list (map) of the known/compiled tests which
#                     inherit from TestBase.
#
###############################################################################
*/
#ifndef TESTDB_H
#define TESTDB_H

#include <map>

#include "Common.h"
#include "TestBase.h"

using namespace std;

class TestDB
{
  // Enums
  public:

  // Constants
  private:

  // Nested Classes
  public:

  // Private Members
  private:
    map<string, TestBase *> * m_testMap;
    TestBase *                m_testToRun;

  // Public Properties
  public:

  public:
    TestDB();

  // Inits
  private:

  // Public Methods
  public:
    void AddTest(TestBase * iTest);
    TestBase * GetTest(string iTestName);
    void SetTestToRun(string iTestName);
    TestBase * GetTestToRun();

  // Private Methods
  private:

};

#endif /* TESTDB_H */
