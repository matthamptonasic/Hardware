/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   TypeBase.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/16/16
#   Description   :   
#
###############################################################################
*/
#ifndef TYPEBASE_H
#define TYPEBASE_H

#include <string>

#include "BitVector.h"
#include "Common.h"
#include "vpi.h"

using namespace std;

// Make this abstract. This will carry the BitVector (move it to this directory).
// Make a 2-state and 4-state abstract base as well.
// 2-state child will only have the 'bit' class.
// 4-state children will be logic, (typedef reg to be same as logic), & integer (32-bit BV).

// TBD - Make a list/map structure to register each signal that is attached to the rtl.
//       Warn the user if the same signal is attached multiple times.

class TypeBase 
{
  // Enums
  public:

  // Nested Classes
  protected:
  class PartSelect
  {
    friend class TypeBase;

    // Private Members
    private:
    UInt32 m_upperIndex;
    UInt32 m_lowerIndex;
    TypeBase * m_parent;

    // Public Properties (get/set)
    public:

    // Constructors
    public:
    PartSelect(TypeBase * iTB, UInt32 iUpperIndex, UInt32 iLowerIndex);

    // Inits
    private:

    // Public Methods
    public:

    // Private Methods
    private:

    // Operators
    public:
    PartSelect & operator= (UInt32 iRhs);
    PartSelect & operator= (UInt64 iRhs);
    PartSelect & operator= (BitVector & iRhs);
    PartSelect & operator= (BitVector && iRhs);
    PartSelect & operator= (const BitVector::PartSelect & iRhs);
    PartSelect & operator= (const TypeBase & iRhs)                  { return *this = (BitVector)iRhs; }
    PartSelect & operator= (const TypeBase::PartSelect & iRhs)      { return *this = (BitVector)iRhs; }
    PartSelect & operator= (long long unsigned int iRhs)            { return *this = (UInt64)iRhs; }
    PartSelect & operator= (long long int iRhs)                     { return *this = (UInt64)iRhs; }
    PartSelect & operator= (Int64 iRhs)                             { return *this = (UInt64)iRhs; }
    PartSelect & operator= (int iRhs)                               { return *this = (UInt32)iRhs; }

    UInt32 operator[] (UInt32 iWordIndex)                     const { return         (*m_parent->m_bv)[iWordIndex]; }
    explicit operator bool()                                  const { return   (bool)(*m_parent->m_bv); }
    explicit operator UInt32()                                const { return (UInt32)(*m_parent->m_bv); }
    explicit operator UInt64()                                const { return (UInt64)(*m_parent->m_bv); }
    explicit operator BitVector()                             const { return (BitVector)((*m_parent->m_bv)(m_upperIndex, m_lowerIndex)); }

    BitVector operator+= (UInt32 iRhs);
    BitVector operator+= (UInt64 iRhs);
    BitVector operator+= (const BitVector & iRhs);
    BitVector operator+= (const BitVector::PartSelect & iRhs)       { return *this += (BitVector)iRhs; }
    BitVector operator+= (const TypeBase & iRhs)                    { return *this += (BitVector)iRhs; }
    BitVector operator+= (const TypeBase::PartSelect & iRhs)        { return *this += (BitVector)iRhs; }
    BitVector operator+= (long long unsigned int iRhs)              { return *this += (UInt64)iRhs; }
    BitVector operator+= (long long int iRhs)                       { return *this += (UInt64)iRhs; }
    BitVector operator+= (Int64 iRhs)                               { return *this += (UInt64)iRhs; }
    BitVector operator+= (int iRhs)                                 { return *this += (UInt32)iRhs; }

    BitVector operator+  (UInt32 iRhs)                        const { return (BitVector)(*this) + iRhs; }
    BitVector operator+  (UInt64 iRhs)                        const { return (BitVector)(*this) + iRhs; }
    BitVector operator+  (const BitVector & iRhs)             const { return (BitVector)(*this) + iRhs; }
    BitVector operator+  (const BitVector::PartSelect & iRhs) const { return (BitVector)(*this) + (BitVector)iRhs; }
    BitVector operator+  (const TypeBase & iRhs)              const { return (BitVector)(*this) + (BitVector)iRhs; }
    BitVector operator+  (const TypeBase::PartSelect & iRhs)  const { return (BitVector)(*this) + (BitVector)iRhs; }
    BitVector operator+  (long long unsigned int iRhs)        const { return (BitVector)(*this) + (UInt64)iRhs; }
    BitVector operator+  (long long int iRhs)                 const { return (BitVector)(*this) + (UInt64)iRhs; }
    BitVector operator+  (Int64 iRhs)                         const { return (BitVector)(*this) + (UInt64)iRhs; }
    BitVector operator+  (int iRhs)                           const { return (BitVector)(*this) + (UInt32)iRhs; }

    BitVector operator++ ()                                         { return *this += 1; }
    BitVector operator++ (int iDummy)                               { return *this += 1; }

    BitVector operator-= (UInt32 iRhs);
    BitVector operator-= (UInt64 iRhs);
    BitVector operator-= (const BitVector & iRhs);
    BitVector operator-= (const BitVector::PartSelect & iRhs)       { return *this -= (BitVector)iRhs; }
    BitVector operator-= (const TypeBase & iRhs)                    { return *this -= (BitVector)iRhs; }
    BitVector operator-= (const TypeBase::PartSelect & iRhs)        { return *this -= (BitVector)iRhs; }
    BitVector operator-= (long long unsigned int iRhs)              { return *this -= (UInt64)iRhs; }
    BitVector operator-= (long long int iRhs)                       { return *this -= (UInt64)iRhs; }
    BitVector operator-= (Int64 iRhs)                               { return *this -= (UInt64)iRhs; }
    BitVector operator-= (int iRhs)                                 { return *this -= (UInt32)iRhs; }

