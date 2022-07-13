#include "equal_absolute_distance_binary_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Equal_Absolute_Distance_Binary_Constraint::Equal_Absolute_Distance_Binary_Constraint (vector<Variable *> & var, int cst): Binary_Constraint (var)
// constructs a new constraint which involves the variable in var and whose relation imposes that the absolute distance between the two variables is equal to cst
{
	constant = cst;
	
	equal = new pair<int,int*> * [4];
	equal[0] = new pair<int,int*> [scope_var[0]->Get_Domain()->Get_Initial_Size()];
	equal[1] = new pair<int,int*> [scope_var[1]->Get_Domain()->Get_Initial_Size()];
	int val;

	for (unsigned int w = 0; w < scope_var[1]->Get_Domain()->Get_Initial_Size(); w++)
	{
		equal[1][w].first = 0;
		equal[1][w].second = new int [4];
	}
	
	for (unsigned int v = 0; v < scope_var[0]->Get_Domain()->Get_Initial_Size(); v++)
	{
		equal[0][v].first = 0;
		equal[0][v].second = new int [4];
		val = abs(scope_var[0]->Get_Domain()->Get_Real_Value(v));
		for (unsigned int w = 0; w < scope_var[1]->Get_Domain()->Get_Initial_Size(); w++)
			if (abs(val - abs(scope_var[1]->Get_Domain()->Get_Real_Value(w))) == constant)
			{
				equal[0][v].second[equal[0][v].first] = w;
				equal[0][v].first++;
				
				equal[1][w].second[equal[1][w].first] = v;
				equal[1][w].first++;
			}
	}
}


Equal_Absolute_Distance_Binary_Constraint::Equal_Absolute_Distance_Binary_Constraint (Equal_Absolute_Distance_Binary_Constraint & c): Binary_Constraint (c)
// constructs a new constraint by copying the constraint c 
{
	equal = new pair<int,int*> * [2];
	equal[0] = new pair<int,int*> [scope_var[0]->Get_Domain()->Get_Initial_Size()];
	equal[1] = new pair<int,int*> [scope_var[1]->Get_Domain()->Get_Initial_Size()];
	
	for (unsigned int v = 0; v < scope_var[0]->Get_Domain()->Get_Initial_Size(); v++)
	{
		equal[0][v].first = c.equal[0][v].first;
		equal[0][v].second = new int [4];
		for (int i = 0; i < c.equal[0][v].first; i++)
			equal[0][v].second[i] = c.equal[0][v].second[i];
	}
	for (unsigned int w = 0; w < scope_var[1]->Get_Domain()->Get_Initial_Size(); w++)
	{
		equal[1][w].first = c.equal[1][w].first;
		equal[1][w].second = new int [4];
		for (int i = 0; i < c.equal[1][w].first; i++)
			equal[1][w].second[i] = c.equal[1][w].second[i];
	}
}


Equal_Absolute_Distance_Binary_Constraint::~Equal_Absolute_Distance_Binary_Constraint ()
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


bool Equal_Absolute_Distance_Binary_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
	return abs(abs(scope_var[0]->Get_Domain()->Get_Real_Value(t[0])) - abs(scope_var[1]->Get_Domain()->Get_Real_Value(t[1]))) == constant;
}


bool Equal_Absolute_Distance_Binary_Constraint::Revise (CSP * pb, unsigned int var, Support * ls, Deletion_Stack * ds)
// returns true if the application of arc-consistency on the constraint w.r.t. the variable var deletes a value in the domain of var, false otherwise
{
  int x = Get_Position (var);
	int y = 1 - x;
	Domain * dx = scope_var[x]->Get_Domain();
	Domain * dy = scope_var[y]->Get_Domain();
	
	bool modif = false;    // true if a deletion is achieved, false otherwise
	int dx_size = dx->Get_Size();

	int v;
  int i = 0;
	int j;
	do
	{
		v = dx->Get_Remaining_Value(i);
	
		j = 0;
		while ((j < equal[x][v].first) && (! dy->Is_Present (equal[x][v].second[j])))
			j++;

		if (j == equal[x][v].first)
		{
			ds->Add_Element (scope_var[x]);
			dx->Filter_Value (v);
			modif = true;
			dx_size--;
		}
		else i++;
	}
	while (i < dx_size);
  
  if (dx_size == 0)
    pb->Set_Conflict (scope_var[x],this);
	
	return modif;
}




string Equal_Absolute_Distance_Binary_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  return "<intension> eq(dist(abs(" + scope_var[0]->Get_Name() + "),abs(" + scope_var[1]->Get_Name()+"))," + to_string(constant) + ") </intension>";
}
