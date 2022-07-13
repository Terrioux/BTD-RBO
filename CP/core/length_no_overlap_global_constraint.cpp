#include "length_no_overlap_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Length_No_Overlap_Global_Constraint::Length_No_Overlap_Global_Constraint (vector<Variable *> & var): Global_Constraint (var)
// constructs a new constraint which involves the variable in var
{	
	object_number = arity/2;
}


Length_No_Overlap_Global_Constraint::Length_No_Overlap_Global_Constraint (Length_No_Overlap_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	object_number = c.object_number;
}


//-----------------
// basic functions
//-----------------


bool Length_No_Overlap_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  for (unsigned int i = 0; i < object_number-1; i++)
  {
    int val_i = scope_var[i]->Get_Domain()->Get_Real_Value(t[i]);
    for (unsigned int j = i+1; j < object_number; j++)
    {
      int val_j = scope_var[j]->Get_Domain()->Get_Real_Value(t[j]);
    
      if ((val_i+scope_var[i+object_number]->Get_Domain()->Get_Real_Value(t[i+object_number]) > val_j) && (val_j+scope_var[j+object_number]->Get_Domain()->Get_Real_Value(t[j+object_number]) > val_i))
        return false;
    }
  }
  
	return true;
}


void Length_No_Overlap_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  
  bool again;     // true if a modification may trigger other modifications, false otherwise
  do
  {
    again = false;
    
    for (unsigned int i = 0; i < object_number; i++)
    {
      if ((event_manager->Exist_Event_Lbc (scope_var[i]->Get_Num(),ref)) || (event_manager->Exist_Event_Ubc (scope_var[i]->Get_Num(),ref)) || (ref == 0))
      {        
        for (unsigned int j = 0; j < object_number; j++)
          if (i != j)
          {
            int ind1 = -1;
            int ind2 = -1;
            
            if (! A.Is_Assigned (scope_var[j]->Get_Num()))
            {
              ind1 = i;
              ind2 = j;
            }
            else
              if (! A.Is_Assigned (scope_var[i]->Get_Num()))
              {
                ind1 = j;
                ind2 = i;
              }              
              
            if (ind1 != -1)
            {
              Domain * dx = scope_var[ind1]->Get_Domain();
              int val_max = dx->Get_Real_Min() + scope_var[ind1+object_number]->Get_Domain()->Get_Real_Min();
              int x_max = dx->Get_Real_Max();

              Domain * dy = scope_var[ind2]->Get_Domain();
              int val_min = x_max - scope_var[ind2+object_number]->Get_Domain()->Get_Real_Min();
              
              int v = dy->Get_Min();
              while ((v != -1) && (dy->Get_Real_Value(v) < val_min))
                v = dy->Next_Value(v);
                  
              if ((v != -1) && (dy->Get_Real_Value(v) < val_max))
              {
                if (dy->Get_Real_Value(v) == val_min)
                  v = dy->Next_Value(v);   // we skip val_min
                
                if ((v != -1) && (dy->Get_Real_Value(v) < val_max))   // we must check again the previous checked pair
                  again = true;
                
                while ((v != -1) && (dy->Get_Real_Value(v) < val_max))
                {
                  ds->Add_Element (scope_var[ind2]);
                  dy->Filter_Value (v);
                  v = dy->Next_Value(v);
                }
              
                if (dy->Get_Size() == 0)
                {
                  pb->Set_Conflict (scope_var[ind2],this);
                  return;
                }
              }
            }
          }
      }
      
      if (event_manager->Exist_Event_Fix (scope_var[i]->Get_Num(),ref))
      {
        int valx = scope_var[i]->Get_Domain()->Get_Remaining_Real_Value(0);
        
        for (unsigned int j = 0; j < object_number; j++)
          if (i != j)
          {
            if (scope_var[j]->Get_Domain()->Get_Real_Min() + scope_var[j+object_number]->Get_Domain()->Get_Real_Min() > valx)
            {
              Domain * d = scope_var[i+object_number]->Get_Domain();
              ds->Add_Element (scope_var[i+object_number]);
              
              unsigned int size = d->Get_Size();
              
              d->Filter_Value_From(scope_var[j]->Get_Domain()->Get_Real_Max() - valx +1);
              
              if (d->Get_Size() == 0)
              {
                pb->Set_Conflict (scope_var[i+object_number],this);
                return;
              }
              else
                if (size != d->Get_Size())
                  again = true;
            }
          }
      }
    }
    ref = event_manager->Get_Current_Time() - 1;
  }
  while (again);
}


string Length_No_Overlap_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<noOverlap>\n";
  expr += "  <origins> ";
  for (unsigned int i = 0; i < object_number; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</origins>\n";
  expr += "  <lengths> ";
  for (unsigned int i = object_number; i < arity; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</lengths>\n";
  expr += "</noOverlap>";
  return expr;
}