    BitVector operator-  (UInt32 iRhs)                        const { return (BitVector)(*this) - iRhs; }
    BitVector operator-  (UInt64 iRhs)                        const { return (BitVector)(*this) - iRhs; }
    BitVector operator-  (const BitVector & iRhs)             const { return (BitVector)(*this) - iRhs; }
    BitVector operator-  (const BitVector::PartSelect & iRhs) const { return (BitVector)(*this) - (BitVector)iRhs; }
    BitVector operator-  (const TypeBase & iRhs)              const { return (BitVector)(*this) - (BitVector)iRhs; }
    BitVector operator-  (const TypeBase::PartSelect & iRhs)  const { return (BitVector)(*this) - (BitVector)iRhs; }
    BitVector operator-  (long long unsigned int iRhs)        const { return (BitVector)(*this) - (UInt64)iRhs; }
    BitVector operator-  (long long int iRhs)                 const { return (BitVector)(*this) - (UInt64)iRhs; }
    BitVector operator-  (Int64 iRhs)                         const { return (BitVector)(*this) - (UInt64)iRhs; }
    BitVector operator-  (int iRhs)                           const { return (BitVector)(*this) - (UInt32)iRhs; }

    BitVector operator-- ()                                         { return *this -= 1; }
    BitVector operator-- (int iDummy)                               { return *this -= 1; }

    bool operator== (UInt32 iRhs)                             const { return (BitVector)(*this) == iRhs; }
    bool operator== (UInt64 iRhs)                             const { return (BitVector)(*this) == iRhs; }
    bool operator== (const BitVector & iRhs)                  const { return (BitVector)(*this) == iRhs; }
    bool operator== (const BitVector::PartSelect & iRhs)      const { return (BitVector)(*this) == (BitVector)iRhs; }
    bool operator== (const TypeBase & iRhs)                   const { return (BitVector)(*this) == (BitVector)iRhs; }
    bool operator== (const TypeBase::PartSelect & iRhs)       const { return (BitVector)(*this) == (BitVector)iRhs; }
    bool operator== (long long unsigned int iRhs)             const { return (BitVector)(*this) == iRhs; }
    bool operator== (long long int iRhs)                      const { return (BitVector)(*this) == iRhs; }
    bool operator== (Int64 iRhs)                              const { return (BitVector)(*this) == iRhs; }
    bool operator== (int iRhs)                                const { return (BitVector)(*this) == iRhs; }

    bool operator!= (UInt32 iRhs)                             const { return (BitVector)(*this) != iRhs; }
    bool operator!= (UInt64 iRhs)                             const { return (BitVector)(*this) != iRhs; }
    bool operator!= (const BitVector & iRhs)                  const { return (BitVector)(*this) != iRhs; }
    bool operator!= (const BitVector::PartSelect & iRhs)      const { return (BitVector)(*this) != (BitVector)iRhs; }
    bool operator!= (const TypeBase & iRhs)                   const { return (BitVector)(*this) != (BitVector)iRhs; }
    bool operator!= (const TypeBase::PartSelect & iRhs)       const { return (BitVector)(*this) != (BitVector)iRhs; }
    bool operator!= (long long unsigned int iRhs)             const { return (BitVector)(*this) != iRhs; }
    bool operator!= (long long int iRhs)                      const { return (BitVector)(*this) != iRhs; }
    bool operator!= (Int64 iRhs)                              const { return (BitVector)(*this) != iRhs; }
    bool operator!= (int iRhs)                                const { return (BitVector)(*this) != iRhs; }

    bool operator<= (UInt32 iRhs)                             const { return (BitVector)(*this) <= iRhs; }
    bool operator<= (UInt64 iRhs)                             const { return (BitVector)(*this) <= iRhs; }
    bool operator<= (const BitVector & iRhs)                  const { return (BitVector)(*this) <= iRhs; }
    bool operator<= (const BitVector::PartSelect & iRhs)      const { return (BitVector)(*this) <= (BitVector)iRhs; }
    bool operator<= (const TypeBase & iRhs)                   const { return (BitVector)(*this) <= (BitVector)iRhs; }
    bool operator<= (const TypeBase::PartSelect & iRhs)       const { return (BitVector)(*this) <= (BitVector)iRhs; }
    bool operator<= (long long unsigned int iRhs)             const { return (BitVector)(*this) <= iRhs; }
    bool operator<= (long long int iRhs)                      const { return (BitVector)(*this) <= iRhs; }
    bool operator<= (Int64 iRhs)                              const { return (BitVector)(*this) <= iRhs; }
    bool operator<= (int iRhs)                                const { return (BitVector)(*this) <= iRhs; }

    bool operator>= (UInt32 iRhs)                             const { return (BitVector)(*this) >= iRhs; }
    bool operator>= (UInt64 iRhs)                             const { return (BitVector)(*this) >= iRhs; }
    bool operator>= (const BitVector & iRhs)                  const { return (BitVector)(*this) >= iRhs; }
    bool operator>= (const BitVector::PartSelect & iRhs)      const { return (BitVector)(*this) >= (BitVector)iRhs; }
    bool operator>= (const TypeBase & iRhs)                   const { return (BitVector)(*this) >= (BitVector)iRhs; }
    bool operator>= (const TypeBase::PartSelect & iRhs)       const { return (BitVector)(*this) >= (BitVector)iRhs; }
    bool operator>= (long long unsigned int iRhs)             const { return (BitVector)(*this) >= iRhs; }
    bool operator>= (long long int iRhs)                      const { return (BitVector)(*this) >= iRhs; }
    bool operator>= (Int64 iRhs)                              const { return (BitVector)(*this) >= iRhs; }
    bool operator>= (int iRhs)                                const { return (BitVector)(*this) >= iRhs; }

