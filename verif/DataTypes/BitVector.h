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
#include "Logger.h"
#include "TypeBase.h"

using namespace std;

class BitVector {
  friend class TypeBase;

  // Enums
  public:

  enum class PRINT_FMT : Byte
  {
    DEC = 0,
    HEX = 1
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
    static bool           s_printHexWordDivider;// Underscore between words.

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
    BitVector(string iName = s_default_name);
    BitVector(string iName, UInt32 iSize, NB_STATES iStates = s_nbStates);
    BitVector(UInt32 iSize = s_default_size);
    BitVector(const BitVector & iSource);
    BitVector(BitVector && iSource);
    ~BitVector();

  // Inits
  private:
    void init(string iName, UInt32 iSize, NB_STATES iStates);

  // Public Methods
  public:
    void    Resize(UInt32 iNewSize);
    UInt32  GetUInt32() const;
    UInt32  GetUInt32(UInt32 iWordNb) const;
    UInt64  GetUInt64() const;
    UInt64  GetUInt64(UInt32 iLowerWordNb) const;
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
    void    add(UInt32 iVal, UInt32 iWordNb = 0);
    void    subtract(UInt32 iVal, UInt32 iWordNb = 0);
    bool    equals(UInt32 iVal, UInt32 iWordNb = 0) const;

  protected:
  class PartSelect
  {
    friend class BitVector;
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
    void getParentBits(BitVector & oBV) const;

    // Operators
    public:
    PartSelect & operator= (UInt32 iRhs);
    PartSelect & operator= (Int32 iRhs) { return *this = (UInt32)iRhs; }

    bool operator== (const PartSelect & iRhs) const;
    bool operator!= (const PartSelect & iRhs) const { return !(*this == iRhs); };
  };

  // BitVector Operators
  public:
  BitVector & operator= (UInt32 iRhs);
  BitVector & operator= (UInt64 iRhs);
  BitVector & operator= (const BitVector & iRhs);
  BitVector & operator= (BitVector && iRhs); // Move if the sizes are the same.
  BitVector & operator= (long long unsigned int iRhs) { return *this = (UInt64)iRhs; }
  BitVector & operator= (long long int iRhs) { return *this = (UInt64)iRhs; }
  BitVector & operator= (Int64 iRhs) { return *this = (UInt64)iRhs; }
  BitVector & operator= (int iRhs) { return *this = (UInt32)iRhs; }

  PartSelect operator() (UInt32 iUpperIndex, UInt32 iLowerIndex);

  BitVector & operator+= (UInt32 iRhs);
  BitVector & operator+= (UInt64 iRhs);
  BitVector & operator+= (const BitVector & iRhs);
  BitVector & operator+= (const PartSelect & iRhs);
  BitVector & operator+= (long long unsigned int iRhs) { return *this += (UInt64)iRhs; }
  BitVector & operator+= (long long int iRhs) { return *this += (UInt64)iRhs; }
  BitVector & operator+= (Int64 iRhs) { return *this += (UInt64)iRhs; }
  BitVector & operator+= (int iRhs) { return *this += (UInt32)iRhs; }

  BitVector & operator++ ();
  BitVector & operator++ (int iDummy);

  friend BitVector operator+ (const BitVector & iLhs, const BitVector::PartSelect & iRhs);
  friend BitVector operator+ (const BitVector::PartSelect & iLhs, const BitVector & iRhs);


  BitVector & operator-= (UInt32 iRhs);
  BitVector & operator-= (UInt64 iRhs);
  BitVector & operator-= (const BitVector & iRhs);
  BitVector & operator-= (const PartSelect & iRhs);
  BitVector & operator-= (long long unsigned int iRhs) { return *this -= (UInt64)iRhs; }
  BitVector & operator-= (long long int iRhs) { return *this -= (UInt64)iRhs; }
  BitVector & operator-= (Int64 iRhs) { return *this -= (UInt64)iRhs; }
  BitVector & operator-= (int iRhs) { return *this -= (UInt32)iRhs; }

  BitVector & operator-- ();
  BitVector & operator-- (int iDummy);

  friend BitVector operator- (const BitVector & iLhs, const BitVector::PartSelect & iRhs);

  bool operator== (UInt32 iRhs) const;
  bool operator== (UInt64 iRhs) const;
  bool operator== (const BitVector & iRhs) const;
  bool operator== (const PartSelect & iRhs) const;
  bool operator== (long long unsigned int iRhs) const { return *this == (UInt64)iRhs; };
  bool operator== (long long int iRhs) const { return *this == (UInt64)iRhs; };
  bool operator== (Int64 iRhs) const { return *this == (UInt64)iRhs; };
  bool operator== (int iRhs) const { return *this == (UInt32)iRhs; };

