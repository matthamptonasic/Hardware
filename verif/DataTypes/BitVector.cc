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

#include <algorithm>
#include <iomanip>
#include <list>
#include <sstream>
#include <utility>
#include <vector>

#include "Logger.h"

#include "BitVector.h"

// ====================================
// ===**  Private Static Members  **===
// ====================================
NB_STATES BitVector::s_nbStates = NB_STATES::TWO_STATE;
bool BitVector::s_useGlobalStates = true;
const string BitVector::s_default_name = "Anonymous";
const UInt32 BitVector::s_default_size = 32;
bool BitVector::s_useGlobalPrintSettings = false;
BitVector::PRINT_FMT BitVector::s_printFmt = BitVector::PRINT_FMT::HEX;
bool BitVector::s_printBasePrefix = true;
bool BitVector::s_printPrependZeros = true;
bool BitVector::s_printFullWord = true;
bool BitVector::s_printHexWordDivider = true;
bool BitVector::s_printDecCommas = true;

// =============================
// ===**   Constructors    **===
// =============================
BitVector::BitVector(string iName)
{
  init(iName, 0, s_nbStates);
}
BitVector::BitVector(string iName, UInt32 iSize, NB_STATES iStates)
{
  init(iName, iSize, iStates);
}
BitVector::BitVector(UInt32 iSize)
{
  init(s_default_name, iSize, s_nbStates);
}
// Copy Constructor
BitVector::BitVector(const BitVector & iSource)
{
  initCopy(iSource, iSource.m_size);
}
BitVector::BitVector(const BitVector & iSource, UInt32 iSize)
{
  initCopy(iSource, iSize);
}
// Move Constructor
BitVector::BitVector(BitVector && iSource)
{
  initMove(iSource, iSource.m_size);
}
BitVector::BitVector(const PartSelect & iSource)
{
  UInt32 l_sz = iSource.m_upperIndex - iSource.m_lowerIndex + 1;
  // The name may be removed or used only in debug cases for better performance.
  string l_name = iSource.m_parent->m_name + "(" + to_string(iSource.m_upperIndex) + "," + to_string(iSource.m_lowerIndex) + ")";
  init(l_name, l_sz, iSource.m_parent->m_nbStates);
  iSource.getParentBits(*this);
}
BitVector::BitVector(const PartSelect & iSource, UInt32 iSize)
{
  // The name may be removed or used only in debug cases for better performance.
  string l_name = iSource.m_parent->m_name + "(" + to_string(iSource.m_upperIndex) + "," + to_string(iSource.m_lowerIndex) + ")";
  init(l_name, iSize, iSource.m_parent->m_nbStates);
  iSource.getParentBits(*this, true);
}
BitVector::~BitVector()
{
  delete m_aval;
  if(m_bval != nullptr)
  {
    delete m_bval;
  }
  LOG_DEBUG << "Destroying " << m_name << endl;
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
  if(m_nbStates == NB_STATES::FOUR_STATE)
  {
    m_bval = new vector<UInt32>(nbWds, 0);
  }
  else
  {
    m_bval = nullptr;
  }
  setMask();
  m_printFmt = s_printFmt;
  m_printBasePrefix = s_printBasePrefix;
  m_printPrependZeros = s_printPrependZeros;
  m_printFullWord = s_printFullWord;
  m_printHexWordDivider = s_printHexWordDivider;
  m_printDecCommas = s_printDecCommas;
}
void BitVector::initCopy(const BitVector & iSource, UInt32 iSize)
{
  initFields(iSource, iSize);
  
  m_name += "_Copy";
  LOG_DEBUG << "Copy constructor (" << m_name << ")." << endl;
  m_aval = new vector<UInt32>(*iSource.m_aval);
  if(m_nbStates == NB_STATES::FOUR_STATE)
  {
    m_bval = new vector<UInt32>(*iSource.m_bval);
  }
  else
  {
    m_bval = nullptr;
  }

  if(iSize != iSource.m_size)
  {
    Resize(iSize);
  }
}
void BitVector::initMove(BitVector & iSource, UInt32 iSize)
{
  initFields(iSource, iSize);

  m_name += "_Moved";
  LOG_DEBUG << "Move constructor (" << m_name << ")." << endl;
  m_aval = iSource.m_aval;
  iSource.m_aval = nullptr;
  m_bval = iSource.m_bval;
  iSource.m_bval = nullptr;

  if(iSize != iSource.m_size)
  {
    Resize(iSize);
  }
}
void BitVector::initFields(const BitVector & iSource, UInt32 iSize)
{
  m_size = iSize;
  m_mask = iSource.m_mask;
  m_name = iSource.m_name;
  m_nbStates = iSource.m_nbStates;
  m_printFmt = iSource.m_printFmt;
  m_printBasePrefix = iSource.m_printBasePrefix;
  m_printPrependZeros = iSource.m_printPrependZeros;
  m_printHexWordDivider = iSource.m_printHexWordDivider;
}

