/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Logic.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   11/08/16
#   Description   :   
#
###############################################################################
*/

#include <utility>

#include "Logger.h"
#include "pli.h"

#include "Logic.h"

// ====================================
// ===**  Private Static Members  **===
// ====================================

// =============================
// ===** Public Properties **===
// =============================

// =============================
// ===**   Constructors    **===
// =============================
Logic::Logic(string iFullName)
  : TypeBase(iFullName, NB_STATES::FOUR_STATE)
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
void Logic::set_Size()
{
  // TBD - Get the size based on the verilog object.
  Vpi::OBJECT l_type = Pli::GetType(get_SigHandle());
  if(l_type != Vpi::OBJECT::REG)
  {
    LOG_WRN_ENV << "Object " << Get_NameFull() << " type (" << (Int32)l_type
                << ") was not the expected type for a logic object." << endl;
  }
  TypeBase::set_Size(Pli::GetSize(get_SigHandle()));
}

// =============================
// ===**     Operators     **===
// =============================
Logic & Logic::operator=  (UInt32 iRhs)
{
  get_BitVector() = iRhs;
  set_RtlValue();
  return *this;
}
Logic & Logic::operator=  (UInt64 iRhs)
{
  get_BitVector() = iRhs;
  set_RtlValue();
  return *this;
}
Logic & Logic::operator=  (const BitVector & iRhs)
{
  get_BitVector() = iRhs;
  set_RtlValue();
  return *this;
}
Logic & Logic::operator=  (BitVector && iRhs)
{
  get_BitVector() = move(iRhs);
  set_RtlValue();
  return *this;
}
Logic & Logic::operator=  (const Logic & iRhs)
{
  get_BitVector() = iRhs.get_BitVector();
  set_RtlValue();
  return *this;
}
Logic & Logic::operator=  (const TypeBase & iRhs)
{
  get_BitVector() = (BitVector)iRhs;
  set_RtlValue();
  return *this;
}
