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
#ifndef TYPEBASE_H
#define TYPEBASE_H

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

  // Private Members
  private:
    string                m_name;
    string                m_nameFull;
    vpiHandle             m_sigHandle;
    UInt32                m_size;
    BitVector *           m_bv;

    BitVector::NB_STATES  m_nbStates;

  // Protected Properties
  protected:

  // Public Properties
  public:

  // Constructors
  public:
    TypeBase(string iFullName, BitVector::NB_STATES iStates);

  // Inits
  private:
    bool init(string iFullName, BitVector::NB_STATES iStates);

  // Public Methods
  public:
    UInt32 GetValue();

  // Private Methods
  private:
    bool setHandle();
    void createBV();

  // Protected Methods
  protected:
    virtual void setSize() = 0;

  // Operators
  public:

};

#endif /* TYPEBASE_H */

