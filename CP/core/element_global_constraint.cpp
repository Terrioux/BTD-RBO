#include "element_global_constraint.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Element_Global_Constraint::Element_Global_Constraint (vector<Variable *> & var): Global_Constraint (var)
// constructs a new element constraint with the last variable in var as the variable value and whose number is n and which involves the variable in var
{
  Domain * dv = scope_var[arity-1]->Get_Domain();
  
  list_value = new int * [dv->Get_Initial_Size()];
  watch1 = new int [dv->Get_Initial_Size()];
  watch2 = new int [dv->Get_Initial_Size()];
  
  for (unsigned int v = 0; v < dv->Get_Initial_Size(); v++)
    if (dv->Is_Present(v))
    {
      list_value[v] = new int [arity-1];
      watch1[v] = -1;
      watch2[v] = -1;
      int value = dv->Get_Real_Value(v);
      
      for (unsigned int i = 0; i < arity-1; i++)
      {
        list_value[v][i] = scope_var[i]->Get_Domain()->Get_Value(value);
        
        if (list_value[v][i] != -1)
        {
          if (watch1[v] == -1)
            watch1[v] = i;
          else
            if (watch2[v] == -1)
              watch2[v] = i;
        }
      }
    }
    else list_value[v] = 0;
}
		
		
Element_Global_Constraint::Element_Global_Constraint (Element_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c
{
  Domain * dv = scope_var[arity-1]->Get_Domain();
  
  list_value = new int * [dv->Get_Initial_Size()];
  
  
  for (unsigned int v = 0; v < dv->Get_Initial_Size(); v++)
    if (dv->Is_Present(v))
    {
      list_value[v] = new int [arity-1];

      for (unsigned int i = 0; i < arity-1; i++)
        list_value[v][i] = c.list_value[v][i];
    }
    else list_value[v] = 0;
}


Element_Global_Constraint::~Element_Global_Constraint ()
// destructor
{
  for (unsigned int v = 0; v < scope_var[arity-1]->Get_Domain()->Get_Initial_Size(); v++)
    delete [] list_value[v];
  delete [] list_value;
  delete [] watch1;
  delete [] watch2;
}


//-----------------
// basic functions
//-----------------


bool Element_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  unsigned int index = 0;
  int value = scope_var[index]->Get_Domain()->Get_Real_Value(t[arity-1]);
  while ((index < arity-1) && (scope_var[index]->Get_Domain()->Get_Real_Value(t[index]) != value))
    index++;
    
  return index < arity-1;
}


bool Element_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  unsigned int x = Get_Position (var);
  Domain * dx = scope_var [x]->Get_Domain();
  bool modif = false;
  
  if (x == arity - 1)
  {
    // var corresponds to the variable value
    unsigned int i = 0;
    int v;
    do
    {
      v = dx->Get_Remaining_Value(i);

      unsigned int j = 0;
      while ((j < arity-1) && ((list_value[v][j] == -1) || (! scope_var[j]->Get_Domain()->Is_Present(list_value[v][j]))))
        j++;
      
      if (j == arity -1)
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
      if (scope_var[arity-1]->Get_Domain()->Get_Size() == 1)
      {
        int w = scope_var[arity-1]->Get_Domain()->Get_Remaining_Value(0);
        
        int nb = 0;
        int index = -1;
        
        for (unsigned int i = 0; (i < arity-1) && (nb <= 1); i++)
          if ((list_value[w][i] != -1) && (scope_var[i]->Get_Domain()->Is_Present(list_value[w][i])))
          {
            nb++;
            index = i;
          }

        if ((nb == 0) || ((nb == 1) && (index == (signed) x)))    // there is no index variable or var corresponds to the index variable, so we can remove some values from the variable var
        {
          Domain * dx = scope_var [x]->Get_Domain();

          unsigned int i = 0;
          int v;

          do
          {
            v = dx->Get_Remaining_Value(i);
            
            if (list_value[w][x] != v)
            {
              ds->Add_Element (scope_var[x]);
              dx->Filter_Value (v);
              
              modif = true;
            }
            else i++;
          }
          while (i < dx->Get_Size());
        }
      }
    }

  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
  
  return modif;
}


void Element_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();

  Domain * d;
  d = scope_var[arity-1]->Get_Domain();

  if (ref == 0)
  {
    for (unsigned int j = 0; j < d->Get_Size();)
    {
      int v = d->Get_Remaining_Value(j);
      
      if ((watch1[v] == -1) && (watch2[v] == -1))
      {
        // there is no variable with a domain containing the require value
        ds->Add_Element (scope_var[arity-1]);
        d->Filter_Value(v);
      }
      else j++;
    }
    
    if (d->Get_Size() == 0)
      pb->Set_Conflict (scope_var[arity-1],this);
  }
    
  if (d->Get_Size() == 1)
  {
    int v = d->Get_Remaining_Value(0);

    if (event_manager->Exist_Event_Dmc (scope_var[watch1[v]]->Get_Num(),list_value[v][watch1[v]],ref))
    {
      // we look for a new watched variable
      int watch = -1;
      for (unsigned int i =0; (i < arity) && (watch == -1); i++)
        if (((signed) i != watch2[v]) && (scope_var[i]->Get_Domain()->Is_Present(list_value[v][i])))
          watch = i;
      
      if (watch == -1)
      {
        // there remains a single watched variable
        ds->Add_Element (scope_var[watch2[v]]);
        scope_var[watch2[v]]->Get_Domain()->Assign(list_value[v][watch2[v]]);
        
        if (scope_var[watch2[v]]->Get_Domain()->Get_Size() == 0)
          pb->Set_Conflict (scope_var[watch2[v]],this);
        return;
      }
      else watch1[v] = watch;
    }
    
    if (event_manager->Exist_Event_Dmc (scope_var[watch2[v]]->Get_Num(),list_value[v][watch2[v]],ref))
    {
      // we look for a new watched variable
      int watch = -1;
      for (unsigned int i =0; (i < arity) && (watch == -1); i++)
        if (((signed) i != watch1[v]) && (scope_var[i]->Get_Domain()->Is_Present(list_value[v][i])))
          watch = i;
      
      if (watch == -1)
      {
        // there remains a single watched variable
        ds->Add_Element (scope_var[watch1[v]]);
        scope_var[watch1[v]]->Get_Domain()->Assign(list_value[v][watch1[v]]);
        
        if (scope_var[watch1[v]]->Get_Domain()->Get_Size() == 0)
          pb->Set_Conflict (scope_var[watch1[v]],this);
      }
      else watch2[v] = watch;
    }
  }
}


string Element_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<element>\n";
  expr += "  <list> ";
  for (unsigned int i = 0; i < arity - 1; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</list>\n";
  
  expr += "  <value> " + scope_var[arity - 1]->Get_Name() + " </value>\n";
  
  expr += "</element>";
  return expr;  
}
