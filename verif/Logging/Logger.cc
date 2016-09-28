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
bool Logger::s_initDone = false;
bool Logger::s_alwaysLogScopes = true;
list<Logger::Scope> Logger::s_scopes;
map<string, UInt32> Logger::s_scopeNameToIdMap;

string Logger::s_fileName = "";
fstream Logger::s_oFile;

// =============================
// ===**      Inits        **===
// =============================
void Logger::Init(string iFileName)
{
  s_fileName = iFileName;
  s_oFile.open(s_fileName, ios::in | ios::out | ios::trunc);

  s_initDone = true;
}

// =============================
// ===**  Public Methods   **===
// =============================

// =============================
// ===**  Private Methods  **===
// =============================

// =============================
// ===**     Operators     **===
// =============================
void Logger::AddDebugScope(const Scope & iScope)
{
  if(&iScope == NULL)
  {
    //TBD - log error.
  }
  // Check if scope is already in the list.
  for(list<Scope>::iterator ii=s_scopes.begin(); ii != s_scopes.end(); ++ii)
  {
    if((*ii).GetId() == iScope.GetId())
    {
      return;
    }
  }
  s_scopes.push_back(iScope);
  s_scopeNameToIdMap[iScope.GetName()] = iScope.GetId();
  UInt32 id = iScope.GetId();
}
bool Logger::RemoveDebugScope(string iName)
{
  bool l_found = false;
  for(list<Scope>::iterator ii=s_scopes.begin(); ii != s_scopes.end(); ++ii)
  {
    if((*ii).GetName() == iName)
    {
      l_found = true;
      s_scopes.erase(ii);
      break;
    }
  }
  if(l_found)
  {
    map<string,UInt32>::iterator kk = s_scopeNameToIdMap.find(iName);
    if(kk != s_scopeNameToIdMap.end())
    {
      s_scopeNameToIdMap.erase(kk);
    }
  }
  return l_found;
}
