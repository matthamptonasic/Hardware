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

#include "V_Signal.h"
#include "vpi_entry.h"
#include "pli.h"

// Constructors
V_Signal::V_Signal(string iFullName)
{
  init(iFullName, V_Signal::NB_STATES::FOUR_STATE);
  UInt32 curVal = (UInt32)Pli::GetScalar(m_sigHandle);
  vpi_printf("Scalar value is 0x%x.\n", curVal);
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

  vpi_printf("Looking for signal '%s'.\n", m_name_full.c_str());
  m_sigHandle = Vpi::vpi_handle_by_name(m_name_full.c_str(), vpi_entry::TopModule_get());
  if(m_sigHandle == NULL) 
  {
    //cout << "Could not find signal '" << m_name_full << "'" << endl;
    vpi_printf("Could not find signal '%s'.\n", m_name_full.c_str());
  }
  else
  {
    //cout << "Found signal '" << m_name_full << "'" << endl;
    vpi_printf("Found signal '%s'.\n", m_name_full.c_str());
    vpi_printf("top hd is '0x%x'.\n", (UInt32)(UInt64)vpi_entry::TopModule_get());
    vpi_printf("Handle is '0x%x'.\n", (UInt32)(UInt64)m_sigHandle);
  }
  return true;
}


