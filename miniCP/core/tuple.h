/// \file 
/// \brief Definitions related to the Tuple class

#ifndef TUPLE_H
#define TUPLE_H

#include "variable.h"
#include "domain.h"
#include <vector>

class Tuple           /// This class allows to represent tuples \ingroup core
{
	protected:
		int * values;     ///< the value of each component of the tuple
		int arity;        ///< the size of the tuple
				
	public:
		// constructors and destructor
		Tuple (int size);			///< constructs an empty tuple whose size is size
		Tuple (Tuple & t);		///< constructs a tuple by copying the tuple t
		~Tuple (); 						///< destructor
		
		
		// operator
		Tuple & operator= (Tuple & t);   			///< operator =
		int & operator[] (int n);							///< operator []
		const int & operator[] (int n) const;	///< operator []
		bool operator== (const Tuple & t);    ///< operator ==
		bool operator!= (const Tuple & t);    ///< operator !=

			
		// basic functions
		bool Is_Valid (vector<Variable *> & var);						///< returns true if the tuple is valid w.r.t. the domains of the variables in var
		virtual void Complete (vector<Variable *> & var);		///< completes the tuple w.r.t. the domains of the variables in var (we assume that the domains are not empty)
		virtual void Complete (vector<Variable *> & var, int pos);										///< completes the tuple w.r.t. the domains of the variables in var (we assume that the domains are not empty) while preserving the values at position pos
		virtual bool Next_Valid (vector<Variable *> & var, int skip_var_num=-1);			///< returns the next valid tuple w.r.t. the domains of the variables in var without modifying the value of the variable skip_var_num  (we assume that the domains are not empty and that the current tuple is valid)
		bool Next_Compatible_Valid (vector<Variable *> & var, int skip_var_num=-1);	///< returns the next valid tuple w.r.t. the domains of the variables in var without modifying the value of the variable skip_var_num  (we assume that the domains are not empty and that the current tuple is not valid)
		bool Next_Compatible_Valid (vector<Variable *> & var, bool * skip_var);			///< returns the next valid tuple w.r.t. the domains of the variables in var without modifying the value of the variable v for which skip_var [v] is set to true  (we assume that the domains are not empty and that the current tuple is not valid)
		bool Next_Compatible_Valid (vector<Variable *> & var, int * variable_order);	///< returns the next valid tuple w.r.t. the domains of the variables in var by modifying the variables w.r.t. the variable order provided by variable_order (we assume that the domains are not empty and that the current tuple is not valid). variable_order[i] provides the position (in var) of the i th variable in the variable order

		int * Get_Values ();																	///< returns a pointer on the array of values
		virtual void Print();																	///< prints the current tuple
};


//-----------------------------
// inline operator definitions
//-----------------------------

inline int & Tuple::operator[](int n)
// operator []
{
	return values[n];
}


inline const int & Tuple::operator[] (int n) const
// operator []
{
	return values[n];
}


inline bool Tuple::operator== (const Tuple & t)
// operator ==
{
	if (arity == t.arity)
	{
		int i = 0;
		while ((i < arity) && (values[i] == t.values[i]))
			i++;
		return i == arity;
	}
	else return false;
}


inline bool Tuple::operator!= (const Tuple & t)
// operator !=
{
	return !(*this == t);
}


//----------------------------
// inline function definition
//----------------------------


inline int * Tuple::Get_Values ()
// returns a pointer on the array of values
{
	return values;
}


inline bool Tuple::Is_Valid (vector<Variable *> & var)
// returns true if the tuple is valid w.r.t. the domains of the variables in var
{
	bool ok = true;
	int i = 0;
	for (vector<Variable *>::iterator iter = var.begin(); (ok) && (iter != var.end()); iter++)
	{
		if (values[i] == -1)  ok = false;
		else
			if ((*iter)->Get_Domain()->Is_Present (values[i])) i++;
			else ok = false;
	}
	return ok;
}


inline void Tuple::Complete (vector<Variable *> & var)
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
		}
		i++;
	}
}

inline void Tuple::Complete (vector<Variable *> & var, int pos)
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
		}
		i++;
	}
}


inline bool Tuple::Next_Valid (vector<Variable *> & var, int skip_var_num)
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
				again = false;
			}
			else
				{
					j = 0;
					while (! (*iter)->Get_Domain()->Is_Present (j))
						j++;
					values[i] = j;		
				}
		}
		i++;
	}

	return ! again;
}


inline bool Tuple::Next_Compatible_Valid (vector<Variable *> & var, int skip_var_num)
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

#endif
