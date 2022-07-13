#include "index_element_global_constraint.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Index_Element_Global_Constraint::Index_Element_Global_Constraint (vector<Variable *> & var): Global_Constraint (var)
// constructs a new element constraint with the first variable in var represents the index variable and the last variable in var as the variable value, and which involves the variable in var
{
  Domain * di = scope_var[arity-2]->Get_Domain();
  
  for (unsigned int v = 0; v < di->Get_Initial_Size(); v++)
    if ((di->Is_Present(v)) && ((di->Get_Real_Value(v) < 0) || (di->Get_Real_Value(v) >= (signed) arity-2)))
      di->Filter_Value (v);   // impossible value
    
  Domain * dv = scope_var[arity-1]->Get_Domain();
  
  list_value = new int * [dv->Get_Initial_Size()];
  
  for (unsigned int v = 0; v < dv->Get_Initial_Size(); v++)
    if (dv->Is_Present(v))
    {
      int value = dv->Get_Real_Value(v);
     
      list_value[v] = new int [arity-2];
      
      for (unsigned i = 0; i < arity-2; i++)
        list_value[v][i] = scope_var[i]->Get_Domain()->Get_Value (value);
    }
    else list_value[v] = 0;
}
		
		
Index_Element_Global_Constraint::Index_Element_Global_Constraint (Index_Element_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c
{
  Domain * di = scope_var[arity-2]->Get_Domain();
  
  for (unsigned int v = 0; v < di->Get_Initial_Size(); v++)
    if ((di->Is_Present(v)) && ((di->Get_Real_Value(v) < 0) || (di->Get_Real_Value(v) >= (signed) arity-2)))
      di->Filter_Value (v);   // impossible value

  Domain * dv = scope_var[arity-1]->Get_Domain();
  
  list_value = new int * [dv->Get_Initial_Size()];
  
  
  for (unsigned int v = 0; v < dv->Get_Initial_Size(); v++)
    if (dv->Is_Present(v))
    {
      list_value[v] = new int [arity-2];

      for (unsigned int i = 0; i < arity-2; i++)
        list_value[v][i] = c.list_value[v][i];
    }
    else list_value[v] = 0;
}


Index_Element_Global_Constraint::~Index_Element_Global_Constraint ()
// destructor
{
  for (unsigned int v = 0; v < scope_var[arity-1]->Get_Domain()->Get_Initial_Size(); v++)
    delete [] list_value[v];
  delete [] list_value;
}


//-----------------
// basic functions
//-----------------


bool Index_Element_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  int index = scope_var[arity-2]->Get_Domain()->Get_Real_Value(t[arity-2]);
  
  return scope_var[index]->Get_Domain()->Get_Real_Value(t[index]) == scope_var[arity-1]->Get_Domain()->Get_Real_Value(t[arity-1]);
}


bool Index_Element_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  unsigned int x = Get_Position (var);
	Domain * dx = scope_var [x]->Get_Domain();
  
  bool modif = false;

  if (x == arity-2)
  {
    // we filter the domain of the index variable
    unsigned int i = 0;
    int index;
    int v;
    Domain * dv = scope_var[arity-1]->Get_Domain();
    
    do
    {
      v = dx->Get_Remaining_Value (i);
      index = dx->Get_Real_Value (v);

      Domain * dy = scope_var [index]->Get_Domain();
      
      unsigned int j = 0;
      while ((j < dv->Get_Size()) && ((list_value[dv->Get_Remaining_Value(j)][index] == -1) || (! dy->Is_Present(list_value[dv->Get_Remaining_Value(j)][index]))))
        j++;
      
      if (j == dv->Get_Size())      // there is no support
      {
        ds->Add_Element (scope_var[x]);
        dx->Filter_Value (v);
        
        modif = true;
      }
      else i++;
    }
    while (i < dx->Get_Size());
  }
  else
    if (x == arity - 1)
    {
      // we filter the domain of the value variable
      unsigned int i = 0;
      int v;
    
      Domain * di = scope_var[arity-2]->Get_Domain();
        
      do
      {
        v = dx->Get_Remaining_Value(i);

        unsigned int j = 0;
        while ((j < arity-2) && ((list_value[v][j] == -1) || (! scope_var[j]->Get_Domain()->Is_Present(list_value[v][j])) || (! di->Is_Present(di->Get_Value(j)))))
          j++;
        
        if (j == arity -2)
        {
          ds->Add_Element (scope_var[x]);
          dx->Filter_Value (v);
          modif = true;
        }
        else i++;
      }
      while (i < dx->Get_Size());
    }
    else
      {
        // we filter the domain of a variable from the variable list
        if ((scope_var[arity-2]->Get_Domain()->Get_Size() == 1) && (scope_var[arity-2]->Get_Domain()->Get_Remaining_Real_Value(arity-2) == (signed) x))
        {
          // we delete all the values except one corresponding to the value
          int v;
          unsigned int i = 0;
          unsigned int j;
          Domain * dv = scope_var[arity-1]->Get_Domain();
          int real_v;
          
          do
          {
            v = dx->Get_Remaining_Value (i);
            real_v = dx->Get_Real_Value (v);

            j = 0;
            while ((j < dv->Get_Size()) && (dv->Get_Remaining_Real_Value(j) != real_v))
              j++;
              
            if (j < dv->Get_Size())
              i++;
            else
              {
                // there is no support
                ds->Add_Element (scope_var[x]);
                dx->Filter_Value (v);
                modif = true;
              }
          }
          while (i < dx->Get_Size());
        }
      }

  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);

  return modif;
}


