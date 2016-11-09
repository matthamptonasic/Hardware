/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Integer.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   11/09/16
#   Description   :   
#
###############################################################################
*/

#include <utility>

#include "Logger.h"
#include "pli.h"

#include "Integer.h"

// ====================================
// ===**  Private Static Members  **===
// ====================================

// =============================
// ===** Public Properties **===
// =============================

// =============================
// ===**   Constructors    **===
// =============================
Integer::Integer(string iFullName)
  : TypeBase(iFullName, NB_STATES::FOUR_STATE)
{
  set_Size();
  createBV();
  get_RtlValue();
}

// =============================
// ===**      Inits        **===
// =============================

// =============================
// ===**  Public Methods   **===
// =============================

// =============================
// ===**  Private Methods  **===
// =============================

// =============================
// ===** Protected Methods **===
// =============================
void Integer::set_Size()
{
  // TBD - Get the size based on the verilog object.
  Vpi::OBJECT l_type = Pli::GetType(get_SigHandle());
  if(l_type != Vpi::OBJECT::INTEGER_VAR)
  {
    LOG_WRN_ENV << "Object " << Get_NameFull() << " type (" << (Int32)l_type
                << ") was not the expected type for a logic object." << endl;
  }
  TypeBase::set_Size(32);
}

// =============================
// ===**     Operators     **===
// =============================
Integer & Integer::operator=  (UInt32 iRhs)
{
  get_BitVector() = iRhs;
  set_RtlValue();
  return *this;
}
Integer & Integer::operator=  (UInt64 iRhs)
{
  get_BitVector() = iRhs;
  set_RtlValue();
  return *this;
}
Integer & Integer::operator=  (const BitVector & iRhs)
{
  get_BitVector() = iRhs;
  set_RtlValue();
  return *this;
}
Integer & Integer::operator=  (BitVector && iRhs)
{
  get_BitVector() = move(iRhs);
  set_RtlValue();
  return *this;
}
Integer & Integer::operator=  (const Integer & iRhs)
{
  get_BitVector() = iRhs.get_BitVector();
  set_RtlValue();
  return *this;
}
Integer & Integer::operator=  (const TypeBase & iRhs)
{
  get_BitVector() = (BitVector)iRhs;
  set_RtlValue();
  return *this;
}
