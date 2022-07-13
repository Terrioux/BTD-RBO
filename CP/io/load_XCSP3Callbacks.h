/// \file 
/// \brief Definitions related to the Load_XCSP3Callbacks class

#ifndef LOAD_XCSP3PCALLBACKS_H
#define LOAD_XCSP3PCALLBACKS_H

#include <XCSP3CoreCallbacks.h>
#include <XCSP3Variable.h>
#include <XCSP3Tree.h>
#include "csp.h"

using XCSP3Core::XCSP3CoreCallbacks;
using XCSP3Core::InstanceType;
using XCSP3Core::XVariable;
using XCSP3Core::OrderType;
using XCSP3Core::XTransition;
using XCSP3Core::XCondition;
using XCSP3Core::XInterval;
using XCSP3Core::RankType;
using XCSP3Core::ExpressionObjective;
using XCSP3Core::Tree;


class Load_XCSP3Callbacks : public XCSP3CoreCallbacks     /// This class makes the link between the current library and the XCSP3 parser from xcsp.org \ingroup io
{
	protected:
		CSP * pb;                                    ///< the CSP which is built from the XCSP3 file
		vector<vector<int>> last_tuples;             ///< the last tuple list (if any)
    bool use_classical_extension_constraint;     ///< true if the classical extension constraints are used, false if we use compact extension constraints
    
  public:
		// constructor 
		Load_XCSP3Callbacks(CSP * pb_ref, bool classical_constraint = false);			///< constructor

    void beginInstance(InstanceType type) override;   ///< checks the type of instance

		void buildVariableInteger(string id, int minValue, int maxValue) override;    ///< creates an integer variable whose domain contains the integer values between minValue and Maxvalue
		void buildVariableInteger(string id, vector<int> &values) override;           ///< creates an integer variable whose domain contains the integer values in vector values

		void buildConstraintExtension(string id, vector<XVariable *> list, vector<vector<int>> &tuples, bool support, bool hasStar) override;    ///< creates an extension constraint where list is the scope, tuples the list of tuples which are allowad if support is set to true, forbidden otherwise (hasStar is true if * is allowed in tuples)
		void buildConstraintExtension(string id, XVariable *variable, vector<int> &tuples, bool support, bool hasStar) override;    ///< creates an extension constraint where variable is the single variable in the scope, tuples the list of tuples which are allowad if support is set to true, forbidden otherwise (hasStar is true if * is allowed in tuples)
		void buildConstraintExtensionAs(string id, vector<XVariable *> list, bool support, bool hasStar) override;    ///< creates an extension constraint where list is the scope, the list of tuples which are allowad if support is set to true, forbidden otherwise, is the same as the previous extension constraint (hasStar is true if * is allowed in tuples)

		void buildConstraintIntension(string id, string expr) override;   ///< creates an intension constraint defined by the expression expr represented by a string
    void buildConstraintIntension(string id, Tree *tree) override;    ///< creates an intension constraint defined by the expression tree  represented by a tree

		void buildConstraintPrimitive(string id, OrderType op, XVariable *x, int k, XVariable *y) override;     ///< creates an intension constraint defined by the expression x + k op y
    void buildConstraintPrimitive(string id, OrderType op, XVariable *x, int k) override;   ///< creates an intension constraint defined by the expression x op k where op is <= or >=
    void buildConstraintPrimitive(string id, XVariable *x,  bool in, int min, int max) override;    ///< creates an intension constraint defined by the expression x belongs (resp. does not belong) to [min,max] if in is set to true (resp. false)
                     
		void buildConstraintRegular(string id, vector<XVariable *> &list, string st, vector<string> &final, vector<XTransition> &transitions) override;   ///< creates a regular global constraint
    void buildConstraintMDD(string id, vector<XVariable *> &list, vector<XTransition> &transitions) override;   ///< creates a MDD global constraint

		void buildConstraintAlldifferent(string id, vector<XVariable *> &list) override;      ///< creates an all-diff global constraint
		void buildConstraintAlldifferentExcept(string id, vector<XVariable *> &list, vector<int> &except) override;     ///< creates an all-diff global constraint with an exception value
    void buildConstraintAlldifferent(string id, vector<Tree *> &list) override;   ///< creates an all-diff global constraint over expressions
		//~ void buildConstraintAlldifferentList(string id, vector<vector<XVariable *>> &lists) override;
		void buildConstraintAlldifferentMatrix(string id, vector<vector<XVariable *>> &matrix) override;      ///< creates a matrix version of the all-diff global constraint

