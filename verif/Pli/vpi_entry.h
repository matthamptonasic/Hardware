/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   vpi_entry.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/11/16
#   Description   :   
#
###############################################################################
*/
#ifndef VPI_ENTRY_H
#define VPI_ENTRY_H

#include "Common.h"
#include "vpi.h"

// This (static) class will serve as a layer of test flow control
// between the simulator and the pli application.
//
// Below the class definition is the export function array.
//
class vpi_entry
{
  // Private Members
  public:
  static vpiHandle m_topModule;

  // Public Methods
  public:
  static Int32  tb_build(char* UNUSED(iUserData));
  static void   tb_build_register();

  // Private Methods
  public:
  static bool   setTopModule(vpiHandle iSysTfCall);
  
};

void (*vlog_startup_routines[])() = {
  vpi_entry::tb_build_register,
  0
};

#endif /* VPI_ENTRY_H */