// =============================
// ===**  Public Methods   **===
// =============================
void   BitVector::Resize(UInt32 iNewSize)
{
  UInt32 l_wds = (iNewSize - 1) / 32 + 1;
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
  applyMask();
}
UInt32 BitVector::GetUInt32() const
{
  return GetUInt32(0);
}
UInt32 BitVector::GetUInt32(UInt32 iWordNb) const
{
  if(m_aval->size() < (iWordNb + 1))
  {
    LOG_WRN_ENV << "Size is " << m_aval->size() 
                << ", less than the selected index of " 
                << iWordNb << ". Returning 0." << endl;
    return 0;
  }
  return (*m_aval)[iWordNb];
}
UInt64 BitVector::GetUInt64() const
{
  return GetUInt64(0);
}
UInt64 BitVector::GetUInt64(UInt32 iLowerWordNb) const
{
  UInt64 retVal = 0;
  if(m_aval->size() < (iLowerWordNb + 1))
  {
    LOG_WRN_ENV << "Size is " << m_aval->size() 
                << ", less than the lower selected index of " 
                << iLowerWordNb << ". Returning 0." << endl;
    return 0;
  }
  UInt64 hi = 0;
  if(m_aval->size() >= (iLowerWordNb + 2))
  {
    hi = (*m_aval)[iLowerWordNb + 1];
    retVal = hi << 32;
  } 
  retVal |=  (*m_aval)[iLowerWordNb];
  return retVal;
}
bool BitVector::bitSet(UInt32 iIndex) const
{
  if(iIndex >= m_size)
  {
    return false;
  }
  UInt32 l_wdNb = getWordNb(iIndex);
  Byte l_pos = getShift(iIndex);
  if((((*m_aval)[l_wdNb] >> l_pos) & 1) == 1)
  {
    return true;
  }
  return false;
}
vector<Byte> BitVector::add(const vector<Byte> * iArr0, const vector<Byte> * iArr1) const
{
  vector<Byte> l_retVal(iArr0->size() > iArr1->size() ? *iArr0 : *iArr1);
  Byte l_carry = 0;
  for(UInt32 ii=0; ii<l_retVal.size(); ii++)
  {
    if((ii < iArr0->size()) && (ii < iArr1->size()))
    {
      l_retVal[ii] = (*iArr0)[ii] + (*iArr1)[ii] + l_carry;
      l_carry = l_retVal[ii] / 10;
      l_retVal[ii] %= 10;
    }
    if(ii >= iArr0->size())
    {
      l_retVal[ii] = (*iArr1)[ii] + l_carry;
      l_carry = 0;
    }
    else if(ii >= iArr1->size())
    {
      l_retVal[ii] = (*iArr0)[ii] + l_carry;
      l_carry = 0;
    }
    if((ii == (l_retVal.size() - 1)) && (l_carry > 0))
    {
      l_retVal.push_back(l_carry);
      break;
    }
  }
  return l_retVal;
}
vector<Byte> * BitVector::getTwoToN(UInt32 iN) const
{
  vector<Byte> * l_retVal = new vector<Byte>(1, 1);
  for(UInt32 ii=0; ii<iN; ii++) {
    *l_retVal = add(l_retVal, l_retVal);
  }
  return l_retVal;
}
string BitVector::toDecimalString() const
{
  // TBD - This needs a LOT of streamlining.
  // 1. Revisit all of the list/vector passing & creation.
  //    Probably memory leaks as it is now.
  // 2. Add a const static array of vectors that have
  //    hard coded the 2^n values up to like 64.
  //    This should reduce a lot of looping.
  //    As it is now, for a 64-bit vector, for every set bit,
  //    the add function is called n-times.
  //    So for 32 set bits, that's an average of 32 * 32 loops
  //    (depending on the set positions).

  // Based on work from http://www.danvk.org/hex2dec.html
  // Create a vector of lists/arrays of digits.
  // Each list/array contains the decimal value of each binary digit.
  // So, for a 6-bit wide BV, we'll need 6 list/arrays of values.
  // Ex: 0x23 = 6'b100011 = 1x2^5 + 0x2^4 ... + 1x2^1 + 1x2^0
  //     The list values are 32={2,3}, {0}, {0}, {0}, {2}, {1}
  //     Then the list/array values are added into a result list/array.
  //     Result[1] = 3
  //     Result[0] = 2 + 2 + 1 = 5
  // So the decimal value is 35.
  // We can prune out any list/array where the bit is 0.
  //
  // To create the array for 2^n, we need to create a function
  // that will add 2 list/arrays and return a resultant list/array.
  // Ex: array0 = 2^7 = {8, 2, 1}
  //     array1 = 2^10 = {4, 2, 0, 1}
  // Return arr = {2, 5, 1, 1}
  // From this we can create 2^n by using a for loop.
  // arr = {1};
  // for(UIn32 ii=0; ii<n; ii++) {
  //   arr = add(arr, arr);
  // }

  bool l_printDecCommas = s_useGlobalPrintSettings ? s_printDecCommas : m_printDecCommas;
  list<vector<Byte>*> l_decList;
  for(UInt32 ii=0; ii<m_size; ii++)
  {
    if(!bitSet(ii))
    {
      continue;
    }
    vector<Byte> * l_tmp = getTwoToN(ii);
    l_decList.push_back(l_tmp);
  }
  vector<Byte> * l_sum = new vector<Byte>(1, 0);
  for(list<vector<Byte>*>::iterator ii=l_decList.begin(); ii!=l_decList.end(); ++ii)
  {
    *l_sum = add(l_sum, *ii);
  }
  string l_retVal("");
  for(Int32 ii=l_sum->size() - 1; ii >= 0; ii--)
  {
    if(l_printDecCommas && ((ii % 3) == 2) && (ii != (l_sum->size() - 1)))
    {
      l_retVal += ",";
    }
    l_retVal += to_string((*l_sum)[ii]);
  }
  return l_retVal;
}
string BitVector::ToString() const
{
  bool      l_glbl = s_useGlobalPrintSettings;
  PRINT_FMT l_printFmt = l_glbl ? s_printFmt : m_printFmt;
  bool      l_printBasePrefix = l_glbl ? s_printBasePrefix : m_printBasePrefix;
  bool      l_printPrependZeros = l_glbl ? s_printPrependZeros : m_printPrependZeros;
  bool      l_printFullWord = l_glbl ? s_printFullWord : m_printFullWord;
  bool      l_printHexWordDivider = l_glbl ? s_printHexWordDivider : m_printHexWordDivider;

  // TBD - Handle decimal for > 32-bits.
  
  stringstream l_ss;
  if(l_printFmt == PRINT_FMT::HEX)
  {
    l_ss << hex;
  }
  else
  {
    return toDecimalString();
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
        Byte l_nbNibbles = 8;
        if(!l_printFullWord)
        {
          l_nbNibbles = ((m_size - 1) % 32) / 4 + 1;
        }
        l_ss << setfill('0') << setw(l_nbNibbles);
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

  return l_ss.str();
}
void BitVector::Print() const
{
    LOG_MSG << m_name << " value is " << ToString() << endl;
}

// =============================
// ===**  Private Methods  **===
// =============================
void BitVector::checkIndices(UInt32 & iUpperIndex, UInt32 & iLowerIndex) const
{
  if(iUpperIndex < iLowerIndex)
  {
    UInt32 tmp = iLowerIndex;
    iLowerIndex = iUpperIndex;
    iUpperIndex = tmp;
  }
}
inline UInt32 BitVector::getWordNb(UInt32 iBitPos) const
{
  // Word Nb is Bit Position / 32.
  return (iBitPos >> 5);
}
inline Byte BitVector::getShift(UInt32 iBitPos) const
{
  return iBitPos % 32;
}
UInt32 BitVector::getMask(UInt32 iUpperIndex, bool iReverse) const
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
void BitVector::applyMask()
{
  UInt32 l_size = m_aval->size();
  if(l_size >= 1)
  {
    (*m_aval)[l_size - 1] &= m_mask;
    if(m_nbStates == NB_STATES::FOUR_STATE)
    {
      (*m_bval)[l_size - 1] &= m_mask;
    }
  }
}
UInt32 BitVector::getBits(UInt32 iUpperIndex, UInt32 iLowerIndex) const
{
  checkIndices(iUpperIndex, iLowerIndex);
  UInt32 l_selSize = iUpperIndex - iLowerIndex + 1;
  if(l_selSize > 32)
  {
    LOG_ERR_ENV << "Selection size (" << iUpperIndex << "," << iLowerIndex << ") is > 32." << endl;
    return 0;
  }
  if(iUpperIndex > m_size)
  {
    LOG_WRN_ENV << "Selection (" << iUpperIndex << "," << iLowerIndex 
                << ") is beyond the BitVector size (" << m_size 
                << "). Shrinking the selection to fit." << endl;
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
    l_retVal |= (l_hiWd << (32 - l_shift));
  }
  UInt32 msk = getMask(l_selSize-1);
  l_retVal &= msk;
  return l_retVal;
}
void BitVector::setUInt32(UInt32 iVal)
{
  // Wipe bits if the BV is < 32 wide.
  if(m_size < 32)
  {
    iVal &= getMask(m_size - 1);
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
  applyMask();
}
void BitVector::setUInt64(UInt64 iVal)
{
  UInt32 hi = iVal >> 32;
  UInt32 lo = (UInt32)iVal;

  // Wipe bits if the BV is < 64 wide.
  m_aval->at(0) = lo;
  if(m_size > 32)
  {
    m_aval->at(1) = hi;
  }
  // Wipe anything above 64 bits if BV is > 64 wide.
  for(UInt32 ii=2; ii<m_aval->size(); ii++)
  {
    m_aval->at(ii) = 0;
  }
  applyMask();
}
void BitVector::add(UInt32 iVal, UInt32 iWordNb)
{
  UInt32 l_nbWords = m_aval->size();
  if(l_nbWords <= iWordNb)
  {
    LOG_WRN_ENV << "Size is " << m_aval->size() 
                << ", less than the selected index of " 
                << iWordNb << ". No change made." << endl;
    return;
  }
  UInt32 l_prevVal = (*m_aval)[iWordNb];
  (*m_aval)[iWordNb] += iVal;
  bool l_carry = false;
  if(l_prevVal > (*m_aval)[iWordNb])
  {
    l_carry = true;
  }

  // Add carry if this isn't the last word.
  if((iWordNb < (l_nbWords - 1)) && l_carry)
  {
    add(1, iWordNb + 1);
    if((iWordNb + 1) == (l_nbWords - 1))
    {
      applyMask();
      return;
    }
  }
  
  // Apply the mask if we're on the last word.
  if(iWordNb == (l_nbWords - 1))
  {
    applyMask();
  }
}
void BitVector::subtract(UInt32 iVal, UInt32 iWordNb)
{
  UInt32 l_nbWords = m_aval->size();
  if(l_nbWords <= iWordNb)
  {
    LOG_WRN_ENV << "Size is " << m_aval->size() 
                << ", less than the selected index of " 
                << iWordNb << ". No change made." << endl;
    return;
  }
  UInt32 l_prevVal = (*m_aval)[iWordNb];
  (*m_aval)[iWordNb] -= iVal;
  bool l_borrow = false;
  if(l_prevVal < (*m_aval)[iWordNb])
  {
    l_borrow = true;
  }

  // Subtract borrow if this isn't the last word.
  if((iWordNb < (l_nbWords - 1)) && l_borrow)
  {
    subtract(1, iWordNb + 1);
    if((iWordNb + 1) == (l_nbWords - 1))
    {
      applyMask();
      return;
    }
  }

  // Apply the mask if we're on the last word.
  if(iWordNb == (l_nbWords - 1))
  {
    applyMask();
  }
}
bool BitVector::equals(UInt32 iVal, UInt32 iWordNb) const
{
  // This function assumes that our last a_val value is already masked.
  // Meaning we don't have to mask it again before comparing.
  if(m_aval == NULL)
  {
    return false;
  }
  if((m_aval->size() < (iWordNb + 1)) && (iVal == 0))
  {
    // If the selected word is beyond our size
    // AND the value to compare is 0, we return true (0 extend the vector).
    return true;
  }
  if((*m_aval)[iWordNb] == iVal)
  {
    return true;
  }
  return false;
}
bool BitVector::ltet(UInt32 iVal, UInt32 iWordNb, bool & oEqual) const
{
  // This function assumes that our last a_val value is already masked.
  // Meaning we don't have to mask it again before comparing.
  if(m_aval == NULL)
  {
    LOG_WRN_ENV << "Trying to compare to a NULL BitVector." << endl;
    if(iVal == 0)
    {
      oEqual = true;
      return false;
    }
  }
  if((m_aval->size() < (iWordNb + 1)) && (iVal == 0))
  {
    // If the selected word is beyond our size
    // AND the value to compare is 0, they're equal, we return true (0 extend the vector).
    oEqual = true;
    return true;
  }
  if((*m_aval)[iWordNb] <= iVal)
  {
    oEqual = ((*m_aval)[iWordNb] == iVal);
    return true;
  }
  oEqual = false;
  return false;
}
bool BitVector::lt(UInt32 iVal, UInt32 iWordNb, bool & oEqual) const
{
  // This function assumes that our last a_val value is already masked.
  // Meaning we don't have to mask it again before comparing.
  if(m_aval == NULL)
  {
    LOG_WRN_ENV << "Trying to compare to a NULL BitVector." << endl;
    if(iVal == 0)
    {
      oEqual = true;
      return false;
    }
  }
  if((m_aval->size() < (iWordNb + 1)) && (iVal == 0))
  {
    // If the selected word is beyond our size
    // AND the value to compare is 0, they're equal, we return false (0 extend the vector).
    oEqual = true;
    return false;
  }
  if((*m_aval)[iWordNb] < iVal)
  {
    oEqual = false;
    return true;
  }
  oEqual = ((*m_aval)[iWordNb] == iVal);
  return false;
}
bool BitVector::gtet(UInt32 iVal, UInt32 iWordNb, bool & oEqual) const
{
  // This function assumes that our last a_val value is already masked.
  // Meaning we don't have to mask it again before comparing.
  if(m_aval == NULL)
  {
    LOG_WRN_ENV << "Trying to compare to a NULL BitVector." << endl;
    if(iVal == 0)
    {
      oEqual = true;
      return false;
    }
  }
  if((m_aval->size() < (iWordNb + 1)) && (iVal == 0))
  {
    // If the selected word is beyond our size
    // AND the value to compare is 0, they're equal, we return true (0 extend the vector).
    oEqual = true;
    return true;
  }
  if((*m_aval)[iWordNb] >= iVal)
  {
    oEqual = ((*m_aval)[iWordNb] == iVal);
    return true;
  }
  oEqual = false;
  return false;
}
bool BitVector::gt(UInt32 iVal, UInt32 iWordNb, bool & oEqual) const
{
  // This function assumes that our last a_val value is already masked.
  // Meaning we don't have to mask it again before comparing.
  if(m_aval == NULL)
  {
    LOG_WRN_ENV << "Trying to compare to a NULL BitVector." << endl;
    if(iVal == 0)
    {
      oEqual = true;
      return false;
    }
  }
  if((m_aval->size() < (iWordNb + 1)) && (iVal == 0))
  {
    // If the selected word is beyond our size
    // AND the value to compare is 0, they're equal, we return false (0 extend the vector).
    oEqual = true;
    return false;
  }
  if((*m_aval)[iWordNb] > iVal)
  {
    oEqual = false;
    return true;
  }
  oEqual = ((*m_aval)[iWordNb] == iVal);
  return false;
}
bool BitVector::allZero (UInt32 iLowerWordNb) const
{
  // Checks if all word above and including iLowerWordNb are 0.
  for(Int32 ii=m_aval->size() - 1; ii >= iLowerWordNb; ii--)
  {
    if((*m_aval)[ii] != 0)
    {
      return false;
    }
  }
  return true;
}

// =============================
// ===**     Operators     **===
// =============================
BitVector & BitVector::operator= (UInt32 iRhs)
{
  if(m_size == 0)
  {
    LOG_ERR_ENV << "BitVector size was 0." << endl;
    return *this;
  }
  setUInt32(iRhs);
  return *this;
}
BitVector & BitVector::operator= (UInt64 iRhs)
{
  if(m_size == 0)
  {
    LOG_ERR_ENV << "BitVector size was 0." << endl;
    return *this;
  }
  setUInt64(iRhs);
  return *this;
}
BitVector & BitVector::operator= (const BitVector & iRhs)
{
  LOG_DEBUG << "Calling const BitVector& operator=" << endl;
  if(this->m_size == 0)
  {
    LOG_WRN_ENV << "Size of '" << this->m_name << "' is 0." << endl;
    this->Resize(iRhs.m_size);
  }
  // He's 48 bits, im 64, use his #wds and mask.
  // He's 48 bits, im 16, use my #wds and mask.
  // Hes  8 bits, im 96, use his #wds and mask, but clear my upper words.
  UInt32 l_upperWord = this->m_aval->size() < iRhs.m_aval->size() ? this->m_aval->size() - 1 : iRhs.m_aval->size() - 1;

    for(UInt32 ii=0; ii<this->m_aval->size(); ii++)
  {
      if(ii <= l_upperWord)
    {
      this->m_aval->at(ii) = iRhs.m_aval->at(ii);
    }
    else
    {
      this->m_aval->at(ii) = 0;
    }
  }
  
  applyMask();
  return *this;
}
BitVector & BitVector::operator= (BitVector && iRhs)
{
  LOG_DEBUG << "Calling BitVector&& operator=" << endl;
  if(m_size != iRhs.m_size)
  {
    // Different sizes, need to do the full copy.
    return operator=(iRhs);
  }
  LOG_DEBUG << "Moving a/b_val." << endl;
  // Simply pull over the vector values.
  m_aval = iRhs.m_aval;
  iRhs.m_aval = nullptr;
  m_bval = iRhs.m_bval;
  iRhs.m_bval = nullptr;
  return *this;
}
UInt32 BitVector::operator[] (UInt32 iWordIndex) const
{
  if(m_aval == nullptr)
  {
    LOG_WRN_ENV << "Trying to index (" << iWordIndex 
                << ") an empty BitVector '" << m_name << "'" << endl;
    return 0;
  }
  if(m_aval->size() <= iWordIndex)
  {
    LOG_WRN_ENV << "Index (" << iWordIndex << ") out of bounds ("
                << (m_aval->size() - 1) << ")." << endl;
    return 0;
  }
  return (*m_aval)[iWordIndex];
}
BitVector::operator bool() const
{
  if(m_aval == nullptr)
  {
    return false;
  }
  for(UInt32 ii=0; ii<m_aval->size(); ii++)
  {
    if((*m_aval)[ii] != 0)
    {
      return true;
    }
  }
  return false;
}
BitVector::operator UInt32() const
{
  if(m_aval == nullptr)
  {
    return 0;
  }
  return (*this)[0];
}
BitVector::operator UInt64() const
{
  if(m_aval == nullptr)
  {
    return 0;
  }
  UInt64 l_retVal = (*this)[0];
  if(m_size > 32)
  {
    l_retVal |= ((UInt64)(*this)[1] << 32);
  }
  return l_retVal;
}
BitVector & BitVector::operator+= (UInt64 iRhs)
{
  UInt32 l_hi = iRhs >> 32;
  UInt32 l_lo = (UInt32)iRhs;
  add(l_lo, 0);
  if(m_size > 32)
  {
    add(l_hi, 1);
  }
  return *this;
}
BitVector & BitVector::operator+= (const BitVector & iRhs)
{
  UInt32 l_smaller = min(iRhs.m_aval->size(), m_aval->size());
  for(UInt32 ii=0; ii<l_smaller; ii++)
  {
    add((*iRhs.m_aval)[ii], ii);
  }
  return *this;
}
BitVector BitVector::operator+ (UInt32 iRhs) const
{
  BitVector l_retVal(*this, max(32U, this->m_size) + 1);
  l_retVal += iRhs;
  return l_retVal;
}
BitVector BitVector::operator+ (UInt64 iRhs) const
{
  BitVector l_retVal(*this, max(64U, this->m_size) + 1);
  l_retVal += iRhs;
  return l_retVal;
}
BitVector BitVector::operator+ (const BitVector & iRhs) const
{
  BitVector l_retVal(*this, max(iRhs.m_size, this->m_size) + 1);
  l_retVal += iRhs;
  return l_retVal;
}
BitVector & BitVector::operator-= (UInt64 iRhs)
{
  UInt32 l_hi = iRhs >> 32;
  UInt32 l_lo = (UInt32)iRhs;
  subtract(l_lo, 0);
  if(m_size > 32)
  {
    subtract(l_hi, 1);
  }
  return *this;
}
BitVector & BitVector::operator-= (const BitVector & iRhs)
{
  UInt32 l_smaller = min(iRhs.m_aval->size(), m_aval->size());
  for(UInt32 ii=0; ii<l_smaller; ii++)
  {
    subtract((*iRhs.m_aval)[ii], ii);
  }
  return *this;
}
BitVector BitVector::operator- (UInt32 iRhs) const
{
  BitVector l_retVal(*this);
  l_retVal -= iRhs;
  return l_retVal;
}
BitVector BitVector::operator- (UInt64 iRhs) const
{
  BitVector l_retVal(*this);
  l_retVal -= iRhs;
  return l_retVal;
}
BitVector BitVector::operator- (const BitVector & iRhs) const
{
  BitVector l_retVal(*this);
  l_retVal -= iRhs;
  return l_retVal;
}
bool BitVector::operator== (UInt32 iRhs) const
{
  if(!allZero(1))
  {
    // At least 1 bit above bit 31 is set.
    return false;
  }
  bool l_retVal = equals(iRhs, 0);
  return l_retVal;
}
bool BitVector::operator== (UInt64 iRhs) const
{
  if(!allZero(2))
  {
    // At least 1 bit above bit 63 is set.
    return false;
  }
  UInt32 l_lo = (UInt32)iRhs;
  UInt32 l_hi = iRhs >> 32;
  bool l_retVal = equals(l_hi, 1);
  l_retVal &= equals(l_lo, 0);
  return l_retVal;
}
bool BitVector::operator== (const BitVector & iRhs) const
{
  UInt32 l_lhsSize = m_aval->size();
  UInt32 l_rhsSize = iRhs.m_aval->size();
  if((l_lhsSize == 0) || (l_rhsSize == 0))
  {
    return false;
  }
  UInt32 l_nbWords = max(l_lhsSize, l_rhsSize);
  for(Int32 ii=l_nbWords-1; ii >= 0; ii--)
  {
    // If the RHS BitVector is smaller, then we 0-extend it.
    // This check below only compares RHS values that exist.
    // If the LHS is larger, the l_cmpVal stays 0 and we compare LHS[ii] to that.
    UInt32 l_cmpVal = 0;
    if(ii < l_rhsSize)
    {
      l_cmpVal = (*iRhs.m_aval)[ii];
    }
    if(!equals(l_cmpVal, ii))
    {
      return false;
    }
  }
  return true;
}
bool BitVector::operator<= (UInt32 iRhs) const
{
  if(m_aval->size() == 0)
  {
    LOG_WRN_ENV << "Comparing an empty BitVector." << endl;
    return false;
  }
  if(!allZero(1))
  {
    // At least 1 bit above bit 31 is set.
    return false;
  }
  bool l_equal;
  return ltet(iRhs, 0, l_equal);
}
bool BitVector::operator<= (UInt64 iRhs) const
{
  if(m_aval->size() == 0)
  {
    LOG_WRN_ENV << "Comparing an empty BitVector." << endl;
    return false;
  }
  if(!allZero(2))
  {
    // At least 1 bit above bit 63 is set.
    return false;
  }
  UInt32 l_lo = (UInt32)iRhs;
  UInt32 l_hi = iRhs >> 32;
  bool l_equal;
  bool l_retVal = ltet(l_hi, 1, l_equal);
  if(!l_retVal || !l_equal)
  {
    return l_retVal;
  }
  l_retVal &= ltet(l_lo, 0, l_equal); 
  return l_retVal;
}
bool BitVector::operator<= (const BitVector & iRhs) const
{
  UInt32 l_lhsSize = m_aval->size();
  UInt32 l_rhsSize = iRhs.m_aval->size();
  if((l_lhsSize == 0) || (l_rhsSize == 0))
  {
    LOG_WRN_ENV << "Comparing an empty BitVector." << endl;
    return false;
  }
  bool l_retVal = false;
  UInt32 l_nbWords = max(l_lhsSize, l_rhsSize);
  for(Int32 ii=l_nbWords-1; ii >= 0; ii--)
  {
    // If the RHS BitVector is smaller, then we 0-extend it.
    // This check below only compares RHS values that exist.
    // If the LHS is larger, the l_cmpVal stays 0 and we compare LHS[ii] to that.
    UInt32 l_cmpVal = 0;
    if(ii < l_rhsSize)
    {
      l_cmpVal = (*iRhs.m_aval)[ii];
    }
    bool l_equal = false;
    l_retVal = ltet(l_cmpVal, ii, l_equal);
    if(!l_retVal || !l_equal)
    {
      return l_retVal;
    }
  }
  return l_retVal;
}
bool BitVector::operator>= (UInt32 iRhs) const
{
  if(m_aval->size() == 0)
  {
    LOG_WRN_ENV << "Comparing an empty BitVector." << endl;
    return false;
  }
  if(!allZero(1))
  {
    // At least 1 bit above bit 31 is set.
    return true;
  }
  bool l_equal;
  return gtet(iRhs, 0, l_equal);
}
bool BitVector::operator>= (UInt64 iRhs) const
{
  if(m_aval->size() == 0)
  {
    LOG_WRN_ENV << "Comparing an empty BitVector." << endl;
    return false;
  }
  if(!allZero(2))
  {
    // At least 1 bit above bit 63 is set.
    return true;
  }
  UInt32 l_lo = (UInt32)iRhs;
  UInt32 l_hi = iRhs >> 32;
  bool l_equal;
  bool l_retVal = gtet(l_hi, 1, l_equal);
  if(!l_retVal || !l_equal)
  {
    return l_retVal;
  }
  l_retVal &= gtet(l_lo, 0, l_equal); 
  return l_retVal;
}
bool BitVector::operator>= (const BitVector & iRhs) const
{
  UInt32 l_lhsSize = m_aval->size();
  UInt32 l_rhsSize = iRhs.m_aval->size();
  if((l_lhsSize == 0) || (l_rhsSize == 0))
  {
    LOG_WRN_ENV << "Comparing an empty BitVector." << endl;
    return false;
  }
  bool l_retVal = false;
  UInt32 l_nbWords = max(l_lhsSize, l_rhsSize);
  for(Int32 ii=l_nbWords-1; ii >= 0; ii--)
  {
    // If the RHS BitVector is smaller, then we 0-extend it.
    // This check below only compares RHS values that exist.
    // If the LHS is larger, the l_cmpVal stays 0 and we compare LHS[ii] to that.
    UInt32 l_cmpVal = 0;
    if(ii < l_rhsSize)
    {
      l_cmpVal = (*iRhs.m_aval)[ii];
    }
    bool l_equal = false;
    l_retVal = gtet(l_cmpVal, ii, l_equal);
    if(!l_retVal || !l_equal)
    {
      return l_retVal;
    }
  }
  return l_retVal;
}
bool BitVector::operator< (UInt32 iRhs) const
{
  if(m_aval->size() == 0)
  {
    LOG_WRN_ENV << "Comparing an empty BitVector." << endl;
    return false;
  }
  if(!allZero(1))
  {
    // At least 1 bit above bit 31 is set.
    return false;
  }
  bool l_equal;
  return lt(iRhs, 0, l_equal);
}
bool BitVector::operator< (UInt64 iRhs) const
{
  if(m_aval->size() == 0)
  {
    LOG_WRN_ENV << "Comparing an empty BitVector." << endl;
    return false;
  }
  if(!allZero(2))
  {
    // At least 1 bit above bit 63 is set.
    return false;
  }
  UInt32 l_lo = (UInt32)iRhs;
  UInt32 l_hi = iRhs >> 32;
  bool l_equal;
  bool l_retVal = lt(l_hi, 1, l_equal);
  if(!l_retVal && !l_equal)
  {
    return false;
  }
  l_retVal = ltet(l_lo, 0, l_equal); 
  return l_retVal;
}
bool BitVector::operator< (const BitVector & iRhs) const
{
  UInt32 l_lhsSize = m_aval->size();
  UInt32 l_rhsSize = iRhs.m_aval->size();
  if((l_lhsSize == 0) || (l_rhsSize == 0))
  {
    LOG_WRN_ENV << "Comparing an empty BitVector." << endl;
    return false;
  }
  bool l_retVal = false;
  UInt32 l_nbWords = max(l_lhsSize, l_rhsSize);
  for(Int32 ii=l_nbWords-1; ii >= 0; ii--)
  {
    // If the RHS BitVector is smaller, then we 0-extend it.
    // This check below only compares RHS values that exist.
    // If the LHS is larger, the l_cmpVal stays 0 and we compare LHS[ii] to that.
    UInt32 l_cmpVal = 0;
    if(ii < l_rhsSize)
    {
      l_cmpVal = (*iRhs.m_aval)[ii];
    }
    bool l_equal = false;
    l_retVal = lt(l_cmpVal, ii, l_equal);
    if(!l_retVal && !l_equal)
    {
      return false;
    }
  }
  return l_retVal;
}
bool BitVector::operator> (UInt32 iRhs) const
{
  if(m_aval->size() == 0)
  {
    LOG_WRN_ENV << "Comparing an empty BitVector." << endl;
    return false;
  }
  if(!allZero(1))
  {
    // At least 1 bit above bit 31 is set.
    return true;
  }
  bool l_equal;
  return gt(iRhs, 0, l_equal);
}
bool BitVector::operator> (UInt64 iRhs) const
{
  if(m_aval->size() == 0)
  {
    LOG_WRN_ENV << "Comparing an empty BitVector." << endl;
    return true;
  }
  if(!allZero(2))
  {
    // At least 1 bit above bit 63 is set.
    return true;
  }
  UInt32 l_lo = (UInt32)iRhs;
  UInt32 l_hi = iRhs >> 32;
  bool l_equal;
  bool l_retVal = gt(l_hi, 1, l_equal);
  if(!l_retVal && !l_equal)
  {
    return l_retVal;
  }
  l_retVal = gt(l_lo, 0, l_equal); 
  return l_retVal;
}
bool BitVector::operator> (const BitVector & iRhs) const
{
  UInt32 l_lhsSize = m_aval->size();
  UInt32 l_rhsSize = iRhs.m_aval->size();
  if((l_lhsSize == 0) || (l_rhsSize == 0))
  {
    LOG_WRN_ENV << "Comparing an empty BitVector." << endl;
    return false;
  }
  bool l_retVal = false;
  UInt32 l_nbWords = max(l_lhsSize, l_rhsSize);
  for(Int32 ii=l_nbWords-1; ii >= 0; ii--)
  {
    // If the RHS BitVector is smaller, then we 0-extend it.
    // This check below only compares RHS values that exist.
    // If the LHS is larger, the l_cmpVal stays 0 and we compare LHS[ii] to that.
    UInt32 l_cmpVal = 0;
    if(ii < l_rhsSize)
    {
      l_cmpVal = (*iRhs.m_aval)[ii];
    }
    bool l_equal = false;
    l_retVal = gt(l_cmpVal, ii, l_equal);
    if(!l_retVal && !l_equal)
    {
      return l_retVal;
    }
  }
  return l_retVal;
}
BitVector & BitVector::operator<<= (UInt32 iRhs)
{
  if(iRhs == 0)
  {
    return *this;
  }
  Int32 l_wordShift = (iRhs - 1) / 32 + 1;
  Int32 l_bitShift =  iRhs % 32;
  for(Int32 ii=m_aval->size()-1; ii >= 0; ii--)
  {
    UInt32 l_xferWord = 0;
    if(((ii - l_wordShift + 1) >= 0) && ((ii - l_wordShift + 1) < m_aval->size()) && (l_bitShift != 0))
    {
      l_xferWord = ((*m_aval)[ii - l_wordShift + 1] << l_bitShift);
    }
    if((ii - l_wordShift) >= 0)
    {
      l_xferWord |=  ((*m_aval)[ii - l_wordShift] >> (32 - l_bitShift));
    }
    (*m_aval)[ii] = l_xferWord;
  }
  applyMask();
  return *this;
}
BitVector & BitVector::operator>>= (UInt32 iRhs)
{
  if(iRhs == 0)
  {
    return *this;
  }
  Int32 l_wordShift = (iRhs - 1) / 32 + 1;
  Int32 l_bitShift =  iRhs % 32;
  for(Int32 ii=0; ii < m_aval->size(); ii++)
  {
    UInt32 l_xferWord = 0;
    if((ii + l_wordShift) < m_aval->size())
    {
      l_xferWord =  ((*m_aval)[ii + l_wordShift] << (32 - l_bitShift));
    }
    if(((ii + l_wordShift - 1) >= 0) && ((ii + l_wordShift - 1) < m_aval->size()) && (l_bitShift != 0))
    {
      l_xferWord |= ((*m_aval)[ii + l_wordShift - 1] >> l_bitShift);
    }
    (*m_aval)[ii] = l_xferWord;
  }
  applyMask();
  return *this;
}
BitVector BitVector::operator<< (UInt32 iRhs) const
{
  UInt32 l_newSz = m_size + iRhs;
  BitVector l_retVal(*this, l_newSz);

  l_retVal <<= iRhs;
  return l_retVal;
}
BitVector BitVector::operator>> (UInt32 iRhs) const
{
  BitVector l_retVal(*this);

  l_retVal >>= iRhs;
  return l_retVal;
}
BitVector BitVector::operator~  () const
{
  BitVector l_retVal(*this);
  for(UInt32 ii=0; ii<m_aval->size(); ii++)
  {
    (*l_retVal.m_aval)[ii] = ~(*l_retVal.m_aval)[ii];
  }
  l_retVal.applyMask();
  return l_retVal;
}
BitVector & BitVector::operator&= (UInt32 iRhs)
{
  for(Int32 ii=m_aval->size() - 1; ii >= 1; ii--)
  {
    (*m_aval)[ii] = 0;
  }
  (*m_aval)[0] &= iRhs;
  return *this;
}
BitVector & BitVector::operator&= (UInt64 iRhs)
{
  for(Int32 ii=m_aval->size() - 1; ii >= 2; ii--)
  {
    (*m_aval)[ii] = 0;
  }
  (*m_aval)[1] &= (iRhs >> 32);
  (*m_aval)[0] &= (UInt32)iRhs;
  return *this;
}
BitVector & BitVector::operator&= (const BitVector & iRhs)
{
  UInt32 l_szRhs = iRhs.m_aval->size();
  UInt32 l_szLhs = m_aval->size();
  // If RHS is smaller, wipe out any words larger than it contains.
  for(Int32 ii=l_szLhs-1; ii>l_szRhs-1; ii--)
  {
    (*m_aval)[ii] = 0;
  }
  for(Int32 kk=0; kk<l_szRhs; kk++)
  {
    (*m_aval)[kk] &= (*iRhs.m_aval)[kk];
  }
  return *this;
}
BitVector BitVector::operator& (UInt32 iRhs) const
{
  BitVector l_retVal(*this);
  l_retVal &= iRhs;
  return l_retVal;
}
BitVector BitVector::operator& (UInt64 iRhs) const
{
  BitVector l_retVal(*this);
  l_retVal &= iRhs;
  return l_retVal;
}
BitVector BitVector::operator& (const BitVector & iRhs) const
{
  BitVector l_retVal(*this);
  l_retVal &= iRhs;
  return l_retVal;
}
BitVector & BitVector::operator|= (UInt32 iRhs)
{
  (*m_aval)[0] |= iRhs;
  applyMask();
  return *this;
}
BitVector & BitVector::operator|= (UInt64 iRhs)
{
  (*m_aval)[1] |= (iRhs >> 32);
  (*m_aval)[0] |= (UInt32)iRhs;
  applyMask();
  return *this;
}
BitVector & BitVector::operator|= (const BitVector & iRhs)
{
  UInt32 l_szRhs = iRhs.m_aval->size();
  UInt32 l_szLhs = m_aval->size();
  UInt32 l_szMin = min(l_szLhs, l_szRhs);
  for(Int32 ii=0; ii<l_szMin; ii++)
  {
    (*m_aval)[ii] |= (*iRhs.m_aval)[ii];
  }
  applyMask();
  return *this;
}
BitVector BitVector::operator| (UInt32 iRhs) const
{
  BitVector l_retVal(*this);
  l_retVal |= iRhs;
  return l_retVal;
}
BitVector BitVector::operator| (UInt64 iRhs) const
{
  BitVector l_retVal(*this);
  l_retVal |= iRhs;
  return l_retVal;
}
BitVector BitVector::operator| (const BitVector & iRhs) const
{
  BitVector l_retVal(*this);
  l_retVal |= iRhs;
  return l_retVal;
}
BitVector & BitVector::operator^= (UInt32 iRhs)
{
  (*m_aval)[0] ^= iRhs;
  applyMask();
  return *this;
}
BitVector & BitVector::operator^= (UInt64 iRhs)
{
  (*m_aval)[1] ^= (iRhs >> 32);
  (*m_aval)[0] ^= (UInt32)iRhs;
  applyMask();
  return *this;
}
BitVector & BitVector::operator^= (const BitVector & iRhs)
{
  UInt32 l_szRhs = iRhs.m_aval->size();
  UInt32 l_szLhs = m_aval->size();
  UInt32 l_szMin = min(l_szLhs, l_szRhs);
  for(Int32 ii=0; ii<l_szMin; ii++)
  {
    (*m_aval)[ii] ^= (*iRhs.m_aval)[ii];
  }
  applyMask();
  return *this;
}
BitVector BitVector::operator^ (UInt32 iRhs) const
{
  BitVector l_retVal(*this);
  l_retVal ^= iRhs;
  return l_retVal;
}
BitVector BitVector::operator^ (UInt64 iRhs) const
{
  BitVector l_retVal(*this);
  l_retVal ^= iRhs;
  return l_retVal;
}
BitVector BitVector::operator^ (const BitVector & iRhs) const
{
  BitVector l_retVal(*this); 
  l_retVal ^= iRhs;
  return l_retVal;
}
BitVector BitVector::operator, (UInt32 iRhs) const
{
  BitVector l_retVal(*this, m_size + 32);
  l_retVal <<= 32;
  l_retVal(31, 0) = iRhs;
  return l_retVal;
}
BitVector BitVector::operator, (UInt64 iRhs) const
{
  BitVector l_retVal(*this, m_size + 64);
  l_retVal <<= 64;
  l_retVal(63, 0) = iRhs;
  return l_retVal;
}
BitVector BitVector::operator, (const BitVector & iRhs) const
{
  BitVector l_retVal(*this, m_size + iRhs.m_size);
  // Because the PartSelect operator= does not take a const BV ref,
  // we must make a copy here.
  // See the PartSelect operator= declaration in the header file for more info.
  l_retVal <<= iRhs.m_size;
  l_retVal(iRhs.m_size - 1, 0) = BitVector(iRhs);
  return l_retVal;
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
  if(iBV == nullptr)
  {
    LOG_ERR_ENV << "iBV was NULL." << endl;
    return;
  }
  m_parent = iBV;
  iBV->checkIndices(iUpperIndex, iLowerIndex);
  m_upperIndex = iUpperIndex;
  m_lowerIndex = iLowerIndex;
  if((m_upperIndex >= m_parent->m_size) && (m_lowerIndex < m_parent->m_size))
  {
    LOG_MEDIUM  << "PartSelect(" << m_upperIndex << "," << m_lowerIndex 
                << ") of " << m_parent->m_name 
                << " is out of bounds (size = " << m_parent->m_size 
                << "). Reducing upper index to " << m_parent->m_size - 1
                << "." << endl;
    m_upperIndex = m_parent->m_size - 1;
  }
}

// =============================
// ===**  Public Methods   **===
// =============================
UInt32 BitVector::PartSelect::getUInt32() const
{
  if(m_parent == NULL)
  {
    LOG_ERR_ENV << "Parent was NULL." << endl;
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
UInt64 BitVector::PartSelect::getUInt64() const
{
  if(m_parent == NULL)
  {
    LOG_ERR_ENV << "Parent was NULL." << endl;
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
    LOG_ERR_ENV << "Parent was NULL." << endl;
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
    LOG_WRN_ENV << "Destination(" << this->m_upperIndex << "," 
                << this->m_lowerIndex << ") and Source(" 
                << l_srcUpperIdx << "," << l_srcLowerIdx
                << ") Part Selects differ in size." << endl;
    // ex: bv_a[15:0] = bv_b[31:8]; => bv_a[15:0] = bv_b[23:8];
    l_srcUpperIdx = l_srcLowerIdx + l_dstSize - 1;
    l_srcSize = l_dstSize;
  }
  else if(l_srcSize < l_dstSize)
  {
    LOG_WRN_ENV << "Destination(" << this->m_upperIndex << "," 
                << this->m_lowerIndex << ") and Source(" 
                << l_srcUpperIdx << "," << l_srcLowerIdx
                << ") Part Selects differ in size." << endl;
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

    if(ii == 0)
    {
      l_transferWord <<= l_dstLowerShift;
      // Get the original word and wipe out the upper part that we're overwriting.
      // Then OR it with our current result which will fit in the part we just wiped out.
      UInt32 msk = this->m_parent->getMask(l_dstLowerShift, true);
      l_transferWord |= this->m_parent->m_aval->at(l_dstLowerWord) & msk;
    }
    if(ii == (l_dstWordCnt - 1))
    {
      UInt32 msk = this->m_parent->getMask(l_dstUpperShift - 1);
      l_transferWord |= (this->m_parent->m_aval->at(l_dstUpperWord) & ~msk);
    }
    this->m_parent->m_aval->at(l_dstLowerWord + ii) = l_transferWord;
    l_nbSrcBitsCopied += l_nbSrcBits;
  }
  m_parent->applyMask();
}
void BitVector::PartSelect::getParentBits(BitVector & oBV, bool iNoResize) const
{
  if(this->m_parent == NULL)
  {
    LOG_ERR_ENV << "Parent was NULL." << endl;
    return;
  }

  // Check oBV size and/or resize.
  UInt32 l_bitCnt = m_upperIndex - m_lowerIndex + 1;
  if((oBV.m_size != l_bitCnt) && !iNoResize)
  {
    oBV.Resize(l_bitCnt);
  }

  UInt32 l_srcLowerWord = m_parent->getWordNb(m_lowerIndex);
  UInt32 l_shift = m_parent->getShift(m_lowerIndex);
  UInt32 l_wordCnt = oBV.m_aval->size();

  for(UInt32 ii=0; ii<l_wordCnt; ii++)
  {
    (*oBV.m_aval)[ii] = (*m_parent->m_aval)[l_srcLowerWord + ii] >> l_shift;
    // Check if there is another word to pull from.
    if(((l_srcLowerWord + ii + 1) < m_parent->m_aval->size()) && (l_shift != 0))
    {
      UInt32 l_orVal = (*m_parent->m_aval)[l_srcLowerWord + ii + 1] << (32 - l_shift);
      (*oBV.m_aval)[ii] |= l_orVal;
    }
  }
  oBV.applyMask();
}

// =============================
// ===**     Operators     **===
// =============================
BitVector::PartSelect & BitVector::PartSelect::operator= (UInt32 iRhs)
{
  if(m_lowerIndex >= m_parent->m_size)
  {
    LOG_MEDIUM  << "PartSelect(" << m_upperIndex << "," << m_lowerIndex 
                << ") of " << m_parent->m_name 
                << " is out of bounds (size = " << m_parent->m_size 
                << ")." << endl;
    return *this;
  }
  UInt32 l_sz = m_upperIndex - m_lowerIndex + 1;
  BitVector l_bv("BitVector::PartSelect::operator=", l_sz);
  l_bv = iRhs;
  setParentBits(l_bv(l_sz - 1,0));
  return *this;
}
BitVector::PartSelect & BitVector::PartSelect::operator= (UInt64 iRhs)
{
  if(m_lowerIndex >= m_parent->m_size)
  {
    return *this;
  }
  UInt32 l_sz = m_upperIndex - m_lowerIndex + 1;
  BitVector l_bv("BitVector::PartSelect::operator=", l_sz);
  l_bv = iRhs;
  setParentBits(l_bv(l_sz - 1,0));
  return *this;
}
BitVector::PartSelect & BitVector::PartSelect::operator= (BitVector & iRhs)
{
  if(m_lowerIndex >= m_parent->m_size)
  {
    return *this;
  }
  UInt32 l_sz = m_upperIndex - m_lowerIndex + 1;
  setParentBits(iRhs(l_sz - 1, 0));
  return *this;
}
BitVector::PartSelect & BitVector::PartSelect::operator= (BitVector && iRhs)
{
  if(m_lowerIndex >= m_parent->m_size)
  {
    return *this;
  }
  UInt32 l_sz = m_upperIndex - m_lowerIndex + 1;
  setParentBits(iRhs(l_sz - 1, 0));
  return *this;
}
BitVector::PartSelect & BitVector::PartSelect::operator= (const PartSelect & iRhs)
{
  if(m_lowerIndex >= m_parent->m_size)
  {
    return *this;
  }
  setParentBits(iRhs);
  return *this;
}
BitVector BitVector::PartSelect::operator+= (UInt32 iRhs)
{
  // BV0 = BV1(3,0) += 0xffffULL;
  // This should return a BV that reflects the size of the selection,
  // NOT an extended size (RHS size).
  BitVector l_bv(*this);
  l_bv += iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator+= (UInt64 iRhs)
{
  BitVector l_bv(*this);
  l_bv += iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator+= (const BitVector & iRhs)
{
  BitVector l_bv(*this);
  l_bv += iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator+ (UInt32 iRhs) const
{
  // As opposed to the += operator which returns a BV sized to 
  // *this size, the + operator expects no truncation due to the
  // size of the PartSelect.  The part select is simply treated
  // as a value to be added to the RHS. The returned BV should
  // have enough space to carry the entire sum.
  // If the returned value is larger than the value being assigned
  // to, the value will be truncated by the assignment operator.
  UInt32 l_sz = max(32U, m_upperIndex - m_lowerIndex + 1) + 1;
  BitVector l_bv(*this, l_sz);
  l_bv += iRhs;
  return l_bv;
}
BitVector BitVector::PartSelect::operator+ (UInt64 iRhs) const
{
  UInt32 l_sz = max(64U, m_upperIndex - m_lowerIndex + 1) + 1;
  BitVector l_bv(*this, l_sz);
  l_bv += iRhs;
  return l_bv;
}
BitVector BitVector::PartSelect::operator-= (UInt32 iRhs)
{
  BitVector l_bv(*this);
  l_bv -= iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator-= (UInt64 iRhs)
{
  BitVector l_bv(*this);
  l_bv -= iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator-= (const BitVector & iRhs)
{
  BitVector l_bv(*this);
  l_bv -= iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator<<= (UInt32 iRhs)
{
  BitVector l_bv(*this);
  l_bv <<= iRhs;
  setParentBits(l_bv(m_upperIndex, m_lowerIndex));
  return l_bv;
}
BitVector BitVector::PartSelect::operator>>= (UInt32 iRhs)
{
  BitVector l_bv(*this);
  l_bv >>= iRhs;
  setParentBits(l_bv(m_upperIndex, m_lowerIndex));
  return l_bv;
}
BitVector BitVector::PartSelect::operator&= (UInt32 iRhs)
{
  BitVector l_bv(*this);
  l_bv &= iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator&= (UInt64 iRhs)
{
  BitVector l_bv(*this);
  l_bv &= iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator&= (const BitVector & iRhs)
{
  BitVector l_bv(*this);
  l_bv &= iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator|= (UInt32 iRhs)
{
  BitVector l_bv(*this);
  l_bv |= iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator|= (UInt64 iRhs)
{
  BitVector l_bv(*this);
  l_bv |= iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator|= (const BitVector & iRhs)
{
  BitVector l_bv(*this);
  l_bv |= iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator^= (UInt32 iRhs)
{
  BitVector l_bv(*this);
  l_bv ^= iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator^= (UInt64 iRhs)
{
  BitVector l_bv(*this);
  l_bv ^= iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}
BitVector BitVector::PartSelect::operator^= (const BitVector & iRhs)
{
  BitVector l_bv(*this);
  l_bv ^= iRhs;
  setParentBits(l_bv(l_bv.m_size-1, 0));
  return l_bv;
}

// ================================
// ===** Non-Member Operators **===
// ================================
BitVector operator- (UInt32 iLhs, const BitVector & iRhs)
{
  UInt32 l_sz = max(32U, iRhs.Size_get());
  BitVector l_lhs("operator-_UInt32_BitVector", l_sz, iRhs.NbStates_get());
  l_lhs = iLhs;
  BitVector l_retVal(l_lhs);
  l_retVal -= iRhs;
  return l_retVal;
}
BitVector operator- (UInt64 iLhs, const BitVector & iRhs)
{
  UInt32 l_sz = max(64U, iRhs.Size_get());
  BitVector l_lhs("operator-_UInt64_BitVector", l_sz, iRhs.NbStates_get());
  l_lhs = iLhs;
  BitVector l_retVal(l_lhs);
  l_retVal -= iRhs;
  return l_retVal;
}
bool operator<= (UInt32 iLhs, const BitVector & iRhs)
{
  UInt32 l_sz = max(32U, iRhs.Size_get());
  BitVector l_lhs("operator<=_UInt32_BitVector", l_sz, iRhs.NbStates_get());
  l_lhs = iLhs;
  bool l_retVal = (l_lhs <= iRhs);
  return l_retVal;
}
bool operator<= (UInt64 iLhs, const BitVector & iRhs)
{
  UInt32 l_sz = max(64U, iRhs.Size_get());
  BitVector l_lhs("operator<=_UInt64_BitVector", l_sz, iRhs.NbStates_get());
  l_lhs = iLhs;
  bool l_retVal = (l_lhs <= iRhs);
  return l_retVal;
}
bool operator>= (UInt32 iLhs, const BitVector & iRhs)
{
  UInt32 l_sz = max(32U, iRhs.Size_get());
  BitVector l_lhs("operator>=_UInt32_BitVector", l_sz, iRhs.NbStates_get());
  l_lhs = iLhs;
  bool l_retVal = (l_lhs >= iRhs);
  return l_retVal;
}
bool operator>= (UInt64 iLhs, const BitVector & iRhs)
{
  UInt32 l_sz = max(64U, iRhs.Size_get());
  BitVector l_lhs("operator>=_UInt64_BitVector", l_sz, iRhs.NbStates_get());
  l_lhs = iLhs;
  bool l_retVal = (l_lhs >= iRhs);
  return l_retVal;
}
bool operator< (UInt32 iLhs, const BitVector & iRhs)
{
  UInt32 l_sz = max(32U, iRhs.Size_get());
  BitVector l_lhs("operator<_UInt32_BitVector", l_sz, iRhs.NbStates_get());
  l_lhs = iLhs;
  bool l_retVal = (l_lhs < iRhs);
  return l_retVal;
}
bool operator< (UInt64 iLhs, const BitVector & iRhs)
{
  UInt32 l_sz = max(64U, iRhs.Size_get());
  BitVector l_lhs("operator<_UInt64_BitVector", l_sz, iRhs.NbStates_get());
  l_lhs = iLhs;
  bool l_retVal = (l_lhs < iRhs);
  return l_retVal;
}
bool operator> (UInt32 iLhs, const BitVector & iRhs)
{
  UInt32 l_sz = max(32U, iRhs.Size_get());
  BitVector l_lhs("operator>_UInt32_BitVector", l_sz, iRhs.NbStates_get());
  l_lhs = iLhs;
  bool l_retVal = (l_lhs > iRhs);
  return l_retVal;
}
bool operator> (UInt64 iLhs, const BitVector & iRhs)
{
  UInt32 l_sz = max(64U, iRhs.Size_get());
  BitVector l_lhs("operator>_UInt64_BitVector", l_sz, iRhs.NbStates_get());
  l_lhs = iLhs;
  bool l_retVal = (l_lhs > iRhs);
  return l_retVal;
}
BitVector operator, (UInt32 iLhs, const BitVector & iRhs)
{
  UInt32 l_sz = 32 + iRhs.Size_get();
  BitVector l_retVal(iRhs, l_sz);
  l_retVal(l_sz - 1, l_sz - 32) = iLhs;
  return l_retVal;
}
BitVector operator, (UInt64 iLhs, const BitVector & iRhs)
{
  UInt32 l_sz = 64 + iRhs.Size_get();
  BitVector l_retVal(iRhs, l_sz);
  l_retVal(l_sz - 1, l_sz - 64) = iLhs;
  return l_retVal;
}
BitVector operator, (UInt32 iLhs, const BitVector::PartSelect & iRhs)
{
  UInt32 l_sz = 32 + iRhs.UpperIndex_get() - iRhs.LowerIndex_get() + 1;
  BitVector l_retVal(iRhs, l_sz);
  l_retVal(l_sz - 1, l_sz - 32) = iLhs;
  return l_retVal;
}
BitVector operator, (UInt64 iLhs, const BitVector::PartSelect & iRhs)
{
  UInt32 l_sz = 64 + iRhs.UpperIndex_get() - iRhs.LowerIndex_get() + 1;
  BitVector l_retVal(iRhs, l_sz);
  l_retVal(l_sz - 1, l_sz - 64) = iLhs;
  return l_retVal;
}
