/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   TypeBase_TwoState.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   10/17/16
#   Description   :   
#
###############################################################################
*/
#ifndef TYPEBASE_TWOSTATE_H
#define TYPEBASE_TWOSTATE_H

#include <string>

#include "Common.h"
#include "TypeBase.h"

using namespace std;

// Make this abstract. This will carry the BitVector (move it to this directory).
// Make a 2-state and 4-state abstract base as well.
// 2-state child will only have the 'bit' class.
// 4-state children will be logic, (typedef reg to be same as logic), & integer (32-bit BV).

class TypeBase_TwoState : TypeBase
{
  // Enums
  public:

  // Private Members
  private:

  // Public Properties
  public:

  // Protected Properties
  protected:
    string      Name_get() const      { return TypeBase::Name_get(); }
    string      NameFull_get() const  { return TypeBase::NameFull_get(); }
    vpiHandle   SigHandle_get() const { return TypeBase::SigHandle_get(); }
    UInt32      Size_get() const      { return TypeBase::Size_get(); }
    void        Size_set(UInt32 iSize) { TypeBase::Size_set(iSize); }
    BitVector & Bv_get() const        { return TypeBase::Bv_get(); };

  // Constructors
  public:
    TypeBase_TwoState(string iFullName);

  // Inits
  private:

  // Public Methods
  public:

  // Private Methods
  private:

  // Protected Methods
  protected:
    virtual void setSize() = 0;
    bool setHandle() { return TypeBase::setHandle(); }
    void createBV() { TypeBase::createBV(); }
  
  // Operators
  public:
  
};

#endif /* TYPEBASE_TWOSTATE_H */
