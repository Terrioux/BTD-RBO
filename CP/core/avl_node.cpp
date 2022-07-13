/// \file 
/// \brief Sources related to the AVL_Node class

#include "avl_node.h"
#include <iostream>

using namespace std;


//-----------------------------
// constructors and destructor 
//-----------------------------


AVL_Node::AVL_Node (int * val, int arity, AVL_Node *** & tuple_list)
// constructs a node with no child and a value val
/// \param[in] val the tuple associated to the node
/// \param[in] arity the arity of the node
/// \param[in,out] tuple_list the current list of tuples
{
	value = new int [arity];
	next = new AVL_Node * [arity];
	prev = new AVL_Node * [arity];
	for (int i = 0; i < arity; i++)
	{
		value[i] = val[i];
		next[i] = tuple_list[i][val[i]];
		prev[i] = 0;
		if (tuple_list[i][val[i]] != 0)
			tuple_list[i][val[i]]->prev[i] = this;
		tuple_list[i][val[i]] = this;
	}
	
	imbalance = 0;
	lchild = 0;
	rchild = 0;	
}

		
AVL_Node::~AVL_Node ()
// destructor
{
	delete [] value;
	delete [] next;
	delete [] prev;
	delete lchild;
	delete rchild;
}


//-----------------
// basic functions 
//-----------------
	
	
int Compare_Values (int * e1, int * e2, int n)
/// compare two tuples e1 and e2 of size n and returns 0 if e1 == e2, 1 if e1 < e2, -1 otherwise
/// \param[in] e1 the first tuple to compare
/// \param[in] e2 the second tuple to compare
/// \param[in] n the arity of both tuples
{
  int i;

  i = 0;

  while ((i < n) && (e1[i] == e2[i]))
    i++;

  if (i == n)  return 0;
  else 
    if (e1[i] < e2[i])  return 1;
    else return -1;
}


bool AVL_Node::Is_Present (int * val, int arity)
// returns true if the element val belongs to the AVL, false otherwise
/// \param[in] val the tuple we look for
/// \param[in] arity the arity of this tuple
{
	AVL_Node * p;
  int comp;
  
  p = this;

  while ((p != 0) && (comp = Compare_Values (val,p->value,arity)))
    if (comp == 1)  p = p->lchild; 
    else p = p->rchild;

  if (p == 0) return false;
  else return true; 
}


AVL_Node * AVL_Node::Get_Node (int * val, int arity)
// returns a pointer on the node containing the element val if it belongs to the AVL, 0 otherwise
/// \param[in] val the tuple we look for
/// \param[in] arity the arity of this tuple
{
	AVL_Node * p;
  int comp;
  
  p = this;

  while ((p != 0) && (comp = Compare_Values (val,p->value,arity)))
    if (comp == 1)  p = p->lchild; 
    else p = p->rchild;

  return p; 
}


