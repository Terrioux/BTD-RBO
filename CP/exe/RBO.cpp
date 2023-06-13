/// \file 
/// \brief Main for the optimizer tool
/// \ingroup exe


// general header files
#include "cop.h"
#include "timer.h"
#include "io_tools.h"
#include "filtering_tools.h"
#include "constraint_tools.h"
#include "rewrite_tools.h"
#include "stat_tools.h"
#include "optimizer_tools.h"
#include "signal_handler.h"

using namespace optimizer_tools;

void Print_Solving_Information (COP * pb, Optimizer_State optimizer_result, Assignment & solution, long cost)
/// prints the information related to the solving
{
  string starter;
  
  starter = "c ";

  switch (optimizer_result)
  {
    case HAS_BEEN_STOPPED: 
              // the solving is stopped due to the timeout 
              cout << "s UNKNOWN" << endl;
              break;
    case HAS_FOUND_INCONSISTENT: 
              // there is no solution
              cout << "s UNSATISFIABLE" << endl;
              break;
    case HAS_FOUND_SOLUTION: 
              // there is a solution
              cout << "s SATISFIABLE" << endl;
              Print_XCSP3_Solution (pb,solution);
              break;
    case HAS_FOUND_OPTIMUM: 
              // there is a solution
              cout << "s OPTIMUM FOUND" << endl;
              Print_XCSP3_Solution (pb,solution);
              break;
    default:  throw ("Error");
  }
		
  cout << starter << "# Constraint checks: " << Solving_Constraint_Check_Counter.Get_Value() << endl;
  cout << starter << "# Nodes: " << Solving_Node_Counter.Get_Value() << endl;
  cout << starter << "# Backtrack: " << Solving_Backtrack_Counter.Get_Value() << endl;
  cout << starter << "# Decisions: " << Solving_Decision_Counter.Get_Value() << endl;
  cout << starter << "# Conflicts: " << Solving_Conflict_Counter.Get_Value() << endl;
  cout << starter << "# Restarts: " << Solving_Restart_Number_Counter.Get_Value() << endl;
  
}


int main (int argc, char * argv [])
/// the main function
{
	try
	{		
    cerr << "c Version " << VERSION << endl;
    
    if (argc != 3) 
    {
      cerr << "c Usage: " << argv[0] << " TIMELIMIT BENCHNAME" << endl;
      return 1;
    }
  
    double time_limit = atof(argv[1]);								// the limit of the solver runtime		

		COP * pb;
    
    pb = new COP (new Event_Manager,argv[argc-1]);
 	
		// read of the file
		try 
		{
			Load (pb,argv[argc-1]);
		}

		catch (...)
		{
			throw;
		}
    		
		AC * preprocessing = 0;
		Restart_Based_Optimizer * optimizer = 0;
		Tree_Decomposition * TD = 0;
		
		try
		{
			// we replace some constraints in intention by constraints in extension
			Solving_Timer.Start();
      
      Replace_Predicate_By_Global_Constraint (pb);

			// initialization of the preprocessing
			bool preprocessing_result = pb->Get_Last_Conflict_Variable() == 0;
      
      if (preprocessing_result)
        preprocessing = AC_initialization (pb);
			
			if (preprocessing != 0)
			{
				// we perform the preprocessing
				preprocessing_result = preprocessing->Achieve (pb);
			}
						
			// initialization of the optimizer
			Optimizer_State result = NOT_RUN_YET;
			
			Assignment solution (pb->Get_N());
      long cost = 0;
			
			if (preprocessing_result)
			{
        optimizer = optimizer_initialization (pb,time_limit,preprocessing);
        
        set_handler (optimizer,pb);

        result = optimizer->Solve ();
        
        if ((result == HAS_FOUND_SOLUTION) || (result == HAS_FOUND_OPTIMUM))
        {
          solution = optimizer->Get_Solution();
          cost = optimizer->Get_Cost();
        }
			}
      else result = HAS_FOUND_INCONSISTENT;

			// print the information related to the solving
			Print_Solving_Information (pb,result,solution,cost);
      
      cout << "c Global Time: " << Solving_Timer.Get_Duration() << " s" << endl;
		}
		
		catch (...)
		{
			delete optimizer;
			delete TD;
			delete pb;
			throw;
		}
		
		delete optimizer;
		delete TD;
		delete pb;
	}
 	
 	catch (string error)
 	{
    if (error.find("not yet supported") != string::npos)
      cerr << "s UNSUPPORTED" << endl;
    else cerr << "c " << error << endl;
		return 1;
	}

 	catch (char const * error)
 	{
    string s (error);
    if (s.find("not yet supported") != string::npos)
      cerr << "s UNSUPPORTED" << endl;
    else cerr << "c " << error << endl;
		return 1;
	}
  
	catch (const runtime_error& error)
  {
    string s = error.what();
  
    if (s.find("not yet supported") != string::npos)
      cerr << "s UNSUPPORTED" << endl;
    else cerr << "c " << error.what() << endl;
    
		return 1;    
  }


  return 0;
}
