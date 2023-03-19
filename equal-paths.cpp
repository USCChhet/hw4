#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>
#endif

#include "equal-paths.h"

using namespace std;

int getHeight(Node* curr)
{
	int heightL = -1;
	int heightR = -1;

	if(curr == NULL)
	{
		return 0;
	}

	if(curr->left != NULL)
	{
		heightL = getHeight(curr->left);
	}

	if(curr->right != NULL)
	{
		heightR = getHeight(curr->right);
	}

	return  1 + std::max(heightL, heightR);
}

bool dopaths(Node* root)
{
    int right = getHeight(root->right);
    int left = getHeight(root->left);

    // Add your code below
    if(right != left)
    {
        return false;
    }

   //Check if there are subtrees under us
	if(right == left)
	{
		equalPaths(root->left);
		equalPaths(root->right);
	}
	
	//If all nodes are balanced return true!
	return true;
}

bool equalPaths(Node* root)
{
    if(root == NULL)
    {
        return true;
    }

    return dopaths(root);
}





