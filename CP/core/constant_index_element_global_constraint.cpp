#include "constant_index_element_global_constraint.h"

//-----------------------------
// constructors and destructor
//-----------------------------


Constant_Index_Element_Global_Constraint::Constant_Index_Element_Global_Constraint (vector<Variable *> & var, int val): Global_Constraint (var)
// constructs a new element constraint with constant val which involves the variables in var (the first variable in var represents the index variable)
{
  value = val;
  
  list_value = new int [arity-1];
  
  Domain * di = scope_var[arity-1]->Get_Domain();

  for (unsigned int i = 0; i < arity-1; i++)
  {
    list_value[i] = scope_var[i]->Get_Domain()->Get_Value(value);
    if (list_value[i] == -1)
    {
      // the required value has been deleted, we remove the value i from the domain of the index variable
      int val = di->Get_Value(i);
      
      if ((val != -1) && (di->Is_Present (val)))
        di->Filter_Value (val);
    }
  }
  
  di->Filter_Value_Before(-1);
  di->Filter_Value_From(arity-1);  
}
		
		
Constant_Index_Element_Global_Constraint::Constant_Index_Element_Global_Constraint (Constant_Index_Element_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the global constraint c
{
  value = c.value;
  
  Domain * di = scope_var[arity-1]->Get_Domain();
  for (unsigned int v = 0; v < di->Get_Initial_Size(); v++)
    if ((di->Is_Present(v)) && ((di->Get_Real_Value(v) < 0) || (di->Get_Real_Value(v) >= (signed)arity-1)))
      di->Filter_Value (v);   // impossible value
}


Constant_Index_Element_Global_Constraint::~Constant_Index_Element_Global_Constraint ()
// destructor
{
  delete [] list_value;
}


//-----------------
// basic functions
//-----------------


bool Constant_Index_Element_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  int index = scope_var[arity-1]->Get_Domain()->Get_Real_Value(t[arity-1]);
  return scope_var[index]->Get_Domain()->Get_Real_Value(t[index]) == value;
}


bool Constant_Index_Element_Global_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  unsigned int x = Get_Position (var);
	Domain * dx = scope_var [x]->Get_Domain();
  
  bool modif = false;

  if (x == arity-1)
  {
    // we filter the domain of the index variable
    unsigned int i = 0;
    int index;
    int v;
    
    do
    {
      v = dx->Get_Remaining_Value (i);
      index = dx->Get_Real_Value (v);

      Domain * dy = scope_var [index]->Get_Domain();
      unsigned int j = 0;
      while ((j < dy->Get_Size()) && (dy->Get_Remaining_Real_Value(j) != value))
        j++;
      
      if (j == dy->Get_Size())      // there is no support
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
  }
  else
    {
      // we filter the domain of a variable from the variable list
      if ((scope_var[arity-1]->Get_Domain()->Get_Size() == 1) && (scope_var[arity-1]->Get_Domain()->Get_Remaining_Real_Value(0) == (signed) x))
      {
        // we delete all the values except one corresponding to the constant value
        int v;
        unsigned int i = 0;
        do
        {
          v = dx->Get_Remaining_Value (i);

          if (dx->Get_Real_Value (v) == value)
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
        
        if (dx->Get_Size() == 0)
          pb->Set_Conflict (scope_var[x],this);
      }
    }
    
  return modif;
}


void Constant_Index_Element_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  Event_Manager * event_manager = pb->Get_Event_Manager();

  for (unsigned int i = 0; i < arity-1; i++)
    if ((list_value[i] != -1) && (event_manager->Exist_Event_Dmc (scope_var[i]->Get_Num(),list_value[i],ref)))
    {
      // the required value has been deleted, we remove the value i from the domain of the index variable
      Domain * d;
      d = scope_var[arity-1]->Get_Domain();
      
      int val = d->Get_Value(i);
      
      if ((val != -1) && (d->Is_Present (val)))
      {
        ds->Add_Element (scope_var[arity-1]);
        d->Filter_Value (val);
        
        if (d->Get_Size() == 0)
        {
          pb->Set_Conflict (scope_var[arity-1],this);
          return;
        }
      }
    }

  if ((event_manager->Exist_Event_Fix (scope_var[arity-1]->Get_Num(),ref)) && (scope_var[arity-1]->Get_Domain()->Get_Size() == 1))
  {
    // we filter the domain of a variable from the variable list
    int index = scope_var[arity-1]->Get_Domain()->Get_Remaining_Real_Value(0);

    ds->Add_Element (scope_var[index]);
    scope_var[index]->Get_Domain()->Assign (list_value[index]);   // we delete all the values except one corresponding to the constant value (if any)
    
    if (scope_var[index]->Get_Domain()->Get_Size() == 0)
      pb->Set_Conflict (scope_var[index],this);
  }
}


string Constant_Index_Element_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<element>\n";
  expr += "  <list> ";
  for (unsigned int i = 0; i < arity - 1; i++)
    expr +=  scope_var[i]->Get_Name() + " ";
  expr += "</list>\n";

  expr += "  <value> " + to_string(value) + " </value>\n";  
  
  expr += "  <index> " + scope_var[arity - 1]->Get_Name() + " </index>\n";
  
  expr += "</element>";
  return expr;  
}
