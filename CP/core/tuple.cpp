#include "tuple.h"
#include <iostream>

//-----------------------------
// constructors and destructor
//-----------------------------


Tuple::Tuple (int size)
// construct an empty tuple whose size is size
{
	arity = size;
	values = new int [arity];
	
	for (int i = 0; i < arity; i++)
		values[i] = -1;
}


Tuple::Tuple (Tuple & t)
// construct a tuple by copying the tuple t
{
	arity = t.arity;
	values = new int [arity];
	
	for (int i = 0; i < arity; i++)		values[i] = t.values[i];
}


Tuple::~Tuple ()
// destructor
{
	delete [] values;
}


//----------
// operator
//----------


Tuple & Tuple::operator= (Tuple & t)
// operator =
{
	if (this != &t)
	{	
		if (arity != t.arity)
		{
			delete values;
			arity = t.arity;
			values = new int [arity];
		}
		
		for (int i = 0; i < arity; i++)
			values[i] = t.values[i];
	}
	return *this;
}



//-----------------
// basic functions
//-----------------


void Tuple::Print()
// prints the current tuple
{
	for (int i = 0; i < arity; i++)
		cout << " " << values[i];
	cout << endl;
}


bool Tuple::Next_Compatible_Valid (vector<Variable *> & var, bool * skip_var)
// returns the next valid tuple w.r.t. the domains of the variables in var without modifying the value of the variable v for which skip_var [v] is set to true  (we assume that the domains are not empty and that the current tuple is not valid)
{
	bool ok = true;
	int i = arity - 1;
	do
	{
		if (values[i] == -1)  ok = false;
		else
			if (var[i]->Get_Domain()->Is_Present (values[i])) i--;
			else ok = false;
	}
	while ((i >= 0) && ok);

	int highest_non_valid_variable = i;
	
	int size;
	int j;
	
	for (i = 0; i < highest_non_valid_variable; i++)
		if (! skip_var[i])
		{
			j = 0;
			while (! var[i]->Get_Domain()->Is_Present (j))
				j++;
			values[i] = j;
		}
    
	bool again = true;

	for (; (again) && (i < arity); i++)
	{
		if (! skip_var[i])
		{
			size = var[i]->Get_Domain()->Get_Initial_Size ();
		  j = values[i]+1;
			while ((j < size) && (! var[i]->Get_Domain()->Is_Present (j)))
				j++;
				
			if (j < size)
			{
				values[i] = j;
				again = false;
			}
			else
				{
					j = 0;
					while (! var[i]->Get_Domain()->Is_Present (j))
						j++;
					values[i] = j;
				}
		}
	}

	return ! again;
}


bool Tuple::Next_Compatible_Valid (vector<Variable *> & var, int * variable_order)
// returns the next valid tuple w.r.t. the domains of the variables in var by modifying the variables w.r.t. the variable order provided by variable_order (we assume that the domains are not empty and that the current tuple is not valid)
// variable_order[i] provides the position (in var) of the i th variable in the variable order
{
	bool ok = true;
	int i = arity - 1;
	do
	{
		if (values[variable_order[i]] == -1)  ok = false;
		else
			if (var[variable_order[i]]->Get_Domain()->Is_Present (values[variable_order[i]])) i--;
			else ok = false;
	}
	while ((i >= 0) && ok);

	int highest_non_valid_variable = i;
	
	int size;
	int j;
	
	for (i = 0; i < highest_non_valid_variable; i++)
	{
		j = 0;
		while (! var[variable_order[i]]->Get_Domain()->Is_Present (j))
			j++;
		values[variable_order[i]] = j;
	}
    
	bool again = true;

	for (; (again) && (i < arity); i++)
	{
		size = var[variable_order[i]]->Get_Domain()->Get_Initial_Size ();
		j = values[variable_order[i]]+1;
		while ((j < size) && (! var[variable_order[i]]->Get_Domain()->Is_Present (j)))
			j++;
			
		if (j < size)
		{
			values[variable_order[i]] = j;
			again = false;
		}
		else
			{
				j = 0;
				while (! var[variable_order[i]]->Get_Domain()->Is_Present (j))
					j++;
				values[variable_order[i]] = j;
			}
	}

	return ! again;
}
