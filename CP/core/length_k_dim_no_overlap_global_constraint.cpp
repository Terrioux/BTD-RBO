#include "length_k_dim_no_overlap_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Length_K_Dim_No_Overlap_Global_Constraint::Length_K_Dim_No_Overlap_Global_Constraint (vector<Variable *> & var, unsigned int nb_dim): Global_Constraint (var)
// constructs a new constraint which involves the variable in var
{	
  dimension_number = nb_dim;
  object_number = arity / (2*dimension_number);
}


Length_K_Dim_No_Overlap_Global_Constraint::Length_K_Dim_No_Overlap_Global_Constraint (Length_K_Dim_No_Overlap_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  dimension_number = c.dimension_number;
  object_number = c.object_number;
  object_number = arity / (2*dimension_number);
}


//-----------------
// basic functions
//-----------------


bool Length_K_Dim_No_Overlap_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  for (unsigned int i = 0; i < object_number-1; i++)
  {
    for (unsigned int j = i+1; j < object_number; j++)
    {
      unsigned int d = 0;
      while ((d < dimension_number) && (scope_var[i*dimension_number+d]->Get_Domain()->Get_Real_Value(t[i*dimension_number+d]) + scope_var[i*dimension_number+d+arity/2]->Get_Domain()->Get_Real_Value(t[i*dimension_number+d+arity/2]) > scope_var[j*dimension_number+d]->Get_Domain()->Get_Real_Value(t[j*dimension_number+d])) && (scope_var[j*dimension_number+d]->Get_Domain()->Get_Real_Value(t[j*dimension_number+d]) + scope_var[j*dimension_number+d+arity/2]->Get_Domain()->Get_Real_Value(t[j*dimension_number+d+arity/2]) > scope_var[i*dimension_number+d]->Get_Domain()->Get_Real_Value(t[i*dimension_number+d])))
        d++;
        
      if (d == dimension_number)
        return false;
    }
  }
  
	return true;
}


