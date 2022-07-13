/// \file 
/// \brief Sources related to the AVL class


#include "avl.h"
#include "domain.h"
#include <iostream>

using namespace std;


//-----------------------------
// constructors and destructor
//-----------------------------


AVL::AVL (vector<Variable *> & var)
// constructs an empty AVL for the tuples over the Cartesian products of the domains of the variables in var
/// \param[in] var the scope to which the AVL is related
{
	root = 0;
	size = 0;
	arity = var.size();
	tuple_list = new AVL_Node ** [arity];
	for (int i = 0; i < arity; i++)
	{
		tuple_list[i] = new AVL_Node * [var[i]->Get_Domain()->Get_Initial_Size()];
		for (unsigned int j = 0; j < var[i]->Get_Domain()->Get_Initial_Size(); j++)
			tuple_list[i][j] = 0;
	}
}

		
AVL::~AVL ()
// destructor
{
	delete root;
	
	for (int i = 0; i < arity; i++)
		delete [] tuple_list[i];
	delete [] tuple_list;
}


//-----------------
// basic functions
//-----------------


void generate_complement_AVL_rec (vector<Variable *> & var, AVL & A, int i, int t [], AVL & C)
// computes recursively the AVL C corresponding to the complement of the set represented by the AVL A over the Cartesian product of the domains of the variables in var
/// \param[in] var the scope to which the AVL is related
/// \param[in] A the AVL whose complement we want to compute
/// \param[in] i the current position
/// \param[in,out] t the current tuple
/// \param[out] C the AVL we want to compute
{
	if (i == (int) var.size ())
	{				
		if (! A.Is_Present (t))
			C.Add_Element (t);
	}
	else
		{
			for (unsigned int j = 0; j < var[i]->Get_Domain()->Get_Initial_Size(); j++)
			{
				t [i] = j;
				generate_complement_AVL_rec (var, A, i+1, t, C);
			}
		}
	
}


void AVL::Generate_Complement (vector<Variable *>  &var, AVL & C)
// computes the AVL C corresponding to the complement of the set represented by the current AVL over the Cartesian product of the domains of the variables in var
/// \param[in] var the scope to which the AVL is related
/// \param[out] C the AVL we want to compute
{
	int t [var.size()];

	C.root = 0;
	C.size = 0;

	generate_complement_AVL_rec (var, *this, 0, t, C);
}
