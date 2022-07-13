#include "last_conflict_combined_variable_heuristic.h"

//----------------------------
// constructor and destructor
//----------------------------


Last_Conflict_Combined_Variable_Heuristic::Last_Conflict_Combined_Variable_Heuristic (CSP * ref_pb, Variable_Heuristic * var_heur): Combined_Variable_Heuristic(ref_pb,var_heur)
// constructs a variable heuristic for the CSP instance ref_pb
{
  last_conflict = 0;
  last_size = 0;
}


//-----------------
// basic functions
//-----------------


Variable * Last_Conflict_Combined_Variable_Heuristic::Next_Variable (Assignment & A, vector<Variable*> & candidates)
// returns the variable which has been selected among the variables of candidates which are unassigned in A
{
  last_size = A.Get_Size();
  if ((last_conflict == 0) || (A.Is_Assigned(last_conflict->Get_Num())))
  {
    last_conflict = 0;
    return vh->Next_Variable(A,candidates);
  }
  else 
    {
      for (auto cand: candidates)
        if (cand == last_conflict)
          return last_conflict;

      last_conflict = 0;
      return vh->Next_Variable(A,candidates);
    }
}
