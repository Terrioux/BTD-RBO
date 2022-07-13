/// \file 
/// \brief Definitions related to the No_Support class

#ifndef NO_SUPPORT_H
#define NO_SUPPORT_H

#include "support.h"
#include "csp.h"

class No_Support: public Support      /// This class implements the absence of supports \ingroup core
{
	public:
		// constuctor and destructor
		No_Support (CSP * pb = 0);	///< constructs an empty support
	
		// basic functions
		void Resize (); 																											///< resizes the data structure
		void Reset_Support (int num, int pos, int val); 											///< resets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
		void Change_Support (int num, int pos, int val, int * t, int arity);  ///< sets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num to the tuple t
		int * Get_Support (int num, int pos, int val);  					 						///< returns a reference to the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
		bool Continue_From_Scratch (); 																			  ///< returns true if the search for a support must continue from scratch, false if it can continue from the next tuple
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void No_Support::Resize ()
// resizes the data structure
{
}

inline void No_Support::Reset_Support (int num, int pos, int val)
// resets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
{
}

inline void No_Support::Change_Support (int num, int pos, int val, int * t, int arity)
// sets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num to the tuple t
{
}


inline int * No_Support::Get_Support (int num, int pos, int val)
// returns a reference to the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
{
	return 0;
}


inline bool No_Support::Continue_From_Scratch ()
// returns true if the search for a support must continue from scratch, false if it can continue from the next tuple
{
	return true;
}

#endif
