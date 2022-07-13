#include "negative_compact_table_extension_constraint.h"
#include "real_tuple.h"
#include <cmath>


// ---------------------------
// constructor and destructor
// ---------------------------


Negative_Compact_Table_Extension_Constraint::Negative_Compact_Table_Extension_Constraint (vector<Variable *> & var): Compact_Table_Extension_Constraint (var)
// constructs an extension constraint based on compact table whose scope is defined by the variables in var
{
}


Negative_Compact_Table_Extension_Constraint::Negative_Compact_Table_Extension_Constraint (Negative_Compact_Table_Extension_Constraint & c): Compact_Table_Extension_Constraint (c)
// constructs a new constraint by copying the constraint c 
{
}

//-----------------
// basic functions
//-----------------


Constraint * Negative_Compact_Table_Extension_Constraint::Duplicate ()
// returns a pointer on a copy of the constraint
{
  return new Negative_Compact_Table_Extension_Constraint(*this);
}


bool Negative_Compact_Table_Extension_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  unsigned int word_num = 0;
  
  for (vector<unsigned long>::iterator iter = data[0][t[0]].begin(); iter != data[0][t[0]].end(); iter++)
  {
    if (*iter != 0)
    {
      for (unsigned int i = 0; i < WORD_SIZE; i++)
      {
        if ((((*iter) >> i) & 1ul) == 1)
        {
          unsigned int j = 1;
          while ((j < arity) && (((data[j][t[j]][word_num] >> i) & 1ul) == 1))
            j++;
          
          if (j == arity)
            return false;
        }
      }
    }
    word_num++;
  }

  return true;
}


bool Negative_Compact_Table_Extension_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
  
	Domain * dx = scope_var [x]->Get_Domain();
	int dx_size = dx->Get_Size();
	
	int i = 0;
	int v;
	
  Tuple t (arity);
  int * ts;
  bool has_no_support;
  bool record_support;
  bool modif = false;    // true if a deletion is achieved, false otherwise
	bool exist_more_tuples;
	
	do
	{    
		v = dx->Get_Remaining_Value (i);
		
		ts = ls->Get_Support (num,x,v);
		
		if (ts == 0) 
		{
			for (unsigned int j = 0; j < arity; j++)
				t[j] = -1;
			t[x] = v;
			t.Complete (scope_var,x);
			has_no_support = true;
			record_support = true;
			exist_more_tuples = true;
		}
		else
			{
				for (unsigned int j = 0; j < arity; j++)
					t[j] = ts[j];
					
				if (t.Is_Valid (scope_var))
				{
					has_no_support = false;
					record_support = false;
					exist_more_tuples = true;
				}
				else 
					{
						has_no_support = true;
						record_support = true;
						
						if (ls->Continue_From_Scratch())
						{
							for (unsigned int j = 0; j < arity; j++)
								t[j] = -1;
							t[x] = v;
							t.Complete (scope_var,x);
							exist_more_tuples = true;							
						}
						else exist_more_tuples = t.Next_Compatible_Valid (scope_var,x);
					}
			}
		
		while ((has_no_support) && (exist_more_tuples))
		{
			if (Is_Satisfied (t.Get_Values()))
				has_no_support = false;
			else exist_more_tuples = t.Next_Valid (scope_var,x);
		}
		
		if (has_no_support)
		{
			ds->Add_Element (scope_var [x]);
			dx->Filter_Value (v);
			dx_size--;
			modif = true;
		}
		else 
			{
				if (record_support)
				{
					ds->Add_Support (num,x,v,arity,ts);
					ls->Change_Support (num,x,v,t.Get_Values(),arity);
				}
				i++;
			}
  }
  while (i < dx_size);

  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
    
  return modif;
}


void Negative_Compact_Table_Extension_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  if (ref == 0)
    event_manager = pb->Get_Event_Manager();
   
  Update_Table(pb,ds,ref);
  
  unsigned long current_possible_tuple_number = 1;
  for (unsigned int i = 0; i < arity; i++)
    current_possible_tuple_number *= scope_var[i]->Get_Domain()->Get_Size();

  if (current_table.One_Number() == current_possible_tuple_number)
  {
    unsigned int i = 0;
    while (A.Is_Assigned(scope_var[i]->Get_Num()))
      i++;
    pb->Set_Conflict (scope_var[i],this);
    
    return;
  }
    
  Filter_Domain(ds);
}


void Negative_Compact_Table_Extension_Constraint::Update_Table (CSP * pb, Deletion_Stack * ds, timestamp ref)
// updates the table and returns true iff the table is empty
{
  for (unsigned int i = 0; i < arity; i++)
  {
    Domain * dx = scope_var[i]->Get_Domain();
    
    current_table.Clear_Mask();
   
    unsigned int delta_x;
    int num_var = scope_var[i]->Get_Num();
    unsigned int nb_removed_values = dx->Get_Initial_Size() - dx->Get_Size();
    
    for (delta_x = 0; (delta_x < nb_removed_values) && (event_manager->Exist_Event_Dmc (num_var, dx->Get_Deleted_Value(delta_x), ref)); delta_x++);

    if (delta_x < dx->Get_Size())
    {
      for (unsigned int j = 0; j < delta_x; j++)
        current_table.Add_To_Mask (data[i][dx->Get_Deleted_Value(j)]);

      current_table.Reverse_Mask();
    }
    else
      {
        for (int j = dx->Get_Size()-1 ; j >= 0; j--)
          current_table.Add_To_Mask (data[i][dx->Get_Remaining_Value(j)]);
      }
      
    current_table.Intersect_With_Mask(ds);
    
    if (current_table.Is_Empty())
      return;
  }
}


void Negative_Compact_Table_Extension_Constraint::Filter_Domain (Deletion_Stack * ds)
// filters the domains
{
  for (unsigned int i = 0; i < arity; i++)
  {
    Domain * dx = scope_var[i]->Get_Domain();
   
    for (int j = dx->Get_Size()-1 ; j >= 0; j--)
    {
      int v = dx->Get_Remaining_Value(j);
      
      unsigned long current_possible_tuple_number = 1;
      for (unsigned int k = 0; k < arity; k++)
        if (i != k)
          current_possible_tuple_number *= scope_var[k]->Get_Domain()->Get_Size();
      
      if (current_table.One_Number(data[i][v]) == current_possible_tuple_number)
      {
        ds->Add_Element (scope_var[i]);
        dx->Filter_Value(v);
  
        current_table.Clear_Mask();
        current_table.Add_To_Mask(data[i][v]);
        current_table.Reverse_Mask();
        current_table.Intersect_With_Mask(ds);
      }      
    }
    
    if (dx->Get_Size() == 0)
      throw ("Domaine vide neg");
  }
}


string Negative_Compact_Table_Extension_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<extension>\n";
  expr += "  <list> ";
  for (vector<Variable *>::iterator iter = scope_var.begin(); iter != scope_var.end(); iter++)
    expr +=  (*iter)->Get_Name() + " ";
  expr += "</list>\n";
  
  expr += "  <conflicts> ";
  
 	Real_Tuple t (arity);
	t.Complete (scope_var);
	int * real_t;
	bool again;
	
	do
	{
    if (! Is_Satisfied (t.Get_Values()))
		{
      real_t = t.Get_Real_Values ();
      expr += "(";
			for (unsigned int i = 0; i < arity; i++)
      {
        expr += to_string(real_t[i]);
        if (i < arity -1)
          expr += ",";
      }
			expr += ")";
		}
		again = t.Next_Valid (scope_var);	
	}
	while (again);

  expr += "  </conflicts>\n";
  
  expr += "</extension>";
  
  return expr;
}
