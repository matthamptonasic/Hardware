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
#   Description   :   A class to contain an n-bit vector.
#                     The vector may represend a 2- or 4-state value.
#                     (4-state being 0/1/x/z).
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
#include "Logger.h"

using namespace std;

enum class NB_STATES : Byte
{
  TWO_STATE = 0,
  FOUR_STATE = 1
};

class TypeBase;

class BitVector {
  friend class TypeBase;
  friend class Bit;
  friend class Logic;
  friend class Integer;

  // Enums
  public:
  enum class PRINT_FMT : Byte
  {
    DEC = 0,
    HEX = 1
  };

  // Nested Classes
  protected:
  class PartSelect
  {
    friend class BitVector;
    friend class TypeBase;
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

    // Private Members
    private:
    UInt32 m_upperIndex;
    UInt32 m_lowerIndex;
    BitVector * m_parent;

    // Public Properties (get/set)
    public:
    NB_STATES NbStates_get() const { return m_parent->m_nbStates; }
    UInt32 UpperIndex_get() const { return m_upperIndex; };
    UInt32 LowerIndex_get() const { return m_lowerIndex; };

    // Constructors
    public:
    PartSelect(BitVector * iBV, UInt32 iUpperIndex, UInt32 iLowerIndex);

    // Inits
    private:
    void init(BitVector * iBV, UInt32 iUpperIndex, UInt32 iLowerIndex);

    // Public Methods
    public:
    UInt32 getUInt32() const;
    UInt64 getUInt64() const;

    // Private Methods
    private:
    void setParentBits(const PartSelect & iBits);
    void getParentBits(BitVector & oBV, bool iNoResize = false) const;

    // Operators
    public:
    PartSelect & operator= (UInt32 iRhs);
    PartSelect & operator= (UInt64 iRhs);
    // Cannot be const because it's address is used in a new temporary PartSelect.
    // To make it const, we would need to change the implementation of setParentBits
    // such that it takes a const BitVector & instead of a PartSelect (overload method).
    PartSelect & operator= (BitVector & iRhs);
    PartSelect & operator= (BitVector && iRhs);
    PartSelect & operator= (const PartSelect & iRhs);
    PartSelect & operator= (long long unsigned int iRhs)      { return *this = (UInt64)iRhs; }
    PartSelect & operator= (long long int iRhs)               { return *this = (UInt64)iRhs; }
    PartSelect & operator= (Int64 iRhs)                       { return *this = (UInt64)iRhs; }
    PartSelect & operator= (int iRhs)                         { return *this = (UInt32)iRhs; }

    UInt32 operator[] (UInt32 iWordIndex)               const { return         ((BitVector)*this)[iWordIndex]; }
    explicit operator bool()                            const { return   (bool)((BitVector)*this); }
    explicit operator UInt32()                          const { return (UInt32)((BitVector)*this); }
    explicit operator UInt64()                          const { return (UInt64)((BitVector)*this); }
    
    // PartSelect operators that return a BitVector return a new BV that is
    // the selected bits of the parent BV (modified as directed).
    // The size of the returned BV will be the size of the PartSelect.
    // Tests show that this is more efficient in most cases since the BV is being created anyway.
    // As long as there is no complexity before the function returns, RVO eliminates extra copies.
    BitVector operator+= (UInt32 iRhs);
    BitVector operator+= (UInt64 iRhs);
    BitVector operator+= (const BitVector & iRhs);
    BitVector operator+= (const PartSelect & iRhs)            { return *this += (BitVector)iRhs; }
    BitVector operator+= (long long unsigned int iRhs)        { return *this += (UInt64)iRhs; }
    BitVector operator+= (long long int iRhs)                 { return *this += (UInt64)iRhs; }
    BitVector operator+= (Int64 iRhs)                         { return *this += (UInt64)iRhs; }
    BitVector operator+= (int iRhs)                           { return *this += (UInt32)iRhs; }

    BitVector operator+  (UInt32 iRhs)                  const;
    BitVector operator+  (UInt64 iRhs)                  const;
    BitVector operator+  (const BitVector & iRhs)       const { return iRhs + *this; }; // BV implements.
    BitVector operator+  (const PartSelect & iRhs)      const { return *this + (BitVector)iRhs; }
    BitVector operator+  (long long unsigned int iRhs)  const { return *this + (UInt64)iRhs; }
    BitVector operator+  (long long int iRhs)           const { return *this + (UInt64)iRhs; }
    BitVector operator+  (Int64 iRhs)                   const { return *this + (UInt64)iRhs; }
    BitVector operator+  (int iRhs)                     const { return *this + (UInt32)iRhs; }

    BitVector operator++ ()                                   { return *this += 1; }
    BitVector operator++ (int iDummy)                         { return *this += 1; }

    BitVector operator-= (UInt32 iRhs);
    BitVector operator-= (UInt64 iRhs);
    BitVector operator-= (const BitVector & iRhs);
    BitVector operator-= (const PartSelect & iRhs)            { return *this -= (BitVector)iRhs; }
    BitVector operator-= (long long unsigned int iRhs)        { return *this -= (UInt64)iRhs; }
    BitVector operator-= (long long int iRhs)                 { return *this -= (UInt64)iRhs; }
    BitVector operator-= (Int64 iRhs)                         { return *this -= (UInt64)iRhs; }
    BitVector operator-= (int iRhs)                           { return *this -= (UInt32)iRhs; }

    BitVector operator-  (UInt32 iRhs)                  const { BitVector l_bv(*this); l_bv -= iRhs; return l_bv; }
    BitVector operator-  (UInt64 iRhs)                  const { BitVector l_bv(*this); l_bv -= iRhs; return l_bv; }
    BitVector operator-  (const BitVector & iRhs)       const { BitVector l_bv(*this); l_bv -= iRhs; return l_bv; }
    BitVector operator-  (const PartSelect & iRhs)      const { return *this - (BitVector)iRhs; }
    BitVector operator-  (long long unsigned int iRhs)  const { return *this - (UInt64)iRhs; }
    BitVector operator-  (long long int iRhs)           const { return *this - (UInt64)iRhs; }
    BitVector operator-  (Int64 iRhs)                   const { return *this - (UInt64)iRhs; }
    BitVector operator-  (int iRhs)                     const { return *this - (UInt32)iRhs; }

    BitVector operator-- ()                                   { return *this -= 1; }
    BitVector operator-- (int iDummy)                         { return *this -= 1; }

    bool operator== (UInt32 iRhs)                       const { BitVector l_bv(*this); return l_bv == iRhs; }
    bool operator== (UInt64 iRhs)                       const { BitVector l_bv(*this); return l_bv == iRhs; }
    bool operator== (const BitVector & iRhs)            const { return iRhs == *this; }
    bool operator== (const PartSelect & iRhs)           const { return *this == (BitVector)iRhs; }
    bool operator== (long long unsigned int iRhs)       const { return *this == (UInt64)iRhs; }
    bool operator== (long long int iRhs)                const { return *this == (UInt64)iRhs; }
    bool operator== (Int64 iRhs)                        const { return *this == (UInt64)iRhs; }
    bool operator== (int iRhs)                          const { return *this == (UInt32)iRhs; }

    bool operator!= (UInt32 iRhs)                       const { return !(*this == iRhs); }
    bool operator!= (UInt64 iRhs)                       const { return !(*this == iRhs); }
    bool operator!= (const BitVector & iRhs)            const { return  (iRhs  != *this); }
    bool operator!= (const PartSelect & iRhs)           const { return !(*this == iRhs); }
    bool operator!= (long long unsigned int iRhs)       const { return  (*this != (UInt64)iRhs); }
    bool operator!= (long long int iRhs)                const { return  (*this != (UInt64)iRhs); }
    bool operator!= (Int64 iRhs)                        const { return  (*this != (UInt64)iRhs); }
    bool operator!= (int iRhs)                          const { return  (*this != (UInt32)iRhs); }

