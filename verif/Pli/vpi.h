/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   vpi_enums.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/18/16
#   Description   :   
#
###############################################################################
*/
#ifndef VPI_ENUMS_H
#define VPI_ENUMS_H

#include <string>

#include "vpi_user.h"

#include "Common.h"

using namespace std;

class Vpi
{

  // Enums
  public:
    enum class SYS_FUNC_TYPE : Int32
    {
      TASK = 1,
      FUNC = 2
    };

    enum class GET_VALUE_FORMAT : Int32
    {
      BIN_STR   = vpiBinStrVal,
      OCT_STR   = vpiOctStrVal,
      DEC_STR   = vpiDecStrVal,
      HEX_STR   = vpiHexStrVal,
      SCALAR    = vpiScalarVal,
      INT       = vpiIntVal,
      REAL      = vpiRealVal,
      STRING    = vpiStringVal,
      VECTOR    = vpiVectorVal,
      STRENGTH  = vpiStrengthVal,
      TIME      = vpiTimeVal,
      OBJ_TYPE  = vpiObjTypeVal,
      SUPPRESS  = vpiSuppressVal
    };

    enum class PROPERTY : Int32
    {
      UNDEFINED   = vpiUndefined,
      TYPE        = vpiType,
      NAME        = vpiName,
      FULL_NAME   = vpiFullName,
      SIZE        = vpiSize,
      FILE        = vpiFile,
      LINE_NO     = vpiLineNo,
      TOP_MODULE  = vpiTopModule,
      CELL_INST   = vpiCellInstance,
      DEF_NAME    = vpiDefName,
      TIME_UNIT   = vpiTimeUnit,
      TIME_PREC   = vpiTimePrecision,
      DEF_FILE    = vpiDefFile,
      DEF_LINE_NO = vpiDefLineNo,
      SCALAR      = vpiScalar,
      VECTOR      = vpiVector
    };

    enum class OBJECT : Int32
    {
      CONSTANT        = vpiConstant,
      FUNCTION        = vpiFunction,
      INTEGER_VAR     = vpiIntegerVar,
      ITERATOR        = vpiIterator,
      MEMORY          = vpiMemory,
      MEMORY_WORD     = vpiMemoryWord,
      MOD_PATH        = vpiModPath,
      MODULE          = vpiModule,
      NAMED_BEGIN     = vpiNamedBegin,
      NAMED_EVENT     = vpiNamedEvent,
      NAMED_FORK      = vpiNamedFork,
      NET             = vpiNet,
      PARAMETER       = vpiParameter,
      PART_SELECT     = vpiPartSelect,
      PATH_TERM       = vpiPathTerm,
      PORT            = vpiPort,
      REAL_VAR        = vpiRealVar,
      REG             = vpiReg,
      SYS_FUNC_CALL   = vpiSysFuncCall,
      SYS_TASK_CALL   = vpiSysTaskCall,
      TASK            = vpiTask,
      TIME_VAR        = vpiTimeVar,
      UDP_DEFN        = vpiUdpDefn,
      USER_SYS_TF     = vpiUserSystf,
      NET_ARRAY       = vpiNetArray,
      INDEX           = vpiIndex,
      LEFT_RANGE      = vpiLeftRange,
      PARENT          = vpiParent,
      RIGHT_RANGE     = vpiRightRange,
      SCOPE           = vpiScope,
      SYS_TF_CALL     = vpiSysTfCall,
      ARGUMENT        = vpiArgument,
      INTERNAL_SCOPE  = vpiInternalScope,
      MOD_PATH_IN     = vpiModPathIn,
      MOD_PATH_OUT    = vpiModPathOut,
      VARIABLES       = vpiVariables,
      EXPR            = vpiExpr
    };
    
  // Structs
  public:
    struct vpi_systf_data 
    {
      SYS_FUNC_TYPE type;
      Int32         sysfunctype;
      const char *  tfname;
      Int32 (*calltf)   (ICARUS_VPI_CONST PLI_BYTE8*);
      Int32 (*compiletf)(ICARUS_VPI_CONST PLI_BYTE8*);
      Int32 (*sizetf)   (PLI_BYTE8*);
      ICARUS_VPI_CONST PLI_BYTE8 *user_data;
    };

  // Public Methods
  static vpiHandle vpi_register_systf(const vpi_systf_data *ss);
};

#endif /* VPI_ENUMS_H */

