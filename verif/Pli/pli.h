/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   pli.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/21/16
#   Description   :   
#
###############################################################################
*/
#ifndef PLI_H
#define PLI_H

#include "Common.h"
#include "vpi.h"

class Pli
{
  // Private Members
  private:

  // Public Properties (get/set)
  public:
  static const UInt32 Invalid_Scalar = 0xffffffff;

  // Public Methods
  public:
  static Vpi::SCALAR_VAL  GetScalar(vpiHandle iHndl);
  static Vpi::OBJECT      GetType(vpiHandle iHndl);

  // Private Methods
  private:

};

#endif /* PLI_H */