    bool operator<= (UInt32 iRhs)                       const { BitVector l_bv(*this); return l_bv <= iRhs; }
    bool operator<= (UInt64 iRhs)                       const { BitVector l_bv(*this); return l_bv <= iRhs; }
    bool operator<= (const BitVector & iRhs)            const { BitVector l_bv(*this); return l_bv <= iRhs; }
    bool operator<= (const PartSelect & iRhs)           const { return  (*this <= (BitVector)iRhs); }
    bool operator<= (long long unsigned int iRhs)       const { return  (*this <= (UInt64)iRhs); }
    bool operator<= (long long int iRhs)                const { return  (*this <= (UInt64)iRhs); }
    bool operator<= (Int64 iRhs)                        const { return  (*this <= (UInt64)iRhs); }
    bool operator<= (int iRhs)                          const { return  (*this <= (UInt32)iRhs); }

    bool operator>= (UInt32 iRhs)                       const { BitVector l_bv(*this); return l_bv >= iRhs; }
    bool operator>= (UInt64 iRhs)                       const { BitVector l_bv(*this); return l_bv >= iRhs; }
    bool operator>= (const BitVector & iRhs)            const { BitVector l_bv(*this); return l_bv >= iRhs; }
    bool operator>= (const PartSelect & iRhs)           const { return  (*this >= (BitVector)iRhs); }
    bool operator>= (long long unsigned int iRhs)       const { return  (*this >= (UInt64)iRhs); }
    bool operator>= (long long int iRhs)                const { return  (*this >= (UInt64)iRhs); }
    bool operator>= (Int64 iRhs)                        const { return  (*this >= (UInt64)iRhs); }
    bool operator>= (int iRhs)                          const { return  (*this >= (UInt32)iRhs); }

    bool operator<  (UInt32 iRhs)                       const { BitVector l_bv(*this); return l_bv < iRhs; }
    bool operator<  (UInt64 iRhs)                       const { BitVector l_bv(*this); return l_bv < iRhs; }
    bool operator<  (const BitVector & iRhs)            const { BitVector l_bv(*this); return l_bv < iRhs; }
    bool operator<  (const PartSelect & iRhs)           const { return  (*this < (BitVector)iRhs); }
    bool operator<  (long long unsigned int iRhs)       const { return  (*this < (UInt64)iRhs); }
    bool operator<  (long long int iRhs)                const { return  (*this < (UInt64)iRhs); }
    bool operator<  (Int64 iRhs)                        const { return  (*this < (UInt64)iRhs); }
    bool operator<  (int iRhs)                          const { return  (*this < (UInt32)iRhs); }

    bool operator>  (UInt32 iRhs)                       const { BitVector l_bv(*this); return l_bv > iRhs; }
    bool operator>  (UInt64 iRhs)                       const { BitVector l_bv(*this); return l_bv > iRhs; }
    bool operator>  (const BitVector & iRhs)            const { BitVector l_bv(*this); return l_bv > iRhs; }
    bool operator>  (const PartSelect & iRhs)           const { return  (*this > (BitVector)iRhs); }
    bool operator>  (long long unsigned int iRhs)       const { return  (*this > (UInt64)iRhs); }
    bool operator>  (long long int iRhs)                const { return  (*this > (UInt64)iRhs); }
    bool operator>  (Int64 iRhs)                        const { return  (*this > (UInt64)iRhs); }
    bool operator>  (int iRhs)                          const { return  (*this > (UInt32)iRhs); }

    BitVector operator<<= (UInt32 iRhs);
    BitVector operator<<= (const BitVector & iRhs)            { return  *this <<= iRhs[0]; }
    BitVector operator<<= (const PartSelect & iRhs)           { return  *this <<= ((BitVector)iRhs)[0]; }
    BitVector operator<<= (int iRhs)                          { return  *this <<= (UInt32)iRhs; }

    BitVector operator>>= (UInt32 iRhs);
    BitVector operator>>= (const BitVector & iRhs)            { return  *this >>= iRhs[0]; }
    BitVector operator>>= (const PartSelect & iRhs)           { return  *this >>= ((BitVector)iRhs)[0]; }
    BitVector operator>>= (int iRhs)                          { return  *this >>= (UInt32)iRhs; }

    BitVector operator<< (UInt32 iRhs)                  const { BitVector l_bv(*this); return l_bv << iRhs; }
    BitVector operator<< (const BitVector & iRhs)       const { return  *this << iRhs[0]; }
    BitVector operator<< (const PartSelect & iRhs)      const { return  *this << ((BitVector)iRhs)[0]; }
    BitVector operator<< (int iRhs)                     const { return  *this << (UInt32)iRhs; }

    BitVector operator>> (UInt32 iRhs)                  const { BitVector l_bv(*this); return l_bv >> iRhs; }
    BitVector operator>> (const BitVector & iRhs)       const { return  *this >> iRhs[0]; }
    BitVector operator>> (const PartSelect & iRhs)      const { return  *this >> ((BitVector)iRhs)[0]; }
    BitVector operator>> (int iRhs)                     const { return  *this >> (UInt32)iRhs; }

    BitVector operator~  ()                             const { BitVector l_bv(*this); return ~l_bv; }

    BitVector operator&= (UInt32 iRhs);
    BitVector operator&= (UInt64 iRhs);
    BitVector operator&= (const BitVector & iRhs);
    BitVector operator&= (const PartSelect & iRhs)            { return  *this &= ((BitVector)iRhs); }
    BitVector operator&= (long long unsigned int iRhs)        { return  *this &= (UInt64)iRhs; }
    BitVector operator&= (long long int iRhs)                 { return  *this &= (UInt64)iRhs; }
    BitVector operator&= (Int64 iRhs)                         { return  *this &= (UInt64)iRhs; }
    BitVector operator&= (int iRhs)                           { return  *this &= (UInt32)iRhs; }

    BitVector operator&  (UInt32 iRhs)                  const { BitVector l_bv(*this); return l_bv & iRhs; }
    BitVector operator&  (UInt64 iRhs)                  const { BitVector l_bv(*this); return l_bv & iRhs; }
    BitVector operator&  (const BitVector & iRhs)       const { BitVector l_bv(*this); return l_bv & iRhs; }
    BitVector operator&  (const PartSelect & iRhs)      const { return  *this & ((BitVector)iRhs); }
    BitVector operator&  (long long unsigned int iRhs)  const { return  *this & (UInt64)iRhs; }
    BitVector operator&  (long long int iRhs)           const { return  *this & (UInt64)iRhs; }
    BitVector operator&  (Int64 iRhs)                   const { return  *this & (UInt64)iRhs; }
    BitVector operator&  (int iRhs)                     const { return  *this & (UInt32)iRhs; }

    BitVector operator|= (UInt32 iRhs);
    BitVector operator|= (UInt64 iRhs);
    BitVector operator|= (const BitVector & iRhs);
    BitVector operator|= (const PartSelect & iRhs)            { return  *this |= ((BitVector)iRhs); }
    BitVector operator|= (long long unsigned int iRhs)        { return  *this |= (UInt64)iRhs; }
    BitVector operator|= (long long int iRhs)                 { return  *this |= (UInt64)iRhs; }
    BitVector operator|= (Int64 iRhs)                         { return  *this |= (UInt64)iRhs; }
    BitVector operator|= (int iRhs)                           { return  *this |= (UInt32)iRhs; }

    BitVector operator|  (UInt32 iRhs)                  const { BitVector l_bv(*this); return l_bv | iRhs; }
    BitVector operator|  (UInt64 iRhs)                  const { BitVector l_bv(*this); return l_bv | iRhs; }
    BitVector operator|  (const BitVector & iRhs)       const { BitVector l_bv(*this); return l_bv | iRhs; }
    BitVector operator|  (const PartSelect & iRhs)      const { return  *this | ((BitVector)iRhs); }
    BitVector operator|  (long long unsigned int iRhs)  const { return  *this | (UInt64)iRhs; }
    BitVector operator|  (long long int iRhs)           const { return  *this | (UInt64)iRhs; }
    BitVector operator|  (Int64 iRhs)                   const { return  *this | (UInt64)iRhs; }
    BitVector operator|  (int iRhs)                     const { return  *this | (UInt32)iRhs; }

    BitVector operator^= (UInt32 iRhs);
    BitVector operator^= (UInt64 iRhs);
    BitVector operator^= (const BitVector & iRhs);
    BitVector operator^= (const PartSelect & iRhs)            { return  *this ^= ((BitVector)iRhs); }
    BitVector operator^= (long long unsigned int iRhs)        { return  *this ^= (UInt64)iRhs; }
    BitVector operator^= (long long int iRhs)                 { return  *this ^= (UInt64)iRhs; }
    BitVector operator^= (Int64 iRhs)                         { return  *this ^= (UInt64)iRhs; }
    BitVector operator^= (int iRhs)                           { return  *this ^= (UInt32)iRhs; }

