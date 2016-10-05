/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   TestController.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/15/16
#   Description   :   Controls the flow of the test.
#                     This includes which test is run.
#
###############################################################################
*/
#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include <map>
#include <string>
#include <vector>

#include "Common.h"
#include "TestDB.h"

using namespace std;

class TestController
{
  // Enums
  public:

  // Constants
  private:
    const string c_cArgsPrefix = "+c_args=";

  // Nested Classes
  public:

  // Private Members
  private:
    // Command Line Arguments
    vector<string> *      m_commandLineArgs;
    string                m_cArgs;
    map<string, Int64> *  m_nameToInt64Map;
    map<string, UInt64> * m_nameToUInt64Map;
    map<string, string> * m_nameToStringMap;
    map<string, bool> *   m_nameToPrintHexMap;

    // Test DataBase
    TestDB                m_testDb;

  // Public Properties
  public:
    vector<string> & CommandLineArgs_get();

  // Singleton Pattern
  public:
    static TestController & Access()
    {
      static TestController ls_inst; // Guaranteed to be destroyed.
      return ls_inst;                // Instantiated on 1st use.
    }
  private:
    TestController();
  public:
    TestController(TestController const &) = delete; // These are public for clarity of debug.
    void operator=(TestController const &) = delete; // Can be made private if intellisense is used.

  // Inits
  private:
    void init();
    void initMaps();

  // Public Methods
  public:
    void PrintCommandLineArgs();
    UInt32 GetCmdArg_UInt32(string iName);
    UInt64 GetCmdArg_UInt64(string iName);
    Int32  GetCmdArg_Int32(string iName);
    Int64  GetCmdArg_Int64(string iName);
    string GetCmdArg_string(string iName);
    
    void   RegisterTest(TestBase * iTest);

  // Private Methods
  private:
    void commandLineArgs_set();
    void findCArgs();
    void parseCArgs();
    void parseTokenValue(string & iToken, string & iValue);

    UInt64 getCmdArg_UInt64(string iName, bool & oFound);
    Int64  getCmdArg_Int64(string iName, bool & oFound);
    string getCmdArg_string(string iName, bool & oFound);

};

#endif /* TESTCONTROLLER_H */
