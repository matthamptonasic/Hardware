/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Manip.cc 
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

namespace Text { namespace Manip {

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

void Split(const string & iText, const char iDelim, vector<string> & oStrings) {
    stringstream l_ss;
    l_ss.str(iText);
    string l_str;
    while (getline(l_ss, l_str, iDelim)) {
        oStrings.push_back(l_str);
    }
}

vector<string> Split(const string & iText, const char iDelim) {
    vector<string> l_retVal;
    Split(iText, iDelim, l_retVal);
    return l_retVal;
}

}}
