/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Logger.cc 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/27/16
#   Description   :   
#
###############################################################################
*/

#include "Logger.h"


// ==================================
// ===**    Logger Instance     **===
// ==================================
Logger * Logger::s_dout = nullptr;

// ==================================
// ===**     Static Scopes      **===
// ==================================
Logger::Scope Logger::s_Scope_NONE  ("Vrb_NONE",    Logger::Scope::Vrb_NONE());
Logger::Scope Logger::s_Scope_LOW   ("Vrb_LOW",     Logger::Scope::Vrb_LOW());
Logger::Scope Logger::s_Scope_MEDIUM("Vrb_MEDIUM",  Logger::Scope::Vrb_MEDIUM());
Logger::Scope Logger::s_Scope_HIGH  ("Vrb_HIGH",    Logger::Scope::Vrb_HIGH());
Logger::Scope Logger::s_Scope_FULL  ("Vrb_FULL",    Logger::Scope::Vrb_FULL());
Logger::Scope Logger::s_Scope_DEBUG ("Vrb_DEBUG",   Logger::Scope::Vrb_DEBUG());
Logger::Scope Logger::s_Scope_MSG   ("Vrb_MSG",     Logger::Scope::Vrb_MEDIUM());
Logger::Scope Logger::s_Scope_WRN   ("Vrb_WRN",     Logger::Scope::Vrb_LOW());
Logger::Scope Logger::s_Scope_ERR   ("Vrb_ERR",     Logger::Scope::Vrb_NONE());
Logger::Scope Logger::s_Scope_WRN_ENV ("Vrb_WRN_ENV", Logger::Scope::Vrb_LOW());
Logger::Scope Logger::s_Scope_ERR_ENV ("Vrb_ERR_ENV", Logger::Scope::Vrb_NONE());

// *==*==*==*==*==*==*==*==*==*==*==*==*
// ===**    Logger::Scope Class    **===
// *==*==*==*==*==*==*==*==*==*==*==*==*

// ==================================
// ===**  Scope Static Members  **===
// ==================================
// Static Message Verbosity Levels
UInt32 Logger::Scope::s_vrb_NONE    = 0;
UInt32 Logger::Scope::s_vrb_LOW     = 100;
UInt32 Logger::Scope::s_vrb_MEDIUM  = 200;
UInt32 Logger::Scope::s_vrb_HIGH    = 300;
UInt32 Logger::Scope::s_vrb_FULL    = 400;
UInt32 Logger::Scope::s_vrb_DEBUG   = 500;

UInt32 Logger::Scope::s_count = 0;

// =============================
// ===**    Constructor    **===
// =============================
Logger::Scope::Scope(string iName, UInt32 iLevel)
{
  m_name = iName;
  m_id = s_count++;
  m_level = iLevel;
}

// ====================================
// ===**  Private Static Members  **===
// ====================================

// =============================
// ===**      Inits        **===
// =============================

// =============================
// ===**  Public Methods   **===
// =============================

// =============================
// ===**  Private Methods  **===
// =============================

// =============================
// ===**     Operators     **===
// =============================

// *==*==*==*==*==*==*==*==*==*==*==*==*
// ===**       Logger Class        **===
// *==*==*==*==*==*==*==*==*==*==*==*==*

// ====================================
// ===**  Private Static Members  **===
// ====================================
Logger & Logger::GetDout()
{
  s_dout->m_currentScope = nullptr; 
  return *s_dout;
}

// =============================
// ===** Public Properties **===
// =============================
void Logger::ConsoleDisable_set(bool iValue)
{
  m_consoleUserDisabled = iValue;
  setConsoleEnable();
}
void Logger::FileDisable_set(bool iValue)
{
  m_fileUserDisabled = iValue;
  setFileEnable();
}

// =============================
// ===**   Constructors    **===
// =============================
Logger::Logger()
{
  init(string(""), nullptr);
}
Logger::Logger(string iFileName, ostream * iStream, UInt32 iVerbosityLevel)
{
  init(iFileName, iStream, iVerbosityLevel);
}

Logger::~Logger()
{
  if(m_fileOut != nullptr)
  {
    if(m_fileOut->is_open())
    {
      m_fileOut->flush();
      m_fileOut->close();
    }
    delete m_fileOut;
  }
}

// =============================
// ===**      Inits        **===
// =============================
void Logger::init(string iFileName, ostream * iStream, UInt32 iVerbosityLevel)
{
  m_initDone = false;
  m_fileOut = nullptr;
  if((iFileName != "") && (iStream != nullptr))
  {
    init_streams(iFileName, iStream);
    m_initDone = true;
  }
  m_fileName = "";
  m_alwaysLogScopes = true;
  m_logMsgLevel = Scope::Vrb_LOW();
  m_scopeLevel = iVerbosityLevel;

  m_testWarnings = 0;
  m_testErrors = 0;
  m_envWarnings = 0;
  m_envErrors = 0;

  m_prettyFunctionErrors = true;
  m_prettyFunctionWarnings = true;
}
void Logger::init_streams(string iFileName, ostream * iStream)
{
  SetConsoleOut(iStream);
  SetFileOut(iFileName);
}

