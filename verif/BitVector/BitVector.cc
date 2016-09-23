/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   BitVector.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/15/16
#   Description   :   
#
###############################################################################
*/

#include "BitVector.h"

BitVector::NB_STATES BitVector::g_nbStates = BitVector::NB_STATES::FOUR_STATE;

BitVector::BitVector(string iName, UInt32 iSize)
{
  init(iName, iSize, g_nbStates);
}

void BitVector::init(string iName, UInt32 iSize, NB_STATES iStates)
{
  m_name = iName;
  m_size = iSize;
  m_nbStates = iStates;
  Int32 nbWds = (iSize - 1) / 32 + 1; 
  m_aval = new vector<UInt32>(nbWds, 0);
  if(m_nbStates == BitVector::NB_STATES::FOUR_STATE)
  {
    m_bval = new vector<UInt32>(nbWds, 0);
  }
}

UInt32 BitVector::GetUInt32()
{
  return (*m_aval)[0];
}
UInt64 BitVector::GetUInt64()
{
  // TBD - Add size checking as well as masking if < 64b.
  UInt64 retVal = 0;
  UInt64 hi = (*m_aval)[1]; 
  retVal = hi << 32;
  retVal |=  (*m_aval)[0];
  return retVal;
}

BitVector & BitVector::operator= (Int32 iRhs)
{
  if(m_size <= 0)
  {
    // TBD - log error.
    return *this;
  }
  UInt32 temp = (UInt32)iRhs;
  (*m_aval)[0] = temp;
  return *this;
}