  friend bool operator== (const PartSelect & iLhs,  const BitVector & iRhs);

  bool operator!= (UInt32 iRhs) const { return !(*this == iRhs); };
  bool operator!= (UInt64 iRhs) const { return !(*this == iRhs); };
  bool operator!= (const BitVector & iRhs) const { return !(*this == iRhs); };
  bool operator!= (const PartSelect & iRhs) const { return !(*this == iRhs); };
  bool operator!= (long long unsigned int iRhs) const { return !(*this == iRhs); };
  bool operator!= (long long int iRhs) const { return !(*this == iRhs); };
  bool operator!= (Int64 iRhs) const { return !(*this == iRhs); };
  bool operator!= (int iRhs) const { return !(*this == iRhs); };

  friend bool operator!= (const PartSelect & iLhs,  const BitVector & iRhs);

  //================
  // TBD operators:
  //================
  // TBD - Create copy constructor.
  // Binary arithmetic operators should return a copy, not a reference to the first operand.
  // Move the binary operators outside and base them on the compound operators (+=, etc).
  // Implement both sides of binary operators.
  // operator!=
  // operator!
  // operator&&
  // operator||
  // operator<=
  // operator>=
  // operator<
  // operator>
  // operator<<
  // operator<<=
  // operator>>
  // operator>>=
  // operator~
  // operator&
  // operator&=
  // operator|
  // operator|=
  // operator^
  // operator^=
  // operator, (concatination)
  // operator bool
  // operator UInt32 (make explicit if there are operator ambiguity problems)
  // operator UInt64
  // operator Int32
  // operator Int64
  // Add corresponding overloads for PartSelect so we can do things like BV0 = BV1(11,0) + BV2(3,0);

  // TBD - Add operators for interacting with strings for 4-state values.

};

BitVector operator+ (const BitVector & iLhs, UInt32 iRhs);
BitVector operator+ (const BitVector & iLhs, UInt64 iRhs);
BitVector operator+ (const BitVector & iLhs, const BitVector & iRhs);
BitVector operator+ (const BitVector & iLhs, const BitVector::PartSelect & iRhs);
BitVector operator+ (UInt64 iLhs, const BitVector & iRhs);
BitVector operator+ (const BitVector::PartSelect & iLhs, const BitVector & iRhs);
BitVector operator+ (const BitVector & iLhs, long long unsigned int iRhs);
BitVector operator+ (const BitVector & iLhs, long long int iRhs);
BitVector operator+ (const BitVector & iLhs, Int64 iRhs);
BitVector operator+ (const BitVector & iLhs, int iRhs);

BitVector operator- (const BitVector & iLhs, UInt32 iRhs);
BitVector operator- (const BitVector & iLhs, UInt64 iRhs);
BitVector operator- (const BitVector & iLhs, const BitVector & iRhs);
BitVector operator- (const BitVector & iLhs, const BitVector::PartSelect & iRhs);
BitVector operator- (const BitVector & iLhs, long long unsigned int iRhs);
BitVector operator- (const BitVector & iLhs, long long int iRhs);
BitVector operator- (const BitVector & iLhs, Int64 iRhs);
BitVector operator- (const BitVector & iLhs, int iRhs);

bool operator== (UInt32 iLhs,                         const BitVector & iRhs);
bool operator== (UInt64 iLhs,                         const BitVector & iRhs);
bool operator== (const BitVector::PartSelect & iLhs,  const BitVector & iRhs);
bool operator== (long long unsigned int iLhs,         const BitVector & iRhs);
bool operator== (long long int iLhs,                  const BitVector & iRhs);
bool operator== (Int64 iLhs,                          const BitVector & iRhs);
bool operator== (int iLhs,                            const BitVector & iRhs);

bool operator!= (UInt32 iLhs,                         const BitVector & iRhs);
bool operator!= (UInt64 iLhs,                         const BitVector & iRhs);
bool operator!= (const BitVector::PartSelect & iLhs,  const BitVector & iRhs);
bool operator!= (long long unsigned int iLhs,         const BitVector & iRhs);
bool operator!= (long long int iLhs,                  const BitVector & iRhs);
bool operator!= (Int64 iLhs,                          const BitVector & iRhs);
bool operator!= (int iLhs,                            const BitVector & iRhs);

#endif /* BITVECTOR_H */

