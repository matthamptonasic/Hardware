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

#include "Manip.h"
#include "Parsing.h"

#include "vpi.h"

using namespace std;

namespace Text {

const string Manip::g_DefaultTrimChars = " \t\n\r\"\'";

string Manip::Replace(const string & iText, const string & iPattern, const string & iReplacement)
{
  string l_retVal = string(iText);
  if(iPattern.size() > iText.size())
  {
    return l_retVal;
  }
  Int32 l_pos = l_retVal.find(iPattern);
  if(l_pos != string::npos)
  {
    l_retVal = l_retVal.replace(l_pos, iPattern.size(), iReplacement);
  }
  return l_retVal;
}

void Manip::Split(const string & iText, const char iDelim, vector<string> & oStrings) {
    stringstream l_ss;
    l_ss.str(iText);
    string l_str;
    while (getline(l_ss, l_str, iDelim)) {
        oStrings.push_back(l_str);
    }
}

vector<string> Manip::Split(const string & iText, const char iDelim) {
    vector<string> l_retVal;
    Split(iText, iDelim, l_retVal);
    return l_retVal;
}

string Manip::TrimLeft(const string & iText, string iTrimChars)
{
  Int32 l_firstKeepChar = iText.find_first_not_of(iTrimChars);
  if(l_firstKeepChar == string::npos)
  {
    return ""; // No keep characters, only trim character or empty string.
  }
  else
  {
    return iText.substr(l_firstKeepChar);
  }
}

string Manip::TrimRight(const string & iText, string iTrimChars)
{
  Int32 l_lastKeepChar = iText.find_last_not_of(iTrimChars);
  if(l_lastKeepChar == string::npos)
  {
    return ""; // No keep characters, only trim character or empty string.
  }
  else
  {
    return iText.substr(0, l_lastKeepChar + 1);
  }
}

string Manip::Trim(const string & iText, string iTrimChars)
{
  // TBD - Would be better to have at least 1 modify-in-place methods here.
  return TrimLeft(TrimRight(iText, iTrimChars), iTrimChars);
}

}
