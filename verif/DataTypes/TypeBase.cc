/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   TypeBase.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/16/16
#   Description   :   
#
###############################################################################
*/

#include <iostream>

#include "BitVector.h"
#include "Logger.h"
#include "Manip.h"
#include "pli.h"
#include "vpi_entry.h"

#include "TypeBase.h"

using namespace Text;

// ====================================
// ===**  Private Static Members  **===
// ====================================

// ================================
// ===** Protected Properties **===
// ================================

// =============================
// ===** Public Properties **===
// =============================

// =============================
// ===**   Constructors    **===
// =============================
TypeBase::TypeBase(string iFullName, BitVector::NB_STATES iStates)
{
  init(iFullName, iStates);
  UInt32 curVal = (UInt32)Pli::GetScalar(m_sigHandle);
  LOG_DEBUG << "Scalar value is 0x" << hex << curVal << dec << "." << endl;
  curVal = (UInt32)Pli::GetVector(m_sigHandle);
  LOG_DEBUG << "Vector value is 0x" << hex << curVal << dec << "." << endl;
}

// =============================
// ===**      Inits        **===
// =============================
bool TypeBase::init(string iFullName, BitVector::NB_STATES iValue)
{
  m_nameFull = iFullName;
  m_nbStates = iValue;
  vector<string> l_layers = Manip::Split(iFullName, '.');
  if(l_layers.size() > 0)
  {
    // Take the last element as the name.
    m_name = l_layers[l_layers.size() - 1];
  }
  else
  {
    m_name = m_nameFull;
  }

  if(!setHandle())
  {
    return false;
  }
  // Set the bitvector size based on the type and/or size of the verilog object.
  setSize();
}

// =============================
// ===**  Public Methods   **===
// =============================
UInt32 TypeBase::GetValue()
{
  UInt32 retVal = 0xffffffff;

  return retVal;
}

// =============================
// ===**  Private Methods  **===
// =============================
bool TypeBase::setHandle()
{

  LOG_DEBUG << "Looking for signal '" << m_nameFull << "'" << endl;
  m_sigHandle = Vpi::vpi_handle_by_name(m_nameFull.c_str(), vpi_entry::TopModule_get());
  if(m_sigHandle == NULL) 
  {
    LOG_WRN_ENV << "Could not find signal '" << m_nameFull << "'" << endl;
    return false;
  }
  else
  {
    LOG_DEBUG << "Found signal '" << m_nameFull << "'" << endl;
    return true;
  }
}

void TypeBase::createBV()
{
  m_bv = new BitVector(m_nameFull, m_size, m_nbStates);
}

// =============================
// ===** Protected Methods **===
// =============================

// =============================
// ===**     Operators     **===
// =============================

