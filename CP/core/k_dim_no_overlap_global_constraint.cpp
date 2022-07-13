#include "k_dim_no_overlap_global_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------

K_Dim_No_Overlap_Global_Constraint::K_Dim_No_Overlap_Global_Constraint (vector<Variable *> & var, vector<int> & vec_lengths, unsigned int nb_dim): Global_Constraint (var)
// constructs a new constraint which involves the variable in var
{	
  dimension_number = nb_dim;
  object_number = arity / dimension_number;
	lengths = vec_lengths;  
}


K_Dim_No_Overlap_Global_Constraint::K_Dim_No_Overlap_Global_Constraint (K_Dim_No_Overlap_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  dimension_number = c.dimension_number;
  object_number = c.object_number;
	lengths = c.lengths;
}


//-----------------
// basic functions
//-----------------


bool K_Dim_No_Overlap_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  for (unsigned int i = 0; i < object_number-1; i++)
  {
    for (unsigned int j = i+1; j < object_number; j++)
    {
      unsigned int d = 0;
      while ((d < dimension_number) && (scope_var[i*dimension_number+d]->Get_Domain()->Get_Real_Value(t[i*dimension_number+d]) + lengths[i*dimension_number+d] > scope_var[j*dimension_number+d]->Get_Domain()->Get_Real_Value(t[j*dimension_number+d])) && (scope_var[i*dimension_number+d]->Get_Domain()->Get_Real_Value(t[j*dimension_number+d]) + lengths[j*dimension_number+d] > scope_var[i*dimension_number+d]->Get_Domain()->Get_Real_Value(t[i*dimension_number+d])))
        d++;
        
      if (d == dimension_number)
        return false;
    }
  }
  
	return true;
}


void K_Dim_No_Overlap_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
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
      {
        unsigned int num_var = scope_var[i*dimension_number+d]->Get_Num();
        if ((event_manager->Exist_Event_Fix (num_var,ref)) || (event_manager->Exist_Event_Lbc (num_var,ref)) || (event_manager->Exist_Event_Ubc (num_var,ref)))
          to_be_checked = true;
      }
      
      if (to_be_checked)
      {
        for (unsigned int j = 0; j < object_number; j++)
          if (i != j)
          {
            unsigned int nb_ok = 0;
            int selected_dim = -1;

            for (unsigned int d = 0; (d < dimension_number) && (nb_ok < 2); d++)
              if ((scope_var[i*dimension_number+d]->Get_Domain()->Get_Real_Min() + lengths[i*dimension_number+d] <= scope_var[j*dimension_number+d]->Get_Domain()->Get_Real_Max()) || (scope_var[j*dimension_number+d]->Get_Domain()->Get_Real_Min() + lengths[j*dimension_number+d] <= scope_var[i*dimension_number+d]->Get_Domain()->Get_Real_Max()))
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
                int val_max = dx->Get_Real_Min() + lengths[ind1*dimension_number+selected_dim];
                int val_min = dx->Get_Real_Max() - lengths[ind2*dimension_number+selected_dim];
                
                Domain * dy = scope_var[ind2*dimension_number+selected_dim]->Get_Domain();
               
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
            }
          }
        }
      }
    
    ref = event_manager->Get_Current_Time() - 1;
  }
  while (again);
}


string K_Dim_No_Overlap_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<noOverlap>\n";
  expr += "  <origins> ";
  for (unsigned int i = 0; i < arity; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</origins>\n";
  expr += "  <lengths> ";
  for (unsigned int i = 0; i < arity; i++)
    expr +=  to_string(lengths[i])+ " ";
  expr += "</lengths>\n";
  expr += "</noOverlap>";
  return expr;
}
