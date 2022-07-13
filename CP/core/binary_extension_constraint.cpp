#include "binary_extension_constraint.h"


Binary_Extension_Constraint::Binary_Extension_Constraint (vector<Variable *> & var, bool default_value): Extension_Constraint (var)
// constructs a new binary extension constraint whose all the tuples are forbidden if default_value is false, allowed otherwise
{
	if (var.size() != 2)
		throw ("Error: the constraint must be binary");
		
	int d1 = scope_var[0]->Get_Domain()->Get_Initial_Size();
	int d2 = scope_var[1]->Get_Domain()->Get_Initial_Size();
		
  relation = new bool * [d1];
  for (int i = 0; i < d1; i++)
  {
  	relation[i] = new bool [d2];
  	for (int j = 0; j < d2; j++)
  		relation[i][j] = default_value;
  }

  if (default_value)
  {
  	tightness = 0;
  	allowed_tuple_number = d1 * d2;
  }
  else
    {
			tightness = d1 * d2;
			allowed_tuple_number = 0;
		}
}



Binary_Extension_Constraint::Binary_Extension_Constraint (vector<Variable *> & var, bool positive, int ** tuple_list, int nb_tuples): Extension_Constraint (var)
// constructs a binary extension new constraint whose all the tuples of tuple_list are allowed if positive is false, forbidden otherwise
{
	if (var.size() != 2)
		throw ("Error: the constraint must be binary");
	
	Domain * D1 = scope_var[0]->Get_Domain();
	Domain * D2 = scope_var[1]->Get_Domain();
	
	int d1 = D1->Get_Initial_Size();
	int d2 = D2->Get_Initial_Size();

	relation = new bool * [d1];
  for (int i = 0; i < d1; i++)
  {
  	relation[i] = new bool [d2];
  	for (int j = 0; j < d2; j++)
  		relation[i][j] = ! positive;
  }

  if (positive)
  {
  	tightness = d1 * d2;
  	allowed_tuple_number = 0;
  	
  	for (int i = 0; i < nb_tuples; i++)
  	{		
			int v_0,v_1;
			v_0 = 0;
			while ((v_0 < d1) && (tuple_list[i][0] != D1->Get_Real_Value (v_0)))
				v_0++;
			
			v_1 = 0;
			while ((v_1 < d2) && (tuple_list[i][1] != D2->Get_Real_Value (v_1)))
				v_1++;
				
  		if (! relation[v_0][v_1])
			{
				relation[v_0][v_1] = true;
				tightness--;
				allowed_tuple_number++;
			}
		}
  }
  else
    {
			tightness = 0;
			allowed_tuple_number = d1 * d2;

			for (int i = 0; i < nb_tuples; i++)
			{			
				int v_0,v_1;
				v_0 = 0;
				while ((v_0 < d1) && (tuple_list[i][0] != D1->Get_Real_Value (v_0)))
					v_0++;
				
				v_1 = 0;
				while ((v_1 < d2) && (tuple_list[i][1] != D2->Get_Real_Value (v_1)))
					v_1++;
								
				if (relation[v_0][v_1])
				{
					relation[v_0][v_1] = false;
					tightness++;
					allowed_tuple_number--;
				}
			}
		}
}


Binary_Extension_Constraint::Binary_Extension_Constraint (Binary_Extension_Constraint & c): Extension_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	int d1 = scope_var[0]->Get_Domain()->Get_Initial_Size();
	int d2 = scope_var[1]->Get_Domain()->Get_Initial_Size();
	relation = new bool * [d1];
  for (int i = 0; i < d1; i++)
  {
  	relation[i] = new bool [d2];
  	for (int j = 0; j < d2; j++)
  		relation[i][j] = c.relation[i][j];
  }
}


Binary_Extension_Constraint::~Binary_Extension_Constraint ()
// destructor
{
	int d1 = scope_var[0]->Get_Domain()->Get_Initial_Size();
	for (int i = 0; i < d1; i++)
  	delete [] relation[i];
  delete [] relation;
}


//------------
// operator =
//------------

Binary_Extension_Constraint & Binary_Extension_Constraint::operator= (Binary_Extension_Constraint & c)
{
	if (this != &c)
	{
		int d1 = scope_var[0]->Get_Domain()->Get_Initial_Size();
		for (int i = 0; i < d1; i++)
			delete [] relation[i];
		delete [] relation;

		num = c.num;
		arity = 2;
		scope = c.scope;
		tightness = c.tightness;
		allowed_tuple_number = c.allowed_tuple_number;

		d1 = scope_var[0]->Get_Domain()->Get_Initial_Size();
		int d2 = scope_var[1]->Get_Domain()->Get_Initial_Size();
		relation = new bool * [d1];
		for (int i = 0; i < d1; i++)
		{
			relation[i] = new bool [d2];
			for (int j = 0; j < d2; j++)
				relation[i][j] = c.relation[i][j];
		}

		unassigned_variable_number = c.unassigned_variable_number;
		position = c.position;
	}
	return *this;
}


//-----------------
// basic functions
//-----------------


