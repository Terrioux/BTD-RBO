/// \file 
/// \brief Definitions related to the Short_Positive_Compact_Table_Extension_Constraint class

#ifndef SHORT_POSITIVE_COMPACT_TABLE_EXTENSION_CONSTRAINT_H
#define SHORT_POSITIVE_COMPACT_TABLE_EXTENSION_CONSTRAINT_H

#include "positive_compact_table_extension_constraint.h"

class Short_Positive_Compact_Table_Extension_Constraint: public Positive_Compact_Table_Extension_Constraint    /// This class allows to represent constraints in extension \ingroup core
{		
  protected:
    vector<vector<vector<unsigned long>>> supports_star;      ///< indicates if the support is strictly accepted or not (i.e. the support contains a star value)
    
  public:			
		// constructor
		Short_Positive_Compact_Table_Extension_Constraint (vector<Variable *> & var);				///< constructs an extension constraint whose scope is defined by the variables in var
		Short_Positive_Compact_Table_Extension_Constraint (Short_Positive_Compact_Table_Extension_Constraint & c);     ///< constructs a new constraint by copying the constraint c 

	
		// basic functions
 		double Get_Tightness () override;  				 						///< returns the tightness of the constraint
		double Get_Allowed_Tuple_Number () override;					///< returns the number of allowed tuples
    Constraint * Duplicate () override;	                  ///< returns a pointer on a copy of the constraint
		void Allow_Tuple (int * t) override;   								///< allows the tuple t
		void Forbid_Tuple (int * t) override;  								///< forbids the tuple t
    void Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref) override;	 ///< applies the event-based propagator of the constraint by considering the events occurred since ref
  
  private:
    bool Update_Table (CSP * pb, Deletion_Stack * ds, timestamp ref);   ///< updates the table and returns true iff the table is empty
};


//-----------------------------
// inline function definitions
//-----------------------------


inline double Short_Positive_Compact_Table_Extension_Constraint::Get_Tightness ()
// returns the tightness of the constraint
{
  return tightness;
}

   				 						
inline double Short_Positive_Compact_Table_Extension_Constraint::Get_Allowed_Tuple_Number ()
// returns the number of allowed tuples
{
  return allowed_tuple_number;
}
#endif
