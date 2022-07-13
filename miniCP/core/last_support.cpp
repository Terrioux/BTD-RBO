#include "last_support.h"
#include "nogood_constraint.h"
#include <typeinfo>

//---------------------------
// constuctor and destructor
//---------------------------

Last_Support::Last_Support (CSP * pb_init)
// constructs an empty support for each variable x for each value v w.r.t. each constraint num
{
	pb = pb_init;
	m = pb->Get_M();
	support = new int *** [m];
	is_support_set = new bool ** [m];
	
	int arity,pos;
	Constraint * C;
	
	for (unsigned int num = 0; num < m; num++)
	{
		C = pb->Get_Constraint (num);
		
		if (typeid(*C) != typeid (Nogood_Constraint))
		{
			arity = C->Get_Arity();
			support[num] = new int ** [arity];
			is_support_set[num] = new bool * [arity];
			
			for (pos = 0; pos < arity; pos++)
			{
				support[num][pos] = new int * [C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size()];
				is_support_set[num][pos] = new bool [C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size()];
				for (unsigned int v = 0; v < C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size(); v++)
				{
					support[num][pos][v] = 0;
					is_support_set[num][pos][v] = false;
				}
			}
		}
	}
}


Last_Support::Last_Support (CSP * pb_init, Last_Support * ls)
// constructs for each variable x for each value v w.r.t. each constraint num by copying the supports of ls
{
	pb = pb_init;
	m = pb->Get_M();
	support = new int *** [m];
	is_support_set = new bool ** [m];
	
	int arity, pos;
	Constraint * C;
	
	// we copy the last supports from ls
	for (unsigned int num = 0; num < m; num++)
	{	
		C = pb->Get_Constraint (num);
		
		if (typeid(*C) != typeid (Nogood_Constraint))
		{
			arity = C->Get_Arity();
			support[num] = new int ** [arity];
			is_support_set[num] = new bool * [arity];
			
			for (pos = 0; pos < arity; pos++)
			{
				support[num][pos] = new int * [C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size()];
				is_support_set[num][pos] = new bool [C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size()];
				
				for (unsigned int v = 0; v < C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size(); v++)
				{
					int * t = (ls == 0) ? 0 : ls->Get_Support (num,pos,v);

					if ((t != 0)	&& (C->Get_Scope_Variable(pos)->Get_Domain()->Is_Present(v)))
					{
						is_support_set[num][pos][v] = true;
						support[num][pos][v] = new int [arity];
						for (int i = 0; i < arity; i++)
							support[num][pos][v][i] = t[i];
					}
					else 
						{
							support[num][pos][v] = 0;
							is_support_set[num][pos][v] = false;
						}
				}
			}
		}
    else 
      {
        support[num] = 0;
        is_support_set[num] = 0;
      }
	}
}


Last_Support::~Last_Support ()
// destructor
{
	int arity, pos;
	Constraint * C;
	
	for (unsigned int num = 0; num < m; num++)
	{
		C = pb->Get_Constraint (num);
		
		if (typeid(*C) != typeid (Nogood_Constraint))
		{
			arity = C->Get_Arity();
		
			for (pos = 0; pos < arity; pos++)
			{
				for (unsigned int v = 0; v < C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size(); v++)
					delete [] support[num][pos][v];
				delete [] support[num][pos];
				delete [] is_support_set[num][pos];
			}
			delete [] support[num];
			delete [] is_support_set[num];
		}
	}
	delete [] support;
	delete [] is_support_set;
}


//-----------------
// basic functions
//-----------------


void Last_Support::Resize ()
// resizes the data structure
{
	if (m < pb->Get_M())
	{
		int **** old_support = support;
		bool *** old_is_support_set = is_support_set;
		
		support = new int *** [pb->Get_M()];
		is_support_set = new bool ** [pb->Get_M()];
		
		Constraint * C;
		int arity, pos; 
		
		for (unsigned int num = 0; num < m; num++)
		{
			support [num] = old_support[num];
			is_support_set [num] = old_is_support_set [num]; 
		}
		
		delete [] old_support;
		delete [] old_is_support_set;
		
		for (unsigned int num = m; num < pb->Get_M(); num++)
		{
			C = pb->Get_Constraint (num);
			
			if (typeid(*C) != typeid (Nogood_Constraint))
			{
				arity = C->Get_Arity();
				support[num] = new int ** [arity];
				is_support_set[num] = new bool * [arity];
				
				for (pos = 0; pos < arity; pos++)
				{
					support[num][pos] = new int * [C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size()];
					is_support_set[num][pos] = new bool [C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size()];
					for (unsigned int v = 0; v < C->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size(); v++)
					{
						support[num][pos][v] = 0;
						is_support_set[num][pos][v] = false;
					}
				}
			}
		}
		m = pb->Get_M();
	}
}