    bool operator<  (UInt32 iRhs)                             const { return (BitVector)(*this) < iRhs; }
    bool operator<  (UInt64 iRhs)                             const { return (BitVector)(*this) < iRhs; }
    bool operator<  (const BitVector & iRhs)                  const { return (BitVector)(*this) < iRhs; }
    bool operator<  (const BitVector::PartSelect & iRhs)      const { return (BitVector)(*this) < (BitVector)iRhs; }
    bool operator<  (const TypeBase & iRhs)                   const { return (BitVector)(*this) < (BitVector)iRhs; }
    bool operator<  (const TypeBase::PartSelect & iRhs)       const { return (BitVector)(*this) < (BitVector)iRhs; }
    bool operator<  (long long unsigned int iRhs)             const { return (BitVector)(*this) < iRhs; }
    bool operator<  (long long int iRhs)                      const { return (BitVector)(*this) < iRhs; }
    bool operator<  (Int64 iRhs)                              const { return (BitVector)(*this) < iRhs; }
    bool operator<  (int iRhs)                                const { return (BitVector)(*this) < iRhs; }

    bool operator>  (UInt32 iRhs)                             const { return (BitVector)(*this) > iRhs; }
    bool operator>  (UInt64 iRhs)                             const { return (BitVector)(*this) > iRhs; }
    bool operator>  (const BitVector & iRhs)                  const { return (BitVector)(*this) > iRhs; }
    bool operator>  (const BitVector::PartSelect & iRhs)      const { return (BitVector)(*this) > (BitVector)iRhs; }
    bool operator>  (const TypeBase & iRhs)                   const { return (BitVector)(*this) > (BitVector)iRhs; }
    bool operator>  (const TypeBase::PartSelect & iRhs)       const { return (BitVector)(*this) > (BitVector)iRhs; }
    bool operator>  (long long unsigned int iRhs)             const { return (BitVector)(*this) > iRhs; }
    bool operator>  (long long int iRhs)                      const { return (BitVector)(*this) > iRhs; }
    bool operator>  (Int64 iRhs)                              const { return (BitVector)(*this) > iRhs; }
    bool operator>  (int iRhs)                                const { return (BitVector)(*this) > iRhs; }

    BitVector operator<<= (UInt32 iRhs);
    BitVector operator<<= (const BitVector & iRhs)                  { return *this <<= iRhs[0]; }
    BitVector operator<<= (const BitVector::PartSelect & iRhs)      { return *this <<= ((BitVector)iRhs)[0]; }
    BitVector operator<<= (const TypeBase & iRhs)                   { return *this <<= ((BitVector)iRhs)[0]; }
    BitVector operator<<= (const TypeBase::PartSelect & iRhs)       { return *this <<= ((BitVector)iRhs)[0]; }
    BitVector operator<<= (int iRhs)                                { return *this <<= (UInt32)iRhs; }

    BitVector operator>>= (UInt32 iRhs);
    BitVector operator>>= (const BitVector & iRhs)                  { return *this >>= iRhs[0]; }
    BitVector operator>>= (const BitVector::PartSelect & iRhs)      { return *this >>= ((BitVector)iRhs)[0]; }
    BitVector operator>>= (const TypeBase & iRhs)                   { return *this >>= ((BitVector)iRhs)[0]; }
    BitVector operator>>= (const TypeBase::PartSelect & iRhs)       { return *this >>= ((BitVector)iRhs)[0]; }
    BitVector operator>>= (int iRhs)                                { return *this >>= (UInt32)iRhs; }

    BitVector operator<< (UInt32 iRhs)                        const { return (BitVector)(*this) << iRhs; }
    BitVector operator<< (const BitVector & iRhs)             const { return (BitVector)(*this) << iRhs; }
    BitVector operator<< (const BitVector::PartSelect & iRhs) const { return (BitVector)(*this) << (BitVector)iRhs; }
    BitVector operator<< (const TypeBase & iRhs)              const { return (BitVector)(*this) << (BitVector)iRhs; }
    BitVector operator<< (const TypeBase::PartSelect & iRhs)  const { return (BitVector)(*this) << (BitVector)iRhs; }
    BitVector operator<< (int iRhs)                           const { return (BitVector)(*this) << iRhs; }

    BitVector operator>> (UInt32 iRhs)                        const { return (BitVector)(*this) >> iRhs; }
    BitVector operator>> (const BitVector & iRhs)             const { return (BitVector)(*this) >> iRhs; }
    BitVector operator>> (const BitVector::PartSelect & iRhs) const { return (BitVector)(*this) >> (BitVector)iRhs; }
    BitVector operator>> (const TypeBase & iRhs)              const { return (BitVector)(*this) >> (BitVector)iRhs; }
    BitVector operator>> (const TypeBase::PartSelect & iRhs)  const { return (BitVector)(*this) >> (BitVector)iRhs; }
    BitVector operator>> (int iRhs)                           const { return (BitVector)(*this) >> iRhs; }

    BitVector operator~  ()                                   const { return ~((BitVector)(*this)); }

    BitVector operator&= (UInt32 iRhs);
    BitVector operator&= (UInt64 iRhs);
    BitVector operator&= (const BitVector & iRhs);
    BitVector operator&= (const BitVector::PartSelect & iRhs)       { return  *this &= ((BitVector)iRhs); }
    BitVector operator&= (const TypeBase & iRhs)                    { return  *this &= ((BitVector)iRhs); }
    BitVector operator&= (const TypeBase::PartSelect & iRhs)        { return  *this &= ((BitVector)iRhs); }
    BitVector operator&= (long long unsigned int iRhs)              { return  *this &= (UInt64)iRhs; }
    BitVector operator&= (long long int iRhs)                       { return  *this &= (UInt64)iRhs; }
    BitVector operator&= (Int64 iRhs)                               { return  *this &= (UInt64)iRhs; }
    BitVector operator&= (int iRhs)                                 { return  *this &= (UInt32)iRhs; }

