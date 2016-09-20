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
void Vpi::vpi_get_systf_info(vpiHandle iSysTfHndl, p_vpi_systf_data oData)
{
  ::vpi_get_systf_info(iSysTfHndl, (::p_vpi_systf_data)oData);
}
UInt32 Vpi::vpi_mcd_open(char *iName)
{
  return ::vpi_mcd_open(iName);
}
UInt32 Vpi::vpi_mcd_close(UInt32 iMcd)
{
  return ::vpi_mcd_close(iMcd);
}
char * Vpi::vpi_mcd_name(UInt32 iMcd)
{
  return ::vpi_mcd_name(iMcd);
}
Int32 Vpi::vpi_mcd_printf(UInt32 iMcd, const char *iFmt, ...)
{
  va_list argp;
  va_start(argp, iFmt);
  Int32 retVal = ::vpi_mcd_vprintf(iMcd, iFmt, argp);
  va_end(argp);
  return retVal;
}
Int32 Vpi::vpi_printf(const char *iFmt, ...)
{
  va_list argp;
  va_start(argp, iFmt);
  Int32 retVal = ::vpi_vprintf(iFmt, argp);
  va_end(argp);
  return retVal;
}
Int32 Vpi::vpi_vprintf(const char *iFmt, va_list iArgs)
{
  return ::vpi_vprintf(iFmt, iArgs);
}
Int32 Vpi::vpi_mcd_vprintf(UInt32 iMcd, const char *iFmt, va_list iArgs)
{
  return ::vpi_mcd_vprintf(iMcd, iFmt, iArgs);
}
Int32 Vpi::vpi_flush()
{
  return ::vpi_flush();
}
Int32 Vpi::vpi_mcd_flush(UInt32 iMcd)
{
  return ::vpi_mcd_flush(iMcd);
}
Int32 Vpi::vpi_fopen(const char *iName, const char *iMode)
{
  return ::vpi_fopen(iName, iMode);
}
FILE * Vpi::vpi_get_file(Int32 iFileDesc)
{
  return ::vpi_get_file(iFileDesc);
}

