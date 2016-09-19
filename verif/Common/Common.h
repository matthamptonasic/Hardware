/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Common.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/13/16
#   Description   :   Holds various basic/common definitions and structures.
#
###############################################################################
*/
#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

#ifndef UNUSED
# ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
# else
#  define UNUSED(x) UNUSED_ ## x
# endif
#endif

#ifndef UNUSED_FUNCTION
# ifdef __GNUC__
#  define UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_ ## x
# else
#  define UNUSED_FUNCTION(x) UNUSED_ ## x
# endif
#endif

// Types //
typedef std::uint8_t  Byte;
typedef std::int8_t   SByte;
typedef std::uint16_t UInt16;
typedef std::int16_t  Int16;
typedef std::uint32_t UInt32;
typedef std::int32_t  Int32;
typedef std::uint64_t UInt64;
typedef std::int64_t  Int64;

#endif /* COMMON_H */


