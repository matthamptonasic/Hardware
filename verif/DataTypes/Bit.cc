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
  : TypeBase(iFullName, BitVector::NB_STATES::TWO_STATE)
{
  // Set the bitvector size based on the type and/or size of the verilog object.
  setSize();
  createBV();
  Bv_get() = Pli::GetVector(SigHandle_get());
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
void Bit::setSize()
{
  // TBD - Get the size based on the verilog object.
  Vpi::OBJECT l_type = Pli::GetType(SigHandle_get());
  if(l_type != Vpi::OBJECT::BIT)
  {
    LOG_WRN_ENV << "Object " << NameFull_get() << " type (" << (Int32)l_type
                << ") was not the expected type for a bit object." << endl;
  }
  Size_set(Pli::GetSize(SigHandle_get()));
}

// =============================
// ===**     Operators     **===
// =============================
