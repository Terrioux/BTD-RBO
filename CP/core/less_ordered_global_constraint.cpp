#include "less_ordered_global_constraint.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Less_Ordered_Global_Constraint::Less_Ordered_Global_Constraint (vector<Variable *> & var, vector<int> & len, bool equal): Ordered_Global_Constraint (var,len,equal)
// constructs a new ordered global constraint
{
}
		
		
Less_Ordered_Global_Constraint::Less_Ordered_Global_Constraint (Less_Ordered_Global_Constraint & c): Ordered_Global_Constraint (c)
// constructs a new constraint by copying the global constraint c 
{
}


//-----------------
// basic functions
//-----------------


void Less_Ordered_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  set<int> to_be_checked;

  if (ref == 0)
  {
    for (unsigned int i = 0; i < arity; i++)
      to_be_checked.insert(i);
  }
  else
    {
      for (unsigned int i = 0; i < arity; i++)
      {
        if ((event_manager->Exist_Event_Lbc (scope_var[i]->Get_Num(),ref)) && (i < arity-1))
          to_be_checked.insert(i+1);
        if ((event_manager->Exist_Event_Ubc (scope_var[i]->Get_Num(),ref)) && (i > 0))
          to_be_checked.insert(i-1);
      }
    }

  while (! to_be_checked.empty())
  {
    unsigned int pos = *(to_be_checked.begin());
    to_be_checked.erase(to_be_checked.begin());
    
    Domain * d = scope_var[pos]->Get_Domain();

    ds->Add_Element (scope_var[pos]);
    
    if (pos > 0)
    {
      int ref = scope_var[pos-1]->Get_Domain()->Get_Real_Min() + lengths[pos-1];
      if (or_equal)
        ref--;
      int v_min = d->Get_Min();
      
      if (d->Get_Real_Value(v_min) <= ref)
      {
        d->Filter_Value_Before (ref);
  
        if (d->Get_Size() == 0)
        {
          pb->Set_Conflict (scope_var[pos],this);
          return;
        }
      
        if ((v_min != d->Get_Min()) && (pos < arity-1))
          to_be_checked.insert(pos+1);
      }
    }
    
    if (pos < arity-1)
    {
      int ref = scope_var[pos+1]->Get_Domain()->Get_Real_Max() - lengths[pos];
      if (or_equal)
        ref++;
      int v_max = d->Get_Max();
      
      if (d->Get_Real_Value(v_max) >= ref)
      {
        d->Filter_Value_From (ref);

        if (d->Get_Size() == 0)
        {
          pb->Set_Conflict (scope_var[pos],this);
          return;
        }
      
        if ((v_max != d->Get_Max()) && (pos > 0))
          to_be_checked.insert(pos-1);
      }
    }
  }
}
