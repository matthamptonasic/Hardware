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
    vpiHandle   get_SigHandle() const { return m_sigHandle; }
    BitVector & get_BitVector() const        { return *m_bv; };
    void        set_Size(UInt32 iSize) { m_size = iSize; }

  // Public Properties
  public:
    string      Get_Name() const      { return m_name; }
    string      Get_NameFull() const  { return m_nameFull; }
    UInt32      Get_Size() const      { return m_size; }

  // Constructors
  public:
    TypeBase(string iFullName, NB_STATES iStates);

  // Inits
  private:
    bool init(string iFullName, NB_STATES iStates);

  // Public Methods
  public:
    UInt32 Get_Value();

  // Private Methods
  private:
    

  // Protected Methods
  protected:
    virtual void set_Size() = 0;
    bool set_Handle();
    void createBV();
    void get_RtlValue();
    void set_RtlValue();

  // Operators
  public:
    
};

#endif /* TYPEBASE_H */

