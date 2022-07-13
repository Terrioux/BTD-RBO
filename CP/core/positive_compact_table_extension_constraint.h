/// \file 
/// \brief Definitions related to the Positive_Compact_Table_Extension_Constraint class

#ifndef POSITIVE_COMPACT_TABLE_EXTENSION_CONSTRAINT_H
#define POSITIVE_COMPACT_TABLE_EXTENSION_CONSTRAINT_H

#include "compact_table_extension_constraint.h"

class Positive_Compact_Table_Extension_Constraint: public Compact_Table_Extension_Constraint    /// This class allows to represent constraints in extension \ingroup core
{		
  protected:
    vector<vector<int>> residues;   ///< the index of the word where a support was found for each pair (variable,value)
    set<unsigned int> S_val;        ///< the set of variables whose domain has been reduced
    set<unsigned int> S_sup;        ///< the set of variables whose domain contains more than one value
  
  public:			
		// constructor
		Positive_Compact_Table_Extension_Constraint (vector<Variable *> & var);				///< constructs an extension constraint whose scope is defined by the variables in var
		Positive_Compact_Table_Extension_Constraint (Positive_Compact_Table_Extension_Constraint & c);     ///< constructs a new constraint by copying the constraint c 

	
		// basic functions
 		double Get_Tightness () override;  				 						///< returns the tightness of the constraint
		double Get_Allowed_Tuple_Number () override;					///< returns the number of allowed tuples
    Constraint * Duplicate () override;	                  ///< returns a pointer on a copy of the constraint
		void Allow_Tuple (int * t) override;   								///< allows the tuple t
		void Forbid_Tuple (int * t) override;  								///< forbids the tuple t
 		bool Is_Satisfied (int * t) override;	 		 						///< returns true if the tuple t satisfies the constraint, false otherwise
		bool Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds) override;	 ///< returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
    string Get_XCSP3_Expression() override;               ///< returns the string corresponding to the expression of the constraint in XCSP 3 format
  
  private:
    bool Update_Table (CSP * pb, Deletion_Stack * ds, timestamp ref);   ///< updates the table and returns true iff the table is empty
    
  protected:
    void Filter_Domain (Deletion_Stack * ds);                           ///< filters the domain
};


//-----------------------------
// inline function definitions
//-----------------------------


inline double Positive_Compact_Table_Extension_Constraint::Get_Tightness ()
// returns the tightness of the constraint
{
  return possible_tuple_number - tuple_number;
}

   				 						
inline double Positive_Compact_Table_Extension_Constraint::Get_Allowed_Tuple_Number ()
// returns the number of allowed tuples
{
  return tuple_number;
}


inline void Positive_Compact_Table_Extension_Constraint::Allow_Tuple (int * t)
// allows the tuple t
{
  for (unsigned int j = 0; j < arity; j++)
    if (residues[j][t[j]] == -1)
      residues[j][t[j]] = tuple_number / WORD_SIZE;

  Add_Tuple (t);
}


inline void Positive_Compact_Table_Extension_Constraint::Forbid_Tuple (int * t)
// forbids the tuple t
{
  Remove_Tuple (t);
}
#endif
