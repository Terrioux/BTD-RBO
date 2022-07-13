#include "constant_element_global_constraint.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Constant_Element_Global_Constraint::Constant_Element_Global_Constraint (vector<Variable *> & var, int val): Global_Constraint (var)
// constructs a new element constraint with constant val which involves the variable in var
{
  value = val;
  list_value = new int [arity];
  
  watch1 = -1;
  watch2 = -1;
  
  for (unsigned int i = 0; i < arity; i++)
  {
    list_value[i] = scope_var[i]->Get_Domain()->Get_Value(value);
    
    if (list_value[i] != -1)
    {
      if (watch1 == -1)
        watch1 = i;
      else
        if (watch2 == -1)
          watch2 = i;
    }
  }  
}
		
		
Constant_Element_Global_Constraint::Constant_Element_Global_Constraint (Constant_Element_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c
{
  value = c.value;
  list_value = new int [arity];
  watch1 = c.watch1;
  watch2 = c.watch2;

  for (unsigned int i = 0; i < arity; i++)
    list_value[i] = c.list_value[i];  
}


Constant_Element_Global_Constraint::~Constant_Element_Global_Constraint ()
// destructor
{
  delete [] list_value;
}


//-----------------
// basic functions
//-----------------


bool Constant_Element_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  unsigned int index = 0;
  while ((index < arity) && (scope_var[index]->Get_Domain()->Get_Real_Value(t[index]) != value))
    index++;
    
  return index < arity;
}


bool Constant_Element_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  // we count the number of variables in scope_var which have the value corresponding to the constant value in their current domain
  int nb = 0;
  int index = -1;
  
  for (unsigned int i = 0; (i < arity) && (nb <= 1); i++)
    if ((list_value[i] != -1) && (scope_var[i]->Get_Domain()->Is_Present(list_value[i])))
    {
      nb++;
      index = i;
    }

  int x = Get_Position (var);
  if ((nb == 0) || ((nb == 1) && (index == x)))    // there is no index variable or var corresponds to the index variable, so we can remove some values from the variable var
  {
    Domain * dx = scope_var [x]->Get_Domain();

    unsigned int i = 0;
    int v;
    bool modif = false;

    do
    {
      v = dx->Get_Remaining_Value(i);
      
   		if (list_value[x] != v)
      {
        ds->Add_Element (scope_var[x]);
        dx->Filter_Value (v);
        modif = true;
      }
      else i++;
    }
    while (i < dx->Get_Size());

    if (dx->Get_Size() == 0)
      pb->Set_Conflict (scope_var[x],this);
    
    return modif;
  }
  else return false;  
}


void Constant_Element_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();
  Domain * d;
  
  if (watch1 == -1)
  {
    int x;
    if (watch2 != -1)
    {
      // there remains a single watched variable
      ds->Add_Element (scope_var[watch2]);
      d = scope_var[watch2]->Get_Domain();
      d->Assign(list_value[watch2]);
      x = watch2;
    }
    else
      {
        // there is no variable with a domain containing the require value
        ds->Add_Element (scope_var[0]);
        d = scope_var[0]->Get_Domain();
        d->WipeOut();
        x = 0;
      }

    if (d->Get_Size() == 0)
      pb->Set_Conflict (scope_var[x],this);
    
    return;
  }
  else
    if (watch2 != -1)
    {
      // there remains a single watched variable
      ds->Add_Element (scope_var[watch1]);
      d = scope_var[watch1]->Get_Domain();
      d->Assign(list_value[watch1]);
      
      if (d->Get_Size() == 0)
        pb->Set_Conflict (scope_var[watch1],this);

      return;
    }

  if (event_manager->Exist_Event_Dmc (scope_var[watch1]->Get_Num(),list_value[watch1],ref))
  {
    // we look for a new watched variable
    int watch = -1;
    for (unsigned int i =0; (i < arity) && (watch == -1); i++)
      if (((signed) i != watch2) && (scope_var[i]->Get_Domain()->Is_Present(list_value[i])))
        watch = i;
    
    if (watch == -1)
    {
      // there remains a single watched variable
      ds->Add_Element (scope_var[watch2]);
      d = scope_var[watch2]->Get_Domain();
      d->Assign(list_value[watch2]);

      if (d->Get_Size() == 0)
        pb->Set_Conflict (scope_var[watch2],this);
      
      return;
    }
    else watch1 = watch;
  }
  
  if (event_manager->Exist_Event_Dmc (scope_var[watch2]->Get_Num(),list_value[watch2],ref))
  {
    // we look for a new watched variable
    int watch = -1;
    for (unsigned int i =0; (i < arity) && (watch == -1); i++)
      if (((signed) i != watch1) && (scope_var[i]->Get_Domain()->Is_Present(list_value[i])))
        watch = i;
    
    if (watch == -1)
    {
      // there remains a single watched variable
      ds->Add_Element (scope_var[watch1]);
      d = scope_var[watch1]->Get_Domain();
      d->Assign(list_value[watch1]);
      
      if (d->Get_Size() == 0)
        pb->Set_Conflict (scope_var[watch1],this);

    }
    else watch2 = watch;
  }
}


string Constant_Element_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<element>\n";
  expr += "  <list> ";
  for (unsigned int i = 0; i < arity; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</list>\n";

  expr += "  <value> " + to_string(value) + " </value>\n";  
  
  expr += "</element>";
  return expr;  
}
