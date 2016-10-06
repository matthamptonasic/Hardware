/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   TestBase.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/15/16
#   Description   :   
#
###############################################################################
*/
#ifndef TESTBASE_H
#define TESTBASE_H

#include <string>

#include "Common.h"

using namespace std;

class TestBase
{
  // Enums
  public:

  // Constants
  private:

  // Nested Classes
  public:

  // Private Members
  private:
    string m_name;

  // Public Properties
  public:
    string getName() const { return m_name; }

  // Constructors
  public:
    TestBase(string iTestName = "Unnamed Test");

  // Inits
  private:

  // Public Methods
  public:
    virtual void Run() = 0;

  // Private Methods
  private:

};

#endif /* TESTBASE_H */

