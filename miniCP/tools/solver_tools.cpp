#include "solver_tools.h"
#include "common_tools.h"
#include "information.h"
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
#include "solver.h"


// constraint types
#include "compact_table_extension_constraint.h"


// merge policy header file
#include "merge_policy.h"


namespace solver_tools
{
  Merge_Policy * Merge_Policy_Initialization (int cluster_nb)
  // initialization of the verify policy
  {
    return new Merge_Policy(50,1.0,100,cluster_nb);
  }


  Solver * solver_initialization (CSP * pb, Tree_Decomposition * & TD, double time_limit, AC * ac)
  // initialization of the solver
  {	
    // initialization of the variable heuristic
    Variable_Heuristic * var_heur = Variable_Heuristic_Initialization (pb);

    // initialization of elements specific to BTD
    Cluster_Heuristic * next_cluster_heur=0;
    Root_Heuristic * root_cluster_heur=0;
    Root_Heuristic * restart_root_cluster_heur=0;
        
    // computation of the tree-decomposition related to the CSP pb
    TD = Tree_Decomposition_Initialization (pb);      

    next_cluster_heur = Next_Cluster_Heuristic_Initialization ();
    root_cluster_heur = Root_Heuristic_Initialization (6,var_heur);
    restart_root_cluster_heur = Root_Heuristic_Initialization (7,var_heur);
      
    // initialization of the solver
    Solver * solver = 0;
    Deletion_Stack * ds = 0;
    
    int size;

    size = 0;
    for (unsigned int i = 0; i < pb->Get_N(); i++)
      size += pb->Get_Variable(i)->Get_Domain()->Get_Size();

    if (pb->Requires_Constraint_Restoration())
      ds = new Constraint_Basic_Deletion_Stack(pb,size);
    else ds = new Basic_Deletion_Stack(pb,size);
    
    // initialization of the value heuristic
    Value_Heuristic * val_heur = Value_Heuristic_Initialization (pb);
    
    // we build the solver      
    solver = new Solver (pb,var_heur, val_heur, ac, ds, TD, root_cluster_heur, next_cluster_heur, Restart_Policy_Initialization (pb),Restart_Policy_Initialization (pb), restart_root_cluster_heur, Merge_Policy_Initialization(TD->Get_Max_Label()+1), time_limit);

    return solver;
  }
}
