#include "channel_global_constraint.h"


//--------------
// constructors 
//--------------


Channel_Global_Constraint::Channel_Global_Constraint (vector<Variable *> & var, int start): Global_Constraint (var)
// constructs a new channel constraint which involves the variable in var and whose start indices are respectively start1 and start2
{
  start_index = start;
  for (unsigned int i = 0; i < arity; i++)
  {
    Domain * d = scope_var[i]->Get_Domain();
    for (unsigned j = 0; j < d->Get_Initial_Size(); j++)
      if (d->Is_Present (j))
      {
        int val = d->Get_Real_Value (j) - start_index;
        
        if ((val < 0) || (val >= (signed) arity))
          d->Filter_Value(j);
      }
  }
}


Channel_Global_Constraint::Channel_Global_Constraint (Channel_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  start_index = c.start_index;
}


//-----------------
// basic functions
//-----------------


bool Channel_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{  
  for (unsigned int i = 0; i < arity; i++)
  {
    int j = scope_var[i]->Get_Domain()->Get_Real_Value(t[i]) - start_index;
    if (scope_var[i]->Get_Domain()->Get_Real_Value(t[j]) != (start_index+(signed) i))
      return false;
  }
	return true;
}


bool Channel_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
	Domain * dx = scope_var[x]->Get_Domain();
	bool modif = false;
			
	int i = dx->Get_Size()-1;
  
  int y = 0;
  while ((y < (signed) arity) && ((y == x) || (scope_var[y]->Get_Domain()->Get_Size() != 1) || (scope_var[y]->Get_Domain()->Get_Remaining_Real_Value(0)-start_index != x)))
    y++;
    
  if (y < (signed) arity)
  {
    ds->Add_Element (scope_var[x]);
    dx->Assign (dx->Get_Value (y+start_index));
    modif = true;    
  }
  else
    {
      do
      {
        int val = dx->Get_Remaining_Real_Value (i) - start_index;

        if (val != x)
        {
          Domain * d = scope_var[val]->Get_Domain();
          int w = d->Get_Value(x+start_index);
        
          if ((w == -1) || (! d->Is_Present (w)))
          {
            ds->Add_Element (scope_var[x]);
            dx->Filter_Value (dx->Get_Remaining_Value (i));
            modif = true;          
          }
        }
        
        i--;
      }
      while (i >= 0);
    }
  
  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
	
  return modif;
}


void Channel_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
 
  bool again;
  
  do
  {
    again = false;
 
    for (unsigned int i = 0; i < arity; i++)
    {
      if (event_manager->Exist_Event_Fix(scope_var[i]->Get_Num(),ref))
      {
        int j = scope_var[i]->Get_Domain()->Get_Remaining_Real_Value(0) - start_index;
        
        Domain * d = scope_var[j]->Get_Domain();
        int v = d->Get_Value (i + start_index);
        
        ds->Add_Element (scope_var[j]);
        
        if (v == -1)
          d->WipeOut();
        else d->Assign(v);
        
        if (d->Get_Size() == 0)
        {
          pb->Set_Conflict (scope_var[j],this);
          return;
        }
        
        for (unsigned int k = 0; k < arity; k++)
          if ((k != i) && ((signed) k != j))
          {       
            Domain * d = scope_var[k]->Get_Domain();
            
            int v = d->Get_Value (i + start_index);
            
            if ((v != -1) && (d->Is_Present (v)))
            {
              ds->Add_Element (scope_var[k]);
              d->Filter_Value (v);
            }
            
            v = d->Get_Value (j + start_index);
            
            if ((v != -1) && (d->Is_Present (v)))
            {
              ds->Add_Element (scope_var[k]);
              d->Filter_Value (v);
            }
            
            if (d->Get_Size() == 0)
            {
              pb->Set_Conflict (scope_var[j],this);
              return;
            }
            else
              if ((k < i) && (d->Get_Size() == 1))
                again = true;
          }
      }
      else
        if (event_manager->Exist_Event_Dmc(scope_var[i]->Get_Num(),ref))
        {
          unsigned int delta_x;
          Domain * dx = scope_var[i]->Get_Domain();
          int num_var = scope_var[i]->Get_Num();
          unsigned int nb_removed_values = dx->Get_Initial_Size() - dx->Get_Size();
          
          for (delta_x = 0; (delta_x < nb_removed_values) && (event_manager->Exist_Event_Dmc (num_var, dx->Get_Deleted_Value(delta_x), ref)); delta_x++);
          
          for (unsigned int pos = 0; pos < delta_x; pos++)
          {
            int j = dx->Get_Deleted_Real_Value(pos) - start_index;
            
            if ((j >= 0) && (j < (signed) arity))
            {
              Domain * d = scope_var[j]->Get_Domain();
              
              int v = d->Get_Value (i + start_index);
              
              if ((v != -1) && (d->Is_Present (v)))
              {
                ds->Add_Element (scope_var[j]);
                d->Filter_Value (v);

                if (d->Get_Size() == 0)
                {
                  pb->Set_Conflict (scope_var[j],this);
                  return;
                }
                else
                  if ((j < (signed) i) && (d->Get_Size() == 1))
                    again = true;
              }
            }
          }
        }
    }
    
    ref = event_manager->Get_Current_Time() - 1;
  }
  while (again);
}


string Channel_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<channel> ";
  expr += "  <list> ";
  for (vector<Variable *>::iterator iter = scope_var.begin(); iter != scope_var.end(); iter++)
    expr +=  (*iter)->Get_Name() + " ";
  expr += "<list>\n";
  expr += "</channel>";
  return expr;
}
