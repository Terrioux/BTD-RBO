#include "reset_restart_erwa_variable_heuristic.h"


//-------------
// constructor
//-------------


Reset_Restart_Erwa_Variable_Heuristic::Reset_Restart_Erwa_Variable_Heuristic (CSP * ref_pb, double alpha_val, double beta_val, double epsilon_val, double delta_val): Variable_Heuristic (ref_pb)
// constructs a variable heuristic for the CSP instance ref_pb
{
  delta = delta_val;
  weights = new Erwa_Weights (pb, alpha_val, beta_val, epsilon_val);
}


Reset_Restart_Erwa_Variable_Heuristic::Reset_Restart_Erwa_Variable_Heuristic (CSP * ref_pb, Erwa_Weights * w, double delta_val): Variable_Heuristic (ref_pb)
// constructs a variable heuristic for the CSP instance ref_pb with weights w
{
  delta = delta_val;
  weights = w;
  has_original_weights = false;
}


//-----------------
// basic functions
//-----------------


Variable * Reset_Restart_Erwa_Variable_Heuristic::Next_Variable (Assignment & A, vector<Variable*> & candidates)
// returns the variable which has been selected among the variables of candidates which are unassigned in A
{
	Variable * x = 0;
	double max = -1.0;
	double ratio;
	vector<Variable *>::iterator iter2;
	Multi_Hypergraph * h = pb->Get_Structure ();
	list<Edge *>::iterator iter3;
	int num;
	
	if (candidates.size() == 1)
	{
		iter2 = candidates.begin();
		x = *iter2;
	}
	else
		{
			for (vector<Variable * >::iterator iter = candidates.begin(); iter != candidates.end(); iter++)
				if (! A.Is_Assigned ((*iter)->Get_Num()))
				{
					num = (*iter)->Get_Num();
					ratio = 0.0;
					for (iter3 = h->Begin_Edge_List (num); iter3 != h->End_Edge_List (num); iter3++)
            if (weights->Get_Constraint_Weight((*iter3)->Get_Num()) > 0)
              ratio += delta + weights->Get_Constraint_Weight((*iter3)->Get_Num());

					ratio /= (*iter)->Get_Domain()->Get_Size();

					if ((ratio > max) || ((ratio == max) && ((*iter)->Get_Num() < x->Get_Num())))   // the last check is used for ensuring the compatibility with old C version
					{
						max = ratio;
						x = *iter;
						iter2 = iter;
					}
				}
		}
	
	return x;
}
