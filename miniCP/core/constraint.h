/// \file 
/// \brief Definitions related to the Constraint class

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <vector>
#include <map>
#include <set>
#include <list>

class Assignment;
class CSP;

#include "support.h"
#include "deletion_stack.h"
#include "variable.h"
#include "tuple.h"
#include "event_manager.h"

class Constraint      /// This class allows to represent constraints \ingroup core
{
	protected:
		unsigned int num;					  							///< the number of the constraint
		unsigned int arity;												///< the arity of the constraint
		set<unsigned int> scope;   								///< the number of the variables involved in the constraint
		vector<Variable *> scope_var;					    ///< the variables involved in the constraint	
		map<int,int> position;										///< the position of a variable x in the scope of the constraint
		unsigned int unassigned_variable_number;	///< the number of unassigned variables in the scope of the constraint
		
	public:
		// constructors and destructor
		Constraint ();		                        	 ///< constructs a constraint with an empty scope
		Constraint (vector<Variable *> & var);			 ///< constructs a constraint whose scope is defined by the variables in var
		Constraint (Constraint & c); 								 ///< constructs a constraint by copying the constraint c
		virtual ~Constraint();			 								 ///< destructor
			
		// iterators
		set<unsigned int>::iterator Begin();   			 ///< returns an iterator referring to the first variable of the scope
    set<unsigned int>::iterator End();					 ///< returns an iterator referring to the position after the last variable of the scope
    
    vector<Variable *>::iterator Begin_Var();    ///< returns an iterator referring to the first variable of the scope
    vector<Variable *>::iterator End_Var();	     ///< returns an iterator referring to the position after the last variable of the scope
			
		// basic functions
		virtual Constraint * Duplicate () = 0;							 ///< returns a pointer on a copy of the constraint
		set<unsigned int> & Get_Scope ();	   				 				 ///< returns the scope of the constraint
		vector<Variable *> & Get_Scope_Var ();		 				   ///< returns the scope of the constraint
		Variable * Get_Scope_Variable (int i);			 				 ///< returns the i th variable of the scope
		void Change_Scope (vector<Variable *> & new_scope);  ///< changes the scope of the constraint (we assume that the current scope and of the new one have the same arity and that for all i, the ith variable of the new scope has the same values as the ith variable of the current scope)
		int Get_Position (int var);													 ///< returns the position of the variable var in the scope of the constraint, -1 if the variable does not belong to this scope
		unsigned int Get_Arity ();									 				 ///< returns the arity of the constraint
		unsigned int Get_Num ();														 ///< returns the number of the constraint
		void Set_Num (unsigned int n);		  								 ///< sets the number of the constraint to n
		bool Involve (int x);																 ///< returns true if the variable x is involved in the scope of the cosntraint, false otherwise	
		unsigned int Get_Unassigned_Variable_Number ();			 ///< returns the number of unassigned variables in the scope of the constraint
		void Inc_Unassigned_Variable_Number ();							 ///< increasWes by one the number of unassigned variables in the scope of the constraint
		void Dec_Unassigned_Variable_Number ();			 				 ///< decreases by one the number of unassigned variables in the scope of the constraint
		virtual bool Is_Satisfied (int * t)=0; 			 				 ///< returns true if the tuple t satisfies the constraint, false otherwise
		virtual bool Is_Satisfied (Assignment & A);  				 ///< returns true if the assignment a satisfies the constraint, false otherwise
		virtual bool Is_Satisfied (Tuple &t); 			 				 ///< returns true if the tuple t satisfies the constraint, false otherwise
		virtual bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds);	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
		virtual void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref);	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    virtual bool Impacted_By_Last_Events (CSP * pb,timestamp ref);     ///< true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
		virtual void Get_Allowed_Tuple_List (list<int *> & list);   ///< constructs the list of tuples allowed by the constraint
    virtual void Restore ();                             ///< restores the constraint in its previous state
    virtual string Get_XCSP3_Expression();               ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


//-----------
// iterators
//-----------


inline set<unsigned int>::iterator Constraint::Begin()
// returns an iterator referring to the first variable of the scope
{
	return scope.begin();
}


inline set<unsigned int>::iterator Constraint::End()
// returns an iterator referring to the position after the last variable of the scope
{
	return scope.end();
}


inline vector<Variable *>::iterator Constraint::Begin_Var()
// returns an iterator referring to the first variable of the scope
{
	return scope_var.begin();
}


inline vector<Variable *>::iterator Constraint::End_Var()
// returns an iterator referring to the position after the last variable of the scope
{
	return scope_var.end();
}


//-----------------
// basic functions
//-----------------

inline set<unsigned int> & Constraint::Get_Scope ()
// returns the scope of the constraint
{
	return scope;
}


inline vector<Variable *> & Constraint::Get_Scope_Var ()
// returns the scope of the constraint
{
	return scope_var;
}


inline Variable * Constraint::Get_Scope_Variable (int i)
// returns the i th variable of the scope
{
	return scope_var[i];
}


inline void Constraint::Change_Scope (vector<Variable *> & new_scope)
// changes the scope of the constraint (we assume that the current scope and of the new one have the same arity and that for all i, the ith variable of the new scope has the same values as the ith variable of the current scope)
{
	scope_var = new_scope;
	scope.clear();
	position.clear();
	int num, i = 0;
	for (vector<Variable *>::iterator iter = scope_var.begin(); iter != scope_var.end (); iter++)
	{
		num = (*iter)->Get_Num();
		scope.insert (num);
		position.insert(pair<int,int>(num,i));
		i++;
	}
}


inline unsigned int Constraint::Get_Arity ()
// returns the arity of the constraint
{
	return arity;
}


inline unsigned int Constraint::Get_Num ()
// returns the number of the constraint
{
	return num;
}


inline void Constraint::Set_Num (unsigned int n)
// sets the number of the constraint to n
{
	num = n;
}


inline bool Constraint::Involve (int x)
// returns true if the variable x is involved in the scope of the cosntraint, false otherwise
{
	return position.find (x) != position.end();
}


inline unsigned int Constraint::Get_Unassigned_Variable_Number ()
// returns the number of unassigned variables in the scope of the constraint
{
	return unassigned_variable_number;
}


inline void Constraint::Inc_Unassigned_Variable_Number ()
// increases by one the number of unassigned variables in the scope of the constraint
{
	unassigned_variable_number++;
}


inline void Constraint::Dec_Unassigned_Variable_Number ()
// decreases by one the number of unassigned variables in the scope of the constraint
{
	unassigned_variable_number--;
}


inline int Constraint::Get_Position (int var)
// returns the position of the variable var in the scope of the constraint, -1 if the variable does not belong to this scope
{
	map<int,int>::iterator pos = position.find (var);

	if (pos == position.end())  return -1;
	else return pos->second;	
}

inline bool Constraint::Is_Satisfied (Tuple &t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	return Is_Satisfied (t.Get_Values());
}


inline bool Constraint::Impacted_By_Last_Events (CSP * pb, timestamp ref)
// true if the constraint may be impacted by the last events w.r.t. ref, false otherwise
{
  return true;
}


inline void Constraint::Restore ()
// restores the constraint in its previous state
{
}


inline string Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  //~ throw ("Error: unkown XCSP 3 expression");
  return "Error: unkown XCSP 3 expression";
}

#endif
