/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Manip.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/28/16
#   Description   :   
#
###############################################################################
*/

#include <sstream>
#include <string>
#include <vector>

#include "Common.h"

using namespace std;

namespace Text { namespace Manip {

string          Replace(const string & iText, const string & iPattern, const string & iReplacement);
void            Split(const string & iText, const char iDelim, vector<string> & oStrings);
vector<string>  Split(const string & iText, const char iDelim);

}}
