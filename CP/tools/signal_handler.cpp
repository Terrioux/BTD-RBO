#include "signal_handler.h"
#include "io_tools.h"

using namespace std;

Restart_Based_Optimizer * optimizer;
COP * pb;

volatile sig_atomic_t has_received_signal = 0;

void signal_handler(int signal)
{
  has_received_signal = signal;
  
  Assignment solution (pb->Get_N());
  solution = optimizer->Get_Solution();
  
  if (solution.Get_Size() == pb->Get_N())
  {
    cout << "s SATISFIABLE" << endl;
    Print_XCSP3_Solution (pb,solution);
  }
  
  throw ("Timeout");
}


void set_handler (Restart_Based_Optimizer * rbo, COP * inst)
{
  optimizer = rbo;
  pb = inst;
  signal(SIGTERM, signal_handler);  
}
