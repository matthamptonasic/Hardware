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
  // If key found and value is larger than UInt32, log warning and return max UInt32.
  // If no key, check string and return error.
  bool oFound;
  UInt64 retVal = getCmdArg_UInt64(iName, oFound);
  if(oFound && (retVal > 0xffffffffULL))
  {
    // TBD - log warning that value will be truncated.
  }
  return (UInt32)retVal;
}
UInt64 TestController::GetCmdArg_UInt64(string iName)
{
  bool l_found;
  return getCmdArg_UInt64(iName, l_found);
}

Int32  TestController::GetCmdArg_Int32(string iName)
{
  // Check in Int64 map.
  // If key found and value is larger than Int32, log warning and return max Int32.
  // If no key, check string and return error.
  bool oFound;
  UInt64 retVal = getCmdArg_Int64(iName, oFound);
  if((oFound && (retVal > 2147483647LL)) || (oFound && (retVal < -2147483648LL)))
  {
    // TBD - log warning that value will be truncated.
  }
  return (Int32)retVal;
}
Int64  TestController::GetCmdArg_Int64(string iName)
{
  bool l_found = false;
  return getCmdArg_Int64(iName, l_found);
}
string TestController::GetCmdArg_string(string iName)
{
  // Check string and return error if it doesn't exist.
  bool l_found = false;
  string l_retVal = getCmdArg_string(iName, l_found);
  if(!l_found)
  {
    // TBD - log error.
  }
  return l_retVal;
}

void TestController::RegisterTest(TestBase * iTest)
{
  m_testDb.AddTest(iTest);
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

UInt64 TestController::getCmdArg_UInt64(string iName, bool & oFound)
{
  // Check in UInt64 map.
  // If no key, check string and return error.
  map<string, UInt64>::iterator l_itr;
  l_itr = m_nameToUInt64Map->find(iName);
  if(l_itr != m_nameToUInt64Map->end())
  {
    oFound = true;
    return (*m_nameToUInt64Map)[iName];
  }
  oFound = false;
  bool l_strFound = false;
  string l_retVal = getCmdArg_string(iName, l_strFound);
  if(l_strFound)
  {
    // TBD - log error and show string name/value.
    return 0;
  }
  // TBD - log error.
  return 0;
}

Int64 TestController::getCmdArg_Int64(string iName, bool & oFound)
{
  // Check in Int64 map.
  // If no key, check string and return error.
  map<string, Int64>::iterator l_itr;
  l_itr = m_nameToInt64Map->find(iName);
  if(l_itr != m_nameToInt64Map->end())
  {
    oFound = true;
    return (*m_nameToInt64Map)[iName];
  }
  oFound = false;
  bool l_strFound = false;
  string l_retVal = getCmdArg_string(iName, l_strFound);
  if(l_strFound)
  {
    // TBD - log error and show string name/value.
    return 0;
  }
  // TBD - log error.
  return 0;
}

string TestController::getCmdArg_string(string iName, bool & oFound)
{
  oFound = false;
  // Check string map.
  map<string, string>::iterator l_itr2;
  l_itr2 = m_nameToStringMap->find(iName);
  if(l_itr2 != m_nameToStringMap->end())
  {
    oFound = true;
    return (*m_nameToStringMap)[iName];
  }
  return "";
}

// =============================
// ===**     Operators     **===
// =============================