void Length_K_Dim_No_Overlap_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  
  bool again;     // true if a modification may trigger other modifications, false otherwise
  do
  {
    again = false;
    
    for (unsigned int i = 0; i < object_number; i++)
    {
      bool to_be_checked = ref == 0;
      for (unsigned int d = 0; (d < dimension_number) && (! to_be_checked); d++)
        if ((event_manager->Exist_Event_Fix (scope_var[i*dimension_number+d]->Get_Num(),ref)) || (event_manager->Exist_Event_Lbc (scope_var[i*dimension_number+d]->Get_Num(),ref)) || (event_manager->Exist_Event_Ubc (scope_var[i*dimension_number+d]->Get_Num(),ref)))
          to_be_checked = true;
          
      if (to_be_checked)
      {
        for (unsigned int j = 0; j < object_number; j++)
          if (i != j)
          {
            unsigned int nb_ok = 0;
            int selected_dim = -1;
            
            for (unsigned int d = 0; (d < dimension_number) && (nb_ok < 2); d++)
              if ((scope_var[i*dimension_number+d]->Get_Domain()->Get_Real_Min() + scope_var[i*dimension_number+d+arity/2]->Get_Domain()->Get_Real_Min() <= scope_var[j*dimension_number+d]->Get_Domain()->Get_Real_Max()) || (scope_var[j*dimension_number+d]->Get_Domain()->Get_Real_Min() + scope_var[j*dimension_number+d+arity/2]->Get_Domain()->Get_Real_Min() <= scope_var[i*dimension_number+d]->Get_Domain()->Get_Real_Max()))
              {
                selected_dim = d;
                nb_ok++;
              }

            if (nb_ok == 0)
            {
              pb->Set_Conflict (scope_var[j*dimension_number],this);
              return;
            }
            
            if (nb_ok == 1)
            {
              int ind1 = -1;
              int ind2 = -1;
              
              if (! A.Is_Assigned (scope_var[j*dimension_number+selected_dim]->Get_Num()))
              {
                ind1 = i;
                ind2 = j;
              }
              else
                if (! A.Is_Assigned (scope_var[i*dimension_number+selected_dim]->Get_Num()))
                {
                  ind1 = j;
                  ind2 = i;
                }              
                
              if (ind1 != -1)
              {
                Domain * dx = scope_var[ind1*dimension_number+selected_dim]->Get_Domain();
                int val_max = dx->Get_Real_Min() + scope_var[ind1*dimension_number+selected_dim+arity/2]->Get_Domain()->Get_Real_Min();
                int x_max = dx->Get_Real_Max();        
                
                Domain * dy = scope_var[ind2*dimension_number+selected_dim]->Get_Domain();
                int val_min = x_max - scope_var[ind2*dimension_number+selected_dim+arity/2]->Get_Domain()->Get_Real_Min();;
                
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
                    ds->Add_Element (scope_var[ind2*dimension_number+selected_dim]);
                    dy->Filter_Value (v);
                    v = dy->Next_Value(v);
                  }
                
                  if (dy->Get_Size() == 0)
                  {
                    pb->Set_Conflict (scope_var[ind2*dimension_number+selected_dim],this);
                    return;
                  }
                }
              }
              
              if (scope_var[i*dimension_number+selected_dim]->Get_Domain()->Get_Size() == 1)
              {
                int valx = scope_var[i*dimension_number+selected_dim]->Get_Domain()->Get_Remaining_Real_Value(0);
      
                if (scope_var[j*dimension_number+selected_dim]->Get_Domain()->Get_Real_Min() + scope_var[j*dimension_number+selected_dim+arity/2]->Get_Domain()->Get_Real_Min() > valx)
                {
                  Domain * d = scope_var[i*dimension_number+selected_dim+arity/2]->Get_Domain();
                  ds->Add_Element (scope_var[i*dimension_number+selected_dim+arity/2]);
                  
                  unsigned int size = d->Get_Size();
                  
                  d->Filter_Value_From(scope_var[j*dimension_number+selected_dim]->Get_Domain()->Get_Real_Max() - valx +1);
            
                  if (d->Get_Size() == 0)
                  {
                    pb->Set_Conflict (scope_var[i*dimension_number+selected_dim+arity/2],this);
                    return;
                  }
                  else
                    if (size != d->Get_Size())
                      again = true;
                }
              }
              
              if (scope_var[j*dimension_number+selected_dim]->Get_Domain()->Get_Size() == 1)
              {
                int valx = scope_var[j*dimension_number+selected_dim]->Get_Domain()->Get_Remaining_Real_Value(0);
      
                if (scope_var[i*dimension_number+selected_dim]->Get_Domain()->Get_Real_Min() + scope_var[i*dimension_number+selected_dim+arity/2]->Get_Domain()->Get_Real_Min() > valx)
                {
                  Domain * d = scope_var[j*dimension_number+selected_dim+arity/2]->Get_Domain();
                  ds->Add_Element (scope_var[j*dimension_number+selected_dim+arity/2]);
                  
                  unsigned int size = d->Get_Size();
                  
                  d->Filter_Value_From(scope_var[i*dimension_number+selected_dim]->Get_Domain()->Get_Real_Max() - valx +1);
            
                  if (d->Get_Size() == 0)
                  {
                    pb->Set_Conflict (scope_var[j*dimension_number+selected_dim+arity/2],this);
                    return;
                  }
                  else
                    if (size != d->Get_Size())
                      again = true;
                }
              }
            }
          }
      }
    }
    
    ref = event_manager->Get_Current_Time() - 1;
  }
  while (again);
}


string Length_K_Dim_No_Overlap_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<noOverlap>\n";
  expr += "  <origins> ";
  for (unsigned int i = 0; i < arity/2; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</origins>\n";
  expr += "  <lengths> ";
  for (unsigned int i = arity/2; i < arity; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</lengths>\n";
  expr += "</noOverlap>";
  return expr;
}
