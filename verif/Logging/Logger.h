/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Logger.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   09/27/16
#   Description   :   Class/defines for logging to console and file.
#
###############################################################################
*/
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <list>

#include "Common.h"

using namespace std;

class Logger
{
  // Enums
  public:

  // Nested Classes
  public:
  class Scope
  {
    private:
      static UInt32 s_vrb_NONE;
      static UInt32 s_vrb_LOW;
      static UInt32 s_vrb_MEDIUM;
      static UInt32 s_vrb_HIGH;
      static UInt32 s_vrb_FULL;
      static UInt32 s_vrb_DEBUG;
      static UInt32 s_count;
             UInt32 m_id;
             string m_name;
             UInt32 m_level;

    public:
      static UInt32 Vrb_NONE()    { return s_vrb_NONE; }
      static UInt32 Vrb_LOW()     { return s_vrb_LOW; }
      static UInt32 Vrb_MEDIUM()  { return s_vrb_MEDIUM; }
      static UInt32 Vrb_HIGH()    { return s_vrb_HIGH; }
      static UInt32 Vrb_FULL()    { return s_vrb_FULL; }
      static UInt32 Vrb_DEBUG()   { return s_vrb_DEBUG; }

      inline UInt32 GetId()       const { return m_id; }
      inline string GetName()     const { return m_name; }
      inline UInt32 GetLevel()    const { return m_level; }

    public:
      Scope(string iName, UInt32 iLevel = s_vrb_DEBUG);
  };

  // Private Members
  private:
    // Logging Control
    bool m_initDone;
    bool m_alwaysLogScopes;
    list<Scope> m_scopes;
    map<string, UInt32> m_scopeNameToIdMap;

    // Logger Data
    string m_fileName;
    ofstream * m_fileOut;
    ostream * m_consoleOut;

  // Public Properties
  public:
    bool AlwaysLogScopes_get() { return m_alwaysLogScopes; }
    void AlwaysLogScopes_set(bool iValue) { m_alwaysLogScopes = iValue; }

  // Constructors
  public:
    Logger();
    Logger(string iFileName, ostream * iStream = &cout);

  // Inits
  private:
    void init(string iFileName, ostream * iStream = &cout);
    void init_streams(string iFileName, ostream * iStream);

  // Public Methods
  public:
    void SetConsoleOut(ostream * iStream = &cout);
    bool SetFileOut(ofstream * iStream);
    void SetFileOut(string iFileName);
    void AddDebugScope(const Scope & iScope);
    bool RemoveDebugScope(string iName);

  // Private Methods
  private:

};

#endif /* LOGGER_H */
