/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   pli.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/21/16
#   Description   :   
#
###############################################################################
*/

#include "pli.h"

// ====================================
// ===**  Private Static Members  **===
// ====================================


// ===============================
// ===**  Public Properties  **===
// ===============================


// ============================
// ===**  Public Methods  **===
// ============================
Vpi::SCALAR_VAL Pli::GetScalar(vpiHandle iHndl)
{
  Vpi::SCALAR_VAL retVal = Vpi::SCALAR_VAL::DONT_CARE;
  
  if(iHndl == NULL)
  {
    // TBD - log error message.
    return retVal;
  }
  Vpi::OBJECT type = GetType(iHndl);
  switch(type) {
    case Vpi::OBJECT::NET:
    case Vpi::OBJECT::NET_ARRAY:
    case Vpi::OBJECT::REG:
    case Vpi::OBJECT::REG_ARRAY:
    case Vpi::OBJECT::PART_SELECT:
    case Vpi::OBJECT::CONSTANT:
    case Vpi::OBJECT::INTEGER_VAR:
    case Vpi::OBJECT::TIME_VAR:
    case Vpi::OBJECT::REAL_VAR:
    case Vpi::OBJECT::FUNCTION:
    case Vpi::OBJECT::SYS_FUNC_CALL:
    case Vpi::OBJECT::MEMORY_WORD:
    case Vpi::OBJECT::VARIABLES:
      vpi_printf("Scalar type is %d.\n", type);
      break;
    default:
    // TBD - log error message.
    // Object type was not a value type.
      return retVal;
  }


  // void vpi_get_value(vpiHandle iHndl, p_vpi_value oValue);
  Vpi::p_vpi_value data = new Vpi::t_vpi_value();
  data->format = Vpi::VALUE_FORMAT::SCALAR;
  Vpi::vpi_get_value(iHndl, data);

  retVal = data->value.scalar;
  delete data;

  return retVal;
}

Vpi::OBJECT Pli::GetType(vpiHandle iHndl)
{
  if(iHndl == NULL)
  {
    return Vpi::OBJECT::UNKNOWN;
  }
  return (Vpi::OBJECT)Vpi::vpi_get(Vpi::PROPERTY::TYPE, iHndl);
}

//
// =============================
// ===**  Private Methods  **===
// =============================

