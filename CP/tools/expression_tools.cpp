#include "expression_tools.h"

unsigned long compute_allowed_tuple_number_rec (vector<Variable *> & var, int arity, Expression & e, int i, int real_t [])
// computes recursively the number of tuples of the Cartesian product of the domains of the variables in var which are allowed by the expression e
{
	if (i == arity)
	{
		if (e.Evaluate (real_t) != 0) return 1;
		else return 0;
	}
	else
		{
			int sum = 0;
			for (unsigned int j = 0; j < var[i]->Get_Domain()->Get_Initial_Size(); j++)
			{
				real_t [i] = var[i]->Get_Domain()->Get_Real_Value (j);
				sum += compute_allowed_tuple_number_rec (var,arity,e,i+1,real_t);
			}
			return sum;
		}
}


unsigned long compute_allowed_tuple_number (vector<Variable *> & var, int arity, Expression & e)
// returns the number of tuples of the Cartesian product of the domains of the variables in var which are allowed by the expression e
{
	int real_t [arity];
	
	return compute_allowed_tuple_number_rec (var,arity, e, 0, real_t);
}




string extract_term (string expression, unsigned int & pos)
// returns the term located from the position pos
{
	string term;
	while ((pos < expression.length()) && (expression[pos] != '(') && (expression[pos] != ',') && (expression[pos] != ')'))
	{
		term.push_back (expression[pos]);
		pos++;
	}
	
	return term;
}


bool is_operator (string term, operator_type & op)
// returns true if the string term corresponds to an operator (if so op will contain this operator), false otherwise
{
  if (term.compare ("neg") == 0)  op = NEG;
  else
    if (term.compare ("abs") == 0)  op = ABS;
    else
      if (term.compare ("add") == 0)  op = ADD;
      else
        if (term.compare ("sub") == 0)  op = SUB;
        else
          if (term.compare ("mul") == 0)  op = MUL;
          else
            if (term.compare ("div") == 0)  op = DIV;
            else
              if (term.compare ("mod") == 0)  op = MOD;
              else
                if (term.compare ("pow") == 0)  op = POW;
                else
                  if (term.compare ("min") == 0)  op = MIN;
                  else
                    if (term.compare ("max") == 0)  op = MAX;
                    else
                      if (term.compare ("eq") == 0)  op = EQ;
                      else
                        if (term.compare ("ne") == 0)  op = NE;
                        else
                         if (term.compare ("ge") == 0)  op = GE;
                         else
                           if (term.compare ("gt") == 0)  op = GT;
                           else
                             if (term.compare ("le") == 0)  op = LE;
                             else
                               if (term.compare ("lt") == 0)  op = LT;
                               else
                                 if (term.compare ("not") == 0)  op = NOT;
                                 else
                                   if (term.compare ("and") == 0)  op = AND;
                                   else
                                     if (term.compare ("or") == 0)  op = OR;
                                     else
                                       if (term.compare ("xor") == 0)  op = XOR;
                                       else
                                         if (term.compare ("iff") == 0)  op = IFF;
                                         else
                                           if (term.compare ("if") == 0)  op = IF;
																					 else
																						 if (term.compare ("dist") == 0)  op = DIST;
																						 else
                                               if (term.compare ("sqr") == 0)  op = SQR;
                                               else
                                                 if (term.compare ("imp") == 0)  op = IMP;
                                                 else
                                                   if (term.compare ("set") == 0)  op = SET;
                                                   else
                                                     if (term.compare ("in") == 0)  op = IN;
                                                     else return false;
  return true;
}


Expression string_to_expression (string expression, unsigned int & pos, CSP * pb, vector<Variable *> & scope)
// reads a functional predicate in the file whose descriptor is file and returns the corresponding expression
{
	//~ int init = pos;
	
	string term;

	term = extract_term (expression,pos);

	if ((isdigit(term[0])) || (term[0] == '-'))
	{
		// the current expression is a value
		return Expression (VAL,stoi(term));
	}
	else
		{
			// the current expression corresponds to a variable or an operator
			
			operator_type op;
			if (is_operator (term,op))
			{
				// the expression corresponds to an operator
				
				vector<Expression> subexpr;
				pos++;
				
				while (expression[pos] != ')')
				{
					subexpr.push_back (string_to_expression(expression,pos,pb,scope));
					
					if (expression[pos] == ',')
						pos++;
				}
				pos++;
				
				if (subexpr.size() > 2)
				{
					// if the operator has more than two operands, we replace some operators by their n-ary version
					switch (op)
					{
						case ADD: op = SUM; break;
						case MUL: op = PROD; break;
						case EQ : op = EQU; break;
						default: ;		// for avoiding warning
					}
				}
				
				return Expression (op,subexpr);
			}
			else
				{
					// the expression is the name of a variable
					unsigned int i = 0;
					while ((i < scope.size()) && (scope[i]->Get_Name().compare (term) != 0))
						i++;
					
					if (i == scope.size())
						scope.push_back (pb->Get_Variable(term));
						
					return Expression (VAR,i);
				}
		}
}


Expression  string_to_expression (string expression, CSP * pb, vector<Variable *> & scope)
// converts a string to an expression
{
	unsigned int pos = 0;
	return string_to_expression (expression, pos, pb, scope);
}
