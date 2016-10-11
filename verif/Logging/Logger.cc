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
//Logger * dout;
Logger * Logger::s_dout = nullptr;

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


// =============================
// ===**   Constructors    **===
// =============================
Logger::Logger()
{
  init(string(""), nullptr);
}
Logger::Logger(string iFileName, ostream * iStream)
{
  init(iFileName, iStream);
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
void Logger::init(string iFileName, ostream * iStream)
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
}
bool Logger::SetFileOut(ofstream * iStream)
{
  if(iStream->is_open())
  {
    m_fileOut = iStream;
    return true;
  }
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
}
void Logger::AddDebugScope(const Scope & iScope)
{
  if(&iScope == NULL)
  {
    //TBD - log error.
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

// =============================
// ===**     Operators     **===
// =============================
Logger & operator<< (Logger & iLog, bool iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, Int16 iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, UInt16 iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, Int32 iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, UInt32 iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, Int64 iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, UInt64 iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, long long int iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, long long unsigned int iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, float iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, double iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, long double iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, void * iVal)
{
  (*(iLog.m_consoleOut)) << iVal;
  (*(iLog.m_fileOut)) << iVal;
  return iLog;
}
Logger & operator<< (Logger & iLog, ostream & (*manip)(ostream &))
{
	manip(*(iLog.m_consoleOut));
	manip(*(iLog.m_fileOut));
	return iLog;
}
Logger & operator<< (Logger & iLog, ios & (*manip)(ios &))
{
	manip(*(iLog.m_consoleOut));
	manip(*(iLog.m_fileOut));
	return iLog;
}
Logger & operator<< (Logger & iLog, ios_base & (*manip)(ios_base &))
{
	manip(*(iLog.m_consoleOut));
	manip(*(iLog.m_fileOut));
	return iLog;
}
Logger & operator<< (Logger & iLog, _Setw iSetw)
{
  (*(iLog.m_consoleOut)) << iSetw;
  (*(iLog.m_fileOut)) << iSetw;
  return iLog;
}
Logger & operator<< (Logger & iLog, _Setfill<char> iSetfill)
{
  (*(iLog.m_consoleOut)) << iSetfill;
  (*(iLog.m_fileOut)) << iSetfill;
  return iLog;
}
