/// \file 
/// \brief Definitions related to the Assignment class

#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "csp.h"
#include <utility>

class Assignment                    /// This class allows to represent assignments \ingroup core
{
	protected:
    unsigned int size;              ///< the size of the assignment
    unsigned int maximal_size;      ///< the maximal size of the assignment
		vector<pair<int,int>> values;   ///< the assignment of value to variable 
		vector<int> position;           ///< the position of each variable in the assginment
		
	public:
		// constructors and destructor
		Assignment (int max_size);		 	  ///< construct an empty assignment whose maximum size is max_size and which involves variables of label at most max_size
		Assignment (int max_size, int max_label);			  ///< construct an empty assignment whose maximum size is max_size and which involves variables of label at most max_label
		Assignment (Assignment & a);		  ///< construct an assignment by copying the assignment a
		
		// operator
		Assignment & operator= (Assignment & a);   ///< operator =
			
		// basic functions
		unsigned int Get_Size ();							///< returns the number of assigned variables in the current assignment
		void Assign (int var, int val);				///< assignes the value val to the variable var in the current assignment
		void Reassign (int var, int val);			///< changes the assignment of the variable var to the value val in the current assignment
		void Unassign ();											///< unassignes the last assigned variable in the current assignment
		void Unassign (int var);							///< unassignes the variable var in the current assignment
		void Clear ();												///< unassignes all the variables
		void Merge (Assignment & A);					///< merges the current assignment and A if they do not share any variable
		int Last_Assigned ();									///< returns the last assigned variable (-1 if the assignment is empty)
		bool Is_Assigned (int var);						///< returns true if the variable var is assigned, false otherwise
		int Get_Variable (unsigned int pos);	///< returns the number of the variable assigned at position pos, -1 if there are less than pos assigned variables
		int Get_Value (int var);							///< returns the value of the assigned variable var
		int Get_Position (int var);						///< returns the position of the variable var, -1 if the variable is not assigned
		bool Is_Consistent (CSP * pb);				///< returns true if the assignment is consistent w.r.t CSP pb, false otherwise
		bool Is_Solution (CSP * pb);					///< returns true if the assignment involves all the variables of the CSP pb and satisfies all its constraints, false otherwise
		void Print (CSP * pb = 0);	          ///< prints the current assignment with the real name of variables and values if pb is not null
};


//-----------------------------
// inline function definitions
//-----------------------------


inline unsigned int Assignment::Get_Size ()
// returns the number of assigned variables in the current assignment
{
	return size;
}


inline void Assignment::Assign (int var, int val)
// assignes the value val to the variable var in the current assignment
/// param[in] var the number of the variable we want to assign
/// param[in] val the value chosen for the assignment
{
	position[var] = size;
	values[size].first = var;
  values[size].second = val;
  size++;
}


inline void Assignment::Reassign (int var, int val)
// changes the assignment of the variable var to the value val in the current assignment
/// param[in] var the number of the variable we want to reassign
/// param[in] val the value chosen for the reassignment
{
	values[position[var]].second = val;	
}


inline void Assignment::Unassign ()
// unassignes the last assigned variable in the current assignment
{
	position[values[size-1].first] = -1;
	size--;	
}


inline int Assignment::Last_Assigned ()
// returns the last assigned variable (-1 if the assignment is empty)
{
	if (size == 0)  return -1;
	else return values[size-1].first;
}


inline bool Assignment::Is_Assigned (int var)
// returns true if the variable var is assigned, false otherwise
/// param[in] var the number of the variable for which we want to know if it is assigned
{
	return position[var] != -1;
}


inline int Assignment::Get_Variable (unsigned int pos)
// returns the number of the variable assigned at position pos, -1 if there are less than pos assigned variables
/// param[in] pos the position in the assignment for which we want to know the number of the corresponding assigned variable
{
	if ((pos < 0) || (pos >= size))
		return -1;
	else return values[pos].first;
}


inline int Assignment::Get_Value (int var)
// returns the value of the assigned variable var
/// param[in] var the number of the variable for which we want to know the value in the assignment
{
	return values[position[var]].second;
}


inline int Assignment::Get_Position (int var)
// returns the position of the variable var, -1 if the variable is not assigned
/// param[in] var the number of the variable for which we want to know its position in the assignment
{
	return position[var];
}


inline bool Assignment::Is_Solution (CSP * pb)
// returns true if the assignment involves all the variables of the CSP pb and satisfies all its constraints, false otherwise
/// param[in] pb the CSP instance for which we want to know if the current assignment is a solution
{
	return (size == pb->Get_Mandatory_N()) && Is_Consistent (pb);
}
#endif
