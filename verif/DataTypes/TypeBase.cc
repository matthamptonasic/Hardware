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
TypeBase::~TypeBase()
{
  unregisterValChangeCB();
}

// =============================
// ===**      Inits        **===
// =============================
bool TypeBase::init(string iFullName, NB_STATES iValue)
{
  m_nameFull = iFullName;
  m_nbStates = iValue;
  m_maskValChange = false;
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
  registerValChangeCB();
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
void TypeBase::registerValChangeCB()
{
  Vpi::t_cb_data l_cb_data;
  Vpi::t_vpi_time l_vpi_time;
  Vpi::t_vpi_value l_vpi_value;

  l_vpi_time.type = Vpi::TIME_TYPE::SCALED_REAL_TIME;
  l_vpi_value.format = Vpi::VALUE_FORMAT::VECTOR;

  l_cb_data.reason = Vpi::CB_REASON::VALUE_CHANGE;
  l_cb_data.cb_rtn = s_valueChangedCB;
  l_cb_data.obj = m_sigHandle;
  l_cb_data.time = &l_vpi_time;
  l_cb_data.value = &l_vpi_value;
  l_cb_data.index = 0;
  l_cb_data.user_data = (char *)this;

  m_callBackHandle = Vpi::vpi_register_cb(&l_cb_data);
}
void TypeBase::unregisterValChangeCB()
{
  if(!Vpi::vpi_remove_cb(m_callBackHandle))
  {
    LOG_WRN_ENV << "vpi_remove_cb should have returned 1 and did not." << endl;
  }
}
Int32 TypeBase::valueChangedCB(Vpi::t_cb_data * iData)
{
  if(!m_maskValChange)
  {
    Pli::ImportVector(iData->value, m_bv->m_aval->size(), m_bv->m_aval, m_bv->m_bval);
  }
  else
  {
    m_maskValChange = false;
  }
  return 0;
}
Int32 TypeBase::s_valueChangedCB(Vpi::t_cb_data * iData)
{
  TypeBase* l_inst = (TypeBase*)iData->user_data;
  l_inst->valueChangedCB(iData);
  return 0;
}

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
  // * This masking is not thread safe. *
  // Not fixing it now because I can't think of any scenario
  // where a C thread is actively running in parallel with
  // the simulator (the scheduler running).
  // As long as there is no chance the C thread will be interrupted
  // while the simulator continues, we should be okay.
  m_maskValChange = true;
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
  //return l_retVal;
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
BitVector TypeBase::PartSelect::operator-= (UInt32 iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) -= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator-= (UInt64 iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) -= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator-= (const BitVector & iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) -= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator<<= (UInt32 iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) <<= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator>>= (UInt32 iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) >>= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator&= (UInt32 iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) &= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator&= (UInt64 iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) &= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator&= (const BitVector & iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) &= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator|= (UInt32 iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) |= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator|= (UInt64 iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) |= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator|= (const BitVector & iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) |= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator^= (UInt32 iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) ^= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator^= (UInt64 iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) ^= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
BitVector TypeBase::PartSelect::operator^= (const BitVector & iRhs)
{
  BitVector && l_retVal = (*m_parent->m_bv)(m_upperIndex, m_lowerIndex) ^= iRhs;
  m_parent->set_RtlValue();
  return move(l_retVal);
}
