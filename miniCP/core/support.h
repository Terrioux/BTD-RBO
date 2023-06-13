/// \file 
/// \brief Definitions related to the Support class

#ifndef SUPPORT_H
#define SUPPORT_H

using namespace std;

class Constraint;

class Support           /// This class allows to represent the data structure for storing supports \ingroup core
{
	protected:
		int **** support;		///< support[num][x][v] is the support (i.e. a tuple) of the variable x for the value v w.r.t. the constraint num
		unsigned int m;			///< the initial number of constraints
		
	public:	
		// destructor
		virtual ~Support (){};  ///< destructor
	
		// basic functions
		virtual void Resize ()=0; 																											///< resizes the data structure
		virtual void Reset_Support (int num, int pos, int val)=0; 											///< resets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
		virtual void Reset_Support (Constraint * c)=0; 											            ///< resets all the supports related to the constraint c
		virtual void Change_Support (int num, int pos, int val, int * t, int arity)=0;  ///< sets the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num to the tuple t
		virtual int * Get_Support (int num, int pos, int val)=0;  					 						///< returns a reference to the support of the variable at the position pos in the constraint scope for the value v w.r.t. the constraint num
		virtual bool Continue_From_Scratch ()=0; 																			  ///< returns true if the search for a support must continue from scratch, false if it can continue from the next tuple
};
#endif

