/// \file 
/// \brief Sources related to the COP class

#include "cop.h"
#include "equal_binary_constraint.h"


//-----------------------------
// constructors and destructor
//-----------------------------


COP::COP (Event_Manager * em, string pb_name): CSP (em,pb_name)
// constructs an empty COP whose name is pb_name and for which the events are managed by em
/// \param[in] em the event manager
/// \param[in] pb_name  the name of the COP instance
{
  objective_variable = 0;
}


COP::COP (COP & pb): CSP (pb)
// constructs a COP by copying the COP pb
/// \param[in] pb the COP instance we want to copy
{
  objective_variable = variables[pb.objective_variable->Get_Num()];
}


COP::~COP ()
// destructor
{
}


//-----------------
// basic functions
//-----------------


void COP::Set_Objective (Objective obj, unsigned int num)
// sets the objective criterion to obj and the objective variable to the variable whose number is num
{
  objective = obj;
  
  switch (objective)
  {
    case MINIMIZE: 
        objective_variable = variables[num];
        break;
    case MAXIMIZE:
        Variable * x_max_obj = variables[num];
        Domain * d = x_max_obj->Get_Domain();
        
        set<int> values;
        
        for (unsigned int i = 0; i < d->Get_Size(); i++)
          values.insert (-d->Get_Remaining_Real_Value(i));
        
        Add_Variable (values,x_max_obj->Get_Name()+"_obj",true);
        
        objective_variable = variables[Get_N()-1];
        
        vector<Variable*> scope;
        
        scope.push_back (variables[Get_N()-1]);
        scope.push_back (x_max_obj);
        
        Add_Constraint (new Equal_Binary_Constraint (scope,-1,0),false);        
        break;
  }
}
