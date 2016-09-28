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

#include <string>
#include <vector>

#include "Common.h"

using namespace std;

class TestController
{
  // Enums
  public:

  // Nested Classes
  public:

  // Private Members
  private:
    bool             m_initDone;
    vector<string> * m_commandLineArgs;

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
    void Init();

  // Public Methods
  public:
    void PrintCommandLineArgs();

  // Private Methods
  private:
    void commandLineArgs_set();

};

#endif /* TESTCONTROLLER_H */
