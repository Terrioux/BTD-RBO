/// \file 
/// \brief Definitions related to the Variable class

#ifndef VARIABLE_H
#define VARIABLE_H


class Domain;
class Event_Manager;
#include <string>
#include <set>

using namespace std;

class Variable        /// This class allows to represent variables \ingroup core
{
	protected :
		unsigned int num;  		///< the number of the variable
		string name;          ///< the name of the variable
		Domain * domain;	    ///< the domain related to the variable
    bool is_auxiliary;    ///< is_auxiliary is set to true if the variable is auxiliary (i.e. which is not considered by the variable heuristic), false 
    bool is_initial;      ///< is_initial is set to true if the variable is present initially in the problem, false if it is added to make the modeling easier
		
	public :
		// constructors and destructor
    Variable (set<long> & values, unsigned int num_var, string ch, Event_Manager * event_manager, bool is_init, bool is_aux);  ///< constructs an enumerated variable whose domain is the set values, num is num_var and name is ch, is_init is set to true if the variable appears initially in the problem, is_aux is set to true if the variable is auxiliary
    Variable (long a, long b, unsigned int num_var, string ch, Event_Manager * event_manager, bool is_init, bool is_aux);      ///< constructs an enumerated variable whose domain is [a,b], num is num_var and name is ch, is_init is set to true if the variable appears initially in the problem, is_aux is set to true if the variable is auxiliary
    Variable (long val, unsigned int num_var, string ch, Event_Manager * event_manager);      ///< constructs a variable whose domain is restricted to val, num is num_var and name is ch, is_aux is set to true if the variable is auxiliary
		Variable (Domain * d, string ch="", bool is_aux = false);	  ///< constructs an enumerated variable whose domain is d and name is ch, is_aux is set to true if the variable is auxiliary
		Variable (Variable & v);								///< constructs an enumerated variable by copying the variable v
		~Variable();			 											///< destructor
			
		// operator
		Variable & operator= (Variable & v);		///< operator =
			
		// basic functions	
		Variable * Duplicate ();				///< returns a pointer on a copy of the variable
		unsigned int Get_Num ();			  ///< returns the number of the variable
		string Get_Name ();        	    ///< returns the name of the variable
		Domain * Get_Domain ();    	    ///< returns the domain related to the variable
		void Set_Num (unsigned int n); 	///< sets the number of the variable to n
		void Set_Name (string n);    	 	///< sets the name of the variable to n
    bool Is_Initial ();             ///< returns true if the variable is initial, false otherwise
    bool Is_Auxiliary ();           ///< returns true if the variable is auxiliary, false otherwise
};



//-----------------------------
// inline function definitions
//-----------------------------


inline Variable * Variable::Duplicate ()
// returns a pointer on a copy of the variable
{
	return new Variable (*this);
}


inline unsigned int Variable::Get_Num ()
// returns the number of the variable
{
	return num;
}


inline string Variable::Get_Name ()
// returns the name of the variable
{
	return name;
}


inline Domain * Variable::Get_Domain ()
// returns the domain related to the variable
{
	return domain;
}


inline void Variable::Set_Num (unsigned int n)
// sets the number of the variable to n
{
	num = n;
}


inline void Variable::Set_Name (string n)
// sets the name of the variable to n
{
  name = n;	
}


inline bool Variable::Is_Initial ()
// returns true if the variable is initial, false otherwise
{
  return is_initial;
}


inline bool Variable::Is_Auxiliary ()
// returns true if the variable is auxiliary, false otherwise
{
  return is_auxiliary;
}
#endif
