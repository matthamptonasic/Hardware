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
    static bool             s_initDone;
    static vector<string> * s_commandLineArgs;

  // Public Properties
  public:
    static vector<string> & CommandLineArgs_get();

  // Inits
  public:
    static void Init();

  // Public Methods
  public:
    static void PrintCommandLineArgs();

  // Private Methods
  private:
    static void commandLineArgs_set();

};

#endif /* TESTCONTROLLER_H */