		void buildConstraintAllEqual(string id, vector<XVariable *> &list) override;    ///< creates an all-equal global constraint whose scope is provided by list
    void buildConstraintAllEqual(string id, vector<Tree *> &list) override;         ///< creates an all-equal global constraint over expressions
		void buildConstraintNotAllEqual(string id, vector<XVariable *> &list) override; ///< creates a not-all-equal global constraint whose scope is provided by list

    void buildConstraintOrdered(string id, vector<XVariable *> &list, OrderType order) override;    ///< creates an orderded global constraint without lengths
    void buildConstraintOrdered(string id, vector<XVariable *> &list, vector<int> &lengths, OrderType order) override;   ///< creates an orderded global constraint with length

    void buildConstraintLex(string id, vector<vector<XVariable *>> &lists, OrderType order) override;   ///< creates a lex global constraint on a list of variables
    void buildConstraintLexMatrix(string id, vector<vector<XVariable *>> &matrix, OrderType order) override; ///< creates a lex global constraint on a matrix of variables

		void buildConstraintSum(string id, vector<XVariable *> &list, vector<int> &coeffs, XCondition &cond) override;    ///< creates a weighted sum global constraint whose scope is provided by list and for which coeffs corresponds to the list of coefficients and cond is the considered condition
		void buildConstraintSum(string id, vector<XVariable *> &list, XCondition &cond) override;     ///< creates a sum global constraint whose scope is provided by list and for which cond is the considered condition
    void buildConstraintSum(string id, vector<XVariable *> &list, vector<XVariable *> &coeffs, XCondition &cond) override;    ///< creates a weighted sum global constraint whose scope is provided by list and for which coeffs corresponds to the list of variable coefficients and cond is the considered condition
    void buildConstraintSum(string id, vector<Tree *> &list, vector<int> &coeffs, XCondition &cond) override;  ///< creates a weighted sum global constraint on expressions represented by trees and for which coeffs corresponds to the list of coefficients and cond is the considered condition
    void buildConstraintSum(string id, vector<Tree *> &list, XCondition &cond) override;     ///< creates a sum global constraint on expressions represented by trees and for which cond is the considered condition

    void buildConstraintAtMost(string id, vector<XVariable *> &list, int value, int k) override;    ///< creates an at most global constraint

    void buildConstraintAtLeast(string id, vector<XVariable *> &list, int value, int k) override;   ///< creates an at least global constraint

    void buildConstraintExactlyK(string id, vector<XVariable *> &list, int value, int k) override;  ///< creates an exactly global constraint with an integer value k

    void buildConstraintAmong(string id, vector<XVariable *> &list, vector<int> &values, int k) override;   ///< creates an among global constraint

    void buildConstraintExactlyVariable(string id, vector<XVariable *> &list, int value, XVariable *x) override;    ///< creates an exactly global constraint with a variable x

    void buildConstraintCount(string id, vector<XVariable *> &list, vector<int> &values, XCondition &xc) override;  ///< creates a count global constraint
    void buildConstraintCount(string id, vector<Tree*> &trees, vector<int> &values, XCondition &xc) override;       ///< creates a count global constraint over expressions
    //~ void buildConstraintCount(string id, vector<XVariable *> &list, vector<XVariable *> &values, XCondition &xc) override;
    //~ void buildConstraintCount(string id, vector<Tree*> &trees, vector<XVariable *> &values, XCondition &xc) override;

    void buildConstraintNValues(string id, vector<XVariable *> &list, vector<int> &except, XCondition &xc) override;    ///< creates a nvalue global constraint with exception
    void buildConstraintNValues(string id, vector<XVariable *> &list, XCondition &xc) override;         ///< creates a nvalue global constraint
    void buildConstraintNValues(string id, vector<Tree *> &trees, XCondition &xc) override; ///< creates a nvalue global constraint over expressions

    void buildConstraintCardinality(string id, vector<XVariable *> &list, vector<int> values, vector<int> &occurs, bool closed) override;   ///< creates a cardinality global constraint where objectives are integer
    void buildConstraintCardinality(string id, vector<XVariable *> &list, vector<int> values, vector<XVariable *> &occurs, bool closed) override;   ///< creates a cardinality global constraint where objectives are variables
    void buildConstraintCardinality(string id, vector<XVariable *> &list, vector<int> values, vector<XInterval> &occurs, bool closed) override;   ///< creates a cardinality global constraint where objectives are intervals
    //~ void buildConstraintCardinality(string id, vector<XVariable *> &list, vector<XVariable *> values, vector<int> &occurs, bool closed) override;
    //~ void buildConstraintCardinality(string id, vector<XVariable *> &list, vector<XVariable *> values, vector<XVariable *> &occurs, bool closed) override;
    //~ void buildConstraintCardinality(string id, vector<XVariable *> &list, vector<XVariable *> values, vector<XInterval> &occurs, bool closed) override;

