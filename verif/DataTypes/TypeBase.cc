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
TypeBase::TypeBase(string iFullName, NB_STATES iStates)
{
  if(!init(iFullName, iStates))
  {
    LOG_ERR_ENV << "Could not initialize the signal '" << iFullName << "'." << endl;
    return;
  }
}

// =============================
// ===**      Inits        **===
// =============================
bool TypeBase::init(string iFullName, NB_STATES iValue)
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

  if(!set_Handle())
  {
    return false;
  }
  return true;
}

// =============================
// ===**  Public Methods   **===
// =============================
BitVector TypeBase::Get_Value()
{
  get_RtlValue();
  return *m_bv;
}
string TypeBase::ToString() const
{
  return m_bv->ToString();
}
void TypeBase::Print() const
{
  m_bv->Print();
}

// =============================
// ===**  Private Methods  **===
// =============================

// =============================
// ===** Protected Methods **===
// =============================
bool TypeBase::set_Handle()
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
void TypeBase::get_RtlValue()
{
  if(m_nbStates == NB_STATES::TWO_STATE)
  {
    Pli::GetVector(get_SigHandle(), m_bv->m_aval);
  }
  else
  {
    Pli::GetVector(get_SigHandle(), m_bv->m_aval, m_bv->m_bval);
  }
}
void TypeBase::set_RtlValue()
{
  if(m_nbStates == NB_STATES::TWO_STATE)
  {
    Pli::SetVector(get_SigHandle(), m_bv->m_aval);
  }
  else
  {
    Pli::SetVector(get_SigHandle(), m_bv->m_aval, m_bv->m_bval);
  }
}

// =============================
// ===**     Operators     **===
// =============================
TypeBase::PartSelect TypeBase::operator() (UInt32 iUpperIndex, UInt32 iLowerIndex)
{
  LOG_DEBUG << __PRETTY_FUNCTION__ << endl;
  TypeBase::PartSelect l_retVal(this, iUpperIndex, iLowerIndex);
  return l_retVal;
}


// *==*==*==*==*==*==*==*==*==*==*==*==*
// ===**     Part Select Class    **===
// *==*==*==*==*==*==*==*==*==*==*==*==*

// =============================
// ===**   Constructors    **===
// =============================
TypeBase::PartSelect::PartSelect(TypeBase * iTB, UInt32 iUpperIndex, UInt32 iLowerIndex)
{
  m_parent = iTB;
  m_upperIndex = iUpperIndex;
  m_lowerIndex = iLowerIndex;
}

// =============================
// ===**     Operators     **===
// =============================
TypeBase::PartSelect & TypeBase::PartSelect::operator= (UInt32 iRhs)
{
  (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) = iRhs;
  m_parent->set_RtlValue();
  return *this;
}
TypeBase::PartSelect & TypeBase::PartSelect::operator= (UInt64 iRhs)
{
  (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) = iRhs;
  m_parent->set_RtlValue();
  return *this;
}
TypeBase::PartSelect & TypeBase::PartSelect::operator= (BitVector & iRhs)
{
  (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) = iRhs;
  m_parent->set_RtlValue();
  return *this;
}
TypeBase::PartSelect & TypeBase::PartSelect::operator= (BitVector && iRhs)
{
  (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) = move(iRhs);
  m_parent->set_RtlValue();
  return *this;
}
TypeBase::PartSelect & TypeBase::PartSelect::operator= (const BitVector::PartSelect & iRhs)
{
  (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) = iRhs;
  m_parent->set_RtlValue();
  return *this;
}
BitVector TypeBase::PartSelect::operator+= (UInt32 iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) += iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator+= (UInt64 iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) += iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator+= (const BitVector & iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) += iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
