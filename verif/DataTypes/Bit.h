/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Bit.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   10/18/16
#   Description   :   
#
###############################################################################
*/
#ifndef BIT_H
#define BIT_H

#include <string>

#include "Common.h"
#include "TypeBase_TwoState.h"

using namespace std;

class Bit : TypeBase_TwoState
{
  // Enums
  public:

  // Private Members
  private:

  // Public Properties
  public:

  // Constructors
  public:
    Bit(string iFullName);

  // Inits
  private:

  // Public Methods
  public:

  // Private Methods
  private:

  // Protected Methods
  protected:
    void setSize();
  
  // Operators
  public:

};

#endif /* BIT_H */