    BitVector operator^  (UInt32 iRhs)                  const { BitVector l_bv(*this); return l_bv ^ iRhs; }
    BitVector operator^  (UInt64 iRhs)                  const { BitVector l_bv(*this); return l_bv ^ iRhs; }
    BitVector operator^  (const BitVector & iRhs)       const { BitVector l_bv(*this); return l_bv ^ iRhs; }
    BitVector operator^  (const PartSelect & iRhs)      const { return  *this ^ ((BitVector)iRhs); }
    BitVector operator^  (long long unsigned int iRhs)  const { return  *this ^ (UInt64)iRhs; }
    BitVector operator^  (long long int iRhs)           const { return  *this ^ (UInt64)iRhs; }
    BitVector operator^  (Int64 iRhs)                   const { return  *this ^ (UInt64)iRhs; }
    BitVector operator^  (int iRhs)                     const { return  *this ^ (UInt32)iRhs; }

    BitVector operator,  (UInt32 iRhs)                  const { BitVector l_bv(*this); return (l_bv, iRhs); }
    BitVector operator,  (UInt64 iRhs)                  const { BitVector l_bv(*this); return (l_bv, iRhs); }
    BitVector operator,  (const BitVector & iRhs)       const { BitVector l_bv(*this); return (l_bv, iRhs); }
    BitVector operator,  (const PartSelect & iRhs)      const { return  (*this , ((BitVector)iRhs)); }
    BitVector operator,  (long long unsigned int iRhs)  const { return  (*this , (UInt64)iRhs); }
    BitVector operator,  (long long int iRhs)           const { return  (*this , (UInt64)iRhs); }
    BitVector operator,  (Int64 iRhs)                   const { return  (*this , (UInt64)iRhs); }
    BitVector operator,  (int iRhs)                     const { return  (*this , (UInt32)iRhs); }

  };

  // Static Members
  private:
    static NB_STATES      s_nbStates;
    static bool           s_useGlobalStates;
    static const string   s_default_name;
    static const UInt32   s_default_size;
    static bool           s_useGlobalPrintSettings;
    static PRINT_FMT      s_printFmt;
    static bool           s_printBasePrefix;    // For hex only.
    static bool           s_printPrependZeros;  // For hex only.
    static bool           s_printFullWord;      // Always prints 4 bytes even if they're 0.
    static bool           s_printHexWordDivider;// Underscore between words.
    static bool           s_printDecCommas;     // 1000's comma seperator.

  // Static Properties
  public:
    static NB_STATES s_NbStates_get() { return s_nbStates; }
    static void      s_NbStates_set(NB_STATES iStates) { s_nbStates = iStates; }
    static bool      s_UseGlobalStates_get() { return s_useGlobalStates; }
    static void      s_UseGlobalStates_set(bool iUseGlobal ) { s_useGlobalStates = iUseGlobal; }
    static bool      s_UseGlobalPrintSettings_get() { return s_useGlobalPrintSettings; }
    static void      s_UseGlobalPrintSettings_set(bool iUseGlobal ) { s_useGlobalPrintSettings = iUseGlobal; }
    static PRINT_FMT s_PrintFmt_get() { return s_printFmt; }
    static void      s_PrintFmt_set(PRINT_FMT iFmt) { s_printFmt = iFmt; }
    static bool      s_PrintBasePrefix_get() { return s_printBasePrefix; }
    static void      s_PrintBasePrefix_set(bool iUsePrefix) { s_printBasePrefix = iUsePrefix; } 
    static bool      s_PrintPrependZeros_get() { return s_printPrependZeros; }
    static void      s_PrintPrependZeros_set(bool iPrependZeros) { s_printPrependZeros = iPrependZeros; }
    static bool      s_PrintFullWord_get() { return s_printFullWord; }
    static void      s_PrintFullWord_set(bool iPrintFull) { s_printFullWord = iPrintFull; }

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
    bool m_printFullWord;
    bool m_printHexWordDivider;
    bool m_printDecCommas;

  // Public Properties
  public:
    NB_STATES NbStates_get() const { return m_nbStates; }
    void      NbStates_set(const NB_STATES iStates) { m_nbStates = iStates; }
    UInt32    Size_get() const { return m_size; };
    PRINT_FMT PrintFmt_get() const { return m_printFmt; }
    void      PrintFmt_set(const PRINT_FMT iFmt) { m_printFmt = iFmt; }
    bool      PrintBasePrefix_get() { return m_printBasePrefix; }
    void      PrintBasePrefix_set(bool iUseBase) { m_printBasePrefix = iUseBase; }
    bool      PrintPrependZeros_get() { return m_printPrependZeros; }
    void      PrintPrependZeros_set(bool iPrepend) { m_printPrependZeros = iPrepend; }
    bool      PrintFullWord_get() { return m_printFullWord; }
    void      PrintFullWord_set(bool iPrintFull) { m_printFullWord = iPrintFull; }
    bool      PrintHexWordDivider_get() { return m_printHexWordDivider; }
    void      PrintHexWordDivider_set(bool iUseDivider) { m_printHexWordDivider = iUseDivider; }
    bool      PrintDecCommas_get() { return m_printDecCommas; }
    void      PrintDecCommas_set(bool iUseDivider) { m_printDecCommas = iUseDivider; }

  // Constructors
  public:
    BitVector(string iName = s_default_name);
    BitVector(string iName, UInt32 iSize, NB_STATES iStates = s_nbStates);
    BitVector(UInt32 iSize = s_default_size);
    BitVector(const BitVector & iSource);
    BitVector(const BitVector & iSource, UInt32 iSize);
    BitVector(BitVector && iSource);
    explicit BitVector(const PartSelect & iSource);
    BitVector(const PartSelect & iSource, UInt32 iSize);
    ~BitVector();

  // Inits
  private:
    void init(string iName, UInt32 iSize, NB_STATES iStates);
    void initCopy(const BitVector & iSource, UInt32 iSize);
    void initMove(BitVector & iSource, UInt32 iSize);
    void initFields(const BitVector & iSource, UInt32 iSize);

  // Public Methods
  public:
    void    Resize(UInt32 iNewSize);
    UInt32  GetUInt32() const;
    UInt32  GetUInt32(UInt32 iWordNb) const;
    UInt64  GetUInt64() const;
    UInt64  GetUInt64(UInt32 iLowerWordNb) const;
    string  GetName() const { return m_name; }
    string  ToString() const;
    void    Print() const;

  // Private Methods
  private:
    void    checkIndices(UInt32 & iUpperIndex, UInt32 & iLowerIndex) const;
    UInt32  getWordNb(UInt32 iBitPos) const;
    Byte    getShift(UInt32 iBitPos) const;
    UInt32  getMask(UInt32 iUpperIndex, bool iReverse = false) const;
    void    setMask();
    void    applyMask();
    //Must be 32-bits or less.
    UInt32  getBits(UInt32 iUpperIndex, UInt32 iLowerIndex) const;
    void    setUInt32(UInt32 iVal);
    void    setUInt64(UInt64 iVal);
    void    add     (UInt32 iVal, UInt32 iWordNb);
    void    subtract(UInt32 iVal, UInt32 iWordNb);
    bool    equals  (UInt32 iVal, UInt32 iWordNb) const;
    bool    ltet    (UInt32 iVal, UInt32 iWordNb, bool & oEqual) const;
    bool    lt      (UInt32 iVal, UInt32 iWordNb, bool & oEqual) const;
    bool    gtet    (UInt32 iVal, UInt32 iWordNb, bool & oEqual) const;
    bool    gt      (UInt32 iVal, UInt32 iWordNb, bool & oEqual) const;
    bool    allZero (UInt32 iLowerWordNb = 0) const;
    bool    bitSet(UInt32 iIndex) const;
    vector<Byte>   add(const vector<Byte> * iArr0, const vector<Byte> * iArr1) const;
    vector<Byte> * getTwoToN(UInt32 iN) const;
    string  toDecimalString() const;

