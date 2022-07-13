/// \file 
/// \brief Definitions related to the Automaton class

#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <string>

using namespace std;

typedef unsigned int state;      ///< the type allowing to represent vertices  \ingroup core

class Automaton      /// This class implements deterministic automata \ingroup core
{
  protected:
    state initial_state;                     ///< the initial state of the automaton
    set<state> final_states;                 ///< the final states of the automaton
    map<state,map<int,state>> transitions;   ///< the transition function of the automaton
    
	public:
		// basic functions
    void Set_Initial_State (state s);       ///< sets the initial state to s
    state Get_Initial_State ();             ///< returns the initial state
    void Add_Transition (state from_state, int val, state to_state);       ///< adds a transition from the state from_state to the state to_state for the value val
    bool Exists_Transition (state from_state, int val, state to_state);    ///< returns true if there is a transition from the state from_state to the state to_state for the value val, false otherwise
    state Delta (state s, int val);         ///< returns the state corresponding to the transition function applied from the state s and the value val
		void Set_Final_State (state s);	 	  	 	///< adds s to the set of final states
		void Set_Final_State ();	       	  	 	///< chooses as final state the states which have no successor
    bool Is_Final_State (state s);	 		 	  ///< returns true if s is a final state, false otherwise
    unsigned int Get_State_Number();        ///< returns the number of states
    string Get_XCSP3_Expression();          ///< returns the state corresponding to the automaton in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void Automaton::Set_Initial_State (state s)
// sets the initial state to s
{
  initial_state = s;
}


inline state Automaton::Get_Initial_State ()
// returns the initial state
{
  return initial_state;
}


inline void Automaton::Add_Transition (state from_state, int val, state to_state)
// adds a transition from the state from_state to the state to_state for the value val
{
  map<state,map<int,state>>::iterator iter = transitions.find (from_state);
  if (iter == transitions.end())
  {
    map<int,state> m;
    m.insert (pair<int,state>(val,to_state));
    transitions.insert (pair<state,map<int,state>>(from_state,m));
  }
  else iter->second.insert (pair<int,state>(val,to_state));
  
  iter = transitions.find (to_state);
  if (iter == transitions.end())
  {
    map<int,state> m;
    transitions.insert (pair<state,map<int,state>>(to_state,m));
  }
}


inline bool Automaton::Exists_Transition (state from_state, int val, state to_state)
// returns true if there is a transition from the state from_state to the state to_state for the value val, false otherwise
{
  map<state,map<int,state>>::iterator iter = transitions.find (from_state);
  if (iter != transitions.end())
  {
    map<int,state>::iterator iter2 = iter->second.find(val);
    if (iter2 != iter->second.end())
      return iter2->second == to_state;
    else return false;
  }
  else return false;
}


inline state Automaton::Delta (state s, int val)
// returns the state corresponding to the transition function applied from the state s and the value val
{
  map<state,map<int,state>>::iterator iter = transitions.find (s);
  if (iter != transitions.end())
  {
    map<int,state>::iterator iter2 = iter->second.find(val);
    if (iter2 != iter->second.end())
      return iter2->second;
    else return 0;
  }
  else return 0;
}



inline void Automaton::Set_Final_State (state s)
// adds s to the set of final states
{
  final_states.insert(s);
}


inline bool Automaton::Is_Final_State (state s)
// returns true if s is a final state, false otherwise
{
  return final_states.find(s) != final_states.end();
}


inline unsigned int Automaton::Get_State_Number()
// returns the number of states
{
  return transitions.size();
}


inline string Automaton::Get_XCSP3_Expression()
// returns the state corresponding to the automaton in XCSP 3 format
{
  string expr;
  
  expr += "  <transitions>\n    ";
  for (map<state,map<int,state>>::iterator iter = transitions.begin(); iter != transitions.end(); iter++)
    for (map<int,state>::iterator iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
      expr += "(q" + to_string(iter->first) + "," + to_string(iter2->first) + ",q" + to_string(iter2->second) +")";
  expr += "\n  <transitions>\n";
  
  expr += "  <start> q" + to_string(initial_state) + " </start>\n";
  
  expr += "  <final>";
  for (set<state>::iterator iter = final_states.begin(); iter != final_states.end(); iter++)
    expr += " q" + to_string(*iter);
  expr += " </final>\n";
  return expr;
}

#endif
