/// \file 
/// \brief Definitions related to the Event_Manager class

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "variable.h"
#include <vector>
#include <set>
#include <cassert>

typedef unsigned long timestamp;                ///< the type defining the timestamp

typedef enum possible_event   /// lists the possible events
{
  NOEV,      ///< no event
  FIX,       ///< a fix event
  DMC,       ///< a dmc event
  LBC,       ///< a lbc event
  UBC        ///< a ubc event
} event;     /// the type defining the possible events

class Event_Manager                             /// This class allows the management of events \ingroup core
{
	protected :
    timestamp current_time;                     ///< the current time
    timestamp level_time;                       ///< the time at which starts the current level
		vector<timestamp> fix;  		 					      ///< provides, for each variable, the last time we assign it
		vector<int> fixed_value;  	 					      ///< provides, for each variable having a singleton domain, the remaining value
		vector<timestamp> dmc;	   		              ///< provides, for each pair variable/value, the last time we remove the value from the domain of the variable
		vector<vector<timestamp>> dmc_val;	   	    ///< provides, for each pair variable/value, the last time we remove the value from the domain of the variable
    vector<timestamp> lbc;  		 					      ///< provides, for each variable, the last time its minimum has changed
    vector<timestamp> ubc;  		 					      ///< provides, for each variable, the last time its maximum has changed
    vector<pair<timestamp,event>> last_modif;   ///< provides, for each variable, the last time its domain changed and the corresponding event
    set<int> modified;                          ///< the set of variables which have been modified at the current level

	public :
		// constructor and destructor
    Event_Manager ();                           ///< constructor
    ~Event_Manager (){};                        ///< destructor
    
    // basic functions	
    void Add_Event_Fix (int var, int val);                  ///< adds the event fix for the variable var and the value val
    void Add_Event_Dmc (int var, int val);                  ///< adds the event dmc for the variable var and the value val
    void Add_Event_Lbc (int var);                           ///< adds the event lbc for the variable var
    void Add_Event_Ubc (int var);                           ///< adds the event ubc for the variable var
    
    bool Exist_Event_Fix (int var, timestamp ref_time);     ///< returns true if an event fix occurred for the variable var since the time ref_time, false otherwise
    bool Exist_Event_Dmc (int var, timestamp ref_time);     ///< returns true if an event dmc occurred for the variable var since the time ref_time, false otherwise
    bool Exist_Event_Dmc (int var, int val, timestamp ref_time);  ///< returns true if an event dmc occurred for the value val of the variable var since the time ref_time, false otherwise (avalaible only if the domain is not a singleton)
    bool Exist_Event_Lbc (int var, timestamp ref_time);     ///< returns true if an event lbc occurred for the variable var since the time ref_time, false otherwise
    bool Exist_Event_Ubc (int var, timestamp ref_time);     ///< returns true if an event ubc occurred for the variable var since the time ref_time, false otherwise