  // BitVector Operators
  // Generally speaking, the operator overloads are organized as such (in this order):
  // 1. BitVector class methods when LHS is a BitVector and RHS is * (or unary operators).
  // 2. PartSelect class methods when LHS is a PartSelect and RHS is *.
  //    Often, these will simply use the LHS PartSelect as a BitVector and call the corresponding 
  //    BitVector class method.
  //    Or, if the operator is commititive (e.g. +, ||, etc), the method will reverse the order
  //    of the (PartSelect iRHS, BitVector iLHS) which again calls the BitVector class method. 
  // 3. Write global methods for reverse order arguments (if it makes sense).
  public:
  BitVector & operator=  (UInt32 iRhs);
  BitVector & operator=  (UInt64 iRhs);
  BitVector & operator=  (const BitVector & iRhs);
  BitVector & operator=  (const PartSelect & iRhs)            { return *this = (BitVector)iRhs; }
  BitVector & operator=  (BitVector && iRhs); // Moves ONLY if the sizes are the same.
  BitVector & operator=  (long long unsigned int iRhs)        { return *this = (UInt64)iRhs; }
  BitVector & operator=  (long long int iRhs)                 { return *this = (UInt64)iRhs; }
  BitVector & operator=  (Int64 iRhs)                         { return *this = (UInt64)iRhs; }
  BitVector & operator=  (int iRhs)                           { return *this = (UInt32)iRhs; }

  PartSelect operator() (UInt32 iUpperIndex, UInt32 iLowerIndex) { PartSelect l_retVal(this, iUpperIndex, iLowerIndex); return l_retVal; }
  UInt32 operator[] (UInt32 iWordIndex) const;
  explicit operator bool() const;
  explicit operator UInt32() const;
  explicit operator UInt64() const;

  BitVector & operator+= (UInt32 iRhs)                        { add(iRhs, 0); return *this; };
  BitVector & operator+= (UInt64 iRhs);
  BitVector & operator+= (const BitVector & iRhs);
  BitVector & operator+= (const PartSelect & iRhs)            { return *this += (BitVector)iRhs; }
  BitVector & operator+= (long long unsigned int iRhs)        { return *this += (UInt64)iRhs; }
  BitVector & operator+= (long long int iRhs)                 { return *this += (UInt64)iRhs; }
  BitVector & operator+= (Int64 iRhs)                         { return *this += (UInt64)iRhs; }
  BitVector & operator+= (int iRhs)                           { return *this += (UInt32)iRhs; }

  BitVector   operator+  (UInt32 iRhs)                  const;
  BitVector   operator+  (UInt64 iRhs)                  const;
  BitVector   operator+  (const BitVector & iRhs)       const;
  BitVector   operator+  (const PartSelect & iRhs)      const { return *this + (BitVector)iRhs; }
  BitVector   operator+  (long long unsigned int iRhs)  const { return *this + (UInt64)iRhs; }
  BitVector   operator+  (long long int iRhs)           const { return *this + (UInt64)iRhs; }
  BitVector   operator+  (Int64 iRhs)                   const { return *this + (UInt64)iRhs; }
  BitVector   operator+  (int iRhs)                     const { return *this + (UInt32)iRhs; }

  BitVector & operator++ ()                                   { return *this += 1; }
  BitVector & operator++ (int iDummy)                         { return *this += 1; }

  BitVector & operator-= (UInt32 iRhs)                        { subtract(iRhs, 0); return *this; }
  BitVector & operator-= (UInt64 iRhs);
  BitVector & operator-= (const BitVector & iRhs);
  BitVector & operator-= (const PartSelect & iRhs)            { return *this -= (BitVector)iRhs; }
  BitVector & operator-= (long long unsigned int iRhs)        { return *this -= (UInt64)iRhs; }
  BitVector & operator-= (long long int iRhs)                 { return *this -= (UInt64)iRhs; }
  BitVector & operator-= (Int64 iRhs)                         { return *this -= (UInt64)iRhs; }
  BitVector & operator-= (int iRhs)                           { return *this -= (UInt32)iRhs; }

  BitVector   operator-  (UInt32 iRhs)                  const;
  BitVector   operator-  (UInt64 iRhs)                  const;
  BitVector   operator-  (const BitVector & iRhs)       const;
  BitVector   operator-  (const PartSelect & iRhs)      const { return *this - (BitVector)iRhs; }
  BitVector   operator-  (long long unsigned int iRhs)  const { return *this - (UInt64)iRhs; }
  BitVector   operator-  (long long int iRhs)           const { return *this - (UInt64)iRhs; }
  BitVector   operator-  (Int64 iRhs)                   const { return *this - (UInt64)iRhs; }
  BitVector   operator-  (int iRhs)                     const { return *this - (UInt32)iRhs; }

  BitVector & operator-- ()                                   { return *this -= 1; }
  BitVector & operator-- (int iDummy)                         { return *this -= 1; }

  bool operator== (UInt32 iRhs)                         const;
  bool operator== (UInt64 iRhs)                         const;
  bool operator== (const BitVector & iRhs)              const;
  bool operator== (const PartSelect & iRhs)             const { return *this == (BitVector)iRhs; }
  bool operator== (long long unsigned int iRhs)         const { return *this == (UInt64)iRhs; }
  bool operator== (long long int iRhs)                  const { return *this == (UInt64)iRhs; }
  bool operator== (Int64 iRhs)                          const { return *this == (UInt64)iRhs; }
  bool operator== (int iRhs)                            const { return *this == (UInt32)iRhs; }

  bool operator!= (UInt32 iRhs)                         const { return !(*this == iRhs); }
  bool operator!= (UInt64 iRhs)                         const { return !(*this == iRhs); }
  bool operator!= (const BitVector & iRhs)              const { return !(*this == iRhs); }
  bool operator!= (const PartSelect & iRhs)             const { return !(*this == iRhs); }
  bool operator!= (long long unsigned int iRhs)         const { return  (*this != (UInt64)iRhs); }
  bool operator!= (long long int iRhs)                  const { return  (*this != (UInt64)iRhs); }
  bool operator!= (Int64 iRhs)                          const { return  (*this != (UInt64)iRhs); }
  bool operator!= (int iRhs)                            const { return  (*this != (UInt32)iRhs); }

  bool operator<= (UInt32 iRhs)                         const;
  bool operator<= (UInt64 iRhs)                         const;
  bool operator<= (const BitVector & iRhs)              const;
  bool operator<= (const PartSelect & iRhs)             const { return  (*this <= (BitVector)iRhs); }
  bool operator<= (long long unsigned int iRhs)         const { return  (*this <= (UInt64)iRhs); }
  bool operator<= (long long int iRhs)                  const { return  (*this <= (UInt64)iRhs); }
  bool operator<= (Int64 iRhs)                          const { return  (*this <= (UInt64)iRhs); }
  bool operator<= (int iRhs)                            const { return  (*this <= (UInt32)iRhs); }

  bool operator>= (UInt32 iRhs)                         const;
  bool operator>= (UInt64 iRhs)                         const;
  bool operator>= (const BitVector & iRhs)              const;
  bool operator>= (const PartSelect & iRhs)             const { return  (*this >= (BitVector)iRhs); }
  bool operator>= (long long unsigned int iRhs)         const { return  (*this >= (UInt64)iRhs); }
  bool operator>= (long long int iRhs)                  const { return  (*this >= (UInt64)iRhs); }
  bool operator>= (Int64 iRhs)                          const { return  (*this >= (UInt64)iRhs); }
  bool operator>= (int iRhs)                            const { return  (*this >= (UInt32)iRhs); }

  bool operator>  (UInt32 iRhs)                         const;
  bool operator>  (UInt64 iRhs)                         const;
  bool operator>  (const BitVector & iRhs)              const;
  bool operator>  (const PartSelect & iRhs)             const { return  (*this > (BitVector)iRhs); }
  bool operator>  (long long unsigned int iRhs)         const { return  (*this > (UInt64)iRhs); }
  bool operator>  (long long int iRhs)                  const { return  (*this > (UInt64)iRhs); }
  bool operator>  (Int64 iRhs)                          const { return  (*this > (UInt64)iRhs); }
  bool operator>  (int iRhs)                            const { return  (*this > (UInt32)iRhs); }

