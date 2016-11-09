/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Bit.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   10/18/16
#   Description   :   
#
###############################################################################
*/
#ifndef BIT_H
#define BIT_H

#include <string>

#include "Common.h"
#include "TypeBase.h"

using namespace std;

class Bit : public TypeBase
{
  // Enums
  public:

  // Private Members
  private:

  // Public Properties
  public:

  // Constructors
  public:
    Bit(string iFullName);

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
    Bit & operator=  (UInt32 iRhs);
    Bit & operator=  (UInt64 iRhs);
    Bit & operator=  (const BitVector & iRhs);
    Bit & operator=  (BitVector && iRhs);
    Bit & operator=  (const Bit & iRhs);
    Bit & operator=  (const TypeBase & iRhs);
    Bit & operator=  (const TypeBase::PartSelect & iRhs)  { return *this = (BitVector)iRhs; }
    Bit & operator=  (const BitVector::PartSelect & iRhs) { return *this = (BitVector)iRhs; }
    Bit & operator=  (long long unsigned int iRhs)        { return *this = (UInt64)iRhs; }
    Bit & operator=  (long long int iRhs)                 { return *this = (UInt64)iRhs; }
    Bit & operator=  (Int64 iRhs)                         { return *this = (UInt64)iRhs; }
    Bit & operator=  (int iRhs)                           { return *this = (UInt32)iRhs; }
};

typedef Bit bit;

#endif /* BIT_H */
