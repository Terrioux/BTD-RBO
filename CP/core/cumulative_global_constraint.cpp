#include "cumulative_global_constraint.h"
#include <queue>

#include <iostream>

//-----------------------------
// constructors and destructor
//-----------------------------


Cumulative_Global_Constraint::Cumulative_Global_Constraint (vector<Variable *> & var): Global_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that all the variables are equal
{
  // we assume that the scope contains first the origins, then the ends, the heights and the variable to which the comparison is related.
  task_number = (arity-1) / 3;
  
  min_time = scope_var[0]->Get_Domain()->Get_Real_Min();
  max_time = scope_var[task_number]->Get_Domain()->Get_Real_Max();
  for (unsigned int i = 1; i < task_number; i++)
  {
    if (min_time > scope_var[i]->Get_Domain()->Get_Real_Min())
      min_time = scope_var[i]->Get_Domain()->Get_Real_Min();
    
    if (max_time < scope_var[i+task_number]->Get_Domain()->Get_Real_Max())
      max_time = scope_var[i+task_number]->Get_Domain()->Get_Real_Max();
  }
}


Cumulative_Global_Constraint::Cumulative_Global_Constraint (Cumulative_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
}


//-----------------
// basic functions
//-----------------


bool Cumulative_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  for (int time = min_time; time < max_time; time++)
  {
    int sum = 0;
    for (unsigned int j = 0; j < task_number; j++)
      if ((scope_var[j]->Get_Domain()->Get_Real_Value(t[j]) <= time) && (time < scope_var[j+task_number]->Get_Domain()->Get_Real_Value(t[j+task_number])))
        sum += scope_var[j+2*task_number]->Get_Domain()->Get_Real_Value(t[j+2*task_number]);
        
    if (sum > scope_var[arity-1]->Get_Domain()->Get_Real_Value(t[arity-1]))
      return false;
  }
  return true;
}


void Cumulative_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{  
  Event_Manager * event_manager = pb->Get_Event_Manager();

  to_check.clear();
  
  if ((ref == 0) || (event_manager->Exist_Event_Ubc (scope_var[arity-1]->Get_Num(), ref)))
  {
    for (int time = min_time; time < max_time; time++)
      to_check.insert(time);
  }
  else
    {
      Event_Manager * event_manager = pb->Get_Event_Manager();
      
      // we check if a task has been modified and if a check should be triggered
      for (unsigned int i = 0; i < task_number; i++)
      {
        if ((event_manager->Exist_Event_Ubc (scope_var[i]->Get_Num(), ref)) || (event_manager->Exist_Event_Lbc (scope_var[i+task_number]->Get_Num(), ref)) || (event_manager->Exist_Event_Lbc (scope_var[i+2*task_number]->Get_Num(), ref)))
        {
          // the task i has been modified and so the mandatory part should be checked
          for (int time = scope_var[i]->Get_Domain()->Get_Real_Max(); time < scope_var[i+task_number]->Get_Domain()->Get_Real_Min(); time++)
            to_check.insert(time);
        }
      }
    }

  for (set<int>::iterator iter = to_check.begin(); iter != to_check.end(); iter++)
  {
    int time = *iter;
  
    // we compute the mandatory part w.r.t. to time
    int sum = 0;
    for (unsigned int i = 0; i < task_number; i++)
      if ((scope_var[i]->Get_Domain()->Get_Real_Max() <= time) && (time < scope_var[i+task_number]->Get_Domain()->Get_Real_Min()))
        sum += scope_var[i+2*task_number]->Get_Domain()->Get_Real_Min();
  
    if (sum > scope_var[arity-1]->Get_Domain()->Get_Real_Max())
    {
      // we have a conflict
      pb->Set_Conflict (scope_var[arity-1],this);
      return;
    }
    else
      {
        // we try to update the capacity variable
        if (sum > scope_var[arity-1]->Get_Domain()->Get_Real_Min())
        {
          ds->Add_Element (scope_var[arity-1]);
          scope_var[arity-1]->Get_Domain()->Filter_Value_Before(sum-1);
        }
        
        // we update the origin and end variables
        for (unsigned int i = 0; i < task_number; i++)
          if (scope_var[i]->Get_Domain()->Get_Real_Max() <= time)
          {
            if (time >= scope_var[i+task_number]->Get_Domain()->Get_Real_Min())
            {
              if (sum + scope_var[i+2*task_number]->Get_Domain()->Get_Real_Min() > scope_var[arity-1]->Get_Domain()->Get_Real_Max())
              {
                // the task i cannot participate to the mandatory part for the current time
                Domain * d = scope_var[i+task_number]->Get_Domain();
                ds->Add_Element (scope_var[i+task_number]);
                d->Filter_Value_From(time+1);
                
                if (d->Get_Size() == 0)
                {
                  pb->Set_Conflict (scope_var[i+task_number],this);
                  return;
                }
              }
            }
            else
              {
                // the task i partipates to the mandatory part, we update if possible its height
                if (sum - scope_var[i+2*task_number]->Get_Domain()->Get_Real_Min() + scope_var[i+2*task_number]->Get_Domain()->Get_Real_Max() > scope_var[arity-1]->Get_Domain()->Get_Real_Max())
                {
                  Domain * d = scope_var[i+2*task_number]->Get_Domain();
                  ds->Add_Element (scope_var[i+2*task_number]);
                  d->Filter_Value_From(scope_var[arity-1]->Get_Domain()->Get_Real_Max() - sum + scope_var[i+2*task_number]->Get_Domain()->Get_Real_Min() + 1);
                }
              }
          }
          else
            if (time < scope_var[i+task_number]->Get_Domain()->Get_Real_Min())
            {
              if (sum + scope_var[i+2*task_number]->Get_Domain()->Get_Real_Min() > scope_var[arity-1]->Get_Domain()->Get_Real_Max())
              {
                // the task i cannot participate to the mandatory part for the current time
                Domain * d = scope_var[i]->Get_Domain();
                ds->Add_Element (scope_var[i]);
                d->Filter_Value_Before(time);
          
                if (d->Get_Size() == 0)
                {
                  pb->Set_Conflict (scope_var[i],this);
                  return;
                }
              }
            }
      }
    
  }
}

string Cumulative_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<cumulative>\n";
  expr += "  <origins> ";
  for (unsigned int i = 0; i < task_number; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "<origins>\n";
  // the lengths are wrong
  expr += "  <lengths> ";
  for (unsigned int i = 0; i < task_number; i++)
    expr +=  scope_var[i+task_number]->Get_Name() + " ";
  expr += "<lengths>\n";
  expr += "  <heights> ";
  for (unsigned int i = 0; i < task_number; i++)
    expr +=  scope_var[i+2*task_number]->Get_Name() + " ";
  expr += "<heights>\n";
  expr += "  <condition> (le,"+ scope_var[arity-1]->Get_Name() + ") </condition>";
  expr += "</cumulative>";
  return expr;
}
