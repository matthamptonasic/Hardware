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

class BitVector;

enum class NB_STATES : Byte
{
  TWO_STATE = 0,
  FOUR_STATE = 1
};

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

    NB_STATES             m_nbStates;

  // Protected Properties
  protected:
    string      Name_get() const      { return m_name; }
    string      NameFull_get() const  { return m_nameFull; }
    vpiHandle   SigHandle_get() const { return m_sigHandle; }
    UInt32      Size_get() const      { return m_size; }
    void        Size_set(UInt32 iSize) { m_size = iSize; }
    BitVector & Bv_get() const        { return *m_bv; };

  // Public Properties
  public:

  // Constructors
  public:
    TypeBase(string iFullName, NB_STATES iStates);

  // Inits
  private:
    bool init(string iFullName, NB_STATES iStates);

  // Public Methods
  public:
    UInt32 GetValue();

  // Private Methods
  private:
    

  // Protected Methods
  protected:
    virtual void setSize() = 0;
    bool setHandle();
    void createBV();
    void getRtlValue();
    void setRtlValue();

  // Operators
  public:

};

#endif /* TYPEBASE_H */

