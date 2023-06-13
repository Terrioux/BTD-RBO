/// \file 
/// \brief Definitions related to the Update_Policy class

#ifndef UPDATE_POLICY_H
#define UPDATE_POLICY_H


using namespace std;

#include "cop.h"
#include "ac.h"
#include <cluster_tree.h>
#include "no_deletion_stack.h"


class Update_Policy     /// this class allows to represent the way we update the problem in restart-based optimizers   \ingroup optimizer
{
  protected:
    COP * pb;          ///< the instance to which the update policy refers to  
    long lb;           ///< the lower bound
    long ub;           ///< the upper bound
    Variable * x_lb;   ///< the lower bound variable
    Variable * x_ub;   ///< the upper bound variable
    Assignment A;      ///< an empty assignment used for the AC enforcement
    No_Deletion_Stack nds;    ///< a deletion stack for the AC enforcement

	public:
		// constructor and destructor
		Update_Policy (COP * ref_pb);   ///< constructs an update policy for the COP instance ref_pb
		virtual ~Update_Policy (){};    ///< destructor
    
    // basic functions
    virtual bool Record_Nogoods ();            ///< returns true if nld-nogoods must be recorded when finding a solution, false otherwise
    virtual int Initialize_Problem (AC * ac, Deletion_Stack * ds);   ///< initializes the problem (if needed) before launching the solving and returns the result
    virtual int Update_Problem (int result, Assignment & solution, long cost, AC * ac, Deletion_Stack * ds);   ///< updates the problem (if needed) before relaunching the solving and returns the result
};

//--------------------------------
// definition of inline functions
//--------------------------------


inline bool Update_Policy::Record_Nogoods ()
// returns true if nld-nogoods must be recorded when finding a solution, false otherwise
{
  return true;
}


inline int Update_Policy::Initialize_Problem (AC * ac, Deletion_Stack * ds)
// initializes the problem (if needed) before launching the solving and returns the result
{
  return -1;
}


#endif