bool AVL_Node::Add_Element (AVL_Node * & A, int * val, int arity, AVL_Node *** & tuple_list)
// adds the element val into the AVL (if it does not belong to the AVL yet)
/// \param[in,out] A the AVL in which we want to add the tuple val
/// \param[in] val the tuple we want to add
/// \param[in] arity the arity of the tuple
/// \param[in,out] tuple_list the current list of tuples
{
	bool H;  // true if the imbalance has increased, false otherwise
	
	if (A == 0)  // we insert the new value into a leaf */
	{ 
		A = new AVL_Node (val,arity,tuple_list);
		
		return true;
	} 
	else
	{   
		switch (Compare_Values (val,A->value,arity))
		{
			case 1 :   // e < root->value
					H = Add_Element (A->lchild,val,arity,tuple_list);
					if (H)   // if the height has increased 
					{
						if (A->imbalance == -1) 
						{
							if (Compare_Values (val,A->lchild->value,arity) == 1)
							{ 
								// case left-left
								AVL_Node * B;

								B = A->lchild;
								A->lchild = B->rchild;
								B->rchild = A;  
								A->imbalance = 0;
								A = B; 
								A->imbalance = 0;
							}
							else
								{ 
									// cas left-right
									AVL_Node *A1, *A2;
							
									A1 = A->lchild;
									A2 = A1->rchild;
									A1->rchild = A2->lchild;
									A2->lchild = A1;
									A->lchild = A2->rchild;
									A2->rchild = A;
									if (A2->imbalance == -1)  A->imbalance = 1;
									else A->imbalance = 0;

									if (A2->imbalance == 1)  A1->imbalance = -1;
									else A1->imbalance = 0; 

									A = A2;
									A->imbalance = 0;
								}     
							return false;
						}
						else 
							if (A->imbalance == 1) 
							{ 
								A->imbalance = 0; 
								return false;
							}
							else 
								if (A->imbalance == 0) 
								{
									A->imbalance = -1; 
									return true;
								}
					} 
					else return false; 
					break;  
		case 0 : return false;
		case -1: 
				H = Add_Element (A->rchild,val,arity,tuple_list);
				if (H) // if the heigth has increased
				{
					if (A->imbalance == 1) 
					{
						if (Compare_Values (A->rchild->value,val,arity) == 1)
						{ 
							// case right-right
							AVL_Node * B;

							B = A->rchild;
							A->rchild = B->lchild;
							B->lchild = A;  
							A->imbalance = 0;
							A = B; 
							A->imbalance = 0;
						}
						else
							{ 
								// case right-left
								AVL_Node *A1, *A2;
						
								A1 = A->rchild;
								A2 = A1->lchild;
								A1->lchild = A2->rchild;
								A2->rchild = A1;
								A->rchild = A2->lchild;
								A2->lchild = A;
								if (A2->imbalance == 1)  A->imbalance = -1;
								else A->imbalance = 0;

								if (A2->imbalance == -1)  A1->imbalance = 1;
								else A1->imbalance = 0; 
	
								A = A2;
								A->imbalance = 0;
							}
						return false;
					}  
					else 
						if (A->imbalance == -1) 
						{ 
							A->imbalance = 0; 
							return false;
						}
						else 
							if (A->imbalance == 0) 
							{
								A->imbalance = 1; 
								return true;
							}
				}
				else return false;
				break; 
		}
	}
	return false;
}



bool AVL_Node::Balance_Left (AVL_Node * & A)
// rebalances the AVL after a deletion in the left child
/// \param[in,out] A the AVL we want to rebalance
{
	if (A->imbalance == -1) 
	{
		A->imbalance = 0;
		return true;
	}
	else
		if (A->imbalance == 0) 
		{
			A->imbalance = 1;
			return false;
		}
		else
			{
				AVL_Node * A1;
				A1 = A->rchild;
				int imb1 = A1->imbalance;
				
				if (imb1 >= 0)
				{ 
					// case right-right
					A->rchild = A1->lchild;
					A1->lchild = A;

					if (imb1 == 0)
					{
						A->imbalance = 1;
						A1->imbalance = -1;
						A = A1;
						return false;
					}
					else
						{
							A->imbalance = 0;
							A1->imbalance = 0;
							A = A1;
							return true;
						}
				}
				else
					{ 
						// cas right-left
						AVL_Node *A2;
						A2 = A1->lchild;
						int imb2 = A2->imbalance;
						
						A1->lchild = A2->rchild;
						A2->rchild = A1;
						A->rchild = A2->lchild;
						A2->lchild = A;
						
						if (imb2 == 1)
							A->imbalance = -1;
						else A->imbalance = 0;
						
						if (imb2 == -1)
							A1->imbalance = 1;
						else A1->imbalance = 0;
						
						A = A2;
						A->imbalance = 0;
						
						return true;
					}     
			}
}



bool AVL_Node::Balance_Right (AVL_Node * & A)
// rebalances the AVL after a deletion in the right child
/// \param[in,out] A the AVL we want to rebalance
{
	if (A->imbalance == 1) 
	{
		A->imbalance = 0;
		return true;
	}
	else
		if (A->imbalance == 0)
		{
			A->imbalance = -1;
			return false;
		}
		else
			{
				AVL_Node * A1;
				A1 = A->lchild;
				int imb1 = A1->imbalance;
				
				if (imb1 <= 0)
				{
					// case left-left
					A->lchild = A1->rchild;
					A1->rchild = A;
					
					if (imb1 == 0)
					{
						A->imbalance = -1;
						A1->imbalance = 1;
						A = A1;
						return false;
					}
					else
						{
							A->imbalance = 0;
							A1->imbalance = 0;
							A = A1;
							return true;
						}
				}
				else
					{
						// case left-right
						AVL_Node * A2;
						A2 = A1->rchild;
						int imb2 = A2->imbalance;
						
						A1->rchild = A2->lchild;
						A2->lchild = A1;
						A->lchild = A2->rchild;
						A2->rchild = A;
						
						if (imb2 == -1)
							A->imbalance = 1;
						else A->imbalance = 0;
						
						if (imb2 == 1)
							A1->imbalance = -1;
						else A1->imbalance = 0;
						
						A = A2;
						A->imbalance = 0;
						
						return true;
					}
			}	
}


