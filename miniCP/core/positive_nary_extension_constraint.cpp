#include "positive_nary_extension_constraint.h"
#include "real_tuple.h"

//--------------
// constructors
//--------------


Positive_Nary_Extension_Constraint::Positive_Nary_Extension_Constraint (vector<Variable *> & var): Nary_Extension_Constraint (var)
// constructs a new constraint with number n which involves the variable in var whose all the tuples are forbidden
{
	allowed_tuple_number = 0;
	tightness = 1;

	for (unsigned int i = 0; i < arity; i++)
		tightness *= var[i]->Get_Domain()->Get_Initial_Size();
}


Positive_Nary_Extension_Constraint::Positive_Nary_Extension_Constraint (vector<Variable *> & var, int ** tuples, int nb_tuples): Nary_Extension_Constraint (var)
// constructs a new constraint which involves the variable in var whose the nb_tuples tuples of tuples A are allowed
{
	allowed_tuple_number = 0;
	tightness = 1;

	for (unsigned int i = 0; i < arity; i++)
		tightness *= var[i]->Get_Domain()->Get_Initial_Size();
	
	int real_tuple[arity];
	int v,d;
	Domain * D;
	for (int i = 0; i < nb_tuples; i++)
	{
		for (unsigned int j = 0; j < arity; j++)
		{
			v = 0;
			D = scope_var[j]->Get_Domain();
			d = D->Get_Initial_Size();
			while ((v < d) && (tuples[i][j] != D->Get_Real_Value (v)))
				v++;
			real_tuple[j] = v;
		}
		relation->Add_Element (real_tuple);
	}

	
	allowed_tuple_number = relation->Get_Size();
	tightness -= allowed_tuple_number;
}


Positive_Nary_Extension_Constraint::Positive_Nary_Extension_Constraint (Positive_Nary_Extension_Constraint & c): Nary_Extension_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	int size = c.scope_var[0]->Get_Domain()->Get_Initial_Size();
	
	relation = new AVL (c.scope_var);
	for (int i = 0; i < size; i++)
		for (AVL::iterator iter = c.relation->begin(0,i); iter != c.relation->end(0); iter++)
			relation->Add_Element (*iter);
}


//------------
// operator =
//------------


Positive_Nary_Extension_Constraint & Positive_Nary_Extension_Constraint::operator= (Positive_Nary_Extension_Constraint & c)
{
	if (this != &c)
	{
		num = c.num;
		arity = c.arity;
		scope = c.scope;
		scope_var = c.scope_var;
		tightness = c.tightness;
		allowed_tuple_number = c.allowed_tuple_number;
		
		int size = c.scope_var[0]->Get_Domain()->Get_Initial_Size();
		delete relation;
		relation = new AVL (c.scope_var);
		for (int i = 0; i < size; i++)
		{
			for (AVL::iterator iter = c.relation->begin(0,i); iter != c.relation->end(0); iter++)
				relation->Add_Element (*iter);
		}
		
		unassigned_variable_number = c.unassigned_variable_number;
		position = c.position;
	}
	return *this;
}


//-----------------
// basic functions
//-----------------


void Positive_Nary_Extension_Constraint::Merge (Positive_Nary_Extension_Constraint * c)
// merges the current constraint with the constraint c
{
	if (this != c)
	{
		tightness = 1;
		int pos[arity];
		for (unsigned int i = 0; i < arity; i++)
		{
			tightness *= scope_var[i]->Get_Domain()->Get_Initial_Size();
			pos[i] = c->Get_Position (scope_var[i]->Get_Num());
		}

		AVL * old_relation = relation;
		relation = new AVL (scope_var);
		int size = scope_var[0]->Get_Domain()->Get_Initial_Size();
		allowed_tuple_number = 0;
		int t [arity];
		
		for (int i = 0; i < size; i++)
		{
			for (AVL::iterator iter = old_relation->begin(0,i); iter != old_relation->end(0); iter++)
			{
				for (unsigned int j = 0; j < arity; j++)
					t[pos[j]] = (*iter)[j];
				
				if (c->relation->Is_Present (t))
				{
					relation->Add_Element (*iter);
					allowed_tuple_number++;
					tightness--;
				}
			}
		}
		delete old_relation;
	}
}