    void buildConstraintMinimum(string id, vector<XVariable *> &list, XCondition &xc) override;   ///< creates a minimum global constraint
    void buildConstraintMinimum(string id, vector<Tree *> &list, XCondition &xc) override;    ///< creates a minimum global constraint over expressions
    //~ void buildConstraintMinimum(string id, vector<XVariable *> &list, XVariable *index, int startIndex, RankType rank, XCondition &xc) override;

    void buildConstraintMaximum(string id, vector<XVariable *> &list, XCondition &xc) override;   ///< creates a maximum global constraint
    void buildConstraintMaximum(string id, vector<Tree *> &list, XCondition &xc) override;    ///< creates a maximum global constraint over expressions
    //~ void buildConstraintMaximum(string id, vector<XVariable *> &list, XVariable *index, int startIndex, RankType rank, XCondition &xc) override;

		void buildConstraintElement(string id, vector<XVariable *> &list, int value) override;            ///< creates an element global constraint whose scope is provided by list with a constant value provided by value
		void buildConstraintElement(string id, vector<XVariable *> &list, XVariable *value) override;     ///< creates an element global constraint whose scope is provided by list with a variable value provided by value
		void buildConstraintElement(string id, vector<XVariable *> &list, int startIndex, XVariable *index, RankType rank, int value) override;   ///< creates a element global constraint whose scope is provided by list with a constant value provided by value and index as a index variable 
		void buildConstraintElement(string id, vector<XVariable *> &list, int startIndex, XVariable *index, RankType rank, XVariable *value) override;    ///< creates an element global constraint whose scope is provided by list with a variable value provided by value and index as a index variable 
    void buildConstraintElement(string id, vector<int> &list, int startIndex, XVariable *index, RankType rank, XVariable *value) override;    ///< creates an element global constraint on a list of integers with a variable value provided by value and index as a index variable 

    void buildConstraintElement(string id, vector<XVariable *> &list, XVariable *index, int startIndex, XCondition &xc)  override;  ///< creates an element global constraint to which a condition is imposed

    void buildConstraintElement(string id, vector<vector<XVariable*> > &matrix, int startRowIndex, XVariable *rowIndex, int startColIndex, XVariable* colIndex, XVariable* value) override;   ///< creates an element global constraint whose scope is a matrix 
    void buildConstraintElement(string id, vector<vector<int> > &matrix, int startRowIndex, XVariable *rowIndex, int startColIndex, XVariable* colIndex, XVariable *value) override;  ///< creates an element global constraint whose scope is a matrix 
 
    void buildConstraintCumulative(string id, vector<XVariable *> &origins, vector<int> &lengths, vector<int> &heights, XCondition &xc) override;                 ///< creates a cumulative global constraint for which the lengths and the heights are integers
    void buildConstraintCumulative(string id, vector<XVariable *> &origins, vector<int> &lengths, vector<XVariable *> &varHeights, XCondition &xc) override;      ///< creates a cumulative global constraint for which the lengths are integers and the heights variables
    void buildConstraintCumulative(string id, vector<XVariable *> &origins, vector<XVariable *> &lengths, vector<int> &heights, XCondition &xc) override;         ///< creates a cumulative global constraint for which the lengths are variable and the heights integers
    void buildConstraintCumulative(string id, vector<XVariable *> &origins, vector<XVariable *> &lengths, vector<XVariable *> &heights, XCondition &xc) override; ///< creates a cumulative global constraint for which the lengths and the heights are variables
    
    void buildConstraintCumulative(string id, vector<XVariable *> &origins, vector<int> &lengths, vector<int> &heights, vector<XVariable *> &ends, XCondition &xc) override;                  ///< creates a cumulative global constraint for which the lengths and the heights are integers
    void buildConstraintCumulative(string id, vector<XVariable *> &origins, vector<int> &lengths, vector<XVariable *> &varHeights, vector<XVariable *> &ends,XCondition &xc) override;        ///< creates a cumulative global constraint for which the lengths are integers and the heights variables
    void buildConstraintCumulative(string id, vector<XVariable *> &origins, vector<XVariable *> &lengths, vector<int> &heights, vector<XVariable *> &ends, XCondition &xc) override;          ///< creates a cumulative global constraint for which the lengths are variable and the heights integers
    void buildConstraintCumulative(string id, vector<XVariable *> &origins, vector<XVariable *> &lengths, vector<XVariable *> &heights, vector<XVariable *> &ends, XCondition &xc) override;  ///< creates a cumulative global constraint for which the lengths and the heights are variables

    
    void buildConstraintChannel(string id, vector<XVariable *> &list, int startIndex) override;   ///< creates a channel global constraint on a list of variables
    void buildConstraintChannel(string id, vector<XVariable *> &list1, int startIndex1, vector<XVariable *> &list2, int startIndex2) override;    ///< creates a channel global constraint on two lists of variables
    void buildConstraintChannel(string id, vector<XVariable *> &list, int startIndex, XVariable *value) override;   ///< creates a channel global constraint on Boolean variables and a value variable

