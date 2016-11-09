/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Logic.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   11/08/16
#   Description   :   
#
###############################################################################
*/

#ifndef LOGIC_H
#define LOGIC_H

#include <string>

#include "Common.h"
#include "TypeBase.h"

using namespace std;

class Logic : public TypeBase
{
  // Enums
  public:

  // Private Members
  private:

  // Public Properties
  public:

  // Constructors
  public:
    Logic(string iFullName);

  // Inits
  private:

  // Public Methods
  public:

  // Private Methods
  private:

  // Protected Methods
  protected:
    void set_Size();
  
  // Operators
  public:
    Logic & operator=  (UInt32 iRhs);
    Logic & operator=  (UInt64 iRhs);
    Logic & operator=  (const BitVector & iRhs);
    Logic & operator=  (BitVector && iRhs);
    Logic & operator=  (const Logic & iRhs);
    Logic & operator=  (const TypeBase & iRhs);
    Logic & operator=  (const TypeBase::PartSelect & iRhs)  { return *this = (BitVector)iRhs; }
    Logic & operator=  (const BitVector::PartSelect & iRhs) { return *this = (BitVector)iRhs; }
    Logic & operator=  (long long unsigned int iRhs)        { return *this = (UInt64)iRhs; }
    Logic & operator=  (long long int iRhs)                 { return *this = (UInt64)iRhs; }
    Logic & operator=  (Int64 iRhs)                         { return *this = (UInt64)iRhs; }
    Logic & operator=  (int iRhs)                           { return *this = (UInt32)iRhs; }
};

typedef Logic logic;
typedef Logic reg;

#endif /* LOGIC_H */
