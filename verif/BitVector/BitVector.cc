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
#   Description   :   See header file.
#
###############################################################################
*/

#include "BitVector.h"

// ====================================
// ===**  Private Static Members  **===
// ====================================
BitVector::NB_STATES BitVector::g_nbStates = BitVector::NB_STATES::TWO_STATE;
const string BitVector::g_default_name = "Anonymous";
const UInt32 BitVector::g_default_size = 32;

// =============================
// ===**   Constructors    **===
// =============================
BitVector::BitVector(string iName)
{
  init(iName, 0, g_nbStates);
}
BitVector::BitVector(string iName, UInt32 iSize, BitVector::NB_STATES iStates)
{
  init(iName, iSize, iStates);
}
BitVector::BitVector(UInt32 iSize)
{
  init(g_default_name, iSize, g_nbStates);
}

// =============================
// ===**      Inits        **===
// =============================
void BitVector::init(string iName, UInt32 iSize, NB_STATES iStates)
{
  m_name = iName;
  m_size = iSize;
  m_nbStates = iStates;
  Int32 nbWds = (iSize - 1) / 32 + 1;
  if(m_aval != NULL)
  {
    delete m_aval;
  }
  m_aval = new vector<UInt32>(nbWds, 0);
  if(m_nbStates == BitVector::NB_STATES::FOUR_STATE)
  {
    if(m_bval != NULL)
    {
      delete m_bval;
    }
    m_bval = new vector<UInt32>(nbWds, 0);
  }
}

// =============================
// ===**  Public Methods   **===
// =============================
void   BitVector::Resize(UInt32 iNewSize)
{
  UInt32 l_wds = getWordNb(iNewSize);
  m_size = iNewSize;
  if(m_aval->size() != l_wds)
  {
    m_aval->resize(l_wds, 0);
    if(m_nbStates == NB_STATES::FOUR_STATE)
    {
      m_bval->resize(l_wds, 0);
    }
  }
  setMask();
}
UInt32 BitVector::GetUInt32() const
{
  return (*m_aval)[0];
}
UInt64 BitVector::GetUInt64() const
{
  // TBD - Add size checking as well as masking if < 64b.
  UInt64 retVal = 0;
  UInt64 hi = (*m_aval)[1]; 
  retVal = hi << 32;
  retVal |=  (*m_aval)[0];
  return retVal;
}

// =============================
// ===**  Private Methods  **===
// =============================
void BitVector::checkIndices(UInt32 & iUpperIndex, UInt32 & iLowerIndex)
{
  if(iUpperIndex < iLowerIndex)
  {
    UInt32 tmp = iLowerIndex;
    iLowerIndex = iUpperIndex;
    iUpperIndex = tmp;
  }
}
inline UInt32 BitVector::getWordNb(UInt32 iBitPos)
{
  // Word Nb is Bit Position / 32.
  return (iBitPos >> 5);
}
inline Byte BitVector::getShift(UInt32 iBitPos)
{
  return iBitPos % 32;
}
UInt32 BitVector::getMask(UInt32 iUpperIndex)
{
  UInt32 l_mask = 0;
  Byte l_upperBitPos = iUpperIndex % 32;
  for(UInt32 ii=0; ii<=l_upperBitPos; ii++)
  {
    l_mask |= (1 << ii);
  }
  return l_mask;
}
void BitVector::setMask()
{
  m_mask = 0;
  if(m_size == 0)
  {
    return;
  }
  m_mask = getMask(m_size - 1);
}
UInt32 BitVector::getBits(UInt32 iUpperIndex, UInt32 iLowerIndex)
{
  checkIndices(iUpperIndex, iLowerIndex);
  UInt32 l_selSize = iUpperIndex - iLowerIndex + 1;
  if(l_selSize > 32)
  {
    // TBD - Log error.
    return 0;
  }
  if(iUpperIndex > m_size)
  {
    // TBD - Log warning.
    iUpperIndex = m_size;
    l_selSize = iUpperIndex - iLowerIndex + 1;
  }
  
  UInt32 l_loWdNb = getWordNb(iLowerIndex);
  UInt32 l_hiWdNb = getWordNb(iUpperIndex);
  Byte l_shift = getShift(iLowerIndex);

  UInt32 l_retVal = (*m_aval)[l_loWdNb];
  if(l_shift > 0)
  {
    l_retVal >>= l_shift;
  }

  if(l_loWdNb != l_hiWdNb)
  {
    UInt32 l_hiWd = (*m_aval)[l_loWdNb+1];
    l_hiWd &= getMask(iUpperIndex);
    l_retVal |= (l_hiWd << (32 - l_shift));
  }
  return l_retVal;
}

