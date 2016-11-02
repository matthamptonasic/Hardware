/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Bit.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   10/18/16
#   Description   :   
#
###############################################################################
*/

#include <utility>

#include "BitVector.h"
#include "Logger.h"
#include "pli.h"

#include "Bit.h"

// ====================================
// ===**  Private Static Members  **===
// ====================================

// =============================
// ===** Public Properties **===
// =============================

// =============================
// ===**   Constructors    **===
// =============================
Bit::Bit(string iFullName)
  : TypeBase(iFullName, NB_STATES::TWO_STATE)
{
  // Set the bitvector size based on the type and/or size of the verilog object.
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
void Bit::set_Size()
{
  // TBD - Get the size based on the verilog object.
  Vpi::OBJECT l_type = Pli::GetType(get_SigHandle());
  if(l_type != Vpi::OBJECT::BIT)
  {
    LOG_WRN_ENV << "Object " << Get_NameFull() << " type (" << (Int32)l_type
                << ") was not the expected type for a bit object." << endl;
  }
  TypeBase::set_Size(Pli::GetSize(get_SigHandle()));
}

// =============================
// ===**     Operators     **===
// =============================
Bit & Bit::operator=  (UInt32 iRhs)
{
  get_BitVector() = iRhs;
  set_RtlValue();
  return *this;
}
Bit & Bit::operator=  (UInt64 iRhs)
{
  get_BitVector() = iRhs;
  set_RtlValue();
  return *this;
}
Bit & Bit::operator=  (const BitVector & iRhs)
{
  get_BitVector() = iRhs;
  set_RtlValue();
  return *this;
}
Bit & Bit::operator=  (BitVector && iRhs)
{
  get_BitVector() = move(iRhs);
  set_RtlValue();
  return *this;
}
