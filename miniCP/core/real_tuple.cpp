#include "real_tuple.h"
#include <iostream>

//-----------------------------
// constructors and destructor
//-----------------------------


Real_Tuple::Real_Tuple (int size) : Tuple(size)
// construct an empty tuple whose size is size
{
	real_values = new int [arity];
	for (int i = 0; i < arity; i++)
		real_values[i] = -1;
}


Real_Tuple::~Real_Tuple ()
// destructor
{
	delete [] real_values;
}


//----------
// operator
//----------


Real_Tuple & Real_Tuple::operator= (Real_Tuple & t)
// operator =
{
	if (this != &t)
	{	
		if (arity != t.arity)
		{
			delete values;
			delete real_values;
			arity = t.arity;
			values = new int [arity];
			real_values = new int [arity];
		}
		
		for (int i = 0; i < arity; i++)
		{
			values[i] = t.values[i];
			real_values[i] = t.real_values[i];
		}
	}
	return *this;
}



//-----------------
// basic functions
//-----------------


void Real_Tuple::Print()
// prints the current tuple
{
	for (int i = 0; i < arity; i++)
		cout << " (" << values[i] << "," << real_values[i] << ")";
	cout << endl;
}
