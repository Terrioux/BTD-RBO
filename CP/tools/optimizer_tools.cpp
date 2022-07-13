#include "optimizer_tools.h"
#include "common_tools.h"
#include "tree_decomposition_tools.h"
#include "timer.h"
#include <typeinfo>

// support header files
#include "last_support.h"
#include "residue_support.h"
#include "multiple_residue_support.h"
#include "no_support.h"


// deletion_stack header files
#include "no_deletion_stack.h"
#include "basic_deletion_stack.h"
#include "constraint_basic_deletion_stack.h"
#include "support_deletion_stack.h"


// solver header files
#include "restart_based_optimizer.h"


// constraint types
#include "compact_table_extension_constraint.h"


// update policy header files
#include "update_policy.h"

namespace optimizer_tools
{
  Restart_Based_Optimizer * optimizer_initialization (COP * pb,  double time_limit, AC * ac)
  // initialization of the solver
  {	
    // initialization of the variable heuristic
    Variable_Heuristic * var_heur = Variable_Heuristic_Initialization (pb);
      
    
    // initialization of the solver
    Deletion_Stack * ds = 0;

    try
    {
      int size;

      size = 0;
      for (unsigned int i = 0; i < pb->Get_N(); i++)
        size += pb->Get_Variable(i)->Get_Domain()->Get_Size();

      if (pb->Requires_Constraint_Restoration())
        ds = new Constraint_Basic_Deletion_Stack(pb,size);
      else ds = new Basic_Deletion_Stack(pb,size);
        
      // initialization of the value heuristic
      Value_Heuristic * val_heur = Value_Heuristic_Initialization (pb);
      
      // we build the optimizer
      return new Restart_Based_Optimizer (pb,var_heur, val_heur, ac, ds, Restart_Policy_Initialization (pb), new Update_Policy(pb), time_limit);
    }
    
    catch (...)
    {
      throw;
    }
    
    return 0;
  }
}
