#include "list_channel_global_constraint.h"


//--------------
// constructors 
//--------------


List_Channel_Global_Constraint::List_Channel_Global_Constraint (vector<Variable *> & var, vector<Variable *> & list_1, int start1, vector<Variable *> & list_2, int start2): Global_Constraint (var)
// constructs a new channel constraint which involves the variable in var and whose start index is start
{
  list[0] = list_1;
  list[1] = list_2;
  
  belong_to_list[0].resize(arity,-1);
  belong_to_list[1].resize(arity,-1);

  int pos = 0;
  for (vector<Variable *>::iterator iter = list_1.begin(); iter != list_1.end(); iter++)
  {
    belong_to_list[0][Get_Position((*iter)->Get_Num())] = pos;
    pos++;
  }
  
  pos = 0;
  for (vector<Variable *>::iterator iter = list_2.begin(); iter != list_2.end(); iter++)
  {
    belong_to_list[1][Get_Position((*iter)->Get_Num())] = pos;
    pos++;
  }

  start_index[0] = start1;
  start_index[1] = start2;
}


List_Channel_Global_Constraint::List_Channel_Global_Constraint (List_Channel_Global_Constraint & c): Global_Constraint (c)
// constructs a new constraint by copying the constraint c
{
  for (int k = 0; k < 2; k++)
  {
    list[k] = c.list[k];
    belong_to_list[k] = c.belong_to_list[k];
    start_index[k] = c.start_index[k];
  }
}


//-----------------
// basic functions
//-----------------


string List_Channel_Global_Constraint::Get_XCSP3_Expression()
// returns the string corresponding to the expression of the constraint in XCSP 3 format
{
  string expr;
  expr = "<channel> ";
  expr += "  <list> ";
  for (vector<Variable *>::iterator iter = list[0].begin(); iter != list[0].end(); iter++)
    expr +=  (*iter)->Get_Name() + " ";
  expr += "<list>\n";

  expr += "  <list> ";
  for (vector<Variable *>::iterator iter = list[1].begin(); iter != list[1].end(); iter++)
    expr +=  (*iter)->Get_Name() + " ";
  expr += "<list>\n";
  expr += "</channel>";
  return expr;
}
