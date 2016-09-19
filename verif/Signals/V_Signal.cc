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

// Constructors
V_Signal::V_Signal(string iFullName)
{
  init(iFullName, V_Signal::NB_STATES::FOUR_STATE);
}

// Inits
bool V_Signal::init(string iFullName, V_Signal::NB_STATES iValue)
{
  m_name_full = iFullName;
  m_nb_states = iValue;

  return setHandle();
}

// Public Methods

// Private Methods
bool V_Signal::setHandle()
{
  vpiHandle temp = vpi_handle_by_name(m_name_full.c_str(), NULL);
  if(temp == NULL) 
  {
    //cout << "Could not find signal '" << m_name_full << "'" << endl;
    vpi_printf("Could not find signal '%s'.\n", m_name_full.c_str());
  }
  else
  {
    //cout << "Found signal '" << m_name_full << "'" << endl;
    vpi_printf("Found signal '%s'.\n", m_name_full.c_str());
  }
  return true;
}


