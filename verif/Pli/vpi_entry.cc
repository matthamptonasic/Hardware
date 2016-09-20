/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   vpi_entry.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/06/16
#   Description   :   
#
###############################################################################
*/

#include "vpi_entry.h"
#include "V_Signal.h"

// ====================================
// ===**  Private Static Members  **===
// ====================================
vpiHandle vpi_entry::m_topModule = NULL;

// ============================
// ===**  Public Methods  **===
// ============================

// TBD - create the compile check.
Int32 vpi_entry::tb_build(char* UNUSED(iUserData))
{
  vpi_printf("c++ tb_build: Entry.\n");

  // Get the Sys TF Call Handle
  vpiHandle systf_handle = Vpi::vpi_handle(Vpi::OBJECT::SYS_TF_CALL, NULL);
  if(systf_handle == NULL)
  {
    Vpi::vpi_printf("Failed to get SYS_TF_CALL handle to tb_build.\n");
    return 0;
  }

  if(!setTopModule(systf_handle))
  {
    return 0;
  }
  Vpi::vpi_printf("m_topModule name is '%s'.\n", Vpi::vpi_get_str(Vpi::PROPERTY::NAME, m_topModule));
  
  vpi_printf("c++ tb_build: Exit.\n");
  return 0;
}

void vpi_entry::tb_build_register()
{
  Vpi::t_vpi_systf_data tf_data;

  tf_data.type      = Vpi::SYS_TASK_FUNC::TASK;
  tf_data.tfname    = "$tb_build";
  tf_data.calltf    = tb_build;
  tf_data.compiletf = 0;
  tf_data.sizetf    = 0;

  Vpi::vpi_register_systf(&tf_data);
}

// =============================
// ===**  Private Methods  **===
// =============================

bool vpi_entry::setTopModule(vpiHandle iSysTfCall)
{
  if(iSysTfCall == NULL)
  {
    return false;
  }

  // Get iterator handle for the arguments.
  vpiHandle arg_iterator = Vpi::vpi_iterate(Vpi::OBJECT::ARGUMENT, iSysTfCall);
  vpiHandle arg_handle = Vpi::vpi_scan(arg_iterator);

  Vpi::OBJECT obj = (Vpi::OBJECT)vpi_get((int)Vpi::PROPERTY::TYPE, arg_handle);
  if(obj != Vpi::OBJECT::MODULE)
  {
    // TBD - Added report of systf name, param name.
    vpi_printf("ERROR: Argument was not a module.\n");
    return false;
  }
  bool isTopModule = (bool)Vpi::vpi_get(Vpi::PROPERTY::TOP_MODULE, arg_handle);
  if(!isTopModule)
  {
    // TBD - Added report of systf name, param name.
    Vpi::vpi_printf("ERROR: Argument was not the top-level module.\n");
    return false;
  }
  m_topModule = arg_handle;
  return true;
}

