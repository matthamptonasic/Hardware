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

static PLI_INT32 tb_build(char* UNUSED(iUserData))
{
#ifdef __cplusplus
  vpi_printf("c++ tb_build: Entry.\n");
#else
  vpi_printf("c tb_build: Entry.\n");
#endif
  return 0;
}

static void tb_build_register()
{
  s_vpi_systf_data tf_data;

  tf_data.type      = vpiSysTask;
  tf_data.tfname    = "$tb_build";
  tf_data.calltf    = tb_build;
  tf_data.compiletf = 0;
  tf_data.sizetf    = 0;
  vpi_register_systf(&tf_data);
}

