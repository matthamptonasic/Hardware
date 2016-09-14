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

#include "common.h"
#include <vpi_user.h>

static PLI_INT32 tb_build(char* UNUSED(iUserData));

static void tb_build_register();

void (*vlog_startup_routines[])() = {
  tb_build_register,
  0
};

#endif /* VPI_ENTRY_H */


