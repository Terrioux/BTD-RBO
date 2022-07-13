#include "circuit_global_constraint.h"
#include <queue>


//-----------------------------
// constructors and destructor
//-----------------------------


Circuit_Global_Constraint::Circuit_Global_Constraint (vector<Variable *> & var): Global_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that all the variables are equal
{
  // we remove the values that are out of the set [0...arity-1]
  for (unsigned int i = 0; i < arity; i++)
  {
    scope_var[i]->Get_Domain()->Filter_Value_Before(-1);
    scope_var[i]->Get_Domain()->Filter_Value_From(arity);
  }
}


Circuit_Global_Constraint::Circuit_Global_Constraint (Circuit_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
}


Circuit_Global_Constraint::~Circuit_Global_Constraint ()
// destructor
{
}


//-----------------
// basic functions
//-----------------


bool Circuit_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  int starting_vertex = -1;
  unsigned nb_vertices = 0;
	for (unsigned int i = 0; i < arity; i++)
    if (scope_var[i]->Get_Domain()->Get_Real_Value (t[i]) != (signed) i)
    {
      starting_vertex = i;    // the last vertex is considered as the starting vertex of the tested circuit
      nb_vertices++;
    }

  if (nb_vertices > 1)
  {
    vector<bool> not_used (arity,true);
    int val = scope_var[starting_vertex]->Get_Domain()->Get_Real_Value (t[starting_vertex]);
    unsigned int i = 0;
    while (not_used[val])
    {
      not_used[val] = false;
      val = scope_var[val]->Get_Domain()->Get_Real_Value (t[val]);
      i++;
    }
    return i == nb_vertices;
  }
  else return false;
}


void Circuit_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();

  queue<unsigned int> Q;
  
	for (unsigned int i = 0; i < arity; i++)
  {
    Domain * d = scope_var[i]->Get_Domain();
    int num_var = scope_var[i]->Get_Num();
    
    if (d->Get_Size() == 1)
    {
      if (event_manager->Exist_Event_Fix (num_var, ref))
        Q.push(i);
    }
    else
      {
        if(event_manager->Exist_Event_Dmc (num_var, ref))
          Q.push(i);
      }
  }
  
  while (! Q.empty())
  {
    unsigned int i = Q.front();
    Q.pop();
    
    Domain * d = scope_var[i]->Get_Domain();
    
    if (d->Get_Size() == 1)
    {
      // we remove the assigned value from the domain of any variable different from the i-th variable
      int val_i = d->Get_Remaining_Real_Value(0);
      
      for (unsigned int j = 0; j < arity; j++)
        if (i != j)
        {
          // we remove the value val_i from the domain of the j-th variable
          Domain * dy = scope_var[j]->Get_Domain();
          int val_j = dy->Get_Value(val_i);
          if ((val_j != -1) && (dy->Is_Present (val_j)))
          {
            ds->Add_Element (scope_var[j]);
            dy->Filter_Value(val_j);
            
            if (dy->Get_Size() == 0)
            {
              pb->Set_Conflict (scope_var[j],this);
              return;
            }
            else
              if (dy->Get_Size() == 1)
              {
                Q.push(j);
              }
          }
        } 
    }
    else
      {
        // the variable/vertex has an unknown status or belongs to the desired circuit but at least one of its values has been removed
        timestamp current_time = event_manager->Get_Current_Time();
        int num_var = scope_var[i]->Get_Num();

        for (unsigned int j = 0; j < d->Get_Initial_Size(); j++)
          if ((event_manager->Exist_Event_Dmc (num_var,j,ref)) && (! event_manager->Exist_Event_Dmc (num_var,j,current_time)))
          {
            int real_val = d->Get_Real_Value(j);

            // Existe-t-il encore une variable avec la valeur i ?
            // si une seule, on l'affecte
            // si aucune erreur

            
            // we check whether there is a single variable whose domain contains the value real_val
            int nb_var = 0;
            Variable * var = 0;
            for (unsigned int k = 0; (k < arity) && (nb_var <= 2); k++)
              if (i != k)
              {
                Domain * dk = scope_var[k]->Get_Domain();
                int val = dk->Get_Value(real_val);
              
                if (dk->Is_Present(val))
                {
                  nb_var++;
                  var = scope_var[k];
                }
              }
              
            if (nb_var == 1)
            {
              ds->Add_Element (var);
              Domain * dk = var->Get_Domain();
              int val = dk->Get_Value(real_val);
              dk->Assign(val);
              
              if (dk->Get_Size() == 0)
              {
                pb->Set_Conflict (var,this);
                return;
              }
            }
            else
              if (nb_var == 0)
              {
                pb->Set_Conflict (scope_var[i],this);
                return;
              }
          }
        
      }
  }
  
  if (unassigned_variable_number == 1) // ou s'il ne reste qu'un seul domaine non singleton
  {
    // we look for the unassigned variable
    unsigned int i = 0; 
    while (A.Is_Assigned(scope_var[i]->Get_Num()))
      i++;
    
    int t[arity];
    for (unsigned int j = 0; j < arity; j++)
      if (i != j)
        t[j] = scope_var[j]->Get_Domain()->Get_Remaining_Value(0);

    ds->Add_Element (scope_var[i]);
    Domain * d = scope_var[i]->Get_Domain();
    for (int j = d->Get_Size()-1; j >= 0; j--)
    {
      t[i] = d->Get_Remaining_Value (j);
      if (! Is_Satisfied(t))
        d->Filter_Value(t[i]);
    }
    
    if (d->Get_Size() == 0)
    {
      pb->Set_Conflict (scope_var[i],this);
      return;
    }
  }
  else
    {
      // we check whether there exists a circuit starting from a singleton domain
      bool exists_circuit = false;
      for (unsigned int i = 0; (i < arity) && (! exists_circuit); i++)
      {
        Domain * d = scope_var[i]->Get_Domain();
        if ((d->Get_Size() == 1) && (d->Get_Remaining_Real_Value(0) != (signed) i))
        {
          // we check whether there exists a circuit starting from i
          vector<bool> not_used (arity,true);
          unsigned int val = i;
          unsigned int size = 0;
          while ((not_used[val]) && (scope_var[val]->Get_Domain()->Get_Size() == 1))
          {
            not_used[val] = false;
            val = scope_var[val]->Get_Domain()->Get_Remaining_Real_Value (0);
            size++;
          }
          
          if (val == i)
          {
            exists_circuit = true;
            
            // all the variables which do not belong to this circuit must be assigned to their index
            for (unsigned int j = 0; j < arity; j++)
              if (not_used[j])
              {
                ds->Add_Element (scope_var[j]);
                Domain * dj = scope_var[j]->Get_Domain();
                int val = dj->Get_Value(j);
                
                if (val != -1)
                  dj->Assign(val);
                else dj->WipeOut();
                
                if (dj->Get_Size() == 0)
                {
                  pb->Set_Conflict (scope_var[j],this);
                  return;
                }    
              }
          }
        }
      }
    }  
}


string Circuit_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<circuit> ";
  for (vector<Variable *>::iterator iter = scope_var.begin(); iter != scope_var.end(); iter++)
    expr +=  (*iter)->Get_Name() + " ";
  expr += "</circuit>";
  return expr;
}