  bool operator<  (UInt32 iRhs)                         const;
  bool operator<  (UInt64 iRhs)                         const;
  bool operator<  (const BitVector & iRhs)              const;
  bool operator<  (const PartSelect & iRhs)             const { return  (*this < (BitVector)iRhs); }
  bool operator<  (long long unsigned int iRhs)         const { return  (*this < (UInt64)iRhs); }
  bool operator<  (long long int iRhs)                  const { return  (*this < (UInt64)iRhs); }
  bool operator<  (Int64 iRhs)                          const { return  (*this < (UInt64)iRhs); }
  bool operator<  (int iRhs)                            const { return  (*this < (UInt32)iRhs); }

  BitVector & operator<<= (UInt32 iRhs);
  BitVector & operator<<= (const BitVector & iRhs)            { return  *this <<= iRhs[0]; }
  BitVector & operator<<= (const PartSelect & iRhs)           { return  *this <<= ((BitVector)iRhs)[0]; }
  BitVector & operator<<= (int iRhs)                          { return  *this <<= (UInt32)iRhs; }

  BitVector & operator>>= (UInt32 iRhs);
  BitVector & operator>>= (const BitVector & iRhs)            { return  *this >>= iRhs[0]; }
  BitVector & operator>>= (const PartSelect & iRhs)           { return  *this >>= ((BitVector)iRhs)[0]; }
  BitVector & operator>>= (int iRhs)                          { return  *this >>= (UInt32)iRhs; }

  BitVector   operator<< (UInt32 iRhs)                  const;
  BitVector   operator<< (const BitVector & iRhs)       const { return  *this << iRhs[0]; }
  BitVector   operator<< (const PartSelect & iRhs)      const { return  *this << ((BitVector)iRhs)[0]; }
  BitVector   operator<< (int iRhs)                     const { return  *this << (UInt32)iRhs; }

  BitVector   operator>> (UInt32 iRhs)                  const;
  BitVector   operator>> (const BitVector & iRhs)       const { return  *this >> iRhs[0]; }
  BitVector   operator>> (const PartSelect & iRhs)      const { return  *this >> ((BitVector)iRhs)[0]; }
  BitVector   operator>> (int iRhs)                     const { return  *this >> (UInt32)iRhs; }

  BitVector   operator~  () const;

  BitVector & operator&= (UInt32 iRhs);
  BitVector & operator&= (UInt64 iRhs);
  BitVector & operator&= (const BitVector & iRhs);
  BitVector & operator&= (const PartSelect & iRhs)            { return  *this &= ((BitVector)iRhs); }
  BitVector & operator&= (long long unsigned int iRhs)        { return  *this &= (UInt64)iRhs; }
  BitVector & operator&= (long long int iRhs)                 { return  *this &= (UInt64)iRhs; }
  BitVector & operator&= (Int64 iRhs)                         { return  *this &= (UInt64)iRhs; }
  BitVector & operator&= (int iRhs)                           { return  *this &= (UInt32)iRhs; }

  BitVector   operator&  (UInt32 iRhs) const;
  BitVector   operator&  (UInt64 iRhs) const;
  BitVector   operator&  (const BitVector & iRhs) const;
  BitVector   operator&  (const PartSelect & iRhs)  const     { return  *this & ((BitVector)iRhs); }
  BitVector   operator&  (long long unsigned int iRhs)  const { return  *this & (UInt64)iRhs; }
  BitVector   operator&  (long long int iRhs)  const          { return  *this & (UInt64)iRhs; }
  BitVector   operator&  (Int64 iRhs)  const                  { return  *this & (UInt64)iRhs; }
  BitVector   operator&  (int iRhs)  const                    { return  *this & (UInt32)iRhs; }

  BitVector & operator|= (UInt32 iRhs);
  BitVector & operator|= (UInt64 iRhs);
  BitVector & operator|= (const BitVector & iRhs);
  BitVector & operator|= (const PartSelect & iRhs)            { return  *this |= ((BitVector)iRhs); }
  BitVector & operator|= (long long unsigned int iRhs)        { return  *this |= (UInt64)iRhs; }
  BitVector & operator|= (long long int iRhs)                 { return  *this |= (UInt64)iRhs; }
  BitVector & operator|= (Int64 iRhs)                         { return  *this |= (UInt64)iRhs; }
  BitVector & operator|= (int iRhs)                           { return  *this |= (UInt32)iRhs; }

  BitVector   operator|  (UInt32 iRhs) const;
  BitVector   operator|  (UInt64 iRhs) const;
  BitVector   operator|  (const BitVector & iRhs) const;
  BitVector   operator|  (const PartSelect & iRhs)  const     { return  *this | ((BitVector)iRhs); }
  BitVector   operator|  (long long unsigned int iRhs) const  { return  *this | (UInt64)iRhs; }
  BitVector   operator|  (long long int iRhs)  const          { return  *this | (UInt64)iRhs; }
  BitVector   operator|  (Int64 iRhs)  const                  { return  *this | (UInt64)iRhs; }
  BitVector   operator|  (int iRhs)  const                    { return  *this | (UInt32)iRhs; }

  BitVector & operator^= (UInt32 iRhs);
  BitVector & operator^= (UInt64 iRhs);
  BitVector & operator^= (const BitVector & iRhs);
  BitVector & operator^= (const PartSelect & iRhs)            { return  *this ^= ((BitVector)iRhs); }
  BitVector & operator^= (long long unsigned int iRhs)        { return  *this ^= (UInt64)iRhs; }
  BitVector & operator^= (long long int iRhs)                 { return  *this ^= (UInt64)iRhs; }
  BitVector & operator^= (Int64 iRhs)                         { return  *this ^= (UInt64)iRhs; }
  BitVector & operator^= (int iRhs)                           { return  *this ^= (UInt32)iRhs; }

  BitVector   operator^  (UInt32 iRhs)                  const;
  BitVector   operator^  (UInt64 iRhs)                  const;
  BitVector   operator^  (const BitVector & iRhs)       const;
  BitVector   operator^  (const PartSelect & iRhs)      const { return  *this ^ ((BitVector)iRhs); }
  BitVector   operator^  (long long unsigned int iRhs)  const { return  *this ^ (UInt64)iRhs; }
  BitVector   operator^  (long long int iRhs)           const { return  *this ^ (UInt64)iRhs; }
  BitVector   operator^  (Int64 iRhs)                   const { return  *this ^ (UInt64)iRhs; }
  BitVector   operator^  (int iRhs)                     const { return  *this ^ (UInt32)iRhs; }

  BitVector   operator,  (UInt32 iRhs)                  const;
  BitVector   operator,  (UInt64 iRhs)                  const;
  BitVector   operator,  (const BitVector & iRhs)       const;
  BitVector   operator,  (const PartSelect & iRhs)      const { return  (*this , ((BitVector)iRhs)); }
  BitVector   operator,  (long long unsigned int iRhs)  const { return  (*this , (UInt64)iRhs); }
  BitVector   operator,  (long long int iRhs)           const { return  (*this , (UInt64)iRhs); }
  BitVector   operator,  (Int64 iRhs)                   const { return  (*this , (UInt64)iRhs); }
  BitVector   operator,  (int iRhs)                     const { return  (*this , (UInt32)iRhs); }

  // Part Select friend operators
  friend ostream & operator<< (ostream & iStream,         const BitVector::PartSelect & iPs);
  friend Logger  & operator<< (Logger  & iLogger,         const BitVector::PartSelect & iPs);

  friend BitVector operator+ (UInt32 iLhs,                  const BitVector::PartSelect & iRhs);
  friend BitVector operator+ (UInt64 iLhs,                  const BitVector::PartSelect & iRhs);
  friend BitVector operator+ (long long unsigned int iLhs,  const BitVector::PartSelect & iRhs);
  friend BitVector operator+ (long long int iLhs,           const BitVector::PartSelect & iRhs);
  friend BitVector operator+ (Int64 iLhs,                   const BitVector::PartSelect & iRhs);
  friend BitVector operator+ (int iLhs,                     const BitVector::PartSelect & iRhs);

  friend BitVector operator- (UInt32 iLhs,                  const BitVector::PartSelect & iRhs);
  friend BitVector operator- (UInt64 iLhs,                  const BitVector::PartSelect & iRhs);
  friend BitVector operator- (long long unsigned int iLhs,  const BitVector::PartSelect & iRhs);
  friend BitVector operator- (long long int iLhs,           const BitVector::PartSelect & iRhs);
  friend BitVector operator- (Int64 iLhs,                   const BitVector::PartSelect & iRhs);
  friend BitVector operator- (int iLhs,                     const BitVector::PartSelect & iRhs);

