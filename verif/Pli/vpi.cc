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
vpiHandle Vpi::vpi_register_cb(p_cb_data iCbData)
{
  return ::vpi_register_cb((::p_cb_data)iCbData);
}
Int32 Vpi::vpi_remove_cb(vpiHandle iCbHndl)
{
  return ::vpi_remove_cb(iCbHndl);
}
void Vpi::vpi_control(Int32 iOperation)
{
  ::vpi_control(iOperation);
}
void Vpi::vpi_sim_control(Int32 iOperation)
{
  ::vpi_sim_control(iOperation);
}
vpiHandle Vpi::vpi_handle(Vpi::OBJECT iType, vpiHandle iRef)
{
  return ::vpi_handle((Int32)iType, iRef);
}
vpiHandle Vpi::vpi_iterate(Vpi::OBJECT iType, vpiHandle iRef)
{
  return ::vpi_iterate((Int32)iType, iRef);
}
vpiHandle Vpi::vpi_scan(vpiHandle iIterator)
{
  return ::vpi_scan(iIterator);
}
vpiHandle Vpi::vpi_handle_by_index(vpiHandle iRef, Int32 iIndex)
{
  return ::vpi_handle_by_index(iRef, iIndex);
}
vpiHandle Vpi::vpi_handle_by_name(const char *iName, vpiHandle iScope)
{
  return ::vpi_handle_by_name(iName, iScope);
}
void Vpi::vpi_get_time(vpiHandle iObj, p_vpi_time oTime)
{
  ::vpi_get_time(iObj, (::p_vpi_time)oTime);
}
Int32 Vpi::vpi_get(Vpi::PROPERTY iProperty, vpiHandle iRef)
{
  return ::vpi_get((Int32)iProperty, iRef);
}
char * Vpi::vpi_get_str(Vpi::PROPERTY iProperty, vpiHandle iRef)
{
  return ::vpi_get_str((Int32)iProperty, iRef);
}
void Vpi::vpi_get_value(vpiHandle iHndl, p_vpi_value oValue)
{
  ::vpi_get_value(iHndl, (::p_vpi_value)oValue);
}
vpiHandle Vpi::vpi_put_value(vpiHandle iObj, p_vpi_value iValue,
                             p_vpi_time iWhen, Int32 iFlags)
{
  return ::vpi_put_value(iObj, (::p_vpi_value)iValue, (::p_vpi_time)iWhen, iFlags);
}
Int32 Vpi::vpi_free_object(vpiHandle iRef)
{
  return ::vpi_free_object(iRef);
}
Int32 Vpi::vpi_get_vlog_info(p_vpi_vlog_info oVlogInfo)
{
  return ::vpi_get_vlog_info((::p_vpi_vlog_info)oVlogInfo);
}
Int32 Vpi::vpi_compare_objects(vpiHandle iObj1, vpiHandle iObj2)
{
  return ::vpi_compare_objects(iObj1, iObj2);
}
void Vpi::vpi_get_delays(vpiHandle iHndl, t_vpi_delay *oDelays)
{
  ::vpi_get_delays(iHndl, (::t_vpi_delay *)oDelays);
}
void Vpi::vpi_put_delays(vpiHandle iHndl, t_vpi_delay *iDelays)
{
  ::vpi_put_delays(iHndl, (::t_vpi_delay *)iDelays);
}
Int32 Vpi::vpi_put_userdata(vpiHandle iObj, void *iData)
{
  return ::vpi_put_userdata(iObj, iData);
}
void * Vpi::vpi_get_userdata(vpiHandle iObj)
{
  return ::vpi_get_userdata(iObj);
}
Int32 Vpi::vpi_chk_error(t_vpi_error_info *oInfo)
{
  return ::vpi_chk_error((::t_vpi_error_info *)oInfo);
}

