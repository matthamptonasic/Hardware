/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   sample_test.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   10/05/16
#   Description   :   
#
###############################################################################
*/

#include <iostream>
#include <string>

#include "Common.h"
#include "TestBase.h"
#include "vpi.h"

using namespace std;

class sample_test : public TestBase
{
  public:
    sample_test(string iTestName)
      : TestBase(iTestName) {
    };
    virtual void Run();
};

sample_test sample_test_instance("sample_test");

void sample_test::Run()
{
  // Stub
}

