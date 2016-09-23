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

  // ===== Part Select Class =====
  // Why?
  // Example #1:
  // BitVector bvA = new BitVector("bvA", 32);
  // bvA[15,0] = 0xabcd;
  //
  // When you pass 0xabcd to the = operator, it will return a reference to a BV.
  // On the left side, we've passed 2 Int32's to the [] operator.
  // This will return a reference to a BV as well.
  // However, this reference is not to bvA because bvA is 32 bits wide and this
  // BV object is 16 bits wide.
  // The bit selected BV is related to bvA, but they are not the same.
  // The PartSelect will be a nested class within BitVector that refers to 
  // the outer class.
  // When the above statement happens, the child class (PartSelect) that
  // returns from the [] operator will be assigned a value of 0xabcd.
  // The part select class overloads the = operator which assigns the value 
  // 0xabcd to the outer BV class, bits 15,0.
  //
  // Example #2:
  // BitVector bvA = new BitVector("bvA", 32);
  // bvA[15,0] = 0xabcd;
  // bvA[31,16] = bvA[15,0];
  // 
  // As above, the first step creates a temporary PartSelect object that is
  // returned from bvA[15,0]. This temporary object resides on the stack
  // since there is no 'new' statement anywhere. Once the function returns,
  // the object will be gone. Additionally, it has no name or reference.
  // It cannot be used again. The function of its existance ends when
  // the overloaded assignment operator assigns the corresponding bits
  // in the outer BitVector class.
  // 
  // The second step reduces to the following:
  // PartSelect& (parent=bvA, hi=31, lo=16) = PartSelect& (parent=bvA, hi=15, lo=0)
  // The assignment operator used is now:
  // PartSelect::operator= (PartSelect &)
  // Which will get the selected bits from the argument's outer BV and
  // assign them to the operand's outer BV.
  protected:
  class PartSelect
  {
    // Private Members
    private:
    UInt32 m_upperIndex;
    UInt32 m_lowerIndex;
    BitVector * m_parent;

    // Public Properties (get/set)
    public:

    // Constructors
    public:

    // Inits
    private:

    // Public Methods
    public:

    // Private Methods
    private:

    // Operators
    public:

  };

};

#endif /* BITVECTOR_H */

