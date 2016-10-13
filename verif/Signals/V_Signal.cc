/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   V_Signal.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/16/16
#   Description   :   
#
###############################################################################
*/

#include <iostream>

#include "Logger.h"
#include "V_Signal.h"
#include "vpi_entry.h"
#include "pli.h"

// Constructors
V_Signal::V_Signal(string iFullName)
{
  init(iFullName, V_Signal::NB_STATES::FOUR_STATE);
  UInt32 curVal = (UInt32)Pli::GetScalar(m_sigHandle);
  LOG_DEBUG << "Scalar value is 0x" << hex << curVal << dec << "." << endl;
  curVal = (UInt32)Pli::GetVector(m_sigHandle);
  LOG_DEBUG << "Vector value is 0x" << hex << curVal << dec << "." << endl;
}

// Inits
bool V_Signal::init(string iFullName, V_Signal::NB_STATES iValue)
{
  m_name_full = iFullName;
  m_nb_states = iValue;

  return setHandle();
}

// Public Methods
UInt32 V_Signal::GetValue()
{
  UInt32 retVal = 0xffffffff;

  return retVal;
}

// Private Methods
bool V_Signal::setHandle()
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


