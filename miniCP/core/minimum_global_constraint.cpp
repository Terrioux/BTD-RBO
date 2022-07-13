#include "minimum_global_constraint.h"
#include <climits>

//-----------------------------
// constructors and destructor
//-----------------------------


Minimum_Global_Constraint::Minimum_Global_Constraint (vector<Variable *> & var): Global_Constraint (var)
// constructs a minimum global constraint 
{
}
		
		
Minimum_Global_Constraint::Minimum_Global_Constraint (Minimum_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c 
{
}


//-----------------
// basic functions
//-----------------


bool Minimum_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  int min = scope_var[1]->Get_Domain()->Get_Real_Value(t[1]);
  
  for (unsigned int i = 2; i < arity; i++)
  {
    int value = scope_var[i]->Get_Domain()->Get_Real_Value(t[i]);
    
    if (value < min)
      min = value;
  }
  
  return min == scope_var[0]->Get_Domain()->Get_Real_Value(t[0]);
}


bool Minimum_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  unsigned int x = Get_Position (var);
  
	Domain * dx = scope_var [x]->Get_Domain();
  bool modif = false;
  
  if (x == 0)
  {
    int min = INT_MAX;
    for (unsigned int i = 1; i < arity; i++)
      if ((scope_var[i]->Get_Domain()->Get_Size() == 1) && (min > scope_var[i]->Get_Domain()->Get_Remaining_Real_Value(0)))
        min = scope_var[i]->Get_Domain()->Get_Remaining_Real_Value(0);
    
    unsigned int current_size = dx->Get_Size();
    
    if (min != INT_MAX)
    {
      ds->Add_Element (scope_var[0]);
      dx->Filter_Value_From (min+1);
    
      if (dx->Get_Size() == 0)
      {
        pb->Set_Conflict (scope_var[x],this);
        return true;
      }
      
      modif = current_size != dx->Get_Size();
    }
    
    int j = dx->Get_Size()-1;
    while (j >= 0)
    {
      int v = dx->Get_Remaining_Real_Value(j);
      int w;
      unsigned int i = 1; 
      bool has_no_support = true;
      Domain * dy;
      
      while ((i < arity) && (has_no_support))
      {
        dy = scope_var[i]->Get_Domain();
        w = dy->Get_Value(v);
        if ((w != -1) && (dy->Is_Present (w)))
          has_no_support = false;
        i++;
      }
      
      if (has_no_support)
      {
        modif = true;
        ds->Add_Element (scope_var[x]);
        dx->Filter_Value (dx->Get_Remaining_Value(j));
      }
      j--;
    }
  }
  else
    {
      // we remove the values greater than the minimum of the reference variable x0
      Domain * dy;
      
      dy = scope_var[0]->Get_Domain();
      int min = dy->Get_Real_Min();
  
      unsigned int current_size = dx->Get_Size();
      ds->Add_Element (scope_var[x]);
      dx->Filter_Value_Before (min-1);

      modif = current_size != dx->Get_Size();

      if (dx->Get_Size() == 0)
      {
        pb->Set_Conflict (scope_var[x],this);
        return true;
      }
      
      // we check whether the values greater than the minimum of the other list variables has a support w.r.t. x0
      min = INT_MAX;
      unsigned int i = 1;
      
      while (i < arity)
      {
        if (i != x)
        {
          dy = scope_var[i]->Get_Domain(); 
          int w = dy->Get_Real_Min();
          if (w < min)
            min = w;
        }
        i++;
      }
      
      dy = scope_var[0]->Get_Domain();
      int v = dx->Get_Min();
      int real_v;
      
      while ((v != -1) && ((real_v = dx->Get_Real_Value (v)) <= min))
      {
        int w = dy->Get_Value (real_v);
        if ((w == -1) || (! dy->Is_Present(w)))
        {
          modif = true;
          ds->Add_Element (scope_var[x]);
          dx->Filter_Value (v);
        }
        v = dx->Next_Value (v);
      }
    }
    
  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
    
  return modif;
}

