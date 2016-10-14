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

#include <iostream>

#include "BitVector.h"
#include "EnvManager.h"
#include "Logger.h"
#include "TestController.h"
#include "vpi_entry.h"
#include "V_Signal.h"

// ===============================
// ===**  Master Export List **===
// ===============================
// Not a part of vpi_entry class.
// This is the list of routines that are imported by the simulator
// and used as $name in the Verilog environment.
// It also may contain callbacks for certain events (i.e. simulation start).
void (*vlog_startup_routines[])() = {
  vpi_entry::EndOfCompilationCB_register,
  vpi_entry::StartOfSimulationCB_register,
  vpi_entry::EndOfSimulationCB_register,
  vpi_entry::tb_build_register,
  0
};

// ====================================
// ===**  Private Static Members  **===
// ====================================
vpiHandle vpi_entry::m_topModule = NULL;

// ===============================
// ===**  Public Properties  **===
// ===============================
vpiHandle vpi_entry::TopModule_get()
{
  return m_topModule;
}

// ============================
// ===**  Public Methods  **===
// ============================

// TBD - create the compile check.
Int32 vpi_entry::tb_build(char * UNUSED(iUserData))
{
  LOG_DEBUG << "tb_build: Entry." << endl;

  // Get the Sys TF Call Handle
  vpiHandle systf_handle = Vpi::vpi_handle(Vpi::OBJECT::SYS_TF_CALL, NULL);
  if(systf_handle == NULL)
  {
    LOG_ERR_ENV << "Failed to get SYS_TF_CALL handle to tb_build." << endl;
    return 0;
  }

  if(!setTopModule(systf_handle))
  {
    return 0;
  }
  LOG_DEBUG << "m_topModule name is '" 
            << Vpi::vpi_get_str(Vpi::PROPERTY::NAME, m_topModule)
            << "'." << endl;
  
  LOG_DEBUG << "tb_build: Exit." << endl;
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

Int32 vpi_entry::EndOfCompilationCB(Vpi::t_cb_data * UNUSED(iCbData))
{
  cout << LINE_HDR << "========= End of Compilation =========" << endl;
  // Creating the EnvManager runs inits on the environment.
  EnvManager::Access();
  return 0;
}
void vpi_entry::EndOfCompilationCB_register()
{
  /*
    CB_REASON   reason;
    Int32       (*cb_rtn)(t_cb_data *cb);
    vpiHandle   obj;
    p_vpi_time  time;
    p_vpi_value value;
    Int32       index;
    char *      user_data;
  */
  Vpi::t_cb_data l_cb_data;

  l_cb_data.reason = Vpi::CB_REASON::END_OF_COMPILE;
  l_cb_data.cb_rtn = EndOfCompilationCB;
  l_cb_data.obj = NULL;
  l_cb_data.time = NULL;
  l_cb_data.value = NULL;
  l_cb_data.index = 0;
  l_cb_data.user_data = 0;

  Vpi::vpi_register_cb(&l_cb_data);
}

Int32 vpi_entry::StartOfSimulationCB(Vpi::t_cb_data * UNUSED(iCbData))
{
  LOG_DEBUG << "========= Start of Simulation =========" << endl;
  // Stub
  return 0;
}
void vpi_entry::StartOfSimulationCB_register()
{
  /*
    CB_REASON   reason;
    Int32       (*cb_rtn)(t_cb_data *cb);
    vpiHandle   obj;
    p_vpi_time  time;
    p_vpi_value value;
    Int32       index;
    char *      user_data;
  */
  Vpi::t_cb_data l_cb_data;

  l_cb_data.reason = Vpi::CB_REASON::START_OF_SIMULATION;
  l_cb_data.cb_rtn = StartOfSimulationCB;
  l_cb_data.obj = NULL;
  l_cb_data.time = NULL;
  l_cb_data.value = NULL;
  l_cb_data.index = 0;
  l_cb_data.user_data = 0;

  Vpi::vpi_register_cb(&l_cb_data);
}

Int32 vpi_entry::EndOfSimulationCB(Vpi::t_cb_data * UNUSED(iCbData))
{
  // Stub
  LOG_DEBUG << "========= End of Simulation =========" << endl;
  return 0;
}
void vpi_entry::EndOfSimulationCB_register()
{
  /*
    CB_REASON   reason;
    Int32       (*cb_rtn)(t_cb_data *cb);
    vpiHandle   obj;
    p_vpi_time  time;
    p_vpi_value value;
    Int32       index;
    char *      user_data;
  */
  Vpi::t_cb_data l_cb_data;

  l_cb_data.reason = Vpi::CB_REASON::END_OF_SIMULATION;
  l_cb_data.cb_rtn = EndOfSimulationCB;
  l_cb_data.obj = NULL;
  l_cb_data.time = NULL;
  l_cb_data.value = NULL;
  l_cb_data.index = 0;
  l_cb_data.user_data = 0;

  Vpi::vpi_register_cb(&l_cb_data);
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
    // TBD - Add report of systf name, param name.
    //vpi_printf("ERROR: Argument was not a module.\n");
    LOG_ERR_ENV << "Argument was not a module." << endl;
    return false;
  }
  bool isTopModule = (bool)Vpi::vpi_get(Vpi::PROPERTY::TOP_MODULE, arg_handle);
  if(!isTopModule)
  {
    // TBD - Add report of systf name, param name.
    LOG_ERR_ENV << "Argument was not the top-level module." << endl;
    return false;
  }
  m_topModule = arg_handle;
  return true;
}

