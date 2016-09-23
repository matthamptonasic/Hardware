/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   BitVector.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/15/16
#   Description   :   A class to contain an n-bit vector as well as 
#                     (optionally) mirroring/updating an equivalent wire/reg
#                     in the Verilog hierarchy.
#
#                     Note on composition vs inheritance:
#                       Normally we would create a BitVector class that would
#                       not have any of the Verilog transport (PLI), then 
#                       simply inherit from BitVector another class that would
#                       have the Verilog mirror logic.
#                       The problem I see is that, the child class would have
#                       to override each of the BitVector methods in order to
#                       add the Verilog transport functionality. That's not a
#                       big deal. However, if someone adds a function or 
#                       operator overload to BitVector and forgets to add the
#                       same override in the child class, the child class will
#                       still appear to work in the C-world, but fail to
#                       reflect in the Verilog world. This could be very time
#                       consuming and frustrating.
#                       Instead, we're using composition so that a containing
#                       class 'has a' BitVector. The same operator overload and 
#                       access functions still must be implemented in the 
#                       containing class, but the issue stated above will not 
#                       happen. If the operator/function is not implemented
#                       in the containing class, it will result in a 
#                       compile error, not a runtime bug.
#
###############################################################################
*/
#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <string>
#include <vector>

#include "Common.h"

using namespace std;

class BitVector {
  // Enums
  public:
  enum class NB_STATES : Byte
  {
    TWO_STATE = 0,
    FOUR_STATE = 1
  };

  // Static Members
  private:
    static NB_STATES g_nbStates;

  // Private Members
  private:
    UInt32 m_size;
    UInt32 m_mask;
    string m_name;
    vector<UInt32> * m_aval;
    vector<UInt32> * m_bval;
    NB_STATES m_nbStates;

  // Constructors
  public:
    BitVector(string iName, UInt32 iSize);

  // Inits
  private:
    void init(string iName, UInt32 iSize, NB_STATES iStates);

  // Public Methods
  public:
    UInt32 GetUInt32();
    UInt64 GetUInt64();

  // Private Methods
  private:

  // Operators
  public:
  BitVector & operator= (Int32 iRhs);


};

#endif /* BITVECTOR_H */

