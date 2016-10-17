/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   TypeBase.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/16/16
#   Description   :   
#
###############################################################################
*/
#ifndef TypeBase_H
#define TypeBase_H

#include <string>

#include "Common.h"
#include "vpi.h"

using namespace std;

// Make this abstract. This will carry the BitVector (move it to this directory).
// Make a 2-state and 4-state abstract base as well.
// 2-state child will only have the 'bit' class.
// 4-state children will be logic, (typedef reg to be same as logic), & integer (32-bit BV).

class TypeBase 
{
  // Enums
  public:
  enum class NB_STATES : Byte
  {
    TWO_STATE = 0,
    FOUR_STATE = 1
  };

  // Members
  private:
    string m_name;
    string m_name_full;
    vpiHandle m_sigHandle;

    NB_STATES m_nb_states;

  // Constructors
  public:
    TypeBase(string iFullName);

  // Inits
  private:
    bool init(string iFullName, TypeBase::NB_STATES iValue);

  // Public Methods
  public:
    UInt32 GetValue();

  // Private Methods
  private:
    bool setHandle();
};

#endif /* TypeBase_H */

