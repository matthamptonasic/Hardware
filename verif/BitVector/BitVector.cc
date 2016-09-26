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

#include <iomanip>
#include <sstream>

#include "BitVector.h"
#include "vpi.h"

// ====================================
// ===**  Private Static Members  **===
// ====================================
BitVector::NB_STATES BitVector::g_nbStates = BitVector::NB_STATES::TWO_STATE;
bool BitVector::g_useGlobalStates = true;
const string BitVector::g_default_name = "Anonymous";
const UInt32 BitVector::g_default_size = 32;
bool BitVector::g_useGlobalPrintSettings = false;
BitVector::PRINT_FMT BitVector::g_printFmt = BitVector::PRINT_FMT::HEX;
bool BitVector::g_printBasePrefix = true;
bool BitVector::g_printPrependZeros = true;
bool BitVector::g_printHexWordDivider = true;

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
  m_aval = new vector<UInt32>(nbWds, 0);
  if(m_nbStates == BitVector::NB_STATES::FOUR_STATE)
  {
    m_bval = new vector<UInt32>(nbWds, 0);
  }
  m_printFmt = g_printFmt;
  m_printBasePrefix = g_printBasePrefix;
  m_printPrependZeros = g_printPrependZeros;
  m_printHexWordDivider = g_printHexWordDivider;
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
string BitVector::ToString() const
{
  bool      l_glbl = g_useGlobalPrintSettings;
  PRINT_FMT l_printFmt = l_glbl ? g_printFmt : m_printFmt;
  bool      l_printBasePrefix = l_glbl ? g_printBasePrefix : m_printBasePrefix;
  bool      l_printPrependZeros = l_glbl ? g_printPrependZeros : m_printPrependZeros;
  bool      l_printHexWordDivider = l_glbl ? g_printHexWordDivider : m_printHexWordDivider;

  // TBD - Handle decimal for > 32-bits.
  // TBD - Trim off bits that don't exist (ex. 65-bit BV will print 3 full words).
  stringstream l_ss;
  if(l_printFmt == PRINT_FMT::HEX)
  {
    l_ss << hex;
  }
  for(Int32 ii=m_aval->size()-1; ii>=0; ii--)
  {
    if(l_printFmt == PRINT_FMT::HEX)
    {
      if(l_printBasePrefix && (ii == m_aval->size()-1))
      {
        l_ss << "0x";
      }
      if(l_printPrependZeros && (ii == m_aval->size()-1))
      {
        // Prepend to the first word as well as the rest.
        l_ss << setfill('0') << setw(8);
      }
    }
    if(ii != m_aval->size()-1)
    {
      l_ss << setfill('0') << setw(8);
    }
    l_ss << m_aval->at(ii);
    if(l_printHexWordDivider && (ii > 0))
    {
      l_ss << "_";
    }
  }
  //const string l_retVal = l_ss.str();
  //return l_retVal;
  return l_ss.str();
}
void BitVector::Print() const
{
    Vpi::vpi_printf("%s value is %s\n", m_name.c_str(), ToString().c_str());
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
UInt32 BitVector::getMask(UInt32 iUpperIndex, bool iReverse)
{
  // Forward mask with upperIndex = 7 (keep bits 7:0)
  // 0000 0000 0000 0000 0000 0000 1111 1111
  // Reverse mask with upperIndex = 8 (keep bits 31:8)
  // 1111 1111 1111 1111 1111 1111 0000 0000
  // So when Forward, index is inclusive.
  // When reverse, index is 1 above what we want to set.
  // So if the index is 0, just return 0.
  UInt32 l_mask = 0;
  Byte l_upperBitPos = iUpperIndex % 32;
  if(iReverse && (l_upperBitPos == 0))
  {
    return l_mask;
  }
  if(iReverse)
  {
    l_upperBitPos = l_upperBitPos - 1;
  }
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
void BitVector::setUInt32(UInt32 iVal)
{
  // Wipe bits if the BV is < 32 wide.
  if(m_size < 32)
  {
    iVal &= ~getMask(m_size);
  }
  m_aval->at(0) = iVal;
  // Wipe anything above 32 bits if BV is > 32 wide.
  if(m_size > 32)
  {
    for(UInt32 ii=1; ii<m_aval->size(); ii++)
    {
      m_aval->at(ii) = 0;
    }
  }
}
void BitVector::setUInt64(UInt64 iVal)
{
  UInt32 hi = iVal >> 32;
  UInt32 lo = (UInt32)iVal;

  // Wipe bits if the BV is < 64 wide.
  if(m_size < 64)
  {
    if(m_size < 32)
    {
      hi = 0;
      lo &= ~getMask(m_size);
    }
    else
    {
      hi &= ~getMask(m_size);
    }
  }
  m_aval->at(0) = lo;
  if(m_size > 32)
  {
    m_aval->at(1) = hi;
  }
  // Wipe anything above 64 bits if BV is > 64 wide.
  if(m_size > 64)
  {
    for(UInt32 ii=2; ii<m_aval->size(); ii++)
    {
      m_aval->at(ii) = 0;
    }
  }
}

// =============================
// ===**     Operators     **===
// =============================
BitVector & BitVector::operator= (UInt32 iRhs)
{
  if(m_size == 0)
  {
    // TBD - log error.
    return *this;
  }
  //UInt32 temp = (UInt32)iRhs;
  (*m_aval)[0] = iRhs; //temp;
  return *this;
}
BitVector::PartSelect BitVector::operator() (UInt32 iUpperIndex, UInt32 iLowerIndex)
{
  PartSelect retVal(this, iUpperIndex, iLowerIndex);
  return retVal;
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
  //       are already validated. This should be done in the () function.
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
  UInt32 l_dstUpperShift = iBits.m_parent->getShift(this->m_upperIndex) + 1;
  
  UInt32 l_nbSrcBitsCopied = 0;
  UInt32 l_transferWord = 0;
  UInt32 l_dstWordCnt = l_dstUpperWord - l_dstLowerWord + 1;
/*
  Vpi::vpi_printf("setParentBits: l_dstWordCnt %d\n", l_dstWordCnt);
  Vpi::vpi_printf("setParentBits: l_srcLowerWord %d\n", l_srcLowerWord);
  Vpi::vpi_printf("setParentBits: l_srcUpperWord %d\n", l_srcUpperWord);
  Vpi::vpi_printf("setParentBits: l_dstLowerWord %d\n", l_dstLowerWord);
  Vpi::vpi_printf("setParentBits: l_dstUpperWord %d\n", l_dstUpperWord);
  Vpi::vpi_printf("setParentBits: l_srcLowerShift %d\n", l_srcLowerShift);
  Vpi::vpi_printf("setParentBits: l_dstLowerShift %d\n", l_dstLowerShift);
  Vpi::vpi_printf("setParentBits: l_dstUpperShift %d\n", l_dstUpperShift);
*/
  for(UInt32 ii=0; ii<l_dstWordCnt; ii++)
  {
    UInt32 l_transferWord = 0;
    UInt32 l_nbSrcBits = 32;
    if(l_dstWordCnt == 1)
    {
      l_nbSrcBits = l_dstSize;
    }
    else if(ii == 0)
    {
      l_nbSrcBits = 32 - l_dstLowerShift;
    }
    else if(ii == (l_dstWordCnt - 1))
    {
      l_nbSrcBits = l_dstUpperShift;
    }
    UInt32 l_srcLo = l_srcLowerIdx + l_nbSrcBitsCopied;
    UInt32 l_srcHi = l_srcLowerIdx + l_nbSrcBitsCopied + l_nbSrcBits - 1;
    l_transferWord = iBits.m_parent->getBits(l_srcHi, l_srcLo);
/*
    Vpi::vpi_printf("setParentBits: l_nbSrcBits %d\n", l_nbSrcBits);
    Vpi::vpi_printf("setParentBits: l_srcHi %d\n", l_srcHi);
    Vpi::vpi_printf("setParentBits: l_srcLo %d\n", l_srcLo);
    Vpi::vpi_printf("setParentBits: l_transferWord %x\n", l_transferWord);
*/
    if(ii == 0)
    {
      l_transferWord <<= l_dstLowerShift;
      // Get the original word and wipe out the upper part that we're overwriting.
      // Then OR it with our current result which will fit in the part we just wiped out.
      UInt32 msk = this->m_parent->getMask(l_dstLowerShift, true);
      l_transferWord |= this->m_parent->m_aval->at(l_dstLowerWord) & msk;
      //Vpi::vpi_printf("setParentBits: msk %x\n", msk);
      //Vpi::vpi_printf("setParentBits: l_transferWord %x\n", l_transferWord);
    }
    if(ii == (l_dstWordCnt - 1))
    {
      UInt32 msk = this->m_parent->getMask(l_dstUpperShift);
      l_transferWord |= this->m_parent->m_aval->at(l_dstUpperWord) & msk;
      //Vpi::vpi_printf("setParentBits: msk %x\n", msk);
      //Vpi::vpi_printf("setParentBits: l_transferWord %x\n", l_transferWord);
    }
    this->m_parent->m_aval->at(l_dstLowerWord + ii) = l_transferWord;
    l_nbSrcBitsCopied += l_nbSrcBits;
  }
}
void BitVector::PartSelect::getParentBits(BitVector & oBV) const
{
  
}

// =============================
// ===**     Operators     **===
// =============================
BitVector::PartSelect & BitVector::PartSelect::operator= (UInt32 iRhs)
{
  BitVector l_bv("BitVector::PartSelect::operator=", 32);
  l_bv = iRhs;
  l_bv.Print();
  setParentBits(l_bv(31,0));
  return *this;
}