// =============================
// ===**     Operators     **===
// =============================
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


// *==*==*==*==*==*==*==*==*==*==*==*==*
// ===**     Part Select Class    **===
// *==*==*==*==*==*==*==*==*==*==*==*==*

// =============================
// ===**   Constructors    **===
// =============================
BitVector::PartSelect::PartSelect(BitVector * iBV, UInt32 iUpperIndex, UInt32 iLowerIndex)
{
  init(iBV, iUpperIndex, iLowerIndex);
}

// =============================
// ===**      Inits        **===
// =============================
void BitVector::PartSelect::init(BitVector * iBV, UInt32 iUpperIndex, UInt32 iLowerIndex)
{
  if(iBV == NULL)
  {
    // TBD - log error.
    return;
  }
  m_parent = iBV;
  iBV->checkIndices(iUpperIndex, iLowerIndex);
  m_upperIndex = iUpperIndex;
  m_lowerIndex = iLowerIndex;
  if(m_upperIndex > m_parent->m_size)
  {
    m_upperIndex = m_parent->m_size;
  }
}

// =============================
// ===**  Public Methods   **===
// =============================
UInt32 BitVector::PartSelect::getUInt32()
{
  if(m_parent == NULL)
  {
    // TBD - log error.
    return 0;
  }
  UInt32 l_selSize = m_upperIndex - m_lowerIndex + 1;
  UInt32 l_upperIndex = m_upperIndex;
  if(l_selSize > 32)
  {
    l_selSize = 32;
    l_upperIndex = m_lowerIndex + 32 - 1;
  }
  UInt32 l_loWdNb = m_parent->getWordNb(m_lowerIndex);
  UInt32 l_hiWdNb = m_parent->getWordNb(l_upperIndex);
  Byte l_shift = m_parent->getShift(m_lowerIndex);
  UInt32 l_retVal = (*(m_parent->m_aval))[l_loWdNb] >> l_shift;
  if(l_loWdNb != l_hiWdNb)
  {
    UInt32 l_hiWd = (*(m_parent->m_aval))[l_loWdNb+1];
    l_hiWd &= m_parent->getMask(l_upperIndex);
    l_retVal |= (l_hiWd >> (32 - l_shift));
  }
  return l_retVal;
}
UInt64 BitVector::PartSelect::getUInt64()
{
  if(m_parent == NULL)
  {
    // TBD - log error.
    return 0;
  }
  UInt32 l_selSize = m_upperIndex - m_lowerIndex + 1;
  UInt32 l_upperIndex = m_upperIndex;
  if(l_selSize > 64)
  {
    l_selSize = 64;
    l_upperIndex = m_lowerIndex + 64 - 1;
  }
  UInt32 l_loWdNb = m_parent->getWordNb(m_lowerIndex);
  UInt32 l_hiWdNb = m_parent->getWordNb(l_upperIndex);
  Byte l_shift = m_parent->getShift(m_lowerIndex);
  
  // Nb words the select is spread over is based on part select size and the shift value.
  // NbWords = (SelSize + l_shift - 1) / 32 + 1
  // [63:0]  => ( 64 + 0 - 1) / 32 + 1 = 2
  // [64:1]  => ( 64 + 1 - 1) / 32 + 1 = 3
  // [63:32] => ( 32 + 0 - 1) / 32 + 1 = 1
  
  UInt32 l_nbWords = (l_selSize + l_shift - 1) / 32 + 1;
  UInt64 l_retVal = (*(m_parent->m_aval))[l_loWdNb] >> l_shift;
  for(UInt32 ii=1; ii<=l_nbWords; ii++)
  {
    UInt32 l_nextWord = (*(m_parent->m_aval))[l_loWdNb+ii];
    if((l_loWdNb + ii) == l_hiWdNb)
    {
      l_nextWord &= m_parent->getMask(l_upperIndex);
    }
    l_retVal |= l_nextWord << ((ii * 32) - l_shift);
  }
  return l_retVal;
}


