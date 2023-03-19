#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
  public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

  protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
  return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
  balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
  balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
  return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
  return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
  return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
  public:
    AVLTree();
    ~AVLTree()
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
  
  protected:

    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    
    // Add helper functions here
    void insertFix(AVLNode<Key, Value>& parent, AVLNode<Key, Value>& child);
    void removeFix(AVLNode<Key, Value>& node, int diff);
    void rotateRight(AVLNode<Key, Value>& node);
    void rotateLeft(AVLNode<Key, Value>& node);
    bool has2Children(AVLNode<Key, Value>& node);
  
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
*/

//constructor
template<class Key, class Value>
BinarySearchTree<Key, Value>::BinarySearchTree() : root_(NULL) {}

//destructor
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
  // TODO
  clear();
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  // TODO
  if(root_ == NULL)
  {
    root_ = new_item;
    root_->setBalance(0);
  }
  else
  {
    AVLNode<Key, Value>* temp = _root;

    //logic of where to insert
    while(temp->getKey() != NULL)
    {
      //equal to
      if( keyValuePair->getKey() == temp->getKey())
      {
        break;
      }
      
      //less than
      else if( keyValuePair->getKey() < temp->getKey())
      {
        temp = temp->getLeft();
      }

      //greater than
      else
      {
        temp = temp->getRight();
      }
    }

    //equal to, and must replace value, everything else same
    if(keyValuePair->getKey() == temp->getKey())
    {
      temp->setValue(keyValuePair->getValue());
    }

    //less than and left child
    else if(keyValuePair->getKey() < temp->getParent->getKey())
    {
      //set child
      temp->setLeft(keyValuePair);

      //set parent
      keyValuePair->setParent(temp);

      //update new node balance
      keyValuePair->setBalance(0);

      //update parent node balance
      temp->updateBalance(-1);

      //check balance of parent
      if(temp->getBalance() != 0)
      {
        insertFix(temp, keyValuePair);
      }

    }
    
    //greater than and right children
    else
    {
      //set child
      temp->setRight(keyValuePair);

      //set parent
      keyValuePair->setParent(temp);

      //update new node balance
      keyValuePair->setBalance(0);

      //update parent node balance
      temp->updateBalance(1);

      //check balance of parent
      if(temp->getBalance() != 0)
      {
        insertFix(temp, keyValuePair);
      }
    }
  }
}

//my helper function
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>& parent, AVLNode<Key, Value>& child)
{
  if(parent == NULL || child == NULL)
  {
    return;
  }

  //initialize grandParent node
  AVLNode<Key, Value>* grandParent = parent->getParent();
  
  //p is left child of gp
  if(grandParent->getLeft() == parent)
  {
    //update grandParent balance
    grandParent->updateBalance(-1);

    //0 balance
    if(grandParent->getBalance() == 0)
    {
      return;
    }

    //1 balance 
    else if(grandParent->getBalance() == -1)
    {
      insertFix(grandParent, parent);
    }

    //2 balance
    else
    {
      //zig-zig, left left
      if((child == parent->getLeft()) && (parent == grandParent->getLeft()))
      {
        rotateRight(grandParent);
        parent->setBalance(0);
        grandParent->setBalance(0);
      }

      //zig-zag
      else
      {
        rotateLeft(parent);
        rotateRight(grandParent);
        
        if(node->getBalance() == -1)
        {
          parent->setBalance(0);
          grandParent->setBalance(1);
          node->setBalance(0);
        }

        else if(node->getBalance() == 0)
        {
          parent->setBalance(0);
          grandParent->setBalance(0);
          node->setBalance(0);
        }

        else
        {
          parent->setBalance(-1);
          grandParent->setBalance(0);
          node->setBalance(0);
        }
      }
    }
  }
  
  //p is right child of gp
  else if(grandParent->getRight() == parent)
  {
    //update grandParent balance
    grandParent->updateBalance(1);

    //0 balance
    if(grandParent->getBalance() == 0)
    {
      return;
    }

    //1 balance 
    else if(grandParent->getBalance() == 1)
    {
      insertFix(grandParent, parent);
    }

    //2 balance
    else
    {
      //zig-zig, left left
      if((child == parent->getRight()) && (parent == grandParent->getRight()))
      {
        rotateLeft(grandParent);
        parent->setBalance(0);
        grandParent->setBalance(0);
      }

      //zig-zag
      else
      {
        rotateRight(grandParent);
        rotateLeft(parent);

        if(node->getBalance() == 1)
        {
          parent->setBalance(0);
          grandParent->setBalance(-1);
          node->setBalance(0);
        }

        else if(node->getBalance() == 0)
        {
          parent->setBalance(0);
          grandParent->setBalance(0);
          node->setBalance(0);
        }

        else
        {
          parent->setBalance(1);
          grandParent->setBalance(0);
          node->setBalance(0);
        }
      }
    }
  }
}

//my helper function
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>& node)
{
  AVLNode<Key, Value>* parent = node->getParent();
  AVLNode<Key, Value>* right = node->getRight();
  AVLNode<Key, Value>* left = node->getLeft();

  parent->setLeft(right);
  node->setParent(right);

  //move up scenario
  if(right->getLeft() != NULL)
  {
    node->setRight(right->getLeft());
  }

  right->setParent(parent);
  right->setLeft(node);
}

