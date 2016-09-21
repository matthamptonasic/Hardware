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
#ifndef VPI_H
#define VPI_H

#include <string>
#include <stdarg.h>
#include <stdio.h>

#include "vpi_user.h"

#include "Common.h"

using namespace std;

class Vpi
{

  // Enums
  public:
    enum class SYS_TASK_FUNC : Int32
    {
      TASK = vpiSysTask,
      FUNC = vpiSysFunc
    };

    enum class VALUE_FORMAT : Int32
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
      VECTOR      = vpiVector,
      DIRECTION   = vpiDirection,
      NET_TYPE    = vpiNetType,
      ARRAY       = vpiArray,
      PORT_INDEX  = vpiPortIndex,
      EDGE        = vpiEdge,
      CONST_TYPE  = vpiConstType,
      FUNC_TYPE   = vpiFuncType,
      SYS_FUNC_TYPE = vpiSysFuncType,
      USER_DEFN   = vpiUserDefn,
      AUTOMATIC   = vpiAutomatic,
      CONST_SEL   = vpiConstantSelect,
      SIGNED      = vpiSigned,
      LOCAL_PARAM = vpiLocalParam
    };

    enum class DIRECTION : Int32
    {
      INPUT       = vpiInput,
      OUTPUT      = vpiOutput,
      INOUT       = vpiInout,
      MIXED_IO    = vpiMixedIO,
      NO_DIR      = vpiNoDirection
    };

    enum class NET_TYPE : Int32
    {
      WIRE      = vpiWire,
      WAND      = vpiWand,
      WOR       = vpiWor,
      TRI       = vpiTri,
      TRI_0     = vpiTri0,
      TRI_1     = vpiTri1,
      TRI_REG   = vpiTriReg,
      TRI_AND   = vpiTriAnd,
      TRI_OR    = vpiTriOr,
      SUPPLY_1  = vpiSupply1,
      SUPPLY_0  = vpiSupply0
    };

    enum class EDGE : Int32
    {
      NO_EDGE   = vpiNoEdge,
      EDGE_01   = vpiEdge01,
      EDGE_10   = vpiEdge10,
      EDGE_0x   = vpiEdge0x,
      EDGE_x1   = vpiEdgex1,
      EDGE_1x   = vpiEdge1x,
      EDGE_x0   = vpiEdgex0,
      POSEDGE   = vpiPosedge,
      NEGEDGE   = vpiNegedge,
      ANY_EDGE  = vpiAnyEdge
    };

    enum class CONST_TYPE : Int32
    {
      DEC     = vpiDecConst,
      REAL    = vpiRealConst,
      BINARY  = vpiBinaryConst,
      OCT     = vpiOctConst,
      HEX     = vpiHexConst,
      STRING  = vpiStringConst
    };

    enum class FUNC_TYPE : Int32
    {
      INT           = vpiIntFunc,
      REAL          = vpiRealFunc,
      TIME          = vpiTimeFunc,
      SIZED         = vpiSizedFunc,
      SIZED_SIGNED  = vpiSizedSignedFunc
    };

    enum class SYS_FUNC_TYPE : Int32
    {
      INT           = vpiSysFuncInt,
      REAL          = vpiSysFuncReal,
      TIME          = vpiSysFuncTime,
      SIZED         = vpiSysFuncSized
    };

    enum class DELAY_MODE : Int32
    {
      NO_DELAY              = vpiNoDelay,
      INERTIAL_DELAY        = vpiInertialDelay,
      TRANSPORT_DELAY       = vpiTransportDelay,
      PURE_TRANSPORT_DLEAY  = vpiPureTransportDelay,
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
      EXPR            = vpiExpr,
      CALLBACK        = vpiCallback,
      REG_ARRAY       = vpiRegArray,
      GEN_SCOPE       = vpiGenScope,
      UNKNOWN         = 0x7fffffff
    };
    
    enum class TIME_TYPE : Int32
    {
      SCALED_REAL_TIME  = vpiScaledRealTime,
      SIM_TIME          = vpiSimTime,
      SUPPRESS_TIME     = vpiSuppressTime
    };

    enum class SCALAR_VAL : Int32
    {
      ZERO      = vpi0,
      ONE       = vpi1,
      Z         = vpiZ,
      X         = vpiX,
      H         = vpiH,
      L         = vpiL,
      DONT_CARE = vpiDontCare
    };

    enum class STRENGTH_VAL : Int32
    {
      SUPPLY_DRIVE  = vpiSupplyDrive,
      STRONG_DRIVE  = vpiStrongDrive,
      PULL_DRIVE    = vpiPullDrive,
      LARGE_CHARGE  = vpiLargeCharge,
      WEAK_DRIVE    = vpiWeakDrive,
      MEDIUM_CHARGE = vpiMediumCharge,
      SMALL_CHARGE  = vpiSmallCharge,
      HI_Z          = vpiHiZ
    };

    enum class ERROR_STATE : Int32
    {
      COMPILE = vpiCompile,
      PLI     = vpiPLI,
      RUN     = vpiRun
    };
  
    enum class ERROR_LEVEL : Int32
    {
      NOTICE      = vpiNotice,
      WARNING     = vpiWarning,
      ERROR       = vpiError,
      SYSTEM      = vpiSystem,
      INTERNAL    = vpiInternal
    };
  
    enum class CB_REASON : Int32
    {
      VALUE_CHANGE              = cbValueChange,
      STMT                      = cbStmt,
      FORCE                     = cbForce,
      RELEASE                   = cbRelease,
      AT_START_OF_SIM_TIME      = cbAtStartOfSimTime,
      READ_WRITE_SYNCH          = cbReadWriteSynch,
      READ_ONLY_SYNCH           = cbReadOnlySynch,
      NEXT_SIM_TIME             = cbNextSimTime,
      AFTER_DELAY               = cbAfterDelay,
      END_OF_COMPILE            = cbEndOfCompile,
      START_OF_SIMULATION       = cbStartOfSimulation,
      END_OF_SIMULATION         = cbEndOfSimulation,
      ERROR                     = cbError,
      TCHK_VIOLATION            = cbTchkViolation,
      START_OF_SAVE             = cbStartOfSave,
      END_OF_SAVE               = cbEndOfSave,
      START_OF_RESTART          = cbStartOfRestart,
      END_OF_RESTART            = cbEndOfRestart,
      START_OF_RESET            = cbStartOfReset,
      END_OF_RESET              = cbEndOfReset,
      ENTER_INTERACTIVE         = cbEnterInteractive,
      EXIT_INTERACTIVE          = cbExitInteractive,
      INTERACTIVE_SCOPE_CHANGE  = cbInteractiveScopeChange,
      UNRESOLVED_SYSTF          = cbUnresolvedSystf
    };

    enum class CTL_OPERATION : Int32
    {
      STOP    = vpiStop,    // $stop
      FINISH  = vpiFinish,  // $finish
      RESET   = vpiReset,   // $reset
      SET_INTERACTIVE_SCOPE = vpiSetInteractiveScope,
      __IVL_LEGACY_STOP     = __ivl_legacy_vpiStop,
      __IVL_LEGACY_FINISH   = __ivl_legacy_vpiFinish
    };
  
  // Structs
  public:
    // For registering system task/functions with the simulator.
    typedef struct t_vpi_systf_data 
    {
      SYS_TASK_FUNC type;
      Int32         sysfunctype;
      const char *  tfname;
      Int32 (*calltf)   (ICARUS_VPI_CONST PLI_BYTE8*);
      Int32 (*compiletf)(ICARUS_VPI_CONST PLI_BYTE8*);
      Int32 (*sizetf)   (PLI_BYTE8*);
      ICARUS_VPI_CONST PLI_BYTE8 *user_data;
    } s_vpi_systf_data, *p_vpi_systf_data;

    // For retrieving information about the simulator command.
    // Used for processing custom command options.
    // e.g. vpi_get_vlog_info
    typedef struct t_vpi_vlog_info
    {
      Int32 argc;
      char  **argv;
      char  *product;
      char  *version;
    } s_vpi_vlog_info, *p_vpi_vlog_info;

    // Multi-purpose structure.
    // Used to hold time values when getting/setting verilog values (t_vpi_value).
    // Used to represent delay values (t_vpi_delay).
    // Used to get the module/simulation time (vpi_get_time).
    typedef struct t_vpi_time
    {
      TIME_TYPE type;
      UInt32    high;
      UInt32    low;
      double    real;
    } s_vpi_time, *p_vpi_time;

    // Used to represent a 4-state value being read or written.
    typedef struct t_vpi_vecval
    {
      // TBD - change to an enum (UInt64)?
      // Reading up on struct data alignment and this should be okay.
      // Since the pair of Int32's are the same size and a nominal alignment
      // of 4-bytes, no padding will ever be inserted between them.
      // And since a pair of Int32's and an Int64 have the same size,
      // they should be able to be cast back and forth without any issues.
      Int32 aval;
      Int32 bval;
    } s_vpi_vecval, *p_vpi_vecval;

    // Used to represent a drive strength value being read or written.
    typedef struct t_vpi_strengthval
    {
      Int32 logic;
      Int32 s0;
      Int32 s1;
    } s_vpi_strengthval, *p_vpi_strengthval;

    // Used to hold values that are passed back and forth between
    // the simulator and the application.
    typedef struct t_vpi_value
    {
      VALUE_FORMAT  format;
      union
      {
        char *              str;
        SCALAR_VAL          scalar;
        Int32               integer;
        double              real;
        t_vpi_time *        time;
        t_vpi_vecval *      vector;
        t_vpi_strengthval * strength;
        char *              misc;
      }                 value;
    } s_vpi_value, *p_vpi_value;

    // Used in various ways to represent delays.
    // No difference between this and the vpi_user struct.
    typedef struct t_vpi_delay
    {
      t_vpi_time *  da;
      Int32         no_of_delays;
      Int32         time_type;
      Int32         mtm_flag;
      Int32         append_flag;
      Int32         plusere_flag;
    } s_vpi_delay, *p_vpi_delay;

    typedef struct t_vpi_error_info
    {
      ERROR_STATE   state;
      ERROR_LEVEL   level;
      char *        message;
      char *        product;
      char *        code;
      char *        file;
      Int32         line;
    } s_vpi_error_info, *p_vpi_error_info;

    typedef struct t_cb_data
    {
      CB_REASON   reason;
      Int32       (*cb_rtn)(t_cb_data *cb);
      vpiHandle   obj;
      p_vpi_time  time;
      p_vpi_value value;
      Int32       index;
      char *      user_data;
    } s_cb_data, *p_cb_data;

  // Public Methods
  public:
  // System Task/Funcion Methods
  static vpiHandle  vpi_register_systf(const struct t_vpi_systf_data *ss);
  static void       vpi_get_systf_info(vpiHandle iSysTfHndl, p_vpi_systf_data oData);
  
  // I/O Methods
  static UInt32     vpi_mcd_open(char *iName);
  static UInt32     vpi_mcd_close(UInt32 iMcd);
  static char *     vpi_mcd_name(UInt32 iMcd);
  static Int32      vpi_mcd_printf(UInt32 iMcd, const char *iFmt, ...);
        __attribute__((format (printf,2,3)));
  static Int32      vpi_printf(const char *iFmt, ...);
        __attribute__((format (printf,1,2)));
  static Int32      vpi_vprintf(const char *iFmt, va_list iArgs);
  static Int32      vpi_mcd_vprintf(UInt32 iMcd, const char *iFmt, va_list iArgs);
  static Int32      vpi_flush();
  static Int32      vpi_mcd_flush(UInt32 iMcd);
  static Int32      vpi_fopen(const char *iName, const char *iMode);
  static FILE *     vpi_get_file(Int32 iFileDesc);

  // Callback Methods
  static vpiHandle  vpi_register_cb(p_cb_data iCbData);
  static Int32      vpi_remove_cb(vpiHandle iCbHndl);

  // Simulator Control Method
  // Note * The vpi_control method cannot be wrapped with varargs (...) so
  // this restrictive call will only support a single operator to be passed.
  // If multiple operators are needed, the global function must be called directly.
  static void       vpi_control(Int32 iOperation);
  // The proper name is vpi_control. vpi_sim_control is added for legacy.
  static void       vpi_sim_control(Int32 iOperation);

  // VPI Handle Methods
  static vpiHandle  vpi_handle(Vpi::OBJECT iType, vpiHandle iRef);
  static vpiHandle  vpi_iterate(Vpi::OBJECT iType, vpiHandle iRef);
  static vpiHandle  vpi_scan(vpiHandle iIterator);
  static vpiHandle  vpi_handle_by_index(vpiHandle iRef, Int32 iIndex);
  static vpiHandle  vpi_handle_by_name(const char *iName, vpiHandle iScope);

  // Get Value Methods
  static void       vpi_get_time(vpiHandle iObj, p_vpi_time oTime);
  static Int32      vpi_get(Vpi::PROPERTY iProperty, vpiHandle iRef);
  static char *     vpi_get_str(Vpi::PROPERTY iProperty, vpiHandle iRef);
  static void       vpi_get_value(vpiHandle iHndl, p_vpi_value oValue);

  // Set Value Methods
  static vpiHandle  vpi_put_value(vpiHandle   iObj, 
                                  p_vpi_value iValue,
                                  p_vpi_time  iWhen, 
                                  Int32       iFlags);

  // Misc Methods
  static Int32      vpi_free_object(vpiHandle iRef);
  static Int32      vpi_get_vlog_info(p_vpi_vlog_info oVlogInfo);
  static Int32      vpi_compare_objects(vpiHandle iObj1, vpiHandle iObj2);

  // Delay Methods
  static void       vpi_get_delays(vpiHandle iHndl, t_vpi_delay *oDelays);
  static void       vpi_put_delays(vpiHandle iHndl, t_vpi_delay *iDelays);

  // Custom User-Data Methods
  static Int32      vpi_put_userdata(vpiHandle iObj, void *iData);
  static void *     vpi_get_userdata(vpiHandle iObj);

  // Error Handling
  static Int32      vpi_chk_error(t_vpi_error_info *oInfo);
  
};

#endif /* VPI_H */

