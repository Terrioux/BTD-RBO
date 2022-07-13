/// \file 
/// \brief Definitions related to the Multiple_Residue_Support class

#ifndef MULTIPLE_RESIDUE_SUPPORT
#define MULTIPLE_RESIDUE_SUPPORT

#include "residue_support.h"
#include "csp.h"

class Multiple_Residue_Support: public Residue_Support      /// This class implements the multiple residue supports \ingroup core
{
	public:
		// constuctor and destructor
		Multiple_Residue_Support (CSP * pb_init);															///< constructs an empty support for each variable x for each value v w.r.t. each constraint num
		Multiple_Residue_Support (CSP * pb_init, Support * ls);								///< constructs for each variable x for each value v w.r.t. each constraint num by copying the supports of ls
		~Multiple_Residue_Support ();																					///< destructor
	
		// basic functions
		void Change_Support (int num, int pos, int val, int * t, int arity); 	///< sets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num to the tuple t
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void Multiple_Residue_Support::Change_Support (int num, int pos, int val, int * t, int arity)
// sets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num to the tuple t
{
  if (support[num] != 0)
  {
    for (int j = 0; j < arity; j++)
    {
      if (support[num][j][t[j]] == 0)
        support[num][j][t[j]] = new int [arity];
      
      for (int i = 0; i < arity; i++)
        support[num][j][t[j]][i] = t[i];
    }
  }
}

#endif