    BitVector operator&  (UInt32 iRhs)                        const { return (BitVector)(*this) & iRhs; }
    BitVector operator&  (UInt64 iRhs)                        const { return (BitVector)(*this) & iRhs; }
    BitVector operator&  (const BitVector & iRhs)             const { return (BitVector)(*this) & iRhs; }
    BitVector operator&  (const BitVector::PartSelect & iRhs) const { return (BitVector)(*this) & (BitVector)iRhs; }
    BitVector operator&  (const TypeBase & iRhs)              const { return (BitVector)(*this) & (BitVector)iRhs; }
    BitVector operator&  (const TypeBase::PartSelect & iRhs)  const { return (BitVector)(*this) & (BitVector)iRhs; }
    BitVector operator&  (long long unsigned int iRhs)        const { return (BitVector)(*this) & iRhs; }
    BitVector operator&  (long long int iRhs)                 const { return (BitVector)(*this) & iRhs; }
    BitVector operator&  (Int64 iRhs)                         const { return (BitVector)(*this) & iRhs; }
    BitVector operator&  (int iRhs)                           const { return (BitVector)(*this) & iRhs; }

    BitVector operator|= (UInt32 iRhs);
    BitVector operator|= (UInt64 iRhs);
    BitVector operator|= (const BitVector & iRhs);
    BitVector operator|= (const BitVector::PartSelect & iRhs)       { return *this |= ((BitVector)iRhs); }
    BitVector operator|= (const TypeBase & iRhs)                    { return *this |= ((BitVector)iRhs); }
    BitVector operator|= (const TypeBase::PartSelect & iRhs)        { return *this |= ((BitVector)iRhs); }
    BitVector operator|= (long long unsigned int iRhs)              { return *this |= (UInt64)iRhs; }
    BitVector operator|= (long long int iRhs)                       { return *this |= (UInt64)iRhs; }
    BitVector operator|= (Int64 iRhs)                               { return *this |= (UInt64)iRhs; }
    BitVector operator|= (int iRhs)                                 { return *this |= (UInt32)iRhs; }

    BitVector operator|  (UInt32 iRhs)                        const { return (BitVector)(*this) | iRhs; }
    BitVector operator|  (UInt64 iRhs)                        const { return (BitVector)(*this) | iRhs; }
    BitVector operator|  (const BitVector & iRhs)             const { return (BitVector)(*this) | iRhs; }
    BitVector operator|  (const BitVector::PartSelect & iRhs) const { return (BitVector)(*this) | (BitVector)iRhs; }
    BitVector operator|  (const TypeBase & iRhs)              const { return (BitVector)(*this) | (BitVector)iRhs; }
    BitVector operator|  (const TypeBase::PartSelect & iRhs)  const { return (BitVector)(*this) | (BitVector)iRhs; }
    BitVector operator|  (long long unsigned int iRhs)        const { return (BitVector)(*this) | iRhs; }
    BitVector operator|  (long long int iRhs)                 const { return (BitVector)(*this) | iRhs; }
    BitVector operator|  (Int64 iRhs)                         const { return (BitVector)(*this) | iRhs; }
    BitVector operator|  (int iRhs)                           const { return (BitVector)(*this) | iRhs; }

    BitVector operator^= (UInt32 iRhs);
    BitVector operator^= (UInt64 iRhs);
    BitVector operator^= (const BitVector & iRhs);
    BitVector operator^= (const BitVector::PartSelect & iRhs)       { return  *this ^= ((BitVector)iRhs); }
    BitVector operator^= (const TypeBase & iRhs)                    { return  *this ^= ((BitVector)iRhs); }
    BitVector operator^= (const TypeBase::PartSelect & iRhs)        { return  *this ^= ((BitVector)iRhs); }
    BitVector operator^= (long long unsigned int iRhs)              { return  *this ^= (UInt64)iRhs; }
    BitVector operator^= (long long int iRhs)                       { return  *this ^= (UInt64)iRhs; }
    BitVector operator^= (Int64 iRhs)                               { return  *this ^= (UInt64)iRhs; }
    BitVector operator^= (int iRhs)                                 { return  *this ^= (UInt32)iRhs; }

    BitVector operator^  (UInt32 iRhs)                        const { return (BitVector)(*this) ^ iRhs; }
    BitVector operator^  (UInt64 iRhs)                        const { return (BitVector)(*this) ^ iRhs; }
    BitVector operator^  (const BitVector & iRhs)             const { return (BitVector)(*this) ^ iRhs; }
    BitVector operator^  (const BitVector::PartSelect & iRhs) const { return (BitVector)(*this) ^ (BitVector)iRhs; }
    BitVector operator^  (const TypeBase & iRhs)              const { return (BitVector)(*this) ^ (BitVector)iRhs; }
    BitVector operator^  (const TypeBase::PartSelect & iRhs)  const { return (BitVector)(*this) ^ (BitVector)iRhs; }
    BitVector operator^  (long long unsigned int iRhs)        const { return (BitVector)(*this) ^ iRhs; }
    BitVector operator^  (long long int iRhs)                 const { return (BitVector)(*this) ^ iRhs; }
    BitVector operator^  (Int64 iRhs)                         const { return (BitVector)(*this) ^ iRhs; }
    BitVector operator^  (int iRhs)                           const { return (BitVector)(*this) ^ iRhs; }

    BitVector operator,  (UInt32 iRhs)                        const { return (BitVector)(*this) , iRhs; }
    BitVector operator,  (UInt64 iRhs)                        const { return (BitVector)(*this) , iRhs; }
    BitVector operator,  (const BitVector & iRhs)             const { return (BitVector)(*this) , iRhs; }
    BitVector operator,  (const BitVector::PartSelect & iRhs) const { return (BitVector)(*this) , (BitVector)iRhs; }
    BitVector operator,  (const TypeBase & iRhs)              const { return (BitVector)(*this) , (BitVector)iRhs; }
    BitVector operator,  (const TypeBase::PartSelect & iRhs)  const { return (BitVector)(*this) , (BitVector)iRhs; }
    BitVector operator,  (long long unsigned int iRhs)        const { return (BitVector)(*this) , iRhs; }
    BitVector operator,  (long long int iRhs)                 const { return (BitVector)(*this) , iRhs; }
    BitVector operator,  (Int64 iRhs)                         const { return (BitVector)(*this) , iRhs; }
    BitVector operator,  (int iRhs)                           const { return (BitVector)(*this) , iRhs; }
  };

