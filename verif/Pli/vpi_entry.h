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
// The PLI export function array is at the top of the .cc file.
//
class vpi_entry
{
  // Private Members
  private:
  static vpiHandle m_topModule;

  // Public Properties (get/set)
  public:
  static vpiHandle TopModule_get();

  // Public Methods
  public:
  static Int32  tb_build(char* UNUSED(iUserData));
  static void   tb_build_register();

  // Private Methods
  private:
  static bool   setTopModule(vpiHandle iSysTfCall);
  
};

#endif /* VPI_ENTRY_H */