  friend bool operator== (UInt32 iLhs,                      const BitVector::PartSelect & iRhs);
  friend bool operator== (UInt64 iLhs,                      const BitVector::PartSelect & iRhs);
  friend bool operator== (long long unsigned int iLhs,      const BitVector::PartSelect & iRhs);
  friend bool operator== (long long int iLhs,               const BitVector::PartSelect & iRhs);
  friend bool operator== (Int64 iLhs,                       const BitVector::PartSelect & iRhs);
  friend bool operator== (int iLhs,                         const BitVector::PartSelect & iRhs);

  friend bool operator!= (UInt32 iLhs,                      const BitVector::PartSelect & iRhs);
  friend bool operator!= (UInt64 iLhs,                      const BitVector::PartSelect & iRhs);
  friend bool operator!= (long long unsigned int iLhs,      const BitVector::PartSelect & iRhs);
  friend bool operator!= (long long int iLhs,               const BitVector::PartSelect & iRhs);
  friend bool operator!= (Int64 iLhs,                       const BitVector::PartSelect & iRhs);
  friend bool operator!= (int iLhs,                         const BitVector::PartSelect & iRhs);

  friend bool operator<= (UInt32 iLhs,                      const BitVector::PartSelect & iRhs);
  friend bool operator<= (UInt64 iLhs,                      const BitVector::PartSelect & iRhs);
  friend bool operator<= (long long unsigned int iLhs,      const BitVector::PartSelect & iRhs);
  friend bool operator<= (long long int iLhs,               const BitVector::PartSelect & iRhs);
  friend bool operator<= (Int64 iLhs,                       const BitVector::PartSelect & iRhs);
  friend bool operator<= (int iLhs,                         const BitVector::PartSelect & iRhs);

  friend bool operator>= (UInt32 iLhs,                      const BitVector::PartSelect & iRhs);
  friend bool operator>= (UInt64 iLhs,                      const BitVector::PartSelect & iRhs);
  friend bool operator>= (long long unsigned int iLhs,      const BitVector::PartSelect & iRhs);
  friend bool operator>= (long long int iLhs,               const BitVector::PartSelect & iRhs);
  friend bool operator>= (Int64 iLhs,                       const BitVector::PartSelect & iRhs);
  friend bool operator>= (int iLhs,                         const BitVector::PartSelect & iRhs);

  friend bool operator<  (UInt32 iLhs,                      const BitVector::PartSelect & iRhs);
  friend bool operator<  (UInt64 iLhs,                      const BitVector::PartSelect & iRhs);
  friend bool operator<  (long long unsigned int iLhs,      const BitVector::PartSelect & iRhs);
  friend bool operator<  (long long int iLhs,               const BitVector::PartSelect & iRhs);
  friend bool operator<  (Int64 iLhs,                       const BitVector::PartSelect & iRhs);
  friend bool operator<  (int iLhs,                         const BitVector::PartSelect & iRhs);

  friend bool operator>  (UInt32 iLhs,                      const BitVector::PartSelect & iRhs);
  friend bool operator>  (UInt64 iLhs,                      const BitVector::PartSelect & iRhs);
  friend bool operator>  (long long unsigned int iLhs,      const BitVector::PartSelect & iRhs);
  friend bool operator>  (long long int iLhs,               const BitVector::PartSelect & iRhs);
  friend bool operator>  (Int64 iLhs,                       const BitVector::PartSelect & iRhs);
  friend bool operator>  (int iLhs,                         const BitVector::PartSelect & iRhs);

  friend BitVector operator& (UInt32 iLhs,                  const BitVector::PartSelect & iRhs);
  friend BitVector operator& (UInt64 iLhs,                  const BitVector::PartSelect & iRhs);
  friend BitVector operator& (long long unsigned int iLhs,  const BitVector::PartSelect & iRhs);
  friend BitVector operator& (long long int iLhs,           const BitVector::PartSelect & iRhs);
  friend BitVector operator& (Int64 iLhs,                   const BitVector::PartSelect & iRhs);
  friend BitVector operator& (int iLhs,                     const BitVector::PartSelect & iRhs);

  friend BitVector operator| (UInt32 iLhs,                  const BitVector::PartSelect & iRhs);
  friend BitVector operator| (UInt64 iLhs,                  const BitVector::PartSelect & iRhs);
  friend BitVector operator| (long long unsigned int iLhs,  const BitVector::PartSelect & iRhs);
  friend BitVector operator| (long long int iLhs,           const BitVector::PartSelect & iRhs);
  friend BitVector operator| (Int64 iLhs,                   const BitVector::PartSelect & iRhs);
  friend BitVector operator| (int iLhs,                     const BitVector::PartSelect & iRhs);

  friend BitVector operator^ (UInt32 iLhs,                  const BitVector::PartSelect & iRhs);
  friend BitVector operator^ (UInt64 iLhs,                  const BitVector::PartSelect & iRhs);
  friend BitVector operator^ (long long unsigned int iLhs,  const BitVector::PartSelect & iRhs);
  friend BitVector operator^ (long long int iLhs,           const BitVector::PartSelect & iRhs);
  friend BitVector operator^ (Int64 iLhs,                   const BitVector::PartSelect & iRhs);
  friend BitVector operator^ (int iLhs,                     const BitVector::PartSelect & iRhs);

  friend BitVector operator, (UInt32 iLhs,                  const BitVector::PartSelect & iRhs);
  friend BitVector operator, (UInt64 iLhs,                  const BitVector::PartSelect & iRhs);
  friend BitVector operator, (long long unsigned int iLhs,  const BitVector::PartSelect & iRhs);
  friend BitVector operator, (long long int iLhs,           const BitVector::PartSelect & iRhs);
  friend BitVector operator, (Int64 iLhs,                   const BitVector::PartSelect & iRhs);
  friend BitVector operator, (int iLhs,                     const BitVector::PartSelect & iRhs);

  //friend BitVector::PartSelect TypeBase::operator() (UInt32 iUpperIndex, UInt32 iLowerIndex);

  //================
  // TBD operators:
  //================
  // Add operators for interacting with strings for 4-state values.

};

inline ostream & operator<< (ostream & iStream, const BitVector & iBv)              { iStream << iBv.ToString(); return iStream; }
inline Logger  & operator<< (Logger &  iLogger, const BitVector & iBv)              { iLogger << iBv.ToString(); return iLogger; }
inline ostream & operator<< (ostream & iStream, const BitVector::PartSelect & iPs)  { iStream << ((BitVector)iPs).ToString(); return iStream; }
inline Logger  & operator<< (Logger &  iLogger, const BitVector::PartSelect & iPs)  { iLogger << ((BitVector)iPs).ToString(); return iLogger; }

inline BitVector operator+ (UInt32 iLhs,                  const BitVector & iRhs)             { return iRhs + iLhs; }
inline BitVector operator+ (UInt64 iLhs,                  const BitVector & iRhs)             { return iRhs + iLhs; }
inline BitVector operator+ (long long unsigned int iLhs,  const BitVector & iRhs)             { return iRhs + iLhs; }
inline BitVector operator+ (long long int iLhs,           const BitVector & iRhs)             { return iRhs + iLhs; }
inline BitVector operator+ (Int64 iLhs,                   const BitVector & iRhs)             { return iRhs + iLhs; }
inline BitVector operator+ (int iLhs,                     const BitVector & iRhs)             { return iRhs + iLhs; }
inline BitVector operator+ (UInt32 iLhs,                  const BitVector::PartSelect & iRhs) { return iRhs + iLhs; }
inline BitVector operator+ (UInt64 iLhs,                  const BitVector::PartSelect & iRhs) { return iRhs + iLhs; }
inline BitVector operator+ (long long unsigned int iLhs,  const BitVector::PartSelect & iRhs) { return iRhs + iLhs; }
inline BitVector operator+ (long long int iLhs,           const BitVector::PartSelect & iRhs) { return iRhs + iLhs; }
inline BitVector operator+ (Int64 iLhs,                   const BitVector::PartSelect & iRhs) { return iRhs + iLhs; }
inline BitVector operator+ (int iLhs,                     const BitVector::PartSelect & iRhs) { return iRhs + iLhs; }

       BitVector operator- (UInt32 iLhs,                  const BitVector & iRhs);
       BitVector operator- (UInt64 iLhs,                  const BitVector & iRhs);
