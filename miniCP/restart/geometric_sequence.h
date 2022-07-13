/// \file 
/// \brief Definitions related to the Geometric_Sequence class

#ifndef GEOMETRIC_SEQUENCE_H
#define GEOMETRIC_SEQUENCE_H

class Geometric_Sequence     /// This class implements a geometric sequence of numbers  \ingroup restart
{
  protected:
    unsigned long initial_term;   ///< the initial term of the sequence
    unsigned long current_term;   ///< the current term of the sequence
    double scaling_factor;	    	///< the scaling factor used to increase the number of allowed backtracks after each restart (must be greater than or equal to 1)
  
	public:
		// constructor
    Geometric_Sequence (unsigned long first_term, double sc_factor);    ///< constructs a geometric sequence based on the factor sc_factor and the first term first_term
		
		// basic functions
		unsigned long Get_Current_Term ();	 ///< returns the next term of the sequence
		void Reset ();       	 	             ///< resets the sequence
};


//-----------------------------
// inline function definitions
//-----------------------------


inline unsigned long Geometric_Sequence::Get_Current_Term ()
// returns the next term of the sequence
{
  unsigned long term = current_term;
  
  current_term *= scaling_factor;
  
  return term;
}


inline void Geometric_Sequence::Reset ()
// resets the sequence
{
  current_term = initial_term;
}


#endif
