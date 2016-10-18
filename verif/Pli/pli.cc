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

#include <algorithm>

#include "Logger.h"

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
void Pli::DollarFinish()
{
  Vpi::vpi_control((Int32)Vpi::CTL_OPERATION::FINISH);
}
Vpi::SCALAR_VAL Pli::GetScalar(vpiHandle iHndl)
{
  Vpi::SCALAR_VAL retVal = Vpi::SCALAR_VAL::DONT_CARE;
  
  if(iHndl == NULL)
  {
    LOG_ERR_ENV << "vpiHandle was NULL." << endl;
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
    case Vpi::OBJECT::BIT:
      break;
    default:
    LOG_ERR_ENV << "Object type (" << (Int32)type << ") of vpiHandle was not a value type." << endl;
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

UInt32 Pli::GetVector(vpiHandle iHndl)
{
  UInt32 retVal = 0xffffffff;
  if(iHndl == NULL)
  {
    LOG_ERR_ENV << "vpiHandle was NULL." << endl;
    return retVal;
  }

  Int32 vec_size = Vpi::vpi_get(Vpi::PROPERTY::SIZE, iHndl);
  if(vec_size <= 0)
  {
    LOG_ERR_ENV << "Vector size was invalid (" << vec_size << ")." << endl;
    return retVal;
  }

  Vpi::p_vpi_value data = new Vpi::t_vpi_value();
  data->format = Vpi::VALUE_FORMAT::VECTOR;
  Vpi::vpi_get_value(iHndl, data);

  Int32 nbReads = (vec_size - 1) / 32 + 1;

  for(Int32 ii=0; ii<nbReads; ii++)
  {
    Int32 aVal = data->value.vector[ii].aval;
    Int32 bVal = data->value.vector[ii].bval;
    retVal = aVal; 
  }
  delete data;
  return retVal;
}

UInt32 Pli::GetSize(vpiHandle iHndl)
{
  UInt32 retVal = 0;
  if(iHndl == NULL)
  {
    LOG_ERR_ENV << "vpiHandle was NULL." << endl;
    return retVal;
  }

  Int32 vec_size = Vpi::vpi_get(Vpi::PROPERTY::SIZE, iHndl);
  if(vec_size <= 0)
  {
    LOG_ERR_ENV << "Vector size was invalid (" << vec_size << ")." << endl;
    return retVal;
  }
  return (UInt32)vec_size;
}

Vpi::OBJECT Pli::GetType(vpiHandle iHndl)
{
  if(iHndl == NULL)
  {
    return Vpi::OBJECT::UNKNOWN;
  }
  return (Vpi::OBJECT)Vpi::vpi_get(Vpi::PROPERTY::TYPE, iHndl);
}

vector<string> * Pli::GetCommandLineArgs()
{
  Vpi::p_vpi_vlog_info l_info = new Vpi::t_vpi_vlog_info();
  Vpi::vpi_get_vlog_info(l_info);
  UInt32 l_size = l_info->argc;
  vector<string> * retVal = new vector<string>(l_size, "");
  for(UInt32 ii=0; ii<l_size; ii++)
  {
    retVal->at(ii) = string(l_info->argv[ii]);
  }
  mergeStringCLArgs(*retVal);
  return retVal;
}

//
// =============================
// ===**  Private Methods  **===
// =============================
// When the arguments are parsed, the simulator may (e.g. in iVerilog)
// break up the CLI arguments based on spaces and ignore surrounding quotation
// marks. For example:
// +c_args="nbPkts=50 dbgMode=2" will be broken up into 2 different strings.
// #1: +c_args="nbPkts=50
// #2: dbgMode=2"
// This method tapes them back together. It will also shrink the vector accordingly. 
void Pli::mergeStringCLArgs(vector<string> & ioArgs)
{
  UInt32 l_appendIndex = 0;
  bool l_even = true;
  UInt32 l_finalSize = ioArgs.size();

  for(UInt32 ii=0; ii<ioArgs.size(); ii++)
  {
    Int32 l_nbQuotes = std::count(ioArgs[ii].begin(), ioArgs[ii].end(), '\"');
    if(l_even && ((l_nbQuotes % 2) == 0))
    {
      // No action, we're still even.
    }
    else if(l_even && ((l_nbQuotes % 2) == 1))
    {
      // We're not even. This is where we'll append to.
      l_appendIndex = ii;
      l_even = false;
    }
    else
    {
      // We were not even before, append to previous index.
      ioArgs[l_appendIndex] += " " + ioArgs[ii];
      ioArgs[ii] = "";
      l_even = (l_nbQuotes % 2) == 1;
      l_finalSize--;
    }
  }

  // Now collapse the vector down.
  for(UInt32 kk=0; kk<ioArgs.size(); kk++)
  {
    if(ioArgs[kk] == "")
    {
      bool l_allEmpty = true;
      // Scan forward to next non-empty string.
      for(UInt32 mm=kk+1; mm<ioArgs.size(); mm++)
      {
        if(ioArgs[mm] != "")
        {
          ioArgs[kk] = ioArgs[mm];
          ioArgs[mm] = "";
          l_allEmpty = false;
          break;
        }
      }
      if(l_allEmpty)
      {
        break;
      }
    }
  }

  ioArgs.resize(l_finalSize);

  // Count number of quotes. If odd, merge with the following
  // string until the total count is even.
  // If we get to the end with an odd number, report an error.


}
