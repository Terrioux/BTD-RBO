/// \file 
/// \brief Definitions related to the Erwa_Weights class

#ifndef ERWA_WEIGHTS_H
#define ERWA_WEIGHTS_H

#include "nogood_base_global_constraint.h"
#include "unary_nogood_base_global_constraint.h"
#include <cmath>
#include <climits>

class Erwa_Weights      /// This class allows to represent ERWA weights for constraints (Exponential Recency Weighted Average)  \ingroup heuristics
{
	protected:
    CSP * pb;                           ///< the CSP instance to which the weights are related 
    vector<double> weights;             ///< the weight related to each constraint
    vector< unsigned long> conflict;    ///< the number of the last conflict for each variable
    unsigned long conflict_number;      ///< the number of conflicts
    double alpha;                       ///< the step-size parameter
    double alpha_init;                  ///< the initial value of the step-size parameter
    double beta;                        ///< the step-size lower bound
    double epsilon;                     ///< the value used to decrement alpha

	public:
		// constructor
    Erwa_Weights (CSP * ref_pb, double alpha_val, double beta_val, double epsilon_val);   ///< constructs the ERWA weights for the constraints of the CSP instance ref_pb
			
		// basic functions
    void Add_Constraint (Constraint * c);                          ///< adds the constraint num
		double Get_Constraint_Weight (unsigned int num);               ///< returns the weight of the constraint whose number is num
    void Update_After_Filtering (Assignment & A, int var = -1);    ///< updates the data structure after the application of a filtering related to the assignment A and the current variable var 
    void Update_When_Restarting ();                                ///< updates the data structure when restarting
    void Update_When_New_Connected_Component();                    ///< updates the data structure when taking into account a new connected component
    double Get_Alpha ();                                           ///< returns the current value of alpha
    double Get_Beta ();                                            ///< returns the current value of beta
    double Get_Epsilon ();                                         ///< returns the current value of epsilon
    unsigned long Get_Conflict_Number ();                          ///< returns the current number of conflicts
    unsigned long Get_Delta_Conflict (int num);                    ///< returns the delta between the current conflict and the last conflict for the constraint num
    void Reset_Alpha ();                                           ///< resets the value of alpha to its initial value
    void Reset_Alpha (double val);                                 ///< resets the value of alpha with the value val
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void Erwa_Weights::Add_Constraint (Constraint * c)
// adds the constraint num
{
  assert (c != 0);
  unsigned int num = c->Get_Num();
  
  if (num >= weights.size())
  {
    weights.resize(num+1);
    conflict.resize (num+1);
    conflict[num] = 0;
    
    if ((dynamic_cast<Nogood_Base_Global_Constraint *>(c) == 0) && (dynamic_cast<Unary_Nogood_Base_Global_Constraint *>(c) == 0))
      weights[num] = -1;
    else weights[num] = 0;
  }
}


inline double Erwa_Weights::Get_Constraint_Weight (unsigned int num)
// returns the weight of the constraint whose number is num
{
  assert (num < weights.size());
  if (weights[num] > 0)
    return weights[num];
  else return 0;
}


inline void Erwa_Weights::Update_After_Filtering (Assignment & A, int var)
// updates the data structure after the application of a filtering related to the assignment A and the current variable var 
{
  Constraint * c = pb->Get_Last_Conflict_Constraint();
  
  if (c != 0) 
  {
    unsigned int num = c->Get_Num();
    if ((num < UINT_MAX) && (weights[num] != 0))
    {
      weights[num] = (1-alpha) * weights[num] + alpha * (1.0 / (conflict_number - conflict[num]+1));
      conflict_number++;
      conflict[num] = conflict_number;
      
      if (alpha > beta)
        alpha -= epsilon;
    }
  }
}


inline void Erwa_Weights::Update_When_Restarting ()
// updates the data structure when restarting
{
  for (unsigned int i = 0; i < pb->Get_M(); i++)
    if (weights[i] > 0)
      weights[i] = weights[i] * pow(0.995,conflict_number - conflict[i]);
}


inline void Erwa_Weights::Update_When_New_Connected_Component()
// updates the data structure when taking into account a new connected component
{
  alpha = alpha_init;
  conflict_number = 0;
}

inline double Erwa_Weights::Get_Alpha ()
// returns the current value of alpha
{
  return alpha;
}


inline double Erwa_Weights::Get_Beta ()
// returns the current value of beta
{
  return beta;
}


inline double Erwa_Weights::Get_Epsilon ()
// returns the current value of epsilon
{
  return epsilon;
}


inline unsigned long Erwa_Weights::Get_Conflict_Number ()
// returns the current number of conflicts
{
  return conflict_number;
}


inline unsigned long Erwa_Weights::Get_Delta_Conflict (int num)
// returns the delta between the current conflict and the last conflict for the constraint num
{
  if (conflict[num] > 0)
    return conflict_number - conflict[num];
  else return conflict_number;
}


inline void Erwa_Weights::Reset_Alpha()
// resets the value of alpha to its initial value
{
  alpha = alpha_init;
}


inline void Erwa_Weights::Reset_Alpha (double val)
// resets the value of alpha with the value val
{
  alpha = val;
}

#endif