// =============================
// ===**  Public Methods   **===
// =============================
void Logger::SetConsoleOut(ostream * iStream)
{
  m_consoleOut = iStream; // Default is cout.
  setConsoleEnable();
}
bool Logger::SetFileOut(ofstream * iStream)
{
  if((iStream != nullptr) && iStream->is_open())
  {
    m_fileOut = iStream;
    setFileEnable();
    return true;
  }
  setFileEnable();
  return false;
}
void Logger::SetFileOut(string iFileName)
{
  m_fileName = iFileName;
  if(m_fileOut != nullptr)
  {
    if(m_fileOut->is_open())
    {
      m_fileOut->flush();
      m_fileOut->close();
    }
    delete m_fileOut;
  }
  m_fileOut = new ofstream(m_fileName, ios::out | ios::trunc);
  (*m_fileOut) << "Logfile set to " << m_fileName << endl;
  (*m_consoleOut) << "Logfile set to " << m_fileName << endl;
  setFileEnable();
}
void Logger::AddDebugScope(const Scope & iScope)
{
  if(&iScope == NULL)
  {
    LOG_ERR_ENV << "Scope argument was null." << endl;
  }
  // Check if scope is already in the list.
  for(list<Scope>::iterator ii=m_scopes.begin(); ii != m_scopes.end(); ++ii)
  {
    if((*ii).GetId() == iScope.GetId())
    {
      return;
    }
  }
  m_scopes.push_back(iScope);
  m_scopeNameToIdMap[iScope.GetName()] = iScope.GetId();
  UInt32 id = iScope.GetId();
}
bool Logger::RemoveDebugScope(string iName)
{
  bool l_found = false;
  for(list<Scope>::iterator ii=m_scopes.begin(); ii != m_scopes.end(); ++ii)
  {
    if((*ii).GetName() == iName)
    {
      l_found = true;
      m_scopes.erase(ii);
      break;
    }
  }
  if(l_found)
  {
    map<string,UInt32>::iterator kk = m_scopeNameToIdMap.find(iName);
    if(kk != m_scopeNameToIdMap.end())
    {
      m_scopeNameToIdMap.erase(kk);
    }
  }
  return l_found;
}
void Logger::Flush()
{
  if((m_fileOut != nullptr) && (m_fileOut->is_open()))
  {
    m_fileOut->flush();
  }
}

// =============================
// ===**  Private Methods  **===
// =============================
void Logger::setConsoleEnable()
{
  if((m_consoleUserDisabled == false) && (m_consoleOut != nullptr))
  {
    m_consoleEnabled = true;
  }
  else
  {
    m_consoleEnabled = false;
  }
}
void Logger::setFileEnable()
{
  if((m_fileUserDisabled == false) && (m_fileOut != nullptr))
  {
    m_fileEnabled = true;
  }
  else
  {
    m_fileEnabled = false;
  }
}
bool Logger::checkScope()
{
  return checkScope(m_currentScope);
}
bool Logger::checkScope(const Scope * iScope)
{
  // Null scope means no scope means a level of m_logMsgLevel.
  // ex. Our logging verbosity level is set to Vrb_MEDIUM.
  //     Meaning we only want to print things with verbosity 
  //     Vrb_MEDIUM and below (down to Vrb_NONE).
  //     So if our current scope being tested (iScope)
  //     is LESS THAN (or equal to) our setting, return true to print it.

  if((iScope == nullptr) && (m_scopeLevel >= m_logMsgLevel))
  {
    return true;
  }
  // Check the scope level.
  if(iScope->GetLevel() <= m_scopeLevel)
  {
    return true;
  }
  // Check if we're always printing scopes in our list.
  if(m_alwaysLogScopes)
  {
    // Check that iScope is in our list.
    for(list<Scope>::iterator ii=m_scopes.begin(); ii != m_scopes.end(); ++ii)
    {
      if(ii->GetId() == iScope->GetId())
      {
        return true;
      }
    }
  }
  return false;
}

// =============================
// ===**     Operators     **===
// =============================
Logger & operator<< (Logger & iLog, char iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, signed char iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, unsigned char iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, const char * iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, const signed char * iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, const unsigned char * iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, const string iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, bool iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, Int16 iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, UInt16 iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, Int32 iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, UInt32 iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, Int64 iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, UInt64 iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, long long int iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, long long unsigned int iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, float iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, double iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, long double iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, void * iVal)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iVal;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iVal;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, ostream & (*manip)(ostream &))
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
	  manip(*(iLog.m_consoleOut));
  }
  if(iLog.m_fileEnabled)
  {
	  manip(*(iLog.m_fileOut));
  }
	return iLog;
}
Logger & operator<< (Logger & iLog, ios & (*manip)(ios &))
{
	if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
	  manip(*(iLog.m_consoleOut));
  }
  if(iLog.m_fileEnabled)
  {
	  manip(*(iLog.m_fileOut));
  }
	return iLog;
}
Logger & operator<< (Logger & iLog, ios_base & (*manip)(ios_base &))
{
	if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
	  manip(*(iLog.m_consoleOut));
  }
  if(iLog.m_fileEnabled)
  {
	  manip(*(iLog.m_fileOut));
  }
	return iLog;
}
Logger & operator<< (Logger & iLog, _Setw iSetw)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iSetw;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iSetw;
  }
  return iLog;
}
Logger & operator<< (Logger & iLog, _Setfill<char> iSetfill)
{
  if(!iLog.checkScope())
  {
    return iLog;
  }
  if(iLog.m_consoleEnabled)
  {
    (*(iLog.m_consoleOut)) << iSetfill;
  }
  if(iLog.m_fileEnabled)
  {
    (*(iLog.m_fileOut)) << iSetfill;
  }
  return iLog;
}

Logger & operator<< (Logger & iLog, Logger::Scope * iScope)
{
  iLog.m_currentScope = iScope;
  return iLog;
}
