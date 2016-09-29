/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Parsing.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/28/16
#   Description   :   
#
###############################################################################
*/

#include <string>

#include "Common.h"

using namespace std;

namespace Text { namespace Parsing {

string Replace(const string & iText, const string & iPattern, const string & iReplacement);
bool StartsWith(const string & iText, const string & iPattern);

}}
