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

#include <string>

#include "Common.h"
#include "Parsing.h"

#include "vpi.h"

using namespace std;

namespace Text { namespace Parsing {

string Replace(const string & iText, const string & iPattern, const string & iReplacement)
{
  string l_retVal = string(iText);
  if(iPattern.size() > iText.size())
  {
    return l_retVal;
  }
  UInt32 l_pos = l_retVal.find(iPattern);
  if(l_pos != string::npos)
  {
    l_retVal = l_retVal.replace(l_pos, iPattern.size(), iReplacement);
  }
  return l_retVal;
}

bool StartsWith(const string & iText, const string & iPattern)
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

}}
