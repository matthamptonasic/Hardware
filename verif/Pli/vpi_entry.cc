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
#include "pli.h"
#include "TestController.h"
#include "TypeBase.h"

#include "vpi_entry.h"

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
// ===**     Static Members       **===
// ====================================
vpiHandle vpi_entry::m_topModule = NULL;
Event<void> vpi_entry::_s_EndOfCompilation;
Event<void> vpi_entry::_s_StartOfSimulation;
Event<void> vpi_entry::_s_EndOfSimulation;

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

Int32 vpi_entry::tb_build_compile(char * UNUSED(iUserData))
{
  vpiHandle systf_handle = Vpi::vpi_handle(Vpi::OBJECT::SYS_TF_CALL, NULL);
  if(systf_handle == NULL)
  {
    cout << "Failed to get SYS_TF_CALL handle to tb_build." << endl;
    Pli::DollarFinish();
    return 0;
  }

  string l_systfName = Vpi::vpi_get_str(Vpi::PROPERTY::NAME, systf_handle);

  // Get iterator handle for the arguments.
  vpiHandle arg_iterator = Vpi::vpi_iterate(Vpi::OBJECT::ARGUMENT, systf_handle);
  if(arg_iterator == NULL)
  {
    cout << "No arguments were passed to sysTf '" << l_systfName << "'. Exiting." << endl;
    Pli::DollarFinish();
    return 0;
  }

  // Check the argument type.
  vpiHandle arg_handle = Vpi::vpi_scan(arg_iterator);
  Vpi::OBJECT obj = (Vpi::OBJECT)Vpi::vpi_get(Vpi::PROPERTY::TYPE, arg_handle);
  if(obj != Vpi::OBJECT::MODULE)
  {
    string l_name = Vpi::vpi_get_str(Vpi::PROPERTY::NAME, arg_handle);
    cout << "Argument (" << l_name << ") to SysTfCall '" << l_systfName << "' was not a module." << endl;
    Pli::DollarFinish();
    return 0;
  }
  bool isTopModule = (bool)Vpi::vpi_get(Vpi::PROPERTY::TOP_MODULE, arg_handle);
  if(!isTopModule)
  {
    string l_name = Vpi::vpi_get_str(Vpi::PROPERTY::NAME, arg_handle);
    cout << "Argument (" << l_name << ") to SysTfCall '" << l_systfName << "' was not the top-level module." << endl;
    Pli::DollarFinish();
    return 0;
  }

  // Check that there are no more arguments.
  arg_handle = Vpi::vpi_scan(arg_iterator);
  if(arg_handle != NULL)
  {
    cout << "SysTfCall '" << l_systfName << "' has too many arguments." << endl;
    Vpi::vpi_free_object(arg_iterator);
    Pli::DollarFinish();
    return 0;
  }
}
Int32 vpi_entry::tb_build(char * UNUSED(iUserData))
{
  LOG_DEBUG << "tb_build: Entry." << endl;

  // Get the Sys TF Call Handle
  vpiHandle systf_handle = Vpi::vpi_handle(Vpi::OBJECT::SYS_TF_CALL, NULL);

  vpiHandle arg_iterator = Vpi::vpi_iterate(Vpi::OBJECT::ARGUMENT, systf_handle);
  m_topModule = Vpi::vpi_scan(arg_iterator);

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
  tf_data.compiletf = tb_build_compile;
  tf_data.sizetf    = 0;

  Vpi::vpi_register_systf(&tf_data);
}

Int32 vpi_entry::EndOfCompilationCB(Vpi::t_cb_data * UNUSED(iCbData))
{
  cout << LINE_HDR << "========= End of Compilation =========" << endl;
  // Creating the EnvManager runs inits on the environment.
  EnvManager::Access();
  _s_EndOfCompilation();
  return 0;
}
void vpi_entry::EndOfCompilationCB_register()
{
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
  _s_StartOfSimulation();
  // Stub
  return 0;
}
void vpi_entry::StartOfSimulationCB_register()
{
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
  LOG_DEBUG << "========= End of Simulation =========" << endl;
  _s_EndOfSimulation();
  // Stub
  return 0;
}
void vpi_entry::EndOfSimulationCB_register()
{
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
