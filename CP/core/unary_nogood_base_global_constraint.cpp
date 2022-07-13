#include "unary_nogood_base_global_constraint.h"
#include <climits>


//-----------------------------
// constructors and destructor
//-----------------------------

Unary_Nogood_Base_Global_Constraint::Unary_Nogood_Base_Global_Constraint (): Global_Constraint ()
// constructs a new constraint with number n which involves the variable in var whose all the tuples are allowed except the tuple ft
{
  unary_nogood_base = 0;
  n = 0;
  num = UINT_MAX;
}


Unary_Nogood_Base_Global_Constraint::Unary_Nogood_Base_Global_Constraint (Unary_Nogood_Base_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  n = c.n;
  
  // we copy the unary nogood base
  if (c.unary_nogood_base == 0)  
    unary_nogood_base = 0;
  else
    {
      unary_nogood_base = new set<int> [n];
      for (unsigned int i = 0; i < n; i++)
        unary_nogood_base[i] = c.unary_nogood_base[i];
    }  
}


Unary_Nogood_Base_Global_Constraint::~Unary_Nogood_Base_Global_Constraint()
// destructor
{
  delete [] unary_nogood_base;
}


//----------
// operator
//----------


Unary_Nogood_Base_Global_Constraint & Unary_Nogood_Base_Global_Constraint::operator= (Unary_Nogood_Base_Global_Constraint & c)
// operator =
{
  if (this != &c)
  {
    n = c.n;
    
    // we copy the unary nogood base
    if (c.unary_nogood_base == 0)  
    {
      if (unary_nogood_base != 0)
        delete [] unary_nogood_base;
      unary_nogood_base = 0;
    }
    else
      {
        if (unary_nogood_base == 0)
          unary_nogood_base = new set<int> [n];
        for (unsigned int i = 0; i < n; i++)
          unary_nogood_base[i] = c.unary_nogood_base[i];
      }
  }
  return *this;
}


//------------------
// basics functions
//------------------


bool Unary_Nogood_Base_Global_Constraint::Is_Satisfied (int * t)
// returns true if the tuple t satisfies the constraint, false otherwise
{
  if (unary_nogood_base != 0)
  {
    for (unsigned int i = 0; i < arity; i++)
    {
      int num = scope_var[i]->Get_Num();
      if (unary_nogood_base[num].find(t[i]) != unary_nogood_base[num].end())
        return false;
    }
  }

  return true;
}


void Unary_Nogood_Base_Global_Constraint::Propagate (CSP * pb, Assignment & A, Support * ls, Deletion_Stack * ds, timestamp ref)
// applies the event-based propagator of the constraint by considering the events occurred since ref
{
  // we check the unary nogoods

  if (unary_nogood_base != 0)
  {
    for (set<unsigned int>::iterator iter_num = scope.begin(); iter_num != scope.end(); iter_num++)
    {
      Variable * y = pb->Get_Variable(*iter_num);
      Domain * dy = y->Get_Domain();
      
      for (set<int>::iterator iter = unary_nogood_base[*iter_num].begin(); iter != unary_nogood_base[*iter_num].end(); iter++)
        if (dy->Is_Present(*iter))
        {      
          ds->Add_Element (y);
          dy->Filter_Value(*iter);

          if (dy->Get_Size() == 0)
          {
            pb->Set_Conflict (y,0);
            return;
          }
        }
    }
  }
}


void Unary_Nogood_Base_Global_Constraint::Add_Nogood (CSP * pb, unsigned int var, int val)
// adds the unary nogood ng involving the variable var and the value val into the nogood base related to the instance pb
{
  if (unary_nogood_base == 0)
  {
    n = pb->Get_N();
    unary_nogood_base = new set<int> [n];
  }
  
  scope.insert (var);
  if (arity < scope.size())   // we add a new variable to the scope
  {
    scope_var.push_back (pb->Get_Variable (var));      
    position.insert (make_pair(var,arity));
    arity++;
  }

  unary_nogood_base[var].insert (val);
}


void Unary_Nogood_Base_Global_Constraint::Reset (CSP * pb)
// resets the nogood base
{
  if (arity > 0)
  {
    for (set<unsigned int>::iterator iter_num = scope.begin(); iter_num != scope.end(); iter_num++)
      unary_nogood_base[*iter_num].clear();
  } 
}
