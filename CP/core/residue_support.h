/// \file 
/// \brief Definitions related to the Residue_Support class

#ifndef RESIDUE_SUPPORT_H
#define RESIDUE_SUPPORT_H

#include "support.h"
#include "csp.h"

class Residue_Support: public Support     /// This class implements the residue supports \ingroup core
{
	protected:
		CSP * pb;									///< the CSP to which the structure is related 
	
	public:
		// constuctor and destructor
		Residue_Support (CSP * pb_init);															///< constructs an empty support for each variable x for each value v w.r.t. each constraint num
		Residue_Support (CSP * pb_init, Support * ls);								///< constructs for each variable x for each value v w.r.t. each constraint num by copying the supports of ls
		~Residue_Support ();																					///< destructor
	
		// basic functions
		void Resize (); 																											///< resizes the data structure
		void Reset_Support (int num, int pos, int val); 											///< resets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
    void Reset_Support (Constraint * c) override; 				  	            ///< resets all the supports related to the constraint c
		void Change_Support (int num, int pos, int val, int * t, int arity); 	///< sets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num to the tuple t
		int * Get_Support (int num, int pos, int val);  			 								///< returns a reference to the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
		bool Continue_From_Scratch (); 																			  ///< returns true if the search for a support must continue from scratch, false if it can continue from the next tuple
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void Residue_Support::Reset_Support (int num, int pos, int val)
// resets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
{
}


inline void Residue_Support::Reset_Support (Constraint * c)
// resets all the supports related to the constraint c
{
  for (unsigned int pos = 0; pos < c->Get_Arity(); pos++)
  {
    for (unsigned int v = 0; v < c->Get_Scope_Variable(pos)->Get_Domain()->Get_Initial_Size(); v++)
    {
      delete [] support[c->Get_Num()][pos][v];
      support[c->Get_Num()][pos][v] = 0;
    }
  }
}


inline void Residue_Support::Change_Support (int num, int pos, int val, int * t, int arity)
// sets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num to the tuple t
{
	if (support[num] != 0)
  {
    if (support[num][pos][val] == 0)
      support[num][pos][val] = new int [arity];
	
    for (int i = 0; i < arity; i++)
      support[num][pos][val][i] = t[i];
  }
}


inline int * Residue_Support::Get_Support (int num, int pos, int val)
// returns a reference to the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
{
	if (((unsigned) num < m) && (support[num] != 0) && (support[num][pos][val] != 0))
		return support[num][pos][val];
	else return 0;
}


inline bool Residue_Support::Continue_From_Scratch ()
// returns true if the search for a support must continue from scratch, false if it can continue from the next tuple
{
	return true;
}
#endif

