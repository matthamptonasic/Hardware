/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   V_Signal.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/16/16
#   Description   :   
#
###############################################################################
*/
#ifndef V_SIGNAL_H
#define V_SIGNAL_H

#include <string>

#include "Common.h"
#include "vpi_user.h"

using namespace std;

class V_Signal 
{
  // Enums
  public:
  enum class NB_STATES : Byte
  {
    TWO_STATE = 0,
    FOUR_STATE = 1
  };

  // Members
  private:
    string m_name;
    string m_name_full;
    vpiHandle m_sigHandle;

    NB_STATES m_nb_states;

  // Constructors
  public:
    V_Signal(string iFullName);

  // Inits
  private:
    bool init(string iFullName, V_Signal::NB_STATES iValue);

  // Public Methods
  public:

  // Private Methods
  private:
    bool setHandle();
};

#endif /* V_SIGNAL_H */

