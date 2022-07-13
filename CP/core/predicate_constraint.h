/// \file 
/// \brief Definitions related to the Predicate_Constraint class

#ifndef PREDICATE_CONSTRAINT_H
#define PREDICATE_CONSTRAINT_H


#include "constraint.h"
#include "expression.h"
#include "real_tuple.h"
#include "assignment.h"
#include <tuple>


class Predicate_Constraint: public Constraint       /// This class implements the predicate constraint \ingroup core
{
	private:
		Real_Tuple t;						///< the tuple used in Revise (the declaration of this member is only motivated by runtime reason)
    vector<tuple<node_type,operator_type,int>> postfix_relation;
    vector<int> values;
	
	protected:
		Expression relation;		///< the relation related to the constraint
		
	public:
		// constructors and destructor
		Predicate_Constraint (vector<Variable *> & var, Expression & e); 	///< constructs a new constraint which involves the variable in var and whose relation is given by the expression e
		Predicate_Constraint (Predicate_Constraint & c);									///< constructs a new constraint by copying the constraint c 
		~Predicate_Constraint ();																					///< destructor
		
		// operator
		Predicate_Constraint & operator= (Predicate_Constraint & c);		///< operator =
		
		// basic functions
		void Merge (Predicate_Constraint * c);     	    ///< merges the current constraint with the constraint c
		Constraint * Duplicate () override; 				    ///< returns a pointer on a copy of the constraint w.r.t. the CSP pb
		Expression & Get_Relation ();						 	 	    ///< returns the relation
		bool Is_Satisfied (int * t) override;	 		    	///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
		void Get_Allowed_Tuple_List (list<int *> & list) override;	///< constructs the list of tuples allowed by the constraint
    int Evaluate (int t []);                        ///< returns the evaluation of the expression such that the value t[i] is the effective value of the variable i
    string Get_XCSP3_Expression() override;         ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
};


//-----------------------------
// inline function definitions
//-----------------------------


inline Expression & Predicate_Constraint::Get_Relation ()
// returns the relation
{
	return relation;	
}


inline Constraint * Predicate_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
	return new Predicate_Constraint (*this);
}

#endif
