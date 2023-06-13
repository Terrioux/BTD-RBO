#include "update_policy.h"


//-------------
// constructor
//-------------


Update_Policy::Update_Policy(COP * ref_pb): A (ref_pb->Get_N())
// constructs an update policy for the COP instance ref_pb
{
  pb = ref_pb;
  x_lb = pb->Get_Lower_Bound_Variable();
  x_ub = pb->Get_Upper_Bound_Variable();
  lb = x_lb->Get_Domain()->Get_Real_Min();
  ub = x_ub->Get_Domain()->Get_Real_Min(); 
}


//-----------------
// basic functions
//-----------------


int Update_Policy::Update_Problem (int result, Assignment & solution, long cost, AC * ac, Deletion_Stack * ds)
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
      // we update the upper bound
      ub = cost - 1;
      
      if (lb <= ub)
      {
        pb->Get_Event_Manager()->New_Level();
        
        pb->Update_Upper_Bound (ub);

        if (! ac->Achieve (pb,A,&nds,x_ub->Get_Num()))
          result = solution.Get_Size() == 0 ? 0 : 1;
        else result = -1;
      }
      else result = solution.Get_Size() == 0 ? 0 : 1;
    }
  
  return result;
}
