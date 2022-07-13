/// \file 
/// \brief Definitions related to the Compact_Table_Extension_Constraint class

#ifndef COMPACT_TABLE_EXTENSION_CONSTRAINT_H
#define COMPACT_TABLE_EXTENSION_CONSTRAINT_H

#include "extension_constraint.h"
#include "assignment.h"
#include "r_sparse_bitset.h"

class Compact_Table_Extension_Constraint: public Extension_Constraint    /// This class allows to represent constraints in extension \ingroup core
{		
  protected:
    R_Sparse_Bitset current_table;                ///< the current state of the set of tuples (that is after some possible deletions and restorations)
    vector<vector<vector<unsigned long>>> data;   ///< the set of supports (for positive table) or conflicts (for negative table) for each pair (variable,value)
    Event_Manager * event_manager;                ///< the event manager of the CSP to which the constraint is related to
    unsigned long possible_tuple_number;          ///< the number of possible tuples (i.e. the product of the size of domains of the variables in the scope)
    unsigned long tuple_number;                   ///< the number of tuples in the table

  
	public:			
		// constructor
		Compact_Table_Extension_Constraint (vector<Variable *> & var);				///< constructs an extension constraint whose scope is defined by the variables in var
		Compact_Table_Extension_Constraint (Compact_Table_Extension_Constraint & c); ///< constructs a new constraint by copying the constraint c 
    virtual ~Compact_Table_Extension_Constraint();      ///< destructor
	
		// basic functions
 		double Get_Tightness () override = 0;  				 						///< returns the tightness of the constraint
		double Get_Allowed_Tuple_Number () override = 0;					///< returns the number of allowed tuples
    bool Is_Satisfied (int * t) override = 0;   		 				  ///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override = 0;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override = 0;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    void Restore () override;                                 ///< restores the constraint in its previous state  

  protected:
    virtual void Add_Tuple (int * t);   		      						///< adds the tuple t to data
    virtual void Remove_Tuple (int * t);   				    				///< removes the tuple t from data

};


//-----------------------------
// inline function definitions
//-----------------------------


inline void Compact_Table_Extension_Constraint::Restore ()
// restores the constraint in its previous state
{
  current_table.Restore();
}                              

#endif
