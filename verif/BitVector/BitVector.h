/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   BitVector.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/15/16
#   Description   :   
#
###############################################################################
*/
#ifndef BITVECTOR_H
#define BITVECTOR_H

#include "Common.h"

class BitVector {
  private:
    UInt32 m_size;
    UInt32 m_lo;
    UInt32 m_hi;
  public:

    BitVector(UInt32 iSize);
};

#endif /* BITVECTOR_H */