void Binary_Extension_Constraint::Merge (Binary_Extension_Constraint * c)
// merges the current constraint with the constraint c
{
	if (this != c)
	{
		tightness = 0;
		allowed_tuple_number = 0;
		
		int d1 = scope_var[0]->Get_Domain()->Get_Initial_Size();
		int d2 = scope_var[1]->Get_Domain()->Get_Initial_Size();
		
		if (scope_var[0]->Get_Num() == c->scope_var[0]->Get_Num())
		{
			for (int i = 0; i < d1; i++)
				for (int j = 0; j < d2; j++)
					if (relation[i][j] && c->relation[i][j])
					{
						relation[i][j] = true;
						allowed_tuple_number++;
					}
					else
						{
							relation[i][j] = false;
							tightness++;
						}
		}
		else
			{
				for (int i = 0; i < d1; i++)
					for (int j = 0; j < d2; j++)
						if (relation[i][j] && c->relation[j][i])
						{
							relation[i][j] = true;
							allowed_tuple_number++;
						}
						else
							{
								relation[i][j] = false;
								tightness++;
							}
			}
	}
}


void Binary_Extension_Constraint::Allow_Tuple (int * t)
// allows the tuple t
{
	if (! relation[t[0]][t[1]])
	{
		relation[t[0]][t[1]] = true;
		tightness--;
		allowed_tuple_number++;
	}
}


void Binary_Extension_Constraint::Allow_Tuple (int val1, int val2)
// allows the tuple t
{
	if (! relation[val1][val2])
	{
		relation[val1][val2] = true;
		tightness--;
		allowed_tuple_number++;
	}
}


void Binary_Extension_Constraint::Forbid_Tuple (int * t)
// forbids the tuple t
{
	if (relation[t[0]][t[1]])
	{
		relation[t[0]][t[1]] = false;
		tightness++;
		allowed_tuple_number--;
	}
}


void Binary_Extension_Constraint::Forbid_Tuple (int val1, int val2)
// allows the tuple t
{
	if (relation[val1][val2])
	{
		relation[val1][val2] = false;
		tightness++;
		allowed_tuple_number--;
	}
}


bool Binary_Extension_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
	int x,y;
	
	Domain * dx, * dy;
	
	if (scope_var[0]->Get_Num() == var)  
	{
		x = 0;
		y = 1;
		dx = scope_var[0]->Get_Domain();
		dy = scope_var[1]->Get_Domain();
	}
	else 
		{
			x = 1;
			y = 0;
			dx = scope_var[1]->Get_Domain();
			dy = scope_var[0]->Get_Domain();
		}

	int dx_size = dx->Get_Size();
		
	int i = 0;
	int v;

  int t [2];
  int * ts;
  bool has_no_support;
  bool modif = false;    // true if a deletion is achieved, false otherwise
	
	do
	{
		v = dx->Get_Remaining_Value (i);
		t[x] = v;
		
		ts = ls->Get_Support (num,x,v);
		
		if ((ts == 0) || (! dy->Is_Present (ts[y])))
		{
			has_no_support = true;
			//~ cout << "Test de la valeur " << v << " pour  " << var << endl;
			
			if ((ts == 0) || (ls->Continue_From_Scratch()))
				t[y] = 0;
			else t[y] = ts[y]+1;
			
			while ((has_no_support) && ((unsigned) t[y] < dy->Get_Initial_Size ()))
      {
        if (dy->Is_Present (t[y]))
        { 
					if (relation[t[0]][t[1]])
						has_no_support = false;
					else t[y]++;
				}
				else t[y]++;
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
					ds->Add_Support (num,x,v,2,ts);
						
					ls->Change_Support (num,x,v,t,2);
					i++;
				}
		}
		else i++;
  }
  while (i < dx_size);

  if (dx_size == 0)
    pb->Set_Conflict (scope_var[x],this);

  return modif;
}


void Binary_Extension_Constraint::Get_Allowed_Tuple_List (list<int *> & list)
// constructs the list of tuples allowed by the constraint
{
  list.clear();
  
	Domain * D1 = scope_var[0]->Get_Domain();
	int d1 = D1->Get_Initial_Size();
	Domain * D2 = scope_var[1]->Get_Domain();
	int d2 = D2->Get_Initial_Size();
	
	int * t;
	
  for (int i = 0; i < d1; i++)
		if (D1->Is_Present (i))
		{
			for (int j = 0; j < d2; j++)
				if ((D2->Is_Present (j)) && (relation[i][j]))
				{
					t = new int [2];
					t[0] = i;
					t[1] = j;
					list.push_back (t);
				}
		}
}


string Binary_Extension_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<extension>\n";
  expr += "  <list> ";
  for (vector<Variable *>::iterator iter = scope_var.begin(); iter != scope_var.end(); iter++)
    expr +=  (*iter)->Get_Name() + " ";
  expr += "</list>\n";
  
  string type;
  bool res;
  if (tightness < allowed_tuple_number)
  {
    type = "conflicts";
    res = false;
  }
  else 
    {
      type = "supports";
      res = true;
    }

  expr += "  <" + type +"> ";
  
  Domain * d1 = scope_var[0]->Get_Domain();
  Domain * d2 = scope_var[1]->Get_Domain();
  
  int v = d1->Get_Min();
  
  while (v != -1)
  {
    int w = d2->Get_Min();
    while (w != -1)
    {
      if (relation[v][w] == res)
        expr += "(" + to_string (d1->Get_Real_Value(v)) + "," + to_string (d2->Get_Real_Value(w)) + ")";
      w = d2->Next_Value(w);
    }
    v = d1->Next_Value(v);
  }
  expr += "  </" + type +">\n";
  
  expr += "</extension>";
  
  return expr;
}
