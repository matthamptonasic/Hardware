/*
###############################################################################
#   Licensing information found at: 
#     https://github.com/matthamptonasic/Hardware/
#   In file LICENSING.md
###############################################################################
#
#   File          :   Event.h 
#   Creator       :   Matt Hampton (matthamptonasic@gmail.com)
#   Owner         :   Matt Hampton (matthamptonasic@gmail.com)
#   Creation Date :   10/14/16
#   Description   :   A template class for Events.
#                     You can declare an event in your code based on any types
#                     you require (any method signature).
#                     You then connect any method with matching signature to
#                     the event using the += operator.
#                     i.e.
#                           _myEventInst += myTestMethod;
#                     
#                     Then, when the event occurs, it will call each method
#                     in the event's list of delegates.
#                     The list of delegates will be called from the first
#                     delegate added to the last in order.
#                     Example:
#                               // Event delegate methods will return void
#                               // and pass an Int32 and a UInt32 as params.
#                       public: Event<void, Int32, UInt32> _myEvent;
#                          ....
#                       void myInit() {
#                         ...
#                         _myEvent += myTestMethod;
#                       }
#                       void myTestMethod(Int32 a, UInt32 b) {
#                         // Do event handling here.
#                       }
#                       void body() {
#                         ...
#                         _myEvent(-16, 32);  // This will call 'myTestMethod'.
#                       }
#
###############################################################################
*/

#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <list>

#include "Common.h"
#include "Logger.h"

template<typename RetTp, typename ...Args>
class Event
{
  // Enums
  public:

  // Nested Classes
  public:

  // Private Members
  private:
    list< function<RetTp(Args...)> > m_list;

  // Public Properties
  public:

  // Constructors
  public:
    Event()
    {

    }
    Event(function<RetTp(Args...)> iFn)
    {
      m_list.push_back(iFn);
    }

  // Inits
  private:

  // Public Methods
  public:

  // Private Methods
  private:
    bool add(function<RetTp(Args...)> iFn)
    {
      m_list.push_back(iFn);
    }
    /*
    TBD - figure out how to compare targets.
    bool addIfNot(function<RetTp(Args...)> iFn)
    {
      for(typename list<function<RetTp(Args...)>>::iterator ii=m_list.begin(); ii != m_list.end(); ++ii)
      {
        if(*ii == iFn)
        {
          cout << "found function already in this event." << endl;
          return false;
        }
      }
      m_list.push_back(iFn);
      cout << "did not find function, so adding it to this event." << endl;
      return true;
    }
    */
    bool remove(function<RetTp(Args...)> iFn)
    {
      // TBD - After figuring out how to compare targets.
      LOG_ERR_ENV << "Currently unsupported." << endl;
    }
    void fire(Args... iArgs)
    {
      for(typename list<function<RetTp(Args...)>>::iterator ii=m_list.begin(); ii != m_list.end(); ++ii)
      {
        if(*ii)
        {
          (*ii)(iArgs...);
        }
      }
    }

  // Operators
  public:
    //PartSelect operator() (UInt32 iUpperIndex, UInt32 iLowerIndex);
    void operator() (Args... iArgs)
    {
      fire(iArgs...);
    }
    void operator+= (function<RetTp(Args...)> iFn)
    {
      add(iFn);
    }
};

#endif /* EVENT_H */

