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
Logger * dout;

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
  (*m_consoleOut) << __PRETTY_FUNCTION__ << " set." << endl;
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
  (*m_fileOut) << __PRETTY_FUNCTION__ << " set." << endl;
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

// =============================
// ===**  Private Methods  **===
// =============================

// =============================
// ===**     Operators     **===
// =============================
