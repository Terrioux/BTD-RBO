/// \file 
/// \brief Definitions related to the CSP class

#ifndef CSP_H
#define CSP_H

#include "domain.h"
#include "variable.h"
#include "constraint.h"
#include <multi_hypergraph.h>
#include <string>
#include "event_manager.h"

using namespace std;

class Unary_Nogood_Base_Global_Constraint;
class Nogood_Base_Global_Constraint;

class CSP     /// This class allows to represent CSP instances \ingroup core
{
	protected:
		vector<Variable *> variables;                 ///< list of the variables of the CSP
		vector<Constraint *> constraints;             ///< list of the constraints of the CSP
		Multi_Hypergraph * h;									      	///< the constraint hypergraph representing the structure of the CSP
    Unary_Nogood_Base_Global_Constraint * unary_nogood_base;  ///< the nogood base used by some solving algorithms
    Nogood_Base_Global_Constraint * nogood_base;  ///< the nogood base used by some solving algorithms
		string name;														      ///< name of the instance
    Event_Manager * event_manager;                ///< the event manager if events are used
    Variable * last_conflict_variable;            ///< the last variable whose domain has been wiped-out
    Constraint * last_conflict_constraint;        ///< the last constraint which has been used for wiping-out a domain
    bool requires_constraint_restoration;         ///< true if the instance contains some constraints which must be restored when backtracking, false otherwise
				
	public:
		// constructors and destructor
		CSP (Event_Manager * em=0, string pb_name="");  ///< constructs an empty CSP whose name is pb_name and for which the events are managed by em
		virtual ~CSP ();					                      ///< destructor
		
		// basic functions
		unsigned int Get_N();												 	  ///< returns the number of variables
		unsigned int Get_D();												 	  ///< returns the size of the largest domain
		unsigned int Get_M();												 	  ///< returns the number of constraints
		unsigned int Get_R();												 	  ///< returns the arity of the largest constraint
		unsigned int Get_Total_Value_Number();			 	  ///< returns the number of values in all the domains
		string Get_Name();													 	  ///< returns the name of the instance
		Multi_Hypergraph * Get_Structure();					 	  ///< returns a pointer on the constraint hypergraph representing the structure of the CSP
		void Add_Variable (set<int> & values, string var_name="", bool is_auxiliary=false);  ///< adds a new variable v to the CSP whose domain is defined by the set values and whose name is var_name, is_auxiliary is set to true if the variable is auxiliary
		void Add_Variable (int a, int b, string var_name="", bool is_auxiliary=false);  ///< adds a new variable v to the CSP whose domain is defined by the values of [a,b] and whose name is var_name, is_auxiliary is set to true if the variable is auxiliary
		Variable * Get_Variable (unsigned int num);	    ///< returns a pointer on the variable number num
		Variable * Get_Variable (string label); 			  ///< returns a pointer on the variable whose name is label
		virtual int Add_Constraint (Constraint * c, bool duplicate = true);		///< adds a new constraint c to the CSP (a new memory space is allocated if duplicate is set to true) and returns the number of the constraint
		Constraint * Get_Constraint (unsigned int num); ///< returns a pointer on the constraint number num
		Unary_Nogood_Base_Global_Constraint * Get_Unary_Nogood_Base ();    ///< returns the nogood base if any, 0 otherwise
		Nogood_Base_Global_Constraint * Get_Nogood_Base ();                ///< returns the nogood base if any, 0 otherwise
    void Replace_Constraint (unsigned int num, Constraint * c);        ///< replaces the constraint whose number is num by the constraint c (we assume that the two constraints have the same scope)
    Event_Manager * Get_Event_Manager ();           ///< returns the event manager of the instance
    void Reset_Conflict ();                         ///< resets the conflict
    void Set_Conflict (Variable * x, Constraint * c);  ///< sets the conflict variable to x and the conflict constraint to c
    Variable * Get_Last_Conflict_Variable ();       ///< returns the last conflict variable
    Constraint * Get_Last_Conflict_Constraint ();   ///< returns the last conflict constraint
    bool Requires_Constraint_Restoration ();        ///< returns true if the instance contains some constraints which must be restored when backtracking, false otherwise
};


//-----------------------------
// inline function definitions
//-----------------------------


inline unsigned int CSP::Get_N()
// returns the number of variables
{
	return variables.size();
}


inline unsigned int CSP::Get_D()
// returns the size of the largest domain
{
  unsigned int d = 0;
  for (Variable * x : variables)
    if (x->Get_Domain()->Get_Initial_Size() > d)
      d = x->Get_Domain()->Get_Initial_Size();
	return d;
}


inline unsigned int CSP::Get_M()
// returns the number of constraints
{
	return constraints.size();
}


inline unsigned int CSP::Get_R()
// returns the arity of the largest constraint
{
  unsigned int r = 0;
  for (Constraint * c: constraints)
    if (c->Get_Arity() > r)
      r = c->Get_Arity();
	return r;
}


inline unsigned int CSP::Get_Total_Value_Number()
// returns the number of values in all the domains
{
  unsigned int total_value_number = 0;
  for (Variable * x : variables)
    total_value_number += x->Get_Domain()->Get_Initial_Size();
	return total_value_number;
}
	
	
inline string CSP::Get_Name()
// returns the name of the instance
{
	return name;
}
	
	
inline Multi_Hypergraph * CSP::Get_Structure()
// returns on a pointer on the constraint hypergraph representing the structure of the CSP
{
	return h;
}


inline Variable * CSP::Get_Variable (unsigned int num)
// returns a pointer on the variable number num
/// \param[in] num the number of the wanted variable
{
	return variables[num];
}


inline Constraint * CSP::Get_Constraint (unsigned int num)
// returns a pointer on the constraint number num
/// \param[in] num the number of the wanted constraint
{
	return constraints[num];
}


inline Unary_Nogood_Base_Global_Constraint * CSP::Get_Unary_Nogood_Base ()
// returns the nogood base if any, 0 otherwise
{
  return unary_nogood_base;
}


inline Nogood_Base_Global_Constraint * CSP::Get_Nogood_Base ()
// returns the nogood base if any, 0 otherwise
{
	return nogood_base;
}


inline Event_Manager * CSP::Get_Event_Manager ()
// returns the event manager of the instance
{
  return event_manager;
}


inline void CSP::Reset_Conflict ()
// resets the conflict
{
  last_conflict_variable = 0;
  last_conflict_constraint = 0;
}


inline void CSP::Set_Conflict (Variable * x, Constraint * c)
// sets the conflict variable to x
{
  last_conflict_variable = x;
  last_conflict_constraint = c;
}


inline Variable * CSP::Get_Last_Conflict_Variable ()
// returns the last conflict variable
{
  return last_conflict_variable;
}


inline Constraint * CSP::Get_Last_Conflict_Constraint ()
// returns the last conflict constraint
{
  return last_conflict_constraint;
}


inline bool CSP::Requires_Constraint_Restoration ()
// returns true if the instance contains some constraints which must be restored when backtracking, false otherwise
// returns true if the instance contains a table constraint, false otherwise
{
  return requires_constraint_restoration;
}
#endif
