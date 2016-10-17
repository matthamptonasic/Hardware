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

#include "Logger.h"
#include "vpi_entry.h"
#include "pli.h"

#include "TypeBase.h"

// Constructors
TypeBase::TypeBase(string iFullName)
{
  init(iFullName, TypeBase::NB_STATES::FOUR_STATE);
  UInt32 curVal = (UInt32)Pli::GetScalar(m_sigHandle);
  LOG_DEBUG << "Scalar value is 0x" << hex << curVal << dec << "." << endl;
  curVal = (UInt32)Pli::GetVector(m_sigHandle);
  LOG_DEBUG << "Vector value is 0x" << hex << curVal << dec << "." << endl;
}

// Inits
bool TypeBase::init(string iFullName, TypeBase::NB_STATES iValue)
{
  m_name_full = iFullName;
  m_nb_states = iValue;

  return setHandle();
}

// Public Methods
UInt32 TypeBase::GetValue()
{
  UInt32 retVal = 0xffffffff;

  return retVal;
}

// Private Methods
bool TypeBase::setHandle()
{

  LOG_DEBUG << "Looking for signal '" << m_name_full << "'" << endl;
  m_sigHandle = Vpi::vpi_handle_by_name(m_name_full.c_str(), vpi_entry::TopModule_get());
  if(m_sigHandle == NULL) 
  {
    LOG_WRN_ENV << "Could not find signal '" << m_name_full << "'" << endl;
  }
  else
  {
    LOG_DEBUG << "Found signal '" << m_name_full << "'" << endl;
  }
  return true;
}