inline BitVector operator- (long long unsigned int iLhs,  const BitVector & iRhs)             { return (UInt64)iLhs - iRhs; }
inline BitVector operator- (long long int iLhs,           const BitVector & iRhs)             { return (UInt64)iLhs - iRhs; }
inline BitVector operator- (Int64 iLhs,                   const BitVector & iRhs)             { return (UInt64)iLhs - iRhs; }
inline BitVector operator- (int iLhs,                     const BitVector & iRhs)             { return (UInt32)iLhs - iRhs; }
inline BitVector operator- (UInt32 iLhs,                  const BitVector::PartSelect & iRhs) { return iLhs - (BitVector)iRhs; }
inline BitVector operator- (UInt64 iLhs,                  const BitVector::PartSelect & iRhs) { return iLhs - (BitVector)iRhs; }
inline BitVector operator- (long long unsigned int iLhs,  const BitVector::PartSelect & iRhs) { return (UInt64)iLhs - iRhs; }
inline BitVector operator- (long long int iLhs,           const BitVector::PartSelect & iRhs) { return (UInt64)iLhs - iRhs; }
inline BitVector operator- (Int64 iLhs,                   const BitVector::PartSelect & iRhs) { return (UInt64)iLhs - iRhs; }
inline BitVector operator- (int iLhs,                     const BitVector::PartSelect & iRhs) { return (UInt32)iLhs - iRhs; }

inline bool operator== (UInt32 iLhs,                      const BitVector & iRhs)             { return iRhs == iLhs; }
inline bool operator== (UInt64 iLhs,                      const BitVector & iRhs)             { return iRhs == iLhs; }
inline bool operator== (long long unsigned int iLhs,      const BitVector & iRhs)             { return iRhs == iLhs; }
inline bool operator== (long long int iLhs,               const BitVector & iRhs)             { return iRhs == iLhs; }
inline bool operator== (Int64 iLhs,                       const BitVector & iRhs)             { return iRhs == iLhs; }
inline bool operator== (int iLhs,                         const BitVector & iRhs)             { return iRhs == iLhs; }
inline bool operator== (UInt32 iLhs,                      const BitVector::PartSelect & iRhs) { return iRhs == iLhs; }
inline bool operator== (UInt64 iLhs,                      const BitVector::PartSelect & iRhs) { return iRhs == iLhs; }
inline bool operator== (long long unsigned int iLhs,      const BitVector::PartSelect & iRhs) { return iRhs == iLhs; }
inline bool operator== (long long int iLhs,               const BitVector::PartSelect & iRhs) { return iRhs == iLhs; }
inline bool operator== (Int64 iLhs,                       const BitVector::PartSelect & iRhs) { return iRhs == iLhs; }
inline bool operator== (int iLhs,                         const BitVector::PartSelect & iRhs) { return iRhs == iLhs; }

inline bool operator!= (UInt32 iLhs,                      const BitVector & iRhs)             { return iRhs != iLhs; }
inline bool operator!= (UInt64 iLhs,                      const BitVector & iRhs)             { return iRhs != iLhs; }
inline bool operator!= (long long unsigned int iLhs,      const BitVector & iRhs)             { return iRhs != iLhs; }
inline bool operator!= (long long int iLhs,               const BitVector & iRhs)             { return iRhs != iLhs; }
inline bool operator!= (Int64 iLhs,                       const BitVector & iRhs)             { return iRhs != iLhs; }
inline bool operator!= (int iLhs,                         const BitVector & iRhs)             { return iRhs != iLhs; }
inline bool operator!= (UInt32 iLhs,                      const BitVector::PartSelect & iRhs) { return iRhs != iLhs; }
inline bool operator!= (UInt64 iLhs,                      const BitVector::PartSelect & iRhs) { return iRhs != iLhs; }
inline bool operator!= (long long unsigned int iLhs,      const BitVector::PartSelect & iRhs) { return iRhs != iLhs; }
inline bool operator!= (long long int iLhs,               const BitVector::PartSelect & iRhs) { return iRhs != iLhs; }
inline bool operator!= (Int64 iLhs,                       const BitVector::PartSelect & iRhs) { return iRhs != iLhs; }
inline bool operator!= (int iLhs,                         const BitVector::PartSelect & iRhs) { return iRhs != iLhs; }

       bool operator<= (UInt32 iLhs,                      const BitVector & iRhs);
       bool operator<= (UInt64 iLhs,                      const BitVector & iRhs);
inline bool operator<= (long long unsigned int iLhs,      const BitVector & iRhs)             { return (UInt64)iLhs <= iRhs; }
inline bool operator<= (long long int iLhs,               const BitVector & iRhs)             { return (UInt64)iLhs <= iRhs; }
inline bool operator<= (Int64 iLhs,                       const BitVector & iRhs)             { return (UInt64)iLhs <= iRhs; }
inline bool operator<= (int iLhs,                         const BitVector & iRhs)             { return (UInt32)iLhs <= iRhs; }
inline bool operator<= (UInt32 iLhs,                      const BitVector::PartSelect & iRhs) { return (iLhs <= (BitVector)iRhs); }
inline bool operator<= (UInt64 iLhs,                      const BitVector::PartSelect & iRhs) { return (iLhs <= (BitVector)iRhs); }
inline bool operator<= (long long unsigned int iLhs,      const BitVector::PartSelect & iRhs) { return (UInt64)iLhs <= iRhs; }
inline bool operator<= (long long int iLhs,               const BitVector::PartSelect & iRhs) { return (UInt64)iLhs <= iRhs; }
inline bool operator<= (Int64 iLhs,                       const BitVector::PartSelect & iRhs) { return (UInt64)iLhs <= iRhs; }
inline bool operator<= (int iLhs,                         const BitVector::PartSelect & iRhs) { return (UInt32)iLhs <= iRhs; }

       bool operator>= (UInt32 iLhs,                      const BitVector & iRhs);
       bool operator>= (UInt64 iLhs,                      const BitVector & iRhs);
inline bool operator>= (long long unsigned int iLhs,      const BitVector & iRhs)             { return (UInt64)iLhs >= iRhs; }
inline bool operator>= (long long int iLhs,               const BitVector & iRhs)             { return (UInt64)iLhs >= iRhs; }
inline bool operator>= (Int64 iLhs,                       const BitVector & iRhs)             { return (UInt64)iLhs >= iRhs; }
inline bool operator>= (int iLhs,                         const BitVector & iRhs)             { return (UInt32)iLhs >= iRhs; }
inline bool operator>= (UInt32 iLhs,                      const BitVector::PartSelect & iRhs) { return (iLhs >= (BitVector)iRhs); }
inline bool operator>= (UInt64 iLhs,                      const BitVector::PartSelect & iRhs) { return (iLhs >= (BitVector)iRhs); }
inline bool operator>= (long long unsigned int iLhs,      const BitVector::PartSelect & iRhs) { return (UInt64)iLhs >= iRhs; }
inline bool operator>= (long long int iLhs,               const BitVector::PartSelect & iRhs) { return (UInt64)iLhs >= iRhs; }
inline bool operator>= (Int64 iLhs,                       const BitVector::PartSelect & iRhs) { return (UInt64)iLhs >= iRhs; }
inline bool operator>= (int iLhs,                         const BitVector::PartSelect & iRhs) { return (UInt32)iLhs >= iRhs; }

       bool operator<  (UInt32 iLhs,                      const BitVector & iRhs);
       bool operator<  (UInt64 iLhs,                      const BitVector & iRhs);
