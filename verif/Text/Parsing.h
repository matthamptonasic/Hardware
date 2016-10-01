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

#include <sstream>
#include <string>
#include <vector>

#include "Common.h"

using namespace std;

namespace Text { namespace Parsing {

bool            StartsWith(const string & iText, const string & iPattern);
bool            CheckNumberBase10(const char & iChar);
bool            CheckNumberBase16(const char & iChar);
bool            CheckUInt64Range(const string & iNumber);
bool            CheckInt64Range(const string & iNumber, bool iIsNegative);

}}
