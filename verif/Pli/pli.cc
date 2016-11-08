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
UInt32 Pli::GetVector(vpiHandle iHndl, UInt32 iWordNb)
{
  UInt32 retVal = 0xffffffff;
  Int32 l_size;
  Vpi::p_vpi_value data = nullptr;
  if(!getVectorData(iHndl, l_size, data))
  {
    return retVal;
  }

  Int32 nbWords = (l_size - 1) / 32 + 1;
  if(iWordNb >= nbWords)
  {
    LOG_ERR_ENV << "Word count is " << nbWords
                << ", word selected is " << iWordNb << " for signal "
                << Vpi::vpi_get_str(Vpi::PROPERTY::NAME, iHndl) << endl;
  }

  retVal = data->value.vector[iWordNb].aval;
  delete data;
  return retVal;
}
void Pli::GetVector(vpiHandle iHndl, vector<UInt32> * oAval, vector<UInt32> * oBval)
{
  Int32 l_size;
  Vpi::p_vpi_value data = nullptr;
  if(!getVectorData(iHndl, l_size, data))
  {
    return;
  }

  Int32 nbWords = (l_size - 1) / 32 + 1;
  if(nbWords > (oAval->size() + 1))
  {
    LOG_ERR_ENV << "Word count is " << nbWords
                << ", aVal vector size is " << oAval->size() << " for signal "
                << Vpi::vpi_get_str(Vpi::PROPERTY::NAME, iHndl) << endl;
    nbWords = oAval->size() + 1;
  }

  ImportVector(data, nbWords, oAval, oBval);
  delete data;
}
void Pli::ImportVector(Vpi::p_vpi_value iData, Int32 iNbWords, vector<UInt32> * oAval, vector<UInt32> * oBval)
{
  for(UInt32 ii=0; ii<iNbWords; ii++)
  {
    oAval->at(ii) = iData->value.vector[ii].aval;
    if(oBval != nullptr)
    {
      oBval->at(ii) = iData->value.vector[ii].bval;
    }
  }
}
void Pli::SetVector(vpiHandle iHndl, vector<UInt32> * iAval, vector<UInt32> * iBval)
{
  Vpi::p_vpi_value l_data = new Vpi::t_vpi_value();
  l_data->format = Vpi::VALUE_FORMAT::VECTOR;
  Int32 l_size = iAval->size();
  l_data->value.vector = new Vpi::t_vpi_vecval[l_size];
  for(UInt32 ii=0; ii<l_size; ii++)
  {
    l_data->value.vector[ii].aval = (*iAval)[ii];
    if(iBval != nullptr)
    {
      l_data->value.vector[ii].bval = (*iBval)[ii];
    }
    else
    {
      l_data->value.vector[ii].bval = 0;
    }
  }

  setVectorData(iHndl, l_data);

  delete[] l_data->value.vector;
  delete l_data;
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

// =============================
// ===**  Private Methods  **===
// =============================
void Pli::mergeStringCLArgs(vector<string> & ioArgs)
{
  // When the arguments are parsed, the simulator may (e.g. in iVerilog)
  // break up the CLI arguments based on spaces and ignore surrounding quotation
  // marks. For example:
  // +c_args="nbPkts=50 dbgMode=2" will be broken up into 2 different strings.
  // #1: +c_args="nbPkts=50
  // #2: dbgMode=2"
  // This method tapes them back together. It will also shrink the vector accordingly. 
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

  // TBD?
  // Count number of quotes. If odd, merge with the following
  // string until the total count is even.
  // If we get to the end with an odd number, report an error.
}
bool Pli::checkInvalidSize(vpiHandle iHndl, Int32 & oSize)
{
  oSize = Vpi::vpi_get(Vpi::PROPERTY::SIZE, iHndl);
  if(oSize <= 0)
  {
    LOG_ERR_ENV << "Vector size was invalid (" << oSize << ")." << endl;
    return false;
  }
  return true;
}
bool Pli::getVectorData(vpiHandle iHndl, Int32 & oSize, Vpi::p_vpi_value & oData)
{
  if(iHndl == NULL)
  {
    LOG_ERR_ENV << "vpiHandle was NULL." << endl;
    return false;
  }
  oSize = 0;
  if(!checkInvalidSize(iHndl, oSize))
  {
    return false;
  }

  oData = new Vpi::t_vpi_value();
  oData->format = Vpi::VALUE_FORMAT::VECTOR;
  Vpi::vpi_get_value(iHndl, oData);
  return true;
}
bool Pli::setVectorData(vpiHandle iHndl, Vpi::p_vpi_value iData)
{
  if(iHndl == NULL)
  {
    LOG_ERR_ENV << "vpiHandle was NULL." << endl;
    return false;
  }
  
  Vpi::vpi_put_value(iHndl, iData, NULL, (Int32)Vpi::DELAY_MODE::NO_DELAY);
  return true;
}