    //~ void buildConstraintStretch(string id, vector<XVariable *> &list, vector<int> &values, vector<XInterval> &widths) override;
    //~ void buildConstraintStretch(string id, vector<XVariable *> &list, vector<int> &values, vector<XInterval> &widths, vector<vector<int>> &patterns) override;

    void buildConstraintNoOverlap(string id, vector<XVariable *> &origins, vector<int> &lengths, bool zeroIgnored) override;            ///< creates a no-overlap global constraint
    void buildConstraintNoOverlap(string id, vector<XVariable *> &origins, vector<XVariable *> &lengths, bool zeroIgnored) override;    ///< creates a no-overlap global constraint for which the lengths are variable 
    void buildConstraintNoOverlap(string id, vector<vector<XVariable *>> &origins, vector<vector<int>> &lengths, bool zeroIgnored) override;            ///< creates a k-dimensional no-overlap global constraint
    void buildConstraintNoOverlap(string id, vector<vector<XVariable *>> &origins, vector<vector<XVariable *>> &lengths, bool zeroIgnored) override;    ///< creates a k-dimensional no-overlap global constraint for which the lengths are variable

    void buildConstraintInstantiation(string id, vector<XVariable *> &list, vector<int> &values) override;    ///< creates an instantiation constraint

    //~ void buildConstraintClause(string id, vector<XVariable *> &positive, vector<XVariable *> &negative) override ;

    void buildConstraintCircuit(string id, vector<XVariable *> &list, int startIndex) override;   ///< creates a circuit constraint
    //~ void buildConstraintCircuit(string id, vector<XVariable *> &list, int startIndex, int size) override;
    //~ void buildConstraintCircuit(string id, vector<XVariable *> &list, int startIndex, XVariable *size) override;
        
    //~ void buildConstraintPrecedence(string id, vector<XVariable *> &list, vector<int> values) override;
    //~ void buildConstraintBinPacking(string id, vector<XVariable *> &list, vector<int> &sizes, XCondition &cond) override;
    //~ void buildConstraintFlow(string id, vector<XVariable *> &list, vector<int> &balance, vector<int> &weights, vector<vector<int> > &arcs, XCondition &xc) override;
    //~ void buildConstraintKnapsack(string id, vector<XVariable *> &list, vector<int> &weights, vector<int> &profits, int limit, XCondition &xc) override;
    //~ void buildConstraintKnapsack(string id, vector<XVariable *> &list, vector<int> &weights, vector<int> &profits, XVariable *limit, XCondition &xc) override;


        
		void buildObjectiveMinimizeExpression(string expr) override;    ///< creates a minimum objective based on an expression
		void buildObjectiveMaximizeExpression(string expr) override;    ///< creates a maximum objective based on an expression

		void buildObjectiveMinimizeVariable(XVariable *x) override;     ///< creates a minimum objective based on a variable
		void buildObjectiveMaximizeVariable(XVariable *x) override;     ///< creates a maximum objective based on a variable

		void buildObjectiveMinimize(ExpressionObjective type, vector<XVariable *> &list, vector<int> &coefs) override;    ///< creates a minimum objective based on a weighted function
		void buildObjectiveMaximize(ExpressionObjective type, vector<XVariable *> &list, vector<int> &coefs) override;    ///< creates a maximum objective based on a weighted function

		void buildObjectiveMinimize(ExpressionObjective type, vector<XVariable *> &list) override;   ///< creates a minimum objective based on a function
		void buildObjectiveMaximize(ExpressionObjective type, vector<XVariable *> &list) override;   ///< creates a maximum objective based on a function

    void buildObjectiveMinimize(ExpressionObjective type, vector<Tree *> &trees, vector<int> &coefs) override;    ///< creates a minimum objective based on a weighted function and expressions
    void buildObjectiveMaximize(ExpressionObjective type, vector<Tree *> &trees, vector<int> &coefs) override;    ///< creates a maximum objective based on a weighted function and expressions
    void buildObjectiveMinimize(ExpressionObjective type, vector<Tree *> &trees) override;    ///< creates a minimum objective based on a weighted function and expressions
    void buildObjectiveMaximize(ExpressionObjective type, vector<Tree *> &trees) override;    ///< creates a maximum objective based on a weighted function and expressions

    //~ void buildAnnotationDecision(vector<XVariable*> &list) override;
};



#endif
