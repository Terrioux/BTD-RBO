#include "constraint_tools.h"
#include <typeinfo>


// constraint types
#include "positive_nary_extension_constraint.h"
#include "negative_nary_extension_constraint.h"
#include "binary_extension_constraint.h"
#include "predicate_constraint.h"

Constraint * Predicate_To_Extension (Predicate_Constraint * c)
// returns an extension constraint corresponding to the predicate constraint c
{
	if (c->Get_Arity () == 2)
	{
		Binary_Extension_Constraint * c_ext;
		
		// we create the new constraint
		
		c_ext = new Binary_Extension_Constraint (c->Get_Scope_Var(),false);

		// we build the relation equivalent to one of c
		int d1 = c_ext->Get_Scope_Variable(0)->Get_Domain()->Get_Initial_Size();
		int d2 = c_ext->Get_Scope_Variable(1)->Get_Domain()->Get_Initial_Size();
		int t[2];

		for (t[0] = 0; t[0] < d1; t[0]++)
			for (t[1] = 0; t[1] < d2; t[1]++)
				if (c->Is_Satisfied (t))
					c_ext->Allow_Tuple(t);
    
    return c_ext;
	}
	else
		{
			Nary_Extension_Constraint * c_ext;
		
      // we create the new constraint
      
      c_ext = new Positive_Nary_Extension_Constraint (c->Get_Scope_Var());
    
      // we build the relation equivalent to one of c
      Real_Tuple t (c->Get_Arity());
      t.Complete (c->Get_Scope_Var());
      
      bool again;
      Expression expr = c->Get_Relation(); 
      
      do
      {
        if (expr.Evaluate (t.Get_Real_Values()) != 0)
          c_ext->Allow_Tuple (t.Get_Values());
        again = t.Next_Valid (c->Get_Scope_Var());	
      }
      while (again);
      
      return c_ext;
    }
}


void All_In_Extension (CSP * pb, int max_arity)
// replaces the constraints in intention whose arity is not greater than max_arity by a corresponding constraint in extenson
{
	Constraint * c;
	
	for (unsigned int i = 0; i < pb->Get_M(); i++)
	{
		c = pb->Get_Constraint (i);
		
		if (((signed) c->Get_Arity () <= max_arity) && (dynamic_cast<Predicate_Constraint *>(c) != 0))
			pb->Replace_Constraint (i,Predicate_To_Extension (dynamic_cast<Predicate_Constraint *>(c)));
	}
}