// =============================
// ===**  Private Methods  **===
// =============================
void BitVector::PartSelect::setParentBits(const PartSelect & iBits)
{
  if((this->m_parent == NULL) || (iBits.m_parent == NULL))
  {
    // TBD - log error.
    return;
  }
  // Note: Assuming that the upper and lower values of each PartSelect
  //       are already validated. This should be done in the [] function.
  //       So here, we only need to make sure that the source selection
  //       will fit into the destination selection.
  //       If the PartSelects are differently sized, shrink the larger
  //       to the size of the smaller.
  // 1. Confirm the source and destination indicis to use (as above).
  // 2. Determine which source and destination words will be used.
  // 3. Determine the masking and shifting of each source word.
  //  - If a source word is partially selected, it requires masking.
  //  - If the source and destination lower indicis have different %32
  //    values, shifting will be involved.
  //  - If a destination word is partially selected, it requires OR'ing.
  UInt32 l_srcLowerIdx = iBits.m_lowerIndex;
  UInt32 l_srcUpperIdx = iBits.m_upperIndex;
  UInt32 l_srcSize = l_srcUpperIdx - l_srcLowerIdx + 1;
  UInt32 l_dstSize = this->m_upperIndex - this->m_lowerIndex + 1;
  if(l_srcSize > l_dstSize)
  {
    // TBD - log warning.
    // ex: bv_a[15:0] = bv_b[31:8]; => bv_a[15:0] = bv_b[23:8];
    l_srcUpperIdx = l_srcLowerIdx + l_dstSize - 1;
    l_srcSize = l_dstSize;
  }
  else if(l_srcSize < l_dstSize)
  {
    // TBD - log warning.
    // ex: bv_a[23:0] = bv_b[23:8]; => bv_a[15:0] = bv_b[23:8];
    this->m_upperIndex = this->m_lowerIndex + l_srcSize - 1;
    l_dstSize = l_srcSize;
  }

  UInt32 l_srcLowerWord = iBits.m_parent->getWordNb(l_srcLowerIdx);
  UInt32 l_srcUpperWord = iBits.m_parent->getWordNb(l_srcUpperIdx);
  UInt32 l_dstLowerWord = iBits.m_parent->getWordNb(this->m_lowerIndex);
  UInt32 l_dstUpperWord = iBits.m_parent->getWordNb(this->m_upperIndex);

  UInt32 l_srcLowerShift = iBits.m_parent->getShift(l_srcLowerIdx);
  UInt32 l_dstLowerShift = iBits.m_parent->getShift(this->m_lowerIndex);
  
  UInt32 l_nbSrcBitsCopied = 0;
  UInt32 l_transferWord = 0;
  UInt32 l_dstWordCnt = l_dstUpperWord - l_dstLowerWord + 1;
  for(UInt32 ii=0; ii<l_dstWordCnt; ii++)
  {
    // WIP - Stopping point.
  }

}
void BitVector::PartSelect::getParentBits(BitVector & oBV) const
{
  
}

// =============================
// ===**     Operators     **===
// =============================