#include<iostream>
void Minimum_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Domain * dx = scope_var[0]->Get_Domain();
  
  if (ref == 0)
  {
    // we check whether each value of the first variable has some support w.r.t. the other variables
    for (int j = dx->Get_Size()-1; j >= 0; j--)
    {
      int real_val = dx->Get_Remaining_Real_Value(j);
      
      bool has_no_support = true;
      for (unsigned int i = 1; (i < arity) && (has_no_support); i++)
      {
        Domain * dy = scope_var[i]->Get_Domain();
        
        int v = dy->Get_Value (real_val);
        
        if ((v != -1) && (dy->Is_Present (v)))
          has_no_support = false;
      }
      
      if (has_no_support)
      {
        ds->Add_Element (scope_var[0]);
        dx->Filter_Value(dx->Get_Remaining_Value(j));
      }
    }
    
    if (dx->Get_Size() == 0)
    {
      pb->Set_Conflict (scope_var[0],this);
      return;
    }
  }
  
  
  Event_Manager * event_manager = pb->Get_Event_Manager();
  
  bool again;
  
  do
  {
    again = false;
  
    if ((event_manager->Exist_Event_Lbc (scope_var[0]->Get_Num(),ref)) || (ref == 0))
    {
      // we look for useless values as soon as the minimum value of the first variable has been modified
      int real_min = dx->Get_Real_Min();

      Domain * dy;
      for (unsigned int i = 1; i < arity; i++)
      {
        dy = scope_var[i]->Get_Domain();
        ds->Add_Element (scope_var[i]);
        dy->Filter_Value_Before (real_min-1);

        if (dy->Get_Size() == 0)
        {
          pb->Set_Conflict (scope_var[i],this);
          return;
        }
      }
    }
    
    timestamp current_time = event_manager->Get_Current_Time();
    bool todo = true;
    
    unsigned int old_size = scope_var[0]->Get_Domain()->Get_Size();
    
    for (unsigned int i = 1; i < arity; i++)
    {
      int num_var = scope_var[i]->Get_Num();
      
      if (event_manager->Exist_Event_Lbc (num_var, ref))
      {
        if (todo)
        {
          int min = scope_var[1]->Get_Domain()->Get_Real_Min();
          for (unsigned int j = 2; j < arity; j++)
            if (scope_var[j]->Get_Domain()->Get_Real_Min() < min)
              min = scope_var[j]->Get_Domain()->Get_Real_Min();
              
          ds->Add_Element (scope_var[0]);
          dx->Filter_Value_Before(min-1);

          if (dx->Get_Size() == 0)
          {
            pb->Set_Conflict (scope_var[0],this);
            return;
          }
          
          todo = false;
        }
        
        if (event_manager->Exist_Event_Fix (num_var, ref))
        {
          // we remove all the values of the first variable which are greater than the value assigned to the ith variable
          int real_val = scope_var[i]->Get_Domain()->Get_Remaining_Real_Value(0);

          ds->Add_Element (scope_var[0]);
          dx->Filter_Value_From(real_val+1);

          if (dx->Get_Size() == 0)
          {
            pb->Set_Conflict (scope_var[0],this);
            return;
          }
        }
      }
      else
        if (event_manager->Exist_Event_Dmc (num_var, ref))
        {
          Domain * dy = scope_var[i]->Get_Domain();
          
          for (unsigned int j = 0; j < dy->Get_Initial_Size(); j++)
            if ((event_manager->Exist_Event_Dmc (num_var,j,ref)) && (! event_manager->Exist_Event_Dmc (num_var,j,current_time)))
            {
              int real_val = dy->Get_Real_Value(j);
              
              int v = dx->Get_Value (real_val);
              if ((v != -1) && (dx->Is_Present (v)))   // the value real_val belongs to dx
              {
                // we look for another support
                
                bool has_no_support = true;
                
                for (unsigned int k = 1; (k < arity) && (has_no_support); k++)
                  if (i != k)
                  {
                    Domain * dz = scope_var[k]->Get_Domain();
                    int w = dz->Get_Value (real_val);
                    
                    if ((w != -1) && (dz->Is_Present(w)))
                      has_no_support = false;
                  }
                  
                if (has_no_support)
                {
                  ds->Add_Element (scope_var[0]);
                  dx->Filter_Value(v);
                  if (dx->Get_Size() == 0)
                  {
                    pb->Set_Conflict (scope_var[0],this);
                    return;
                  }
                }
              }
            }
        }
    }
    if (old_size != scope_var[0]->Get_Domain()->Get_Size())
      again = true;
  }
  while (again);
}


string Minimum_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<minimum>\n";
  expr += "  <list> ";
  for (unsigned int i = 1; i < arity; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</list>\n";
  expr += "  <condition> (eq," + scope_var[0]->Get_Name() + ") </condition>\n";
  expr += "</minimum>";
  return expr;
}
