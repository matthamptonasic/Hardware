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
  : TypeBase_TwoState(iFullName)
{

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
}

// =============================
// ===**     Operators     **===
// =============================