bool AVL_Node::Remove_Element (AVL_Node * & A, int * val, int arity, AVL_Node *** & tuple_list)
// removes the element val into the tree pointed by A (if it belongs)
/// \param[in,out] A the AVL from which we want to remove the tuple val
/// \param[in] val the tuple we want to remove
/// \param[in] arity the arity of the tuple
/// \param[in,out] tuple_list the current list of tuples
{
	bool H;  // true if the imbalance has decreased, false otherwise
	
	if (A == 0)
		return false;
	else
	{   
		switch (Compare_Values (val,A->value,arity))
		{
			case 1 :   // e < root->value
					H = Remove_Element (A->lchild,val,arity,tuple_list);
					
					if (H)   // if the height has decreased 
						return Balance_Left (A);
					else return false;
					break;
			case 0: 
					if (A->lchild == 0)
					{
						// we update the double link chains
						for (int i = 0; i < arity; i++)
						{
							if (A->prev[i] == 0)
								tuple_list[i][val[i]] = A->next[i];
							else A->prev[i]->next[i] = A->next[i];
							
							if (A->next[i] != 0)
								A->next[i]->prev[i] = A->prev[i];
						}
						
						// A has at most a single child
						AVL_Node * B = A;
						
						A = A->rchild;
						
						B->lchild = 0;
						B->rchild = 0;
						delete B;

						return true;
					}
					else
						{
							if (A->rchild == 0)
							{
								// we update the double link chains
								for (int i = 0; i < arity; i++)
								{
									if (A->prev[i] == 0)
										tuple_list[i][val[i]] = A->next[i];
									else A->prev[i]->next[i] = A->next[i];
									
									if (A->next[i] != 0)
										A->next[i]->prev[i] = A->prev[i];
								}
								
								
								// A has a single child
								AVL_Node * B = A;
								
								A = A->lchild;
								
								B->lchild = 0;
								B->rchild = 0;
								delete B;
								
								return true;
							}
							else
								{
									// A has two children
									
									// we look for the largest value in the left subtree
									AVL_Node * B = A->lchild;
									while (B->rchild != 0)
										B = B->rchild;
										
									// we replace the value of A by one of B and we update the double link chains
									for (int i = 0; i < arity; i++)
									{
										// we skip the node A in the i-th double link chain
										if (A->prev[i] == 0)
											tuple_list[i][val[i]] = A->next[i];
										else A->prev[i]->next[i] = A->next[i];
									
										if (A->next[i] != 0)
											A->next[i]->prev[i] = A->prev[i];

										// we replace the i-th part of the value
										A->value[i] = B->value[i];
										
										// we add the new A to the i-th double link chain
										A->next[i] = tuple_list[i][val[i]];
										A->prev[i] = 0;
										if (tuple_list[i][val[i]] != 0)
											tuple_list[i][val[i]]->prev[i] = A;
										tuple_list[i][val[i]] = A;
										
										val[i] = B->value[i];
									}

									H = Remove_Element (A->lchild,val,arity,tuple_list);

									if (H)   // if the height has decreased 
										return Balance_Left (A);
									else return false;
								}
						}
		case -1: 
					H = Remove_Element (A->rchild,val,arity,tuple_list);
					if (H) // if the heigth has decreased
						return Balance_Right (A);
					else return false;
					break; 
		}
	}
	return false;
}


void AVL_Node::Print (int arity)
// prints the content of the AVL
/// \param[in] arity the arity of the tuples in the AVL
{
	if (lchild != 0)  lchild->Print(arity);
	
	cout << "(";
	for (int i = 0; i < arity; i++)
		cout << value[i] << ",";
	cout << ")" << endl;
	
	if (rchild != 0)  rchild->Print(arity);
}


int AVL_Node::Height ()
// returns the height of the subtree rooted in the current node
{
	int lh,rh;
	if (lchild == 0) lh = 0;
	else lh = lchild->Height();

	if (rchild == 0) rh = 0;
	else rh = rchild->Height();

	if (lh < rh) return rh+1;
	else return lh+1;
}