void Index_Element_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  Domain * d_value = scope_var[arity-1]->Get_Domain();

  for (unsigned int j = 0; j < d_value->Get_Size();)
  {
    int v = d_value->Get_Remaining_Value(j);

    // we check whether v has at least one support
    int nb = 0;
    for (unsigned int k = 0; (k < arity-2) && (nb < 1); k++)
      if ((list_value[v][k] != -1) && (scope_var[k]->Get_Domain()->Is_Present (list_value[v][k])))
        nb++;

    if (nb == 0)
    {
      // we remove the value v since it has no support
      ds->Add_Element (scope_var[arity-1]);
      d_value->Filter_Value (v);
    }
    else j++;
  }
  
  if (d_value->Get_Size() == 0)
  {
    pb->Set_Conflict (scope_var[arity-1],this);
    return;
  }
  
  // we filter the domain of the index variable if a variable of the array is assigned to a value that does not belong to the domain of the value variable
  for (unsigned int i = 0; i < arity-2; i++)
    if (event_manager->Exist_Event_Fix (scope_var[i]->Get_Num(),ref))
    {
      int v = scope_var[i]->Get_Domain()->Get_Remaining_Real_Value (0);
      
      Domain * d_index = scope_var[arity-2]->Get_Domain();
      int v_index = d_index->Get_Value(i);

      if ((v_index != -1) && (d_index->Is_Present(v_index)))
      {
        int v_value = d_value->Get_Value(v);

        if ((v_value == -1) || (! d_value->Is_Present(v_value)))
        {
          ds->Add_Element (scope_var[arity-2]);
          d_index->Filter_Value (v_index);

          if (d_index->Get_Size() == 0)
          {
            pb->Set_Conflict (scope_var[arity-2],this);
            return;
          }
        }
      }
    }


  if (event_manager->Exist_Event_Fix (scope_var[arity-1]->Get_Num(),ref))
  {
    // the variable value has only a possible value
    int v = scope_var[arity-1]->Get_Domain()->Get_Remaining_Value(0);
    
    // we only preserve the variable of the index variable which are compatible with the remaining value v of the variable value
    Domain * d_index = scope_var[arity-2]->Get_Domain();
    
    for (unsigned int i = 0; i < d_index->Get_Size(); )
    {
      int val_index = d_index->Get_Remaining_Real_Value(i);
      if ((list_value[v][val_index] != -1) && (scope_var[val_index]->Get_Domain()->Is_Present (list_value[v][val_index])))
        i++;
      else
        {
          ds->Add_Element (scope_var[arity-2]);
          d_index->Filter_Value (d_index->Get_Remaining_Value(i));
        }
    }
    
    if (d_index->Get_Size() == 0)
    {
      pb->Set_Conflict (scope_var[arity-2],this);
      return;
    }
  }
  
  if ((scope_var[arity-2]->Get_Domain()->Get_Size() == 1) && (event_manager->Exist_Event_Fix (scope_var[scope_var[arity-2]->Get_Domain()->Get_Remaining_Real_Value(0)]->Get_Num(),ref)))
  {
    // the variable index has only a possible value and the corresponding variable has also a single value
    
    int index = scope_var[arity-2]->Get_Domain()->Get_Remaining_Real_Value(0);
    int v = d_value->Get_Value(scope_var[index]->Get_Domain()->Get_Remaining_Real_Value(0));

    if (v != -1)
    {
      ds->Add_Element (scope_var[arity-1]);
    
      d_value->Assign (v);

      if (d_value->Get_Size() == 0)
      {
        pb->Set_Conflict (scope_var[arity-1],this);
        return;
      }
    }
  }
  
  if (scope_var[arity-2]->Get_Domain()->Get_Size() == 1)
  {
    // the variable index has only a possible value
    int index = scope_var[arity-2]->Get_Domain()->Get_Remaining_Real_Value(0);
        
    // we only preserve the values of the selected variable in the list which are compatible with the remaining values of the variable value and conversely
    
    Domain * dx = scope_var[index]->Get_Domain();
    
    if (d_value->Get_Size() == 1)
    {
      ds->Add_Element (scope_var[index]);
      
      dx->Assign (list_value[d_value->Get_Remaining_Value(0)][index]);

      if (dx->Get_Size() == 0)
      {
        pb->Set_Conflict (scope_var[index],this);
        return;
      }
    }
    else
      {
        // we remove useless values from the variable value
        for (unsigned int i = 0; i < d_value->Get_Size();)
        {
          if ((list_value[d_value->Get_Remaining_Value(i)][index] != -1) && (dx->Is_Present (list_value[d_value->Get_Remaining_Value(i)][index])))
            i++;
          else
            {
              ds->Add_Element (scope_var[arity-1]);
              d_value->Filter_Value (d_value->Get_Remaining_Value(i));
            }
        }

        if (d_value->Get_Size() == 0)
        {
          pb->Set_Conflict (scope_var[arity-1],this);
          return;
        }
          
        // we remove useless values from the selected variable
        for (unsigned int i = 0; i < dx->Get_Size();)
        {
          int v = dx->Get_Remaining_Value (i);
          unsigned int j = 0; 
          while ((j < d_value->Get_Size()) && (list_value[d_value->Get_Remaining_Value(j)][index] != v))
            j++;
          
          if (j < d_value->Get_Size())
            i++;
          else
            {              
              ds->Add_Element (scope_var[index]);
              dx->Filter_Value (v);
            }
        }
        if (dx->Get_Size() == 0)
          pb->Set_Conflict (scope_var[index],this);
      }
  }  
}


string Index_Element_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<element>\n";
  expr += "  <list> ";
  for (unsigned int i = 0; i < arity - 2; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</list>\n";

  expr += "  <value> " + scope_var[arity - 1]->Get_Name() + " </value>\n";  
  
  expr += "  <index> " + scope_var[arity - 2]->Get_Name() + " </index>\n";
  
  expr += "</element>";
  return expr;  
}
