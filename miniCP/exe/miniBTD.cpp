/// \file 
/// \brief Main for the solver tool
/// \ingroup exe


// general header files
#include "csp.h"
#include "io_tools.h"
#include "filtering_tools.h"
#include "constraint_tools.h"
#include "solver_tools.h"
#include "rewrite_tools.h"

#include "solver.h"

using namespace solver_tools;

void Print_Solving_Information (CSP * pb, Solver_State solver_result, Assignment & solution)
/// prints the information related to the solving
{
  string starter;
  
  starter = "c ";

  switch (solver_result)
  {
    case STOPPED: 
              // the solving is stopped due to the timeout 
              cout << "s UNKNOWN" << endl;
              break;
    case HAS_PROVED_INCONSISTENCY: 
              // there is no solution
              cout << "s UNSATISFIABLE" << endl;
              break;
    case HAS_PROVED_CONSISTENCY: 
              // there is a solution
              cout << "s SATISFIABLE" << endl;
              Print_XCSP3_Solution (pb,solution);
              
              break;
    default:  throw ("Error");
	}
  	
  cout << starter << "# Constraint checks: " << Solving_Constraint_Check_Counter.Get_Value() << endl;
  cout << starter << "# Nodes: " << Solving_Node_Counter.Get_Value() << endl;
  cout << starter << "# Backtrack: " << Solving_Backtrack_Counter.Get_Value() << endl;
  cout << starter << "# Decisions: " << Solving_Decision_Counter.Get_Value() << endl;
  cout << starter << "# Restarts: " << Solving_Restart_Number_Counter.Get_Value() << endl;
}


int main (int argc, char * argv [])
/// the main function
{
	try
	{		
		Solving_Timer.Start();

    cout << "c version " << VERSION << endl;

		if (argc != 3) 
    {
      cerr << "c usage: " << argv[0] << " TIMELIMIT BENCHNAME" << endl;
      return 1;
    }
  
    double time_limit = atof(argv[1]);								// the limit of the solver runtime		

		CSP * pb;
    
    pb = new CSP (new Event_Manager,argv[2]);

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
		Solver * solver = 0;
		Tree_Decomposition * TD = 0;
		
		try
		{
			// we replace some constraints in intention by constraints in extension
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
			
			// initialization of the solver
			Solver_State result = NOT_RUN;
			
			Assignment solution (pb->Get_N());
			
			if (preprocessing_result)
			{
        solver = solver_initialization (pb,TD,time_limit,preprocessing);

        result = solver->Solve ();

        if (result == 1)
          solution = solver->Get_Solution();
			}
      else result = HAS_PROVED_INCONSISTENCY;

			// print the information related to the solving
			if ((solver != 0)	|| (result == HAS_PROVED_INCONSISTENCY))
				Print_Solving_Information (pb,result,solution);
				
			Solving_Timer.Stop();
			
      cout << "c Global Time: " << Solving_Timer.Get_Duration() << " s" << endl;
		}
		
		catch (...)
		{
			delete preprocessing;
			delete solver;
			delete TD;
			delete pb;
			throw;
		}
		
		delete preprocessing;
		delete solver;
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
