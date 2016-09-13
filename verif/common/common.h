/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   common.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/13/16
#   Description   :   Holds various basic/common definitions and structures.
#
###############################################################################
*/
#ifndef COMMON_H
#define COMMON_H

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

#endif /* COMMON_H */