  // Private Members
  private:
    string                m_name;
    string                m_nameFull;
    vpiHandle             m_sigHandle;
    vpiHandle             m_callBackHandle;
    UInt32                m_size;
    BitVector *           m_bv;
    bool                  m_maskValChange;

    NB_STATES             m_nbStates;

  // Protected Properties
  protected:
    vpiHandle   get_SigHandle() const   { return m_sigHandle; }
    BitVector & get_BitVector() const   { return *m_bv; };
    void        set_Size(UInt32 iSize)  { m_size = iSize; }

  // Public Properties
  public:
    string      Get_Name() const      { return m_name; }
    string      Get_NameFull() const  { return m_nameFull; }
    UInt32      Get_Size() const      { return m_size; }

  // Constructors
  public:
    TypeBase(string iFullName, NB_STATES iStates);
    ~TypeBase();

  // Inits
  private:
    bool init(string iFullName, NB_STATES iStates);

  // Public Methods
  public:
    BitVector Get_Value();
    string    ToString() const;
    void      Print() const;

  // Private Methods
  private:
    void          registerValChangeCB();
    void          unregisterValChangeCB();
    Int32         valueChangedCB(Vpi::t_cb_data * iData);
    static  Int32 s_valueChangedCB(Vpi::t_cb_data * iData);

  // Protected Methods
  protected:
    virtual void set_Size() = 0;
    bool set_Handle();
    void createBV();
    void get_RtlValue();
    void set_RtlValue();

  // Operators
  public:
    TypeBase::PartSelect operator() (UInt32 iUpperIndex, UInt32 iLowerIndex);
    UInt32 operator[] (UInt32 iWordIndex)                       const { return         (*m_bv)[iWordIndex]; }
    explicit operator bool()                                    const { return   (bool)(*m_bv); }
    explicit operator UInt32()                                  const { return (UInt32)(*m_bv); }
    explicit operator UInt64()                                  const { return (UInt64)(*m_bv); }
    explicit operator BitVector()                               const { return *m_bv; }

    TypeBase & operator+= (UInt32 iRhs)                               { (*m_bv) += iRhs; set_RtlValue(); return *this; }
    TypeBase & operator+= (UInt64 iRhs)                               { (*m_bv) += iRhs; set_RtlValue(); return *this; }
    TypeBase & operator+= (const BitVector & iRhs)                    { (*m_bv) += iRhs; set_RtlValue(); return *this; }
    TypeBase & operator+= (const BitVector::PartSelect & iRhs)        { return *this += (BitVector)iRhs; }
    TypeBase & operator+= (const TypeBase & iRhs)                     { (*m_bv) += (*iRhs.m_bv); set_RtlValue(); return *this; }
    TypeBase & operator+= (const TypeBase::PartSelect & iRhs)         { return *this += (BitVector)iRhs; }
    TypeBase & operator+= (long long unsigned int iRhs)               { return *this += (UInt64)iRhs; }
    TypeBase & operator+= (long long int iRhs)                        { return *this += (UInt64)iRhs; }
    TypeBase & operator+= (Int64 iRhs)                                { return *this += (UInt64)iRhs; }
    TypeBase & operator+= (int iRhs)                                  { return *this += (UInt32)iRhs; }

    BitVector  operator+  (UInt32 iRhs)                         const { return (*m_bv) + iRhs; }
    BitVector  operator+  (UInt64 iRhs)                         const { return (*m_bv) + iRhs; }
    BitVector  operator+  (const BitVector & iRhs)              const { return (*m_bv) + iRhs; }
    BitVector  operator+  (const BitVector::PartSelect & iRhs)  const { return *this + (BitVector)iRhs; }
    BitVector  operator+  (const TypeBase & iRhs)               const { return (*m_bv) + (*iRhs.m_bv); }
    BitVector  operator+  (const TypeBase::PartSelect & iRhs)   const { return *this + (BitVector)iRhs; }
    BitVector  operator+  (long long unsigned int iRhs)         const { return *this + (UInt64)iRhs; }
    BitVector  operator+  (long long int iRhs)                  const { return *this + (UInt64)iRhs; }
    BitVector  operator+  (Int64 iRhs)                          const { return *this + (UInt64)iRhs; }
    BitVector  operator+  (int iRhs)                            const { return *this + (UInt32)iRhs; }

    TypeBase & operator++ ()                                          { return *this += 1; }
    TypeBase & operator++ (int iDummy)                                { return *this += 1; }

