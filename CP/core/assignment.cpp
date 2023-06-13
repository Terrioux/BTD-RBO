/// \file 
/// \brief Sources related to the Assignment class

#include "assignment.h"
#include <iostream>
#include "unary_nogood_base_global_constraint.h"
#include "nogood_base_global_constraint.h"

//--------------
// constructors
//--------------

Assignment::Assignment (int max_size): values (max_size), position (max_size,-1)
// construct an empty assignment whose maximum size is max_size and which involves variables of label at most max_size
{
  size = 0;
  maximal_size = max_size;  
}

Assignment::Assignment (int max_size, int max_label): values (max_size), position (max_label,-1)
// construct an empty assignment whose maximum size is max_size and which involves variables of label at most max_label-1
/// param[in] max_size the maximal size of the assignment we construct
/// param[in] max_label the maximal label minus one of the variables which could be involved in the assignment
{
  size = 0;
  maximal_size = max_size;
}


Assignment::Assignment (Assignment & a)
// construct an assignment by copying the assignment a
/// param[in] a the assignment we want to copy
{
  size = a.size;
  maximal_size = a.maximal_size;
	values = a.values;
	position = a.position;	
}


//----------
// operator
//----------

Assignment & Assignment::operator= (Assignment & a)
// operator =
/// param[in] a the assignment we want to assign
{
	if (this != &a)
	{
    size = a.size;
  	values = a.values;
    maximal_size = a.maximal_size;
		position = a.position;
	}
	return *this;
}
	
  
//-----------------	
// basic functions
//-----------------


void Assignment::Unassign (int var)
// unassignes the variable var in the current assignment
/// param[in] var the number of the variable we want to unassign
{
	for (unsigned int i = position[var]+1; i < size; i++)
		values[i-1] = values[i];
	
  size--;
	position[var] = -1;
}


void Assignment::Clear ()
// unassignes all the variables
{
	while (size > 0)
	{
    size--;
		position[values[size].first] = -1;
	}
}


void Assignment::Merge (Assignment & A)
// merges the current assignment and A if they do not share any variable
/// param[in] A the assignment we want to merge with the current assignment
{
  for (unsigned int i = 0; i < A.size; i++)
    Assign (A.values[i].first,A.values[i].second);
}

bool Assignment::Is_Consistent (CSP * pb)
// returns true if the assignment is consistent w.r.t CSP pb, false otherwise
/// param[in] pb the CSP instance for which we want to know if the current assignment is consistent
{
	bool is_consistent = true;
	Constraint * c;

	for (unsigned int i = 0; (i < pb->Get_M()) && is_consistent; i++)
	{
		c = pb->Get_Constraint (i);
		
    if ((dynamic_cast<Unary_Nogood_Base_Global_Constraint*>(c) == 0) && (dynamic_cast<Nogood_Base_Global_Constraint*>(c) == 0))
    {
      // we check whether all the variables involved in the constraint c are assigned
      bool all_assigned = true;
      for (set<unsigned int>::iterator iter_scope = c->Begin(); (iter_scope != c->End()) && (all_assigned); iter_scope++)
        all_assigned = Is_Assigned (*iter_scope);
      
      if (all_assigned) 	is_consistent = c->Is_Satisfied (*this);
      
      if (! is_consistent)
      {
        string values;
        values = "<values>";
        for (vector<Variable *>::iterator iter_var = c->Begin_Var(); iter_var != c->End_Var(); iter_var++)
          values += " " + to_string((*iter_var)->Get_Domain()->Get_Real_Value(Get_Value((*iter_var)->Get_Num())));
        values += " </values>";
        throw ("OUPS C"+to_string (i)+" "+c->Get_XCSP3_Expression()+"\n"+values);
      }
    }
	}
	
	return is_consistent;
}


void Assignment::Print (CSP * pb)
// prints the current assignment with the real name of variables and values if pb is not null
/// param[in] pb the CSP instance to which the current assignment refers
{
  if (pb == 0)
  {
      for (unsigned int i = 0; i < size; i++)
        cout << "(" << values[i].first << "," << values[i].second << ") ";
  }
  else
    {
      for (unsigned int i = 0; i < size; i++)
        cout << "(" << pb->Get_Variable(values[i].first)->Get_Name() << "," << pb->Get_Variable(values[i].first)->Get_Domain()->Get_Real_Value (values[i].second) << ") ";
    }
    
	cout << endl;
}
