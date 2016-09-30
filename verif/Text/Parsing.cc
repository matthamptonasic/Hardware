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

namespace Text { namespace Parsing {

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