//my helper function
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>& node)
{
  AVLNode<Key, Value>* parent = node->getParent();
  AVLNode<Key, Value>* right = node->getRight();
  AVLNode<Key, Value>* left = node->getLeft();

  parent->setRight(left);
  node->setParent(left);

  //move up scenario
  if(left->getRight() != NULL)
  {
    node->setLeft(left->getRight());
  }

  left->setParent(parent);
  left->setRight(node);
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  AVLNode<Key, Value>* node = internalFind(key);
  int diff = 0;
  
  if(has2Children(node) == true)
  {
    nodeSwap(node, node->getParent());
  }

  AVLNode<Key, Value>* parent = node->getParent();

  if(parent != NULL)
  {
    //right subtree will increase in height
    if(node == parent->getLeft())
    {
      diff = 1;
    }

    //left subtree will increase in height
    else
    {
      diff = -1;
    }
  }

  //delete node and update pointers
  delete node;
  //idk how to update pointers

  //patch up tree
  removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>& node, int diff)
{
  if(node == NULL)
  {
    return;
  }

  AVLNode<Key, Value>* parent = node->getParent();
  int ndiff = 0;


  if(parent != NULL)
  {
    if(node == parent->getLeft())
    {
      ndiff = 1;
    }

    else
    {
      ndiff = -1;
    }
  }

  if(diff == -1)
  {
    if(node->getBalance() + diff == -2)
    {
      //initialize tallChild to be left child, and change if it isnt true
      AVLNode<Key, Value>* tallChild = node->getLeft();
      if(getNodeHeight(node->left) < getNodeHeight(node->right))
      {
        tallChild = node->getRight();
      }

      //zig-zig 1
      if(tallChild->getBalance() == -1)
      {
        rotateRight(node);
        node->setBalance(0);
        tallChild->setBalance(0);
        removeFix(parent, ndiff);
      }

      //zig-zig 2
      if(tallChild->getBalance() == 0)
      {
        rotateRight(node);
        node->setBalance(-1);
        tallChild->setBalance(1);
      }

      //zig-zag
      else
      {
        //initialize grandParent
        AVLNode<Key, Value>* grandParent = tallChild->getRight();
        rotateLeft(tallChild);
        rotateRight(node);

        if(grandParent->getBalance() == 1)
        {
          node->setBalance(0);
          tallChild->setBalance(-1);
          grandParent->setBalance(0);
        }

        else if(grandParent->getBalance() == 0)
        {
          node->setBalance(0);
          tallChild->setBalance(0);
          grandParent->setBalance(0);
        }

        else if(grandParent->getBalance() == -1)
        {
          node->setBalance(1);
          tallChild->setBalance(0);
          grandParent->setBalance(0);
        }

        removeFix(parent, ndiff);
      }
    }

    else if(node->getBalance() + diff == -1)
    {
      node->setBalance(-1);
    }

    else
    {
      node->setBalance(0);
      removeFix(parent, ndiff);
    }
  }

  else
  {
    if(node->getBalance() + diff == 2)
    {
      //initialize tallChild to be left child, and change if it isnt true
      AVLNode<Key, Value>* tallChild = node->getLeft();
      if(getNodeHeight(node->left) < getNodeHeight(node->right))
      {
        tallChild = node->getRight();
      }

      //zig-zig 1
      if(tallChild->getBalance() == 1)
      {
        rotateLeft(node);
        node->setBalance(0);
        tallChild->setBalance(0);
        removeFix(parent, ndiff);
      }

      //zig-zig 2
      if(tallChild->getBalance() == 0)
      {
        rotateLeft(node);
        node->setBalance(1);
        tallChild->setBalance(-1);
      }

      //zig-zag
      else
      {
        //initialize grandParent
        AVLNode<Key, Value>* grandParent = tallChild->getRight();
        rotateRight(tallChild);
        rotateLeft(node);

        if(grandParent->getBalance() == -1)
        {
          node->setBalance(0);
          tallChild->setBalance(1);
          grandParent->setBalance(0);
        }

        else if(grandParent->getBalance() == 0)
        {
          node->setBalance(0);
          tallChild->setBalance(0);
          grandParent->setBalance(0);
        }

        else if(grandParent->getBalance() == 1)
        {
          node->setBalance(-1);
          tallChild->setBalance(0);
          grandParent->setBalance(0);
        }

        removeFix(parent, ndiff);
      }
    }

    else if(node->getBalance() + diff == 1)
    {
      node->setBalance(1);
    }

    else
    {
      node->setBalance(0);
      removeFix(parent, ndiff);
    }
  }

}

//my helper function
template<class Key, class Value>
bool AVLTree<Key, Value>::has2Children(AVLNode<Key, Value>& node)
{
  if((node->getLeft() != NULL) && (node->getRight() != NULL))
  {
    return true;
  }

  return false;
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
  BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
  int8_t tempB = n1->getBalance();
  n1->setBalance(n2->getBalance());
  n2->setBalance(tempB);
}


#endif
