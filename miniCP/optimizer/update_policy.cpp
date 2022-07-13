#include "update_policy.h"


//-------------
// constructor
//-------------


Update_Policy::Update_Policy(COP * ref_pb): A (ref_pb->Get_N())
// constructs an update policy for the COP instance ref_pb
{
  pb = ref_pb;
  x_obj = pb->Get_Objective_Variable();
  D_obj = x_obj->Get_Domain();
}


//-----------------
// basic functions
//-----------------


int Update_Policy::Update_Problem (int result, Assignment & solution, AC * ac, Deletion_Stack * ds)
// updates the problem (if needed) before relaunching the solving and returns the result
{
  if (result == 0)
  {
    if (solution.Get_Size() > 0)
      result = 1;
  }
  else
    if (result == 1)
    {
      // we update the domain of the objective variable
      int real_val_obj = D_obj->Get_Real_Value(solution.Get_Value(x_obj->Get_Num()));
      D_obj->Filter_Value_From (real_val_obj);
      if (D_obj->Get_Size() == 0)
        result = solution.Get_Size() == 0 ? 0 : 1;
      else
        {
          if (! ac->Achieve (pb,A,&nds,x_obj->Get_Num()))
            result = solution.Get_Size() == 0 ? 0 : 1;
          else result = -1;
        }              
    }
  return result;
}
