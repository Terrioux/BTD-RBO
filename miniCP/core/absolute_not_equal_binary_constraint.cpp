#include "absolute_not_equal_binary_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Absolute_Not_Equal_Binary_Constraint::Absolute_Not_Equal_Binary_Constraint (vector<Variable *> & var): Binary_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that the two variables have different values in absolute value
{
	equal = new pair<int,int*> * [2];
	equal[0] = new pair<int,int*> [scope_var[0]->Get_Domain()->Get_Initial_Size()];
	equal[1] = new pair<int,int*> [scope_var[1]->Get_Domain()->Get_Initial_Size()];
	int val;

	for (unsigned int w = 0; w < scope_var[1]->Get_Domain()->Get_Initial_Size(); w++)
	{
		equal[1][w].first = 0;
		equal[1][w].second = new int [2];
	}
	
	for (unsigned int v = 0; v < scope_var[0]->Get_Domain()->Get_Initial_Size(); v++)
	{
		equal[0][v].first = 0;
		equal[0][v].second = new int [2];
		val = abs(scope_var[0]->Get_Domain()->Get_Real_Value(v));
		for (unsigned int w = 0; w < scope_var[1]->Get_Domain()->Get_Initial_Size(); w++)
			if (val == abs(scope_var[1]->Get_Domain()->Get_Real_Value(w)))
			{
				equal[0][v].second[equal[0][v].first] = w;
				equal[0][v].first++;
				
				equal[1][w].second[equal[1][w].first] = v;
				equal[1][w].first++;
			}
	}
}


Absolute_Not_Equal_Binary_Constraint::Absolute_Not_Equal_Binary_Constraint (Absolute_Not_Equal_Binary_Constraint & c): Binary_Constraint (c)
// constructs a new constraint by copying the constraint c
{
	equal = new pair<int,int*> * [2];
	equal[0] = new pair<int,int*> [scope_var[0]->Get_Domain()->Get_Initial_Size()];
	equal[1] = new pair<int,int*> [scope_var[1]->Get_Domain()->Get_Initial_Size()];
	
	for (unsigned int v = 0; v < scope_var[0]->Get_Domain()->Get_Initial_Size(); v++)
	{
		equal[0][v].first = c.equal[0][v].first;
		equal[0][v].second = new int [2];
		for (int i = 0; i < c.equal[0][v].first; i++)
			equal[0][v].second[i] = c.equal[0][v].second[i];
	}
	for (unsigned int w = 0; w < scope_var[1]->Get_Domain()->Get_Initial_Size(); w++)
	{
		equal[1][w].first = c.equal[1][w].first;
		equal[1][w].second = new int [2];
		for (int i = 0; i < c.equal[1][w].first; i++)
			equal[1][w].second[i] = c.equal[1][w].second[i];
	}
}


Absolute_Not_Equal_Binary_Constraint::~Absolute_Not_Equal_Binary_Constraint ()
// destructor
{
	for (unsigned int v = 0; v < scope_var[0]->Get_Domain()->Get_Initial_Size(); v++)
		delete [] equal[0][v].second;
		
	for (unsigned int w = 0; w < scope_var[1]->Get_Domain()->Get_Initial_Size(); w++)
		delete [] equal[1][w].second;
	delete [] equal[0];
	delete [] equal[1];
	delete [] equal;
}


//-----------------
// basic functions
//-----------------


bool Absolute_Not_Equal_Binary_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	return abs(scope_var[0]->Get_Domain()->Get_Real_Value(t[0])) != abs(scope_var[1]->Get_Domain()->Get_Real_Value(t[1]));
}


bool Absolute_Not_Equal_Binary_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
	int y = 1 - x;
	Domain * dy = scope_var[y]->Get_Domain();
	
	if ((dy->Get_Size() == 1) || ((dy->Get_Size() == 2) && (dy->Get_Remaining_Real_Value(0) == - dy->Get_Remaining_Real_Value(1))))
	{
		Domain * dx = scope_var[x]->Get_Domain();
		
		bool modif = false;    // true if a deletion is achieved, false otherwise

		int v = dy->Get_Remaining_Value(0);
		
		int j = 0;
		while (j < equal[y][v].first) 
		{
			if (dx->Is_Present (equal[y][v].second[j]))
			{
				ds->Add_Element (scope_var[x]);
				dx->Filter_Value (equal[y][v].second[j]);
				modif = true;
			}
			j++;
		}

    if (dx->Get_Size() == 0)
      pb->Set_Conflict (scope_var[x],this);

		return modif;
	}
	else return false;
}


string Absolute_Not_Equal_Binary_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  return "<intension> ne(abs(" + scope_var[0]->Get_Name() + "),abs(" + scope_var[1]->Get_Name()+")) </intension>";
}
