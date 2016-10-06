/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Parsing.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/28/16
#   Description   :   
#
###############################################################################
*/

#include "Parsing.h"

#include "vpi.h"

using namespace std;

namespace Text {

bool Parsing::StartsWith(const string & iText, const string & iPattern)
{
  if(iPattern.size() > iText.size())
  {
    return false;
  }
  for(UInt32 ii=0; ii<iPattern.size(); ii++)
  {
    if(iText[ii] != iPattern[ii])
    {
      return false;
    }
  }
  return true;
}

bool Parsing::CheckNumberBase10(const char & iChar)
{
  if((iChar >= '0') && (iChar <= '9'))
  {
    return true;
  }
  return false;
}
bool Parsing::CheckNumberBase16(const char & iChar)
{
  if(((iChar >= '0') && (iChar <= '9')) || ((iChar >= 'A') && (iChar <= 'F')) || ((iChar >= 'a') && (iChar <= 'f')))
  {
    return true;
  }
  return false;
}
bool Parsing::CheckUInt64Range(const string & iNumber)
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
bool Parsing::CheckInt64Range(const string & iNumber, bool iIsNegative)
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

} // namespace Parsing
