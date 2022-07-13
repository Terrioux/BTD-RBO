/// \file 
/// \brief Definitions related to the Negative_Compact_Table_Extension_Constraint class

#ifndef NEGATIVE_COMPACT_TABLE_EXTENSION_CONSTRAINT_H
#define NEGATIVE_COMPACT_TABLE_EXTENSION_CONSTRAINT_H

#include "compact_table_extension_constraint.h"

class Negative_Compact_Table_Extension_Constraint: public Compact_Table_Extension_Constraint    /// This class allows to represent constraints in extension \ingroup core
{		
	public:			
		// constructor
		Negative_Compact_Table_Extension_Constraint (vector<Variable *> & var);				///< constructs an extension constraint whose scope is defined by the variables in var
		Negative_Compact_Table_Extension_Constraint (Negative_Compact_Table_Extension_Constraint & c);     ///< constructs a new constraint by copying the constraint c 

	
		// basic functions
 		double Get_Tightness () override;   									///< returns the tightness of the constraint
		double Get_Allowed_Tuple_Number () override;					///< returns the number of allowed tuples
    Constraint * Duplicate () override;  				          ///< returns a pointer on a copy of the constraint
		void Allow_Tuple (int * t) override;   								///< allows the tuple t
		void Forbid_Tuple (int * t) override;  								///< forbids the tuple t
 		bool Is_Satisfied (int * t) override;	 		 						///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    string Get_XCSP3_Expression() override;               ///< returns the string corresponding to the expression of the constraint in XCSP 3 format  
    
  private:
    void Update_Table (CSP * pb, Deletion_Stack * ds, timestamp ref);   ///< updates the table and returns true iff the table is empty
    void Filter_Domain (Deletion_Stack * ds);                           ///< filters the domain
};


//-----------------------------
// inline function definitions
//-----------------------------


inline double Negative_Compact_Table_Extension_Constraint::Get_Tightness ()
// returns the tightness of the constraint
{
  return tuple_number;
}

   				 						
inline double Negative_Compact_Table_Extension_Constraint::Get_Allowed_Tuple_Number ()
// returns the number of allowed tuples
{
  return possible_tuple_number - tuple_number;
}


inline void Negative_Compact_Table_Extension_Constraint::Allow_Tuple (int * t)
// allows the tuple t
{
  Remove_Tuple (t);
}


inline void Negative_Compact_Table_Extension_Constraint::Forbid_Tuple (int * t)
// forbids the tuple t
{
  Add_Tuple (t);
}
#endif
