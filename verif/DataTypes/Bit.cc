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
  : TypeBase(iFullName, NB_STATES::TWO_STATE)
{
  // Set the bitvector size based on the type and/or size of the verilog object.
  setSize();
  createBV();
  getRtlValue();
  //LOG_DEBUG << "Bit::Bit: Initial value:     " << Bv_get().ToString() << endl;
  //Bv_get() += 0xffffULL;
  //LOG_DEBUG << "Bit::Bit: Incremental value: " << Bv_get().ToString() << endl;
  
  BitVector test1("testBV1", 96);
  BitVector test2("testBV2", 256);
  test1 = 0x2222222233333333;
  test1(95,64) = 0x11111111;
  test2 = test1;
  test1 (63,0) = 0xcccccccccccccccc;

  test2(159,64) |= test1(63,0) << 64;

  if(0xff >= 1)
  {
    LOG_DEBUG << "=============== true ===============" << endl;
  }
  else
  {
    LOG_DEBUG << "=============== false ===============" << endl;
  }
  LOG_DEBUG << "test1 is          " << test1 << endl;
  LOG_DEBUG << "test2 is " << test2 << endl;
  /*
  Bv_get() = 0x0123456789abcdefULL;
  //Bv_get() = Bv_get() + (UInt64)0xffffffffffffffULL;
  //Bv_get() = 0x89abcdef;
  setRtlValue();
  LOG_DEBUG << "Bit::Bit: After setting to 0x01...ef: " << Bv_get().ToString() << endl;
  BitVector rhs("Bit_temp", 56, NB_STATES::TWO_STATE);
  rhs = 0xffffffffffffffULL;
  LOG_DEBUG << "New BV value is " << rhs.ToString() << endl;
  Bv_get() = Bv_get() + rhs(11,0);
  LOG_DEBUG << "Bit::Bit: After BV addition: " << Bv_get().ToString() << endl;
  */
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