void Positive_Nary_Extension_Constraint::Allow_Tuple (int * t)
// allows the tuple t
{
	relation->Add_Element (t);
	
	if (relation->Get_Size() != allowed_tuple_number)
	{
		// a new tuple is allowed
		tightness--;
		allowed_tuple_number++;
	}
}


void Positive_Nary_Extension_Constraint::Forbid_Tuple (int * t)
// forbids the tuple t
{
	relation->Remove_Element (t);
	
	if (relation->Get_Size() != allowed_tuple_number)
	{
		// a new tuple is forbidden
		tightness++;
		allowed_tuple_number--;
	}
}


bool Positive_Nary_Extension_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
  
	Domain * dx = scope_var [x]->Get_Domain();
	int dx_size = dx->Get_Size();
	
	int i = 0;
	int v;
		
  int * ts;
  bool has_no_support;
  bool record_support;
  bool modif = false;    // true if a deletion is achieved, false otherwise
  AVL::iterator iter;
	
	do
	{    
		v = dx->Get_Remaining_Value (i);
		
		ts = ls->Get_Support (num,x,v);
		
		if (ts == 0) 
		{
			iter = relation->begin (x,v);
			
			has_no_support = true;
			record_support = true;
		}
		else
			{	
				if (Is_Valid (scope_var,ts))
				{
					has_no_support = false;
					record_support = false;
				}
				else 
					{
						if (ls->Continue_From_Scratch())
							iter = relation->begin (x,v);
						else iter = relation->begin (x,ts);
						has_no_support = true;
						record_support = true;
					}
			}
			
		while ((has_no_support) && (iter != relation->end(x)))
		{
			if (Is_Valid (scope_var,*iter))
				has_no_support = false;
			else iter++;			
		}
		
		if (has_no_support)
		{
			ds->Add_Element (scope_var[x]);
			dx->Filter_Value (v);
			dx_size--;

			modif = true;
		}
		else 
			{
				if (record_support)
				{
					ds->Add_Support (num,x,v,arity,ts);
					ls->Change_Support (num,x,v,*iter,arity);
				}
				i++;
			}
  }
  while (i < dx_size);

  if (dx->Get_Size() == 0)
    pb->Set_Conflict (scope_var[x],this);
      
  return modif;
}


void Positive_Nary_Extension_Constraint::Get_Allowed_Tuple_List (list<int *> & list)
// constructs the list of tuples allowed by the constraint
{
  list.clear();
  
	int * t;
	int size = scope_var[0]->Get_Domain()->Get_Initial_Size();	
	
	for (int i = 0; i < size; i++)
		for (AVL::iterator iter = relation->begin(0,i); iter != relation->end(0); iter++)
		{
			// we check if the tuple is valid
			unsigned int j = 0;
			while ((j < arity) && (scope_var[j]->Get_Domain()->Is_Present ((*iter)[j])))
				j++;
			
			// if so, we add the tuple in the list
			if (j == arity)
			{
				t = new int [arity];
				for (j = 0; j < arity; j++)
					t[j] = (*iter)[j];
				list.push_back (t);
			}
		}
}


string Positive_Nary_Extension_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<extension>\n";
  expr += "  <list> ";
  for (vector<Variable *>::iterator iter = scope_var.begin(); iter != scope_var.end(); iter++)
    expr +=  (*iter)->Get_Name() + " ";
  expr += "</list>\n";
  
  expr += "  <supports> ";
  
 	Real_Tuple t (arity);
	t.Complete (scope_var);
	int * real_t;
	bool again;
	
	do
	{
    if (Is_Satisfied (t.Get_Values()))
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

  expr += "  </supports>\n";
  
  expr += "</extension>";
  
  return expr;
}