    TypeBase & operator-= (UInt32 iRhs)                               { (*m_bv) -= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator-= (UInt64 iRhs)                               { (*m_bv) -= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator-= (const BitVector & iRhs)                    { (*m_bv) -= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator-= (const BitVector::PartSelect & iRhs)        { return *this -= (BitVector)iRhs; }
    TypeBase & operator-= (const TypeBase & iRhs)                     { (*m_bv) -= (*iRhs.m_bv); set_RtlValue(); return *this; }
    TypeBase & operator-= (const TypeBase::PartSelect & iRhs)         { return *this -= (BitVector)iRhs; }
    TypeBase & operator-= (long long unsigned int iRhs)               { return *this -= (UInt64)iRhs; }
    TypeBase & operator-= (long long int iRhs)                        { return *this -= (UInt64)iRhs; }
    TypeBase & operator-= (Int64 iRhs)                                { return *this -= (UInt64)iRhs; }
    TypeBase & operator-= (int iRhs)                                  { return *this -= (UInt32)iRhs; }

    BitVector  operator-  (UInt32 iRhs)                         const { return *m_bv - iRhs; }
    BitVector  operator-  (UInt64 iRhs)                         const { return *m_bv - iRhs; }
    BitVector  operator-  (const BitVector & iRhs)              const { return *m_bv - iRhs; }
    BitVector  operator-  (const BitVector::PartSelect & iRhs)  const { return *this - (BitVector)iRhs; }
    BitVector  operator-  (const TypeBase & iRhs)               const { return *m_bv - *iRhs.m_bv; }
    BitVector  operator-  (const TypeBase::PartSelect & iRhs)   const { return *this - (BitVector)iRhs; }
    BitVector  operator-  (long long unsigned int iRhs)         const { return *this - (UInt64)iRhs; }
    BitVector  operator-  (long long int iRhs)                  const { return *this - (UInt64)iRhs; }
    BitVector  operator-  (Int64 iRhs)                          const { return *this - (UInt64)iRhs; }
    BitVector  operator-  (int iRhs)                            const { return *this - (UInt32)iRhs; }

    TypeBase & operator-- ()                                          { return *this -= 1; }
    TypeBase & operator-- (int iDummy)                                { return *this -= 1; }

    bool operator== (UInt32 iRhs)                               const { return *m_bv == iRhs; }
    bool operator== (UInt64 iRhs)                               const { return *m_bv == iRhs; }
    bool operator== (const BitVector & iRhs)                    const { return *m_bv == iRhs; }
    bool operator== (const BitVector::PartSelect & iRhs)        const { return *this == (BitVector)iRhs; }
    bool operator== (const TypeBase & iRhs)                     const { return *m_bv == *iRhs.m_bv; }
    bool operator== (const TypeBase::PartSelect & iRhs)         const { return *this == (BitVector)iRhs; }
    bool operator== (long long unsigned int iRhs)               const { return *this == (UInt64)iRhs; }
    bool operator== (long long int iRhs)                        const { return *this == (UInt64)iRhs; }
    bool operator== (Int64 iRhs)                                const { return *this == (UInt64)iRhs; }
    bool operator== (int iRhs)                                  const { return *this == (UInt32)iRhs; }

    bool operator!= (UInt32 iRhs)                               const { return !(*m_bv == iRhs); }
    bool operator!= (UInt64 iRhs)                               const { return !(*m_bv == iRhs); }
    bool operator!= (const BitVector & iRhs)                    const { return !(*m_bv == iRhs); }
    bool operator!= (const BitVector::PartSelect & iRhs)        const { return  (*this != (BitVector)iRhs); }
    bool operator!= (const TypeBase & iRhs)                     const { return  (*m_bv != *iRhs.m_bv); }
    bool operator!= (const TypeBase::PartSelect & iRhs)         const { return  (*this != (BitVector)iRhs); }
    bool operator!= (long long unsigned int iRhs)               const { return  (*this != (UInt64)iRhs); }
    bool operator!= (long long int iRhs)                        const { return  (*this != (UInt64)iRhs); }
    bool operator!= (Int64 iRhs)                                const { return  (*this != (UInt64)iRhs); }
    bool operator!= (int iRhs)                                  const { return  (*this != (UInt32)iRhs); }

    bool operator<= (UInt32 iRhs)                               const { return *m_bv <= iRhs; }
    bool operator<= (UInt64 iRhs)                               const { return *m_bv <= iRhs; }
    bool operator<= (const BitVector & iRhs)                    const { return *m_bv <= iRhs; }
    bool operator<= (const BitVector::PartSelect & iRhs)        const { return *this <= (BitVector)iRhs; }
    bool operator<= (const TypeBase & iRhs)                     const { return *m_bv <= *iRhs.m_bv; }
    bool operator<= (const TypeBase::PartSelect & iRhs)         const { return *this <= (BitVector)iRhs; }
    bool operator<= (long long unsigned int iRhs)               const { return *this <= (UInt64)iRhs; }
    bool operator<= (long long int iRhs)                        const { return *this <= (UInt64)iRhs; }
    bool operator<= (Int64 iRhs)                                const { return *this <= (UInt64)iRhs; }
    bool operator<= (int iRhs)                                  const { return *this <= (UInt32)iRhs; }

    bool operator>= (UInt32 iRhs)                               const { return *m_bv >= iRhs; }
    bool operator>= (UInt64 iRhs)                               const { return *m_bv >= iRhs; }
    bool operator>= (const BitVector & iRhs)                    const { return *m_bv >= iRhs; }
    bool operator>= (const BitVector::PartSelect & iRhs)        const { return *this >= (BitVector)iRhs; }
    bool operator>= (const TypeBase & iRhs)                     const { return *m_bv >= *iRhs.m_bv; }
    bool operator>= (const TypeBase::PartSelect & iRhs)         const { return *this >= (BitVector)iRhs; }
    bool operator>= (long long unsigned int iRhs)               const { return *this >= (UInt64)iRhs; }
    bool operator>= (long long int iRhs)                        const { return *this >= (UInt64)iRhs; }
    bool operator>= (Int64 iRhs)                                const { return *this >= (UInt64)iRhs; }
    bool operator>= (int iRhs)                                  const { return *this >= (UInt32)iRhs; }

    bool operator>  (UInt32 iRhs)                               const { return *m_bv > iRhs; }
    bool operator>  (UInt64 iRhs)                               const { return *m_bv > iRhs; }
    bool operator>  (const BitVector & iRhs)                    const { return *m_bv > iRhs; }
    bool operator>  (const BitVector::PartSelect & iRhs)        const { return *this > (BitVector)iRhs; }
    bool operator>  (const TypeBase & iRhs)                     const { return *m_bv > *iRhs.m_bv; }
    bool operator>  (const TypeBase::PartSelect & iRhs)         const { return *this > (BitVector)iRhs; }
    bool operator>  (long long unsigned int iRhs)               const { return *this > (UInt64)iRhs; }
    bool operator>  (long long int iRhs)                        const { return *this > (UInt64)iRhs; }
    bool operator>  (Int64 iRhs)                                const { return *this > (UInt64)iRhs; }
    bool operator>  (int iRhs)                                  const { return *this > (UInt32)iRhs; }

    bool operator<  (UInt32 iRhs)                               const { return *m_bv < iRhs; }
    bool operator<  (UInt64 iRhs)                               const { return *m_bv < iRhs; }
    bool operator<  (const BitVector & iRhs)                    const { return *m_bv < iRhs; }
    bool operator<  (const BitVector::PartSelect & iRhs)        const { return *this < (BitVector)iRhs; }
    bool operator<  (const TypeBase & iRhs)                     const { return *m_bv < *iRhs.m_bv; }
    bool operator<  (const TypeBase::PartSelect & iRhs)         const { return *this < (BitVector)iRhs; }
    bool operator<  (long long unsigned int iRhs)               const { return *this < (UInt64)iRhs; }
    bool operator<  (long long int iRhs)                        const { return *this < (UInt64)iRhs; }
    bool operator<  (Int64 iRhs)                                const { return *this < (UInt64)iRhs; }
    bool operator<  (int iRhs)                                  const { return *this < (UInt32)iRhs; }

    TypeBase & operator<<= (UInt32 iRhs)                              { *m_bv <<= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator<<= (const BitVector & iRhs)                   { *m_bv <<= iRhs[0]; set_RtlValue(); return *this; }
    TypeBase & operator<<= (const BitVector::PartSelect & iRhs)       { *m_bv <<= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator<<= (const TypeBase & iRhs)                    { *m_bv <<= (UInt32)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator<<= (const TypeBase::PartSelect & iRhs)        { *m_bv <<= (BitVector)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator<<= (int iRhs)                                 { *m_bv <<= (UInt32)iRhs; set_RtlValue(); return *this; }

    TypeBase & operator>>= (UInt32 iRhs)                              { *m_bv >>= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator>>= (const BitVector & iRhs)                   { *m_bv >>= iRhs[0]; set_RtlValue(); return *this; }
    TypeBase & operator>>= (const BitVector::PartSelect & iRhs)       { *m_bv >>= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator>>= (const TypeBase & iRhs)                    { *m_bv >>= (UInt32)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator>>= (const TypeBase::PartSelect & iRhs)        { *m_bv >>= (BitVector)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator>>= (int iRhs)                                 { *m_bv >>= (UInt32)iRhs; set_RtlValue(); return *this; }

    BitVector  operator<< (UInt32 iRhs)                         const { return *m_bv << iRhs; }
    BitVector  operator<< (const BitVector & iRhs)              const { return *m_bv << iRhs[0]; }
    BitVector  operator<< (const BitVector::PartSelect & iRhs)  const { return *m_bv << ((BitVector)iRhs)[0]; }
    BitVector  operator<< (const TypeBase & iRhs)               const { return *m_bv << (UInt32)iRhs; }
    BitVector  operator<< (const TypeBase::PartSelect & iRhs)   const { return *m_bv << ((BitVector)iRhs)[0]; }
    BitVector  operator<< (int iRhs)                            const { return *m_bv << (UInt32)iRhs; }

    BitVector  operator>> (UInt32 iRhs)                         const { return *m_bv >> iRhs; }
    BitVector  operator>> (const BitVector & iRhs)              const { return *m_bv >> iRhs[0]; }
    BitVector  operator>> (const BitVector::PartSelect & iRhs)  const { return *m_bv >> ((BitVector)iRhs)[0]; }
    BitVector  operator>> (const TypeBase & iRhs)               const { return *m_bv >> (UInt32)iRhs; }
    BitVector  operator>> (const TypeBase::PartSelect & iRhs)   const { return *m_bv >> ((BitVector)iRhs)[0]; }
    BitVector  operator>> (int iRhs)                            const { return *m_bv >> (UInt32)iRhs; }

    BitVector  operator~  ()                                    const { return ~(*m_bv); }

    TypeBase & operator&= (UInt32 iRhs)                               { *m_bv &= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator&= (UInt64 iRhs)                               { *m_bv &= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator&= (const BitVector & iRhs)                    { *m_bv &= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator&= (const BitVector::PartSelect & iRhs)        { *m_bv &= (BitVector)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator&= (const TypeBase & iRhs)                     { *m_bv &= *iRhs.m_bv; set_RtlValue(); return *this; }
    TypeBase & operator&= (const TypeBase::PartSelect & iRhs)         { *m_bv &= (BitVector)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator&= (long long unsigned int iRhs)               { *m_bv &= (UInt64)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator&= (long long int iRhs)                        { *m_bv &= (UInt64)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator&= (Int64 iRhs)                                { *m_bv &= (UInt64)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator&= (int iRhs)                                  { *m_bv &= (UInt32)iRhs; set_RtlValue(); return *this; }

    BitVector  operator&  (UInt32 iRhs)                         const { return *m_bv & iRhs; }
    BitVector  operator&  (UInt64 iRhs)                         const { return *m_bv & iRhs; }
    BitVector  operator&  (const BitVector & iRhs)              const { return *m_bv & iRhs; }
    BitVector  operator&  (const BitVector::PartSelect & iRhs)  const { return *m_bv & ((BitVector)iRhs); }
    BitVector  operator&  (const TypeBase & iRhs)               const { return *m_bv & *iRhs.m_bv; }
    BitVector  operator&  (const TypeBase::PartSelect & iRhs)   const { return *m_bv & ((BitVector)iRhs); }
    BitVector  operator&  (long long unsigned int iRhs)         const { return *m_bv & (UInt64)iRhs; }
    BitVector  operator&  (long long int iRhs)                  const { return *m_bv & (UInt64)iRhs; }
    BitVector  operator&  (Int64 iRhs)                          const { return *m_bv & (UInt64)iRhs; }
    BitVector  operator&  (int iRhs)                            const { return *m_bv & (UInt32)iRhs; }

    TypeBase & operator|= (UInt32 iRhs)                               { *m_bv |= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator|= (UInt64 iRhs)                               { *m_bv |= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator|= (const BitVector & iRhs)                    { *m_bv |= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator|= (const BitVector::PartSelect & iRhs)        { *m_bv |= (BitVector)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator|= (const TypeBase & iRhs)                     { *m_bv |= *iRhs.m_bv; set_RtlValue(); return *this; }
    TypeBase & operator|= (const TypeBase::PartSelect & iRhs)         { *m_bv |= (BitVector)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator|= (long long unsigned int iRhs)               { *m_bv |= (UInt64)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator|= (long long int iRhs)                        { *m_bv |= (UInt64)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator|= (Int64 iRhs)                                { *m_bv |= (UInt64)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator|= (int iRhs)                                  { *m_bv |= (UInt32)iRhs; set_RtlValue(); return *this; }

    BitVector  operator|  (UInt32 iRhs)                         const { return *m_bv | iRhs; }
    BitVector  operator|  (UInt64 iRhs)                         const { return *m_bv | iRhs; }
    BitVector  operator|  (const BitVector & iRhs)              const { return *m_bv | iRhs; }
    BitVector  operator|  (const BitVector::PartSelect & iRhs)  const { return *m_bv | ((BitVector)iRhs); }
    BitVector  operator|  (const TypeBase & iRhs)               const { return *m_bv | *iRhs.m_bv; }
    BitVector  operator|  (const TypeBase::PartSelect & iRhs)   const { return *m_bv | ((BitVector)iRhs); }
    BitVector  operator|  (long long unsigned int iRhs)         const { return *m_bv | (UInt64)iRhs; }
    BitVector  operator|  (long long int iRhs)                  const { return *m_bv | (UInt64)iRhs; }
    BitVector  operator|  (Int64 iRhs)                          const { return *m_bv | (UInt64)iRhs; }
    BitVector  operator|  (int iRhs)                            const { return *m_bv | (UInt32)iRhs; }

    TypeBase & operator^= (UInt32 iRhs)                               { *m_bv ^= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator^= (UInt64 iRhs)                               { *m_bv ^= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator^= (const BitVector & iRhs)                    { *m_bv ^= iRhs; set_RtlValue(); return *this; }
    TypeBase & operator^= (const BitVector::PartSelect & iRhs)        { *m_bv ^= (BitVector)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator^= (const TypeBase & iRhs)                     { *m_bv ^= *iRhs.m_bv; set_RtlValue(); return *this; }
    TypeBase & operator^= (const TypeBase::PartSelect & iRhs)         { *m_bv ^= (BitVector)iRhs; set_RtlValue(); return *this; }
    TypeBase & operator^= (long long unsigned int iRhs)               { return *this ^= (UInt64)iRhs; }
    TypeBase & operator^= (long long int iRhs)                        { return *this ^= (UInt64)iRhs; }
    TypeBase & operator^= (Int64 iRhs)                                { return *this ^= (UInt64)iRhs; }
    TypeBase & operator^= (int iRhs)                                  { return *this ^= (UInt32)iRhs; }

    BitVector  operator^  (UInt32 iRhs)                         const { return *m_bv ^ iRhs; }
    BitVector  operator^  (UInt64 iRhs)                         const { return *m_bv ^ iRhs; }
    BitVector  operator^  (const BitVector & iRhs)              const { return *m_bv ^ iRhs; }
    BitVector  operator^  (const BitVector::PartSelect & iRhs)  const { return *m_bv ^ ((BitVector)iRhs); }
    BitVector  operator^  (const TypeBase & iRhs)               const { return *m_bv ^ *iRhs.m_bv; }
    BitVector  operator^  (const TypeBase::PartSelect & iRhs)   const { return *m_bv ^ ((BitVector)iRhs); }
    BitVector  operator^  (long long unsigned int iRhs)         const { return *m_bv ^ (UInt64)iRhs; }
    BitVector  operator^  (long long int iRhs)                  const { return *m_bv ^ (UInt64)iRhs; }
    BitVector  operator^  (Int64 iRhs)                          const { return *m_bv ^ (UInt64)iRhs; }
    BitVector  operator^  (int iRhs)                            const { return *m_bv ^ (UInt32)iRhs; }

    BitVector  operator,  (UInt32 iRhs)                         const { return (*m_bv , iRhs); }
    BitVector  operator,  (UInt64 iRhs)                         const { return (*m_bv , iRhs); }
    BitVector  operator,  (const BitVector & iRhs)              const { return (*m_bv , iRhs); }
    BitVector  operator,  (const BitVector::PartSelect & iRhs)  const { return (*this , ((BitVector)iRhs)); }
    BitVector  operator,  (const TypeBase & iRhs)               const { return (*m_bv , *iRhs.m_bv); }
    BitVector  operator,  (const TypeBase::PartSelect & iRhs)   const { return (*this , ((BitVector)iRhs)); }
    BitVector  operator,  (long long unsigned int iRhs)         const { return (*this , (UInt64)iRhs); }
    BitVector  operator,  (long long int iRhs)                  const { return (*this , (UInt64)iRhs); }
    BitVector  operator,  (Int64 iRhs)                          const { return (*this , (UInt64)iRhs); }
    BitVector  operator,  (int iRhs)                            const { return (*this , (UInt32)iRhs); }

};

#endif /* TYPEBASE_H */
