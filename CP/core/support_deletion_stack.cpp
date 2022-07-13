/// \file 
/// \brief Sources related to the Support_Deletion_Stack class


#include "support_deletion_stack.h"


//-----------------------------
// constructors and destructor
//-----------------------------


Support_Deletion_Stack::Support_Deletion_Stack (CSP * pb, int level_number, Support * l_s) : Basic_Deletion_Stack (pb,level_number)
// construct an empty deletion stack related to the CSP pb with at most level_number levels
/// \param[in] pb the CSP instance to which the stack is related
/// \param[in] level_number the maximal size of the stack
/// \param[in] l_s the support data structure we have to back up
{
	ref_pb = pb;
	supports = new pair<int, vector <pair<int, pair <int, int *> > > > * [pb->Get_M()];
	is_support_changed = new int * [pb->Get_M()];
	max_used_size = new int * [pb->Get_M()];
	int arity_sum = 0;
	for (unsigned int i = 0; i < pb->Get_M(); i++)
	{
		supports[i] = new pair<int, vector <pair<int, pair<int, int *> > > > [pb->Get_Constraint(i)->Get_Arity()];
		is_support_changed[i] = new int [pb->Get_Constraint(i)->Get_Arity()];
		max_used_size[i] = new int [pb->Get_Constraint(i)->Get_Arity()];
		
		for (unsigned int j = 0; j < pb->Get_Constraint(i)->Get_Arity(); j++)
		{
			is_support_changed[i][j] = -1;
			supports[i][j].first = 0;
			supports[i][j].second.resize (100);
			max_used_size[i][j] = 0;
		}
		
		arity_sum += pb->Get_Constraint(i)->Get_Arity();
	}

	support_change = new pair<int, vector<pair <int,int> > > [level_number];
	
	for (int i = 0; i < level_number; i++)
	{
		support_change[i].first = 0;
		support_change[i].second.resize (arity_sum);
	}
	
	ls = l_s;
}


Support_Deletion_Stack::~Support_Deletion_Stack()
// destructor
{
	for (unsigned int i = 0; i < ref_pb->Get_M(); i++)
	{
		for (unsigned int j = 0; j < ref_pb->Get_Constraint(i)->Get_Arity(); j++)
		{
			for (int k = 0; k < max_used_size[i][j]; k++)
				delete [] supports[i][j].second[k].second.second;
		}
		delete [] max_used_size[i];
		delete [] supports[i];
		delete [] is_support_changed[i];
	}
	delete [] max_used_size;
	delete [] supports;
	delete [] is_support_changed;
		
	delete [] support_change;
}

