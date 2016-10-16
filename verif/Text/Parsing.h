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

namespace Text { 
class Parsing {

  public:
    static bool Contains(const string & iText, const string & iPattern);
    static bool StartsWith(const string & iText, const string & iPattern);
    static bool CheckNumberBase10(const char & iChar);
    static bool CheckNumberBase16(const char & iChar);
    static bool CheckUInt64Range(const string & iNumber);
    static bool CheckInt64Range(const string & iNumber, bool iIsNegative);

};
} // namespace Text
