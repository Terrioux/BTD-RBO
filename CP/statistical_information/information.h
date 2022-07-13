/// \file 
/// \brief Definitions of counters

#ifndef INFORMATION_H
#define INFORMATION_H

#include "statistical_counter.h"
#include "timer.h"

#ifdef EXTERN
  // counters regarding the solving
  extern Timer Solving_Timer;     // timer used for measuring the runtime of the solving step 
  extern Statistical_Counter<double> Solving_Time_Counter;                        // the runtime for the runtime of the solving step 
  extern Statistical_Counter<unsigned long> Solving_Node_Counter;                 // the number of nodes performed during the solving step 
  extern Statistical_Counter<unsigned long> Solving_Backtrack_Counter;            // the number of backtracks performed during the solving step 
  extern Statistical_Counter<unsigned long> Solving_Decision_Counter;             // the number of decisions made during the solving step (for MAC-based solver)
  extern Statistical_Counter<unsigned long> Solving_Conflict_Counter;             // the number of conflicts encountered during the solving step
  extern Statistical_Counter<unsigned long> Solving_Constraint_Check_Counter;     // the number of constraint checks performed during the solving step 
  extern Statistical_Counter<unsigned long> Solving_Restart_Number_Counter;                // the number of restarts performed during the solving step   
#else
  /// \addtogroup information
  ///@{
  
  Timer Solving_Timer;     ///< timer used for measuring the runtime of the solving step 
  Statistical_Counter<double> Solving_Time_Counter;                        ///< the runtime of the solving step 
  Statistical_Counter<unsigned long> Solving_Node_Counter;                 ///< the number of nodes performed during the solving step 
  Statistical_Counter<unsigned long> Solving_Backtrack_Counter;            ///< the number of backtracks performed during the solving step 
  Statistical_Counter<unsigned long> Solving_Decision_Counter;             ///< the number of decisions made during the solving step (for MAC-based solver)
  Statistical_Counter<unsigned long> Solving_Conflict_Counter;             ///< the number of conflicts encountered during the solving step
  Statistical_Counter<unsigned long> Solving_Constraint_Check_Counter;     ///< the number of constraint checks performed during the solving step 
  Statistical_Counter<unsigned long> Solving_Restart_Number_Counter;                ///< the number of restarts performed during the solving step 
  ///@}
#endif

#endif