    void Add_Variable (Variable * x);                       ///< adds the variable x to the set of variables for which events are managed
    void New_Level ();                                      ///< starts a new level
    void Update ();                                         ///< updates the current time
    timestamp Get_Current_Time ();                          ///< returns the current time
    timestamp Get_Current_Level_Time ();                    ///< returns the reference time for the current level
    event Get_Last_Modif (int var);                         ///< returns the last event for the variable var if this event occurs at time level_time, NOEV otherwise
    set<int> & Get_Current_Modified_Variables ();                   ///< returns the set of the variables which have been modified at the current level
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void Event_Manager::Add_Event_Fix (int var, int val)
// adds the event fix for the variable var and the value val
/// \param[in] var the variable we have just assigned
/// \param[in] val the value we want to assign 
{
  modified.insert(var);
  fix[var] = current_time;
  fixed_value[var] = val;
  lbc[var] = current_time;
  ubc[var] = current_time;
  last_modif[var].first = current_time;
  last_modif[var].second = FIX;
}


inline void Event_Manager::Add_Event_Dmc (int var, int val)
// adds the event dmc for the variable var and the value val
/// \param[in] var the variable for which we have just removed a value
/// \param[in] val the value we want to remove
{
  modified.insert(var);
  dmc[var] = current_time;
  dmc_val[var][val] = current_time;
  last_modif[var].first = current_time;
  last_modif[var].second = DMC;
}


inline void Event_Manager::Add_Event_Lbc (int var)
// adds the event lbc for the variable var
/// \param[in] var the variable for which we have just removed the lowest value
{
  lbc[var] = current_time;
  last_modif[var].first = current_time;
  last_modif[var].second = LBC;
}


inline void Event_Manager::Add_Event_Ubc (int var)
// adds the event ubc for the variable var
/// \param[in] var the variable for which we have just removed the uppest value
{
  ubc[var] = current_time;
  last_modif[var].first = current_time;
  last_modif[var].second = UBC;
}


inline bool Event_Manager::Exist_Event_Fix (int var, timestamp ref_time)
// returns true if an event fix occurred for the variable var since the time ref_time, false otherwise
/// \param[in] var the variable for which we want to test the existence of a fix event
/// \param[in] ref_time the reference timestamp
{
  return (fix[var] > ref_time) && (fix[var] >= level_time);
}


inline bool Event_Manager::Exist_Event_Dmc (int var, timestamp ref_time)
// returns true if an event dmc occurred for the variable var since the time ref_time, false otherwise
/// \param[in] var the variable for which we want to test the existence of a dmc event
/// \param[in] ref_time the reference timestamp
{
  return (dmc[var] > ref_time) && (dmc[var] >= level_time);
}


inline bool Event_Manager::Exist_Event_Dmc (int var, int val, timestamp ref_time)
// returns true if an event dmc occurred for the value val of the variable var since the time ref_time, false otherwise  (avalaible only if the domain is not a singleton)
/// \param[in] var the variable for which we want to test the existence of a dmc event
/// \param[in] val the value for which we want to test the existence of a dmc event
/// \param[in] ref_time the reference timestamp
{
  if ((fix[var] > ref_time) && (fix[var] >= level_time))
    return fixed_value[var] != val;
  else return (dmc_val[var][val] > ref_time) && (dmc_val[var][val] >= level_time);
}


inline bool Event_Manager::Exist_Event_Lbc (int var, timestamp ref_time)
// returns true if an event lbc occurred for the variable var since the time ref_time, false otherwise
/// \param[in] var the variable for which we want to test the existence of a lbc event
/// \param[in] ref_time the reference timestamp
{
  return (lbc[var] > ref_time) && (lbc[var] >= level_time);
}


inline bool Event_Manager::Exist_Event_Ubc (int var, timestamp ref_time)
// returns true if an event ubc occurred for the variable var since the time ref_time, false otherwise
/// \param[in] var the variable for which we want to test the existence of a ubc event
/// \param[in] ref_time the reference timestamp
{
  return (ubc[var] > ref_time) && (ubc[var] >= level_time);
}






inline void Event_Manager::New_Level ()
// starts a new level
{
  modified.clear();
  current_time++;
  level_time = current_time;  
}


inline void Event_Manager::Update ()
// updates the current time
{
  modified.clear();
  current_time++;
}


inline timestamp Event_Manager::Get_Current_Time ()
// returns the current time
{
  return current_time;
}


inline timestamp Event_Manager::Get_Current_Level_Time ()
// returns the reference time for the current level
{
  return level_time;
}

inline event Event_Manager::Get_Last_Modif (int var)
// returns the last event for the variable var if this event occurs at time level_time, NOEV otherwise
/// \param[in] var the variable for which we want to get the last event 
{
  if (last_modif[var].first == level_time)
    return last_modif[var].second;
  else return NOEV;
}


inline set<int> & Event_Manager::Get_Current_Modified_Variables ()
// returns the set of the variables which have been modified at the current level
{
  return modified;
}

#endif
