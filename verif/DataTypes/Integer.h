/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Integer.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   11/09/16
#   Description   :   
#
###############################################################################
*/

#ifndef INTEGER_H
#define INTEGER_H

#include <string>

#include "Common.h"
#include "TypeBase.h"

using namespace std;

class Integer : public TypeBase
{
  // Enums
  public:

  // Private Members
  private:

  // Public Properties
  public:

  // Constructors
  public:
    Integer(string iFullName);

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
    Integer & operator=  (UInt32 iRhs);
    Integer & operator=  (UInt64 iRhs);
    Integer & operator=  (const BitVector & iRhs);
    Integer & operator=  (BitVector && iRhs);
    Integer & operator=  (const Integer & iRhs);
    Integer & operator=  (const TypeBase & iRhs);
    Integer & operator=  (const TypeBase::PartSelect & iRhs)  { return *this = (BitVector)iRhs; }
    Integer & operator=  (const BitVector::PartSelect & iRhs) { return *this = (BitVector)iRhs; }
    Integer & operator=  (long long unsigned int iRhs)        { return *this = (UInt64)iRhs; }
    Integer & operator=  (long long int iRhs)                 { return *this = (UInt64)iRhs; }
    Integer & operator=  (Int64 iRhs)                         { return *this = (UInt64)iRhs; }
    Integer & operator=  (int iRhs)                           { return *this = (UInt32)iRhs; }
};

typedef Integer integer;

#endif /* INTEGER_H */
