/// \file 
/// \brief Definitions related to the Restart_Policy class

#ifndef RESTART_POLICY_H
#define RESTART_POLICY_H

#include "geometric_sequence.h"
#include "information.h"
#include "csp.h"

class Restart_Policy      /// This class allows to represent policies deciding whether a solver must restart or not  \ingroup restart
{
  protected:
    CSP * pb;                ///< the problem to which the policy refers
    unsigned long limit;		 ///< the current number of allowed steps before the next restart
		unsigned long conflict_number_since_last_restart;		///< the number of conflict since the last restart
    Geometric_Sequence * sequence;     ///< the sequence used to determine the next limit
    
	public:
		// constructor and destructor
		Restart_Policy (CSP * ref_pb, Geometric_Sequence * seq);   ///< constructs a restart policy based on the sequence seq
		virtual ~Restart_Policy ();        ///< destructor
		
		// basic functions
		virtual bool Restart ();	 ///< returns true if the solver has to restart according to the given policy and reset all information required by the policy, false otherwise
		virtual void Reset ();	 	 ///< resets the policy parameter
};

//-----------------------------
// inline function definitions
//-----------------------------


inline bool Restart_Policy::Restart ()
// returns true if the solver has to restart according to the given policy and reset all information required by the policy, false otherwise
{
  if (pb->Get_Last_Conflict_Constraint() != 0)
  {
    conflict_number_since_last_restart++;
    pb->Reset_Conflict();
  }

	if (limit == 	conflict_number_since_last_restart)
	{
		limit = sequence->Get_Current_Term();
		conflict_number_since_last_restart = 0;
		return true;
	}
	else return false;
}


inline void Restart_Policy::Reset ()
// resets the policy parameter 
{
  sequence->Reset();
  limit = sequence->Get_Current_Term();
	conflict_number_since_last_restart = 0;
}


#endif
