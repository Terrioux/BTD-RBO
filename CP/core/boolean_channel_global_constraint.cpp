#include "boolean_channel_global_constraint.h"


//--------------
// constructors 
//--------------


Boolean_Channel_Global_Constraint::Boolean_Channel_Global_Constraint (vector<Variable *> & var, int start): Global_Constraint (var)
// constructs a new channel constraint which involves the variable in var and whose start indices are respectively start1 and start2
{
  start_index = start;
  for (unsigned int i = 0; i < arity-1; i++)
  {
    Domain * d = scope_var[i]->Get_Domain();
    for (unsigned j = 0; j < d->Get_Initial_Size(); j++)
      if (d->Is_Present (j))
      {
        int val = d->Get_Real_Value (j);
        
        if ((val != 0) && (val != 1))
          d->Filter_Value(j);
      }
  }
  
  Domain * d = scope_var[arity-1]->Get_Domain();
  for (unsigned j = 0; j < d->Get_Initial_Size(); j++)
    if (d->Is_Present (j))
    {
      int val = d->Get_Real_Value (j) - start_index;
      
      if ((val < 0) || (val >= (signed) arity-1))
        d->Filter_Value(j);
    }
}


Boolean_Channel_Global_Constraint::Boolean_Channel_Global_Constraint (Boolean_Channel_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  start_index = c.start_index;
}


//-----------------
// basic functions
//-----------------


bool Boolean_Channel_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{  
  int pos_true = -1;
  for (unsigned int i = 0; i < arity-1; i++)
    if (t[i] == 1)
    {
      if (pos_true == -1)
        pos_true = i;
      else return false;
    }
  
  return (pos_true >= 0) &&  (t[arity-1] - start_index == pos_true);
}


void Boolean_Channel_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();

  for (unsigned int i = 0; i < arity-1; i++)
  {
    if (event_manager->Exist_Event_Fix(scope_var[i]->Get_Num(),ref))
    {
      int bit = scope_var[i]->Get_Domain()->Get_Remaining_Real_Value(0);
      
      if (bit == 1)
      {
        Domain * d = scope_var[arity-1]->Get_Domain();
        int v = d->Get_Value (i + start_index);
        
        ds->Add_Element (scope_var[arity-1]);
        
        if (v == -1)
          d->WipeOut();
        else d->Assign(v);
        
        if (d->Get_Size() == 0)
        {
          pb->Set_Conflict (scope_var[arity-1],this);
          return;
        }
        
        for (unsigned int k = 0; k < arity-1; k++)
          if (k != i)
          {       
            Domain * d = scope_var[k]->Get_Domain();
            
            int v = d->Get_Value (0);
            
            if (v != -1)
            {
              ds->Add_Element (scope_var[k]);
              d->Assign (v);
            
              if (d->Get_Size() == 0)
              {
                pb->Set_Conflict (scope_var[k],this);
                return;
              }
            }
          }
        return;
      }
      else
        {
          Domain * d = scope_var[arity-1]->Get_Domain();
          int v = d->Get_Value (i + start_index);

          if ((v != -1) && (d->Is_Present(v)))
          {
            ds->Add_Element (scope_var[arity-1]);
            d->Filter_Value(v);

            if (d->Get_Size() == 0)
            {
              pb->Set_Conflict (scope_var[arity-1],this);
              return;
            }
          }
        }
    }
  }
  
  if (event_manager->Exist_Event_Fix(scope_var[arity-1]->Get_Num(),ref))
  {
    int j = scope_var[arity-1]->Get_Domain()->Get_Remaining_Real_Value(0) - start_index;
    
    for (unsigned int i = 0; i < arity-1; i++)
    {
      Domain * d = scope_var[i]->Get_Domain();
      
      int v;
      
      if ((signed) i == j)
        v = d->Get_Value (1);
      else v = d->Get_Value (0);
      
      if (v != -1)
      {
        ds->Add_Element (scope_var[i]);
        d->Assign (v);
      
        if (d->Get_Size() == 0)
        {
          pb->Set_Conflict (scope_var[i],this);
          return;
        }
      }
    }
  }
  else
    if (event_manager->Exist_Event_Dmc(scope_var[arity-1]->Get_Num(),ref))
    {
      unsigned int delta_x;
      Domain * dx = scope_var[arity-1]->Get_Domain();
      int num_var = scope_var[arity-1]->Get_Num();
      unsigned int nb_removed_values = dx->Get_Initial_Size() - dx->Get_Size();

      for (delta_x = 0; (delta_x < nb_removed_values) && (event_manager->Exist_Event_Dmc (num_var, dx->Get_Deleted_Value(delta_x), ref)); delta_x++);
      
      for (unsigned int pos = 0; pos < delta_x; pos++)
      {
        int j = dx->Get_Deleted_Real_Value(pos) - start_index;

        if ((j >= 0) && (j < (signed) arity -1))
        {
          Domain * d = scope_var[j]->Get_Domain();
          
          int v = d->Get_Value (0);
          
          if (v != -1)
          {
            ds->Add_Element (scope_var[j]);
            d->Assign (v);
          
            if (d->Get_Size() == 0)
            {
              pb->Set_Conflict (scope_var[j],this);
              return;
            }
          }
        }
      }
    }
}


string Boolean_Channel_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<channel> ";
  expr += "  <list> ";
  for (unsigned int i = 0; i < arity-1; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "<list>\n";
  expr += "  <value> " + scope_var[arity-1]->Get_Name() + " </value> ";
  expr += "</channel>";
  return expr;
}
