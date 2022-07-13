#include "residue_support.h"
#include "nogood_base_global_constraint.h"
#include <typeinfo>

//---------------------------
// constuctor and destructor
//---------------------------

Residue_Support::Residue_Support (CSP * pb_init)
// constructs an empty support for each variable x for each value v w.r.t. each constraint num
{
	pb = pb_init;
	m = pb->Get_M();
	support = new int *** [m];
	int arity,pos;
	Constraint * C;
	
	for (unsigned int num = 0; num < m; num++)
	{
		C = pb->Get_Constraint (num);
		
		if (typeid(*C) != typeid (Nogood_Base_Global_Constraint))
		{
			arity = C->Get_Arity();
			support[num] = new int ** [arity];
			
			for (pos = 0; pos < arity; pos++)
			{
				support[num][pos] = new int * [C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size()];
				for (unsigned int v = 0; v < C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size(); v++)
					support[num][pos][v] = 0;
			}
		}
    else support[num] = 0;
	}
}


Residue_Support::Residue_Support (CSP * pb_init, Support * ls)
// constructs for each variable x for each value v w.r.t. each constraint num by copying the supports of ls
{
	pb = pb_init;
	m = pb->Get_M();
	support = new int *** [m];
	
	int arity, pos;
	Constraint * C;
	
	// we copy the last supports from ls
	for (unsigned int num = 0; num < m; num++)
	{	
		C = pb->Get_Constraint (num);
		
		if (typeid(*C) != typeid (Nogood_Base_Global_Constraint))
		{
			arity = C->Get_Arity();
			support[num] = new int ** [arity];
			
			for (pos = 0; pos < arity; pos++)
			{
				support[num][pos] = new int * [C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size()];
				
				for (unsigned int v = 0; v < C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size(); v++)
				{
					int * t = ls->Get_Support (num,pos,v);

					if ((t != 0)	&& (C->Get_Scope_Variable(pos)->Get_Domain()->Is_Present(v)))
					{
						support[num][pos][v] = new int [arity];
						for (int i = 0; i < arity; i++)
							support[num][pos][v][i] = t[i];
					}
					else support[num][pos][v] = 0;
				}
			}
		}
    else support[num] = 0;
	}
}


Residue_Support::~Residue_Support ()
// destructor
{
	int arity, pos;
	Constraint * C;
	
	for (unsigned int num = 0; num < m; num++)
	{
		C = pb->Get_Constraint (num);
		
		if (typeid(*C) != typeid (Nogood_Base_Global_Constraint))
		{
			arity = C->Get_Arity();
		
			for (pos = 0; pos < arity; pos++)
			{
				for (unsigned int v = 0; v < C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size(); v++)
					delete [] support[num][pos][v];
				delete [] support[num][pos];
			}
			delete [] support[num];
		}
	}
	delete [] support;
}


//-----------------
// basic functions
//-----------------


void Residue_Support::Resize ()
// resizes the data structure
{
	if (m < pb->Get_M())
	{
		int **** old_support = support;
		
		support = new int *** [pb->Get_M()];
		
		Constraint * C;
		int arity, pos; 
		
		for (unsigned int num = 0; num < m; num++)
		{
			support [num] = old_support[num];
		}
		
		delete [] old_support;
		
		for (unsigned int num = m; num < pb->Get_M(); num++)
		{
			C = pb->Get_Constraint (num);
			
			if (typeid(*C) != typeid (Nogood_Base_Global_Constraint))
			{
				arity = C->Get_Arity();
				support[num] = new int ** [arity];
				
				for (pos = 0; pos < arity; pos++)
				{
					support[num][pos] = new int * [C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size()];
					for (unsigned int v = 0; v < C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size(); v++)
						support[num][pos][v] = 0;
				}
			}
      else support[num] = 0;
		}
		m = pb->Get_M();
	}
}

