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
    if(checkNumberBase10(iValue[ii]) || (l_hex && checkNumberBase16(iValue[ii])))
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
      if(!checkInt64Range(l_number, l_signed))
      {
        l_str = true;
        break;
      }
    }
    else if(!l_hex && !l_signed && (l_number.size() == 20))
    {
      if(!checkUInt64Range(l_number))
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

bool TestController::checkNumberBase10(const char & iChar)
{
  if((iChar >= '0') && (iChar <= '9'))
  {
    return true;
  }
  return false;
}
bool TestController::checkNumberBase16(const char & iChar)
{
  if(((iChar >= '0') && (iChar <= '9')) || ((iChar >= 'A') && (iChar <= 'F')) || ((iChar >= 'a') && (iChar <= 'f')))
  {
    return true;
  }
  return false;
}
bool TestController::checkUInt64Range(const string & iNumber)
{
  if(iNumber.size() <= 20)
  {
    return true;
  }
  // Unsigned is 0 to 18,446,744,073,709,551,615
  if((iNumber[0] != '0') && (iNumber[0] != '1'))
  {
    return false;
  }
  try
  {
    string l_lower19 = iNumber.substr(1, 19);
    UInt64 l_val = stoull(l_lower19);
    if(l_val <= 8446744073709551615ULL)
    {
      return true;
    }
    return false;
  }
  catch(...)
  {
    // TBD - log warning.
    return false;
  }
}
bool TestController::checkInt64Range(const string & iNumber, bool iIsNegative)
{
  if(iNumber.size() <= 19)
  {
    return true;
  }
  // Signed range is -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
  try
  {
    UInt64 l_val = stoull(iNumber);
    if((iIsNegative && (l_val <= 9223372036854775808ULL)) || (!iIsNegative && (l_val <= 9223372036854775807ULL)))
    {
      return true;
    }
    return false;
  }
  catch(...)
  {
    // TBD - log warning.
    return false;
  }
}

// =============================
// ===**     Operators     **===
// =============================
