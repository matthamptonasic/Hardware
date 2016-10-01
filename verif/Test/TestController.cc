/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   TestController.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/15/16
#   Description   :   
#
###############################################################################
*/

#include "Manip.h"
#include "pli.h"
#include "Parsing.h"
#include "TestController.h"

using namespace std;
using namespace Text;
using namespace Text::Manip;
using namespace Text::Parsing;

// *==*==*==*==*==*==*==*==*==*==*==*==*
// ===**   TestController Class    **===
// *==*==*==*==*==*==*==*==*==*==*==*==*

// ====================================
// ===**  Private Static Members  **===
// ====================================

// =============================
// ===** Public Properties **===
// =============================
vector<string> & TestController::CommandLineArgs_get()
{
  return *m_commandLineArgs;
}

// =============================
// ===**    Constructor    **===
// =============================
TestController::TestController()
{
  init();
}
// =============================
// ===**      Inits        **===
// =============================
void TestController::init()
{
  initMaps();
  commandLineArgs_set();
  findCArgs();
  parseCArgs();
}

void TestController::initMaps()
{
  m_nameToInt64Map = new map<string, Int64>();
  m_nameToUInt64Map = new map<string, UInt64>();
  m_nameToStringMap = new map<string, string>();
  m_nameToPrintHexMap = new map<string, bool>();
}

// =============================
// ===**  Public Methods   **===
// =============================
void TestController::PrintCommandLineArgs()
{
  for(UInt32 ii=0; ii<m_commandLineArgs->size(); ii++)
  {
    // TBD - change to logger.
    Vpi::vpi_printf("Argument[%d] '%s'\n", ii, m_commandLineArgs->at(ii).c_str());
  }
  Vpi::vpi_printf("cArgs = '%s'\n", m_cArgs.c_str());
}
UInt32 TestController::GetCmdArg_UInt32(string iName)
{
  // Check in UInt64 map.
  // If no key, check string and return error.
  // If larger than UInt32, log warning and return max UInt32.
}
UInt64 TestController::GetCmdArg_UInt64(string iName)
{
  // Check in UInt64 map.
  // If no key, check string and return error.
}
Int32  TestController::GetCmdArg_Int32(string iName)
{
  // Check in Int64 map.
  // If no key, check string and return error.
  // If larger than Int32, log warning and return max or min Int32.
}
Int64  TestController::GetCmdArg_Int64(string iName)
{
  // Check in Int64 map.
  // If no key, check string and return error.
}
string TestController::GetCmdArg_string(string iName, bool iLogError)
{
  // Check string and return error if it doesn't exist.
}

// =============================
// ===**  Private Methods  **===
// =============================
void TestController::commandLineArgs_set()
{
  m_commandLineArgs = Pli::GetCommandLineArgs();
}
void TestController::findCArgs()
{
  // Loop through the vector of arguments and find one that starts with "+c_args=".
  m_cArgs = "";
  for(UInt32 ii=0; ii<m_commandLineArgs->size(); ii++)
  {
    if(Parsing::StartsWith(m_commandLineArgs->at(ii), c_cArgsPrefix))
    {
      if(m_cArgs != "")
      {
        m_cArgs += " ";
      }
      m_cArgs += Manip::Replace(m_commandLineArgs->at(ii), c_cArgsPrefix, "");
    }
  }
}
void TestController::parseCArgs()
{
  // Trim spaces.
  m_cArgs = Manip::Trim(m_cArgs);

  // Split by spaces.
  vector<string> l_cArgsIndiv = Manip::Split(m_cArgs, ' '); 

  // Each string is now its own argument.
  // Now we can split at the '=' to get the token and value.
  for(UInt32 ii=0; ii<l_cArgsIndiv.size(); ii++)
  {
    vector<string> l_tokenValue = Manip::Split(l_cArgsIndiv[ii], '=');
    if(l_tokenValue.size() != 2)
    {
      // TBD - log error.
      continue;
    }
    // Parse token and value.
    parseTokenValue(l_tokenValue[0], l_tokenValue[1]);
  }
}

void TestController::parseTokenValue(string & iToken, string & iValue)
{
  Vpi::vpi_printf("parseTokenValue\n");
  iToken = Manip::Trim(iToken, "\"");
  iValue = Manip::Trim(iValue, "\"");
  // Token should now be set, no more changes.
  // Now parse the value to try to figure out what it's type is.

  bool l_signed = false;
  bool l_hex = false;
  bool l_str = false;
  // Check prefix
  UInt32 ii=0;
  if((iValue.size() > 2) && (iValue[0] == '0') && ((iValue[1] == 'x') || (iValue[1] == 'X')))
  {
    l_hex = true;
    ii = 2;
  }
  else if((iValue.size() > 1) && (iValue[0] == '-'))
  {
    l_signed = true;
    ii = 1;
  }

  string l_number = "";
  for(; ii<iValue.size(); ii++)
  {
    if(Parsing::CheckNumberBase10(iValue[ii]) || (l_hex && Parsing::CheckNumberBase16(iValue[ii])))
    {
      l_number += iValue[ii];
    }
    else
    {
      l_str = true;
      break;
    }
    // Check if too large for 64-bit.
    if(l_hex && (l_number.size() > 16))
    {
      // TBD - log warning.
      l_str = true;
      break;
    }
    else if(!l_hex && (!l_signed && (l_number.size() > 20)) || (l_signed && (l_number.size() > 19)))
    {
      // Greather than 19/20 is too big.
      l_str = true;
      break;
    }
    else if(!l_hex && l_signed && (l_number.size() == 19))
    {
      if(!Parsing::CheckInt64Range(l_number, l_signed))
      {
        l_str = true;
        break;
      }
    }
    else if(!l_hex && !l_signed && (l_number.size() == 20))
    {
      if(!Parsing::CheckUInt64Range(l_number))
      {
        l_str = true;
        break;
      }
    }
  }

  string l_fmt = l_hex ? "0x%llx" : "%lld";
  if(l_str)
  {
    (*m_nameToStringMap)[iToken] = iValue;
    Vpi::vpi_printf("T/V pair (String) = '%s', '%s'\n", iToken.c_str(), (*m_nameToStringMap)[iToken].c_str());
    return;
  }
  if(l_signed)
  {
    // Put into Int64
    (*m_nameToInt64Map)[iToken] = stoll(iValue);
    (*m_nameToPrintHexMap)[iToken] = l_hex;
    Vpi::vpi_printf(string("T/V pair (Int64) = '%s', '" + l_fmt + "'\n").c_str(), iToken.c_str(), (*m_nameToInt64Map)[iToken]);
  }
  else
  {
    // Put into UInt64
    (*m_nameToUInt64Map)[iToken] = stoull(iValue, nullptr, l_hex ? 16 : 10);
    (*m_nameToPrintHexMap)[iToken] = l_hex;
    Vpi::vpi_printf(string("T/V pair (UInt64) = '%s', '" + l_fmt + "'\n").c_str(), iToken.c_str(), (*m_nameToUInt64Map)[iToken]);
  }
}

// =============================
// ===**     Operators     **===
// =============================
