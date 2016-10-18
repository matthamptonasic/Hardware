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

#include <string>
#include <vector>

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
  static void             DollarFinish();
  static Vpi::SCALAR_VAL  GetScalar(vpiHandle iHndl);
  static Vpi::OBJECT      GetType(vpiHandle iHndl);
  static UInt32           GetVector(vpiHandle iHndl);
  static UInt32           GetSize(vpiHandle iHndl);
  static vector<string> * GetCommandLineArgs();

  // Private Methods
  private:
  static void             mergeStringCLArgs(vector<string> & ioArgs);
};

#endif /* PLI_H */