inline bool operator<  (long long unsigned int iLhs,      const BitVector & iRhs)             { return (UInt64)iLhs < iRhs; }
inline bool operator<  (long long int iLhs,               const BitVector & iRhs)             { return (UInt64)iLhs < iRhs; }
inline bool operator<  (Int64 iLhs,                       const BitVector & iRhs)             { return (UInt64)iLhs < iRhs; }
inline bool operator<  (int iLhs,                         const BitVector & iRhs)             { return (UInt32)iLhs < iRhs; }
inline bool operator<  (UInt32 iLhs,                      const BitVector::PartSelect & iRhs) { return (iLhs < (BitVector)iRhs); }
inline bool operator<  (UInt64 iLhs,                      const BitVector::PartSelect & iRhs) { return (iLhs < (BitVector)iRhs); }
inline bool operator<  (long long unsigned int iLhs,      const BitVector::PartSelect & iRhs) { return (UInt64)iLhs < iRhs; }
inline bool operator<  (long long int iLhs,               const BitVector::PartSelect & iRhs) { return (UInt64)iLhs < iRhs; }
inline bool operator<  (Int64 iLhs,                       const BitVector::PartSelect & iRhs) { return (UInt64)iLhs < iRhs; }
inline bool operator<  (int iLhs,                         const BitVector::PartSelect & iRhs) { return (UInt32)iLhs < iRhs; }

       bool operator>  (UInt32 iLhs,                      const BitVector & iRhs);
       bool operator>  (UInt64 iLhs,                      const BitVector & iRhs);
inline bool operator>  (long long unsigned int iLhs,      const BitVector & iRhs)             { return (UInt64)iLhs > iRhs; }
inline bool operator>  (long long int iLhs,               const BitVector & iRhs)             { return (UInt64)iLhs > iRhs; }
inline bool operator>  (Int64 iLhs,                       const BitVector & iRhs)             { return (UInt64)iLhs > iRhs; }
inline bool operator>  (int iLhs,                         const BitVector & iRhs)             { return (UInt32)iLhs > iRhs; }
inline bool operator>  (UInt32 iLhs,                      const BitVector::PartSelect & iRhs) { return (iLhs > (BitVector)iRhs); }
inline bool operator>  (UInt64 iLhs,                      const BitVector::PartSelect & iRhs) { return (iLhs > (BitVector)iRhs); }
inline bool operator>  (long long unsigned int iLhs,      const BitVector::PartSelect & iRhs) { return (UInt64)iLhs > iRhs; }
inline bool operator>  (long long int iLhs,               const BitVector::PartSelect & iRhs) { return (UInt64)iLhs > iRhs; }
inline bool operator>  (Int64 iLhs,                       const BitVector::PartSelect & iRhs) { return (UInt64)iLhs > iRhs; }
inline bool operator>  (int iLhs,                         const BitVector::PartSelect & iRhs) { return (UInt32)iLhs > iRhs; }

inline BitVector operator& (UInt32 iLhs,                  const BitVector & iRhs)             { return iRhs & iLhs; }
inline BitVector operator& (UInt64 iLhs,                  const BitVector & iRhs)             { return iRhs & iLhs; }
inline BitVector operator& (long long unsigned int iLhs,  const BitVector & iRhs)             { return iRhs & iLhs; }
inline BitVector operator& (long long int iLhs,           const BitVector & iRhs)             { return iRhs & iLhs; }
inline BitVector operator& (Int64 iLhs,                   const BitVector & iRhs)             { return iRhs & iLhs; }
inline BitVector operator& (int iLhs,                     const BitVector & iRhs)             { return iRhs & iLhs; }
inline BitVector operator& (UInt32 iLhs,                  const BitVector::PartSelect & iRhs) { return iRhs & iLhs; }
inline BitVector operator& (UInt64 iLhs,                  const BitVector::PartSelect & iRhs) { return iRhs & iLhs; }
inline BitVector operator& (long long unsigned int iLhs,  const BitVector::PartSelect & iRhs) { return iRhs & iLhs; }
inline BitVector operator& (long long int iLhs,           const BitVector::PartSelect & iRhs) { return iRhs & iLhs; }
inline BitVector operator& (Int64 iLhs,                   const BitVector::PartSelect & iRhs) { return iRhs & iLhs; }
inline BitVector operator& (int iLhs,                     const BitVector::PartSelect & iRhs) { return iRhs & iLhs; }

inline BitVector operator| (UInt32 iLhs,                  const BitVector & iRhs)             { return iRhs | iLhs; }
inline BitVector operator| (UInt64 iLhs,                  const BitVector & iRhs)             { return iRhs | iLhs; }
inline BitVector operator| (long long unsigned int iLhs,  const BitVector & iRhs)             { return iRhs | iLhs; }
inline BitVector operator| (long long int iLhs,           const BitVector & iRhs)             { return iRhs | iLhs; }
inline BitVector operator| (Int64 iLhs,                   const BitVector & iRhs)             { return iRhs | iLhs; }
inline BitVector operator| (int iLhs,                     const BitVector & iRhs)             { return iRhs | iLhs; }
inline BitVector operator| (UInt32 iLhs,                  const BitVector::PartSelect & iRhs) { return iRhs | iLhs; }
inline BitVector operator| (UInt64 iLhs,                  const BitVector::PartSelect & iRhs) { return iRhs | iLhs; }
inline BitVector operator| (long long unsigned int iLhs,  const BitVector::PartSelect & iRhs) { return iRhs | iLhs; }
inline BitVector operator| (long long int iLhs,           const BitVector::PartSelect & iRhs) { return iRhs | iLhs; }
inline BitVector operator| (Int64 iLhs,                   const BitVector::PartSelect & iRhs) { return iRhs | iLhs; }
inline BitVector operator| (int iLhs,                     const BitVector::PartSelect & iRhs) { return iRhs | iLhs; }

inline BitVector operator^ (UInt32 iLhs,                  const BitVector & iRhs)             { return iRhs ^ iLhs; }
inline BitVector operator^ (UInt64 iLhs,                  const BitVector & iRhs)             { return iRhs ^ iLhs; }
inline BitVector operator^ (long long unsigned int iLhs,  const BitVector & iRhs)             { return iRhs ^ iLhs; }
inline BitVector operator^ (long long int iLhs,           const BitVector & iRhs)             { return iRhs ^ iLhs; }
inline BitVector operator^ (Int64 iLhs,                   const BitVector & iRhs)             { return iRhs ^ iLhs; }
inline BitVector operator^ (int iLhs,                     const BitVector & iRhs)             { return iRhs ^ iLhs; }
inline BitVector operator^ (UInt32 iLhs,                  const BitVector::PartSelect & iRhs) { return iRhs ^ iLhs; }
inline BitVector operator^ (UInt64 iLhs,                  const BitVector::PartSelect & iRhs) { return iRhs ^ iLhs; }
inline BitVector operator^ (long long unsigned int iLhs,  const BitVector::PartSelect & iRhs) { return iRhs ^ iLhs; }
inline BitVector operator^ (long long int iLhs,           const BitVector::PartSelect & iRhs) { return iRhs ^ iLhs; }
inline BitVector operator^ (Int64 iLhs,                   const BitVector::PartSelect & iRhs) { return iRhs ^ iLhs; }
inline BitVector operator^ (int iLhs,                     const BitVector::PartSelect & iRhs) { return iRhs ^ iLhs; }

       BitVector operator, (UInt32 iLhs,                  const BitVector & iRhs);
       BitVector operator, (UInt64 iLhs,                  const BitVector & iRhs);
inline BitVector operator, (long long unsigned int iLhs,  const BitVector & iRhs)             { return ((UInt64)iLhs, iRhs); }
inline BitVector operator, (long long int iLhs,           const BitVector & iRhs)             { return ((UInt64)iLhs, iRhs); }
inline BitVector operator, (Int64 iLhs,                   const BitVector & iRhs)             { return ((UInt64)iLhs, iRhs); }
inline BitVector operator, (int iLhs,                     const BitVector & iRhs)             { return ((UInt32)iLhs, iRhs); }
       BitVector operator, (UInt32 iLhs,                  const BitVector::PartSelect & iRhs);
       BitVector operator, (UInt64 iLhs,                  const BitVector::PartSelect & iRhs);
inline BitVector operator, (long long unsigned int iLhs,  const BitVector::PartSelect & iRhs) { return ((UInt64)iLhs, iRhs); }
inline BitVector operator, (long long int iLhs,           const BitVector::PartSelect & iRhs) { return ((UInt64)iLhs, iRhs); }
inline BitVector operator, (Int64 iLhs,                   const BitVector::PartSelect & iRhs) { return ((UInt64)iLhs, iRhs); }
inline BitVector operator, (int iLhs,                     const BitVector::PartSelect & iRhs) { return ((UInt32)iLhs, iRhs); }

#endif /* BITVECTOR_H */

