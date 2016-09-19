/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   vpi.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/18/16
#   Description   :   
#
###############################################################################
*/

#include <string>

#include "vpi.h"

vpiHandle Vpi::vpi_register_systf(const struct Vpi::t_vpi_systf_data *ss)
{
  // Call the global function.
  return ::vpi_register_systf((::t_vpi_systf_data*)ss);
}

