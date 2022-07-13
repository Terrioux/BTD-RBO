/// \file 
/// \brief Definitions related to the Real_Tuple class

#ifndef REAL_TUPLE_H
#define REAL_TUPLE_H

#include "tuple.h"


class Real_Tuple: public Tuple      /// This class allows to represent real tuples (i.e. tuples with the real values) \ingroup core
{
	protected:
		int * real_values;    ///< the real values of values assigned in the tuple
				
	public:
		// constructors and destructor
		Real_Tuple (int size);					///< constructs an empty tuple whose size is size
		~Real_Tuple (); 								///< destructor
		
		// operator
		Real_Tuple & operator= (Real_Tuple & t);   			///< operator =
			
		// basic functions
		void Assign (int pos, int val, int real_val);	        ///< assigns the value val to the variable at position pos
		void Complete (vector<Variable *> & var);		          ///< completes the tuple w.r.t. the domains of the variables in var (we assume that the domains are not empty)
		void Complete (vector<Variable *> & var, int pos);		///< completes the tuple w.r.t. the domains of the variables in var (we assume that the domains are not empty) while preserving the values at position pos
		bool Next_Valid (vector<Variable *> & var, int skip_var_num=-1);	///< returns the next valid tuple w.r.t. the domains of the variables in var without modifying the value of the variable skip_var_num  (we assume that the domains are not empty and that the current tuple is valid)
		bool Next_Compatible_Valid (vector<Variable *> & var, int skip_var_num=-1); ///< returns the next valid tuple w.r.t. the domains of the variables in var without modifying the value of the variable skip_var_num  (we assume that the domains are not empty and that the current tuple is not valid)
		int * Get_Real_Values ();										        	///< returns a pointer on the array of real values
		void Print();																	        ///< prints the current tuple
};



//----------------------------
// inline function definition
//----------------------------


inline void Real_Tuple::Assign (int pos, int val, int real_val)
// assigns the value val to the variable at position pos
{
	values[pos] = val;
	real_values[pos] = real_val;
}


inline int * Real_Tuple::Get_Real_Values ()
// returns a pointer on the array of real values
{
	return real_values;
}


inline void Real_Tuple::Complete (vector<Variable *> & var)
// completes the tuple w.r.t. the domains of the variables in var (we assume that the domains are not empty)
{
	int i = 0;
	int j;
	
	for (vector<Variable *>::iterator iter = var.begin(); iter != var.end(); iter++)
	{
		if (values[i] == -1)
		{
			j = 0;
			while (! (*iter)->Get_Domain()->Is_Present (j))
				j++;
			values[i] = j;
			real_values[i] = (*iter)->Get_Domain()->Get_Real_Value (j);
		}
		i++;
	}
}

inline void Real_Tuple::Complete (vector<Variable *> & var, int pos)
// completes the tuple w.r.t. the domains of the variables in var (we assume that the domains are not empty) while preserving the values at position pos
{
	int i = 0;
	int j;
	
	for (vector<Variable *>::iterator iter = var.begin(); iter != var.end(); iter++)
	{
		if (pos != i)
		{
			j = 0;
			while (! (*iter)->Get_Domain()->Is_Present (j))
				j++;
			values[i] = j;
			real_values[i] = (*iter)->Get_Domain()->Get_Real_Value (j);
		}
		i++;
	}
}



inline bool Real_Tuple::Next_Valid (vector<Variable *> & var, int skip_var_num)
// returns the next valid tuple w.r.t. the domains of the variables in var without modifying the value of the variable skip_var_num  (we assume that the domains are not empty and that the current tuple is valid)
{
	int size;
	int j;
	bool again = true;

	
	int i = 0;
	for (vector<Variable *>::iterator iter = var.begin(); (again) && (iter != var.end()); iter++)
	{
		if (i != skip_var_num)
		{
			size = (*iter)->Get_Domain()->Get_Initial_Size ();
		  j = values[i]+1;
			while ((j < size) && (! (*iter)->Get_Domain()->Is_Present (j)))
				j++;
				
			if (j < size)
			{
				values[i] = j;
				real_values[i] = (*iter)->Get_Domain()->Get_Real_Value (j);
				again = false;
			}
			else
				{
					j = 0;
					while (! (*iter)->Get_Domain()->Is_Present (j))
						j++;
					values[i] = j;
					real_values[i] = (*iter)->Get_Domain()->Get_Real_Value (j);
				}
		}
		i++;
	}

	return ! again;
}


inline bool Real_Tuple::Next_Compatible_Valid (vector<Variable *> & var, int skip_var_num)
// returns the next valid tuple w.r.t. the domains of the variables in var without modifying the value of the variable skip_var_num  (we assume that the domains are not empty and that the current tuple is not valid)
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
		if (i != skip_var_num)
		{
			j = 0;
			while (! var[i]->Get_Domain()->Is_Present (j))
				j++;
			values[i] = j;
			real_values[i] = var[i]->Get_Domain()->Get_Real_Value (j);
		}
    
	bool again = true;

	for (; (again) && (i < arity); i++)
	{
		if (i != skip_var_num)
		{
			size = var[i]->Get_Domain()->Get_Initial_Size ();
		  j = values[i]+1;
			while ((j < size) && (! var[i]->Get_Domain()->Is_Present (j)))
				j++;
				
			if (j < size)
			{
				values[i] = j;
				real_values[i] = var[i]->Get_Domain()->Get_Real_Value (j);
				again = false;
			}
			else
				{
					j = 0;
					while (! var[i]->Get_Domain()->Is_Present (j))
						j++;
					values[i] = j;
					real_values[i] = var[i]->Get_Domain()->Get_Real_Value (j);
				}
		}
	}

	return ! again;
}

#endif
