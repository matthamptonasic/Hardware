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
  enum class PRINT_FMT : Byte
  {
    DEC = 0,
    HEX = 1
  };

  // Static Members
  private:
    static NB_STATES      g_nbStates;
    static bool           g_useGlobalStates;
    static const string   g_default_name;
    static const UInt32   g_default_size;
    static bool           g_useGlobalPrintSettings;
    static PRINT_FMT      g_printFmt;
    static bool           g_printBasePrefix;    // For hex only.
    static bool           g_printPrependZeros;  // For hex only.
    static bool           g_printHexWordDivider;// Underscore between words.

  // Static Properties
  public:
    static NB_STATES g_NbStates_get() { return g_nbStates; }
    static void      g_NbStates_set(NB_STATES iStates) { g_nbStates = iStates; }
    static bool      g_UseGlobalStates_get() { return g_useGlobalStates; }
    static void      g_UseGlobalStates_set(bool iUseGlobal ) { g_useGlobalStates = iUseGlobal; }
    static bool      g_UseGlobalPrintSettings_get() { return g_useGlobalPrintSettings; }
    static void      g_UseGlobalPrintSettings_set(bool iUseGlobal ) { g_useGlobalPrintSettings = iUseGlobal; }
    static PRINT_FMT g_PrintFmt_get() { return g_printFmt; }
    static void      g_PrintFmt_set(PRINT_FMT iFmt) { g_printFmt = iFmt; }
    static bool      g_PrintBasePrefix_get() { return g_printBasePrefix; }
    static void      g_PrintBasePrefix_set(bool iUsePrefix) { g_printBasePrefix = iUsePrefix; } 
    static bool      g_PrintPrependZeros_get() { return g_printPrependZeros; }
    static void      g_PrintPrependZeros_set(bool iPrependZeros) { g_printPrependZeros = iPrependZeros; }

  // Private Members
  private:
    UInt32 m_size;
    UInt32 m_mask;
    string m_name;
    vector<UInt32> * m_aval;
    vector<UInt32> * m_bval;
    NB_STATES m_nbStates;
    PRINT_FMT m_printFmt;
    bool m_printBasePrefix;
    bool m_printPrependZeros;
    bool m_printHexWordDivider;

  // Public Properties
  public:
    NB_STATES NbStates_get() const { return m_nbStates; }
    void      NbStates_set(const NB_STATES iStates) { m_nbStates = iStates; }
    PRINT_FMT PrintFmt_get() const { return m_printFmt; }
    void      PrintFmt_set(const PRINT_FMT iFmt) { m_printFmt = iFmt; }
    bool      PrintBasePrefix_get() { return m_printBasePrefix; }
    void      PrintBasePrefix_set(bool iUseBase) { m_printBasePrefix = iUseBase; }
    bool      PrintPrependZeros_get() { return m_printPrependZeros; }
    void      PrintPrependZeros_set(bool iPrepend) { m_printPrependZeros = iPrepend; }
    bool      PrintHexWordDivider_get() { return m_printHexWordDivider; }
    void      PrintHexWordDivider_set(bool iUseDivider) { m_printHexWordDivider = iUseDivider; }

  // Constructors
  public:
    BitVector(string iName = g_default_name);
    BitVector(string iName, UInt32 iSize, NB_STATES iStates = g_nbStates);
    BitVector(UInt32 iSize = g_default_size);

  // Inits
  private:
    void init(string iName, UInt32 iSize, NB_STATES iStates);

  // Public Methods
  public:
    void    Resize(UInt32 iNewSize);
    UInt32  GetUInt32() const;
    UInt64  GetUInt64() const;
    string  ToString() const;
    void    Print() const;

  // Private Methods
  private:
    void    checkIndices(UInt32 & iUpperIndex, UInt32 & iLowerIndex);
    UInt32  getWordNb(UInt32 iBitPos);
    Byte    getShift(UInt32 iBitPos);
    UInt32  getMask(UInt32 iUpperIndex, bool iReverse = false);
    void    setMask();
    //Must be 32-bits or less.
    UInt32  getBits(UInt32 iUpperIndex, UInt32 iLowerIndex);
    void    setUInt32(UInt32 iVal);
    void    setUInt64(UInt64 iVal);


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
    PartSelect(BitVector * iBV, UInt32 iUpperIndex, UInt32 iLowerIndex);

    // Inits
    private:
    void init(BitVector * iBV, UInt32 iUpperIndex, UInt32 iLowerIndex);

    // Public Methods
    public:
    UInt32 getUInt32();
    UInt64 getUInt64();

    // Private Methods
    private:
    void setParentBits(const PartSelect & iBits);
    void getParentBits(BitVector & oBV) const;

    // Operators
    public:
    PartSelect & operator= (UInt32 iRhs);
  };

  // BitVector Operators
  public:
  BitVector & operator= (UInt32 iRhs);
  PartSelect operator() (UInt32 iUpperIndex, UInt32 iLowerIndex);

};

#endif /* BITVECTOR_H */

