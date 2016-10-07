/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   EnvManager.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   10/06/16
#   Description   :   
#
###############################################################################
*/
#ifndef ENVMANAGER_H
#define ENVMANAGER_H

#include <string>

#include "Common.h"

using namespace std;

class EnvManager
{
  // Enums
  public:

  // Constants
  private:

  // Nested Classes
  public:

  // Private Members
  private:

  // Public Properties
  public:

  // Singleton Pattern
  public:
    static EnvManager & Access()
    {
      static EnvManager ls_inst;     // Guaranteed to be destroyed.
      return ls_inst;                // Instantiated on 1st use.
    }
  private:
    EnvManager();
  public:
    EnvManager(EnvManager const &) = delete;     // These are public for clarity of debug.
    void operator=(EnvManager const &) = delete; // Can be made private if intellisense is used.

  // Inits
  private:
    void init();

  // Public Methods
  public:

  // Private Methods
  private:

};

#endif /* ENVMANAGER_H */
