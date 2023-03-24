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
    ~AVLTree();
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
  
  protected:

    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
    
    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);
    void removeFix(AVLNode<Key, Value>* node, int diff);
    void rotateRight(AVLNode<Key, Value>* node);
    void rotateLeft(AVLNode<Key, Value>* node);
    bool has2Children(AVLNode<Key, Value>* node);
    void doClear(AVLNode<Key, Value>* curr);
    void clear();
    int getNodeHeight(const AVLNode<Key, Value>* current) const;
    static AVLNode<Key, Value>* predecessor(AVLNode<Key, Value>* current);

    //override function
    AVLNode<Key, Value>* internalFind(const Key& k) const;

    //root node for AVL
    AVLNode<Key, Value>* rootAVL;
  
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
*/

//constructor
template<class Key, class Value>
AVLTree<Key, Value>::AVLTree() : BinarySearchTree<Key, Value>(), rootAVL(NULL) {}

//destructor
template<typename Key, typename Value>
AVLTree<Key, Value>::~AVLTree()
{
  // TODO
  clear();
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::clear()
{
  //TODO
  BinarySearchTree<Key, Value>::doClear(this->rootAVL);
  rootAVL = NULL;
  this->root_ = NULL;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::internalFind(const Key& k) const
{
  AVLNode<Key, Value>* temp = rootAVL;

  //logic of where to insert
  while(temp != NULL)
  {
    //equal to
    if( k == temp->getKey())
    {
      return temp;
    }
    
    //less than
    else if( k < temp->getKey())
    {
      temp = temp->getLeft();
    }

    //greater than
    else
    {
      temp = temp->getRight();
    }
  }

  return NULL;
}

template<class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::predecessor(AVLNode<Key, Value>* current)
{
  // TODO
  if(current == NULL)
  {
    return NULL;
  }
  
  AVLNode<Key, Value>* suc = NULL;
  if(current->getLeft() != NULL)
  {
    suc = current->getLeft();
    while(suc->getRight() != NULL)
    {
      suc = suc->getRight();
    }
    return suc;
  }

  else
  {
    if(current->getParent() == NULL)
    {
      return NULL;;
    }

    suc = current->getParent();

    if((suc != NULL) && (suc->getRight() != NULL))
    { 
      AVLNode<Key, Value>* temp = suc->getParent()->getRight();

      if((suc->getRight() != NULL)&&(temp->getKey() == current->getKey()))
      {
        return suc;
      }

      else
      {
        return NULL;
      }
    }

    else
    {
      suc = suc->getParent();
      while(suc->getRight() != current)
      {
        suc = suc->getParent();
        if(suc->getParent() == NULL)
        {
          return NULL;
        }
      }
      return suc;
    }
  }

  return NULL;
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
  // TODO
  if(this->root_ == NULL)
  {
    //allocate new memory for inserted pair
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
    rootAVL = newNode;
    rootAVL->setLeft(NULL);
    rootAVL->setRight(NULL);
    rootAVL->setBalance(0); //idk bout this

    this->root_ = rootAVL;
    return;
  }

  AVLNode<Key, Value>* temp = rootAVL;

  if((new_item.first == temp->getKey())) //&& (temp->getRight() == NULL) && (temp->getLeft() == NULL)
  {
    temp->setValue(new_item.second);
    return;
  }

  //allocate new memory for inserted pair
  AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, temp);
  newNode->setBalance(0);

  if((temp->getLeft() == NULL) || (temp->getRight() == NULL))
  {

    if((new_item.first > temp->getKey()) && (temp->getRight() == NULL))
    {
    
      //set child
      temp->setRight(newNode);

      //set parent
      newNode->setParent(temp);

      //update new node balance
      newNode->setBalance(0);

      //update parent node balance
      temp->updateBalance(1);

      //check balance of parent
      if(temp->getBalance() != 0)
      {
        insertFix(temp, newNode);
      }

      return;
    }

    else if((new_item.first < temp->getKey()) && (temp->getLeft() == NULL))
    {
      //set child
      temp->setLeft(newNode);

      //set parent
      newNode->setParent(temp);

      //update parent node balance
      temp->updateBalance(-1);

      //check balance of parent
      if(temp->getBalance() != 0)
      {
        insertFix(temp, newNode);
      }

      return;
    }
  }

  //logic of where to insert
  while(temp != NULL)
  {
    if((new_item.first == temp->getKey()))
    {
      temp->setValue(new_item.second);
      return;
    }

    //less than, and must traverse
    else if((new_item.first < temp->getKey()) && (temp->getLeft() != NULL))
    {
      temp = temp->getLeft();
    }

    //greater than, must traverse
    else if((new_item.first > temp->getKey()) && (temp->getRight() != NULL))
    {
      temp = temp->getRight();
    }

    //less than, able to allocate
    else if((new_item.first < temp->getKey()) && (temp->getLeft() == NULL))
    {
      
      //set child
      temp->setLeft(newNode);

      //update parent node balance
      temp->updateBalance(-1);

      //set parent
      newNode->setParent(temp);

      //check balance of parent
      if(temp->getBalance() != 0)
      {
        insertFix(temp, newNode);
      }

      break;
    }

    //greater than, able to allocate
    else
    {
      //set child
      temp->setRight(newNode);

      //update parent node balance
      temp->updateBalance(1);

      //set parent
      newNode->setParent(temp);

      //check balance of parent
      if(temp->getBalance() != 0)
      {
        insertFix(temp, newNode);
      }

      break;
    }
  }

}

//my helper function
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child)
{
  if(parent == NULL || child == NULL)
  {
    return;
  }

  //initialize grandParent node
  AVLNode<Key, Value>* grandParent = parent->getParent();

  if(grandParent == NULL)
  {
    return;
  }
  
  //p is left child of gp
  if(grandParent->getLeft() == parent)
  {
    //update grandParent balance
    grandParent->updateBalance(-1);

    //0 balance
    if(grandParent->getBalance() == 0)
    {
      //child->setParent(parent);
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
        
        if(child->getBalance() == -1)
        {
          parent->setBalance(0);
          grandParent->setBalance(1);
          child->setBalance(0);
        }

        else if(child->getBalance() == 0)
        {
          parent->setBalance(0);
          grandParent->setBalance(0);
          child->setBalance(0);
        }

        else
        {
          parent->setBalance(-1);
          grandParent->setBalance(0);
          child->setBalance(0);
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
      //child->setParent(parent);
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
      //zig-zig, right right
      if((child == parent->getRight()) && (parent == grandParent->getRight()))
      {
        rotateLeft(grandParent);
        parent->setBalance(0);
        grandParent->setBalance(0);
      }

      //zig-zag
      else
      {
        rotateRight(parent);
        rotateLeft(grandParent);

        if(child->getBalance() == 1)
        {
          parent->setBalance(0);
          grandParent->setBalance(-1);
          child->setBalance(0);
        }

        else if(child->getBalance() == 0)
        {
          parent->setBalance(0);
          grandParent->setBalance(0);
          child->setBalance(0);
        }

        else
        {
          parent->setBalance(1);
          grandParent->setBalance(0);
          child->setBalance(0);
        }
      }
    }
  }
}

//my helper function
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
	AVLNode<Key, Value>* left = node->getLeft();
	AVLNode<Key, Value>* parent = node->getParent();
  
	//node is not a root
	if(parent  != NULL)
	{
    left->setParent(parent);
    if(node == parent->getLeft())
    {
      parent->setLeft(left);
    }
    else
    {
      parent->setRight(left);
    }
    
  }
  //node is the root
  else
  {
    this->rootAVL = left;
    this->root_ = left;
    left->setParent(NULL);
  }
  
  //child switch case
  if(left->getRight() != NULL)
  {
    left->getRight()->setParent(node);
    node->setLeft(left->getRight());
  }
  else
  {
    node->setLeft(NULL);
  }

  left->setRight(node);
  node->setParent(left);
  return;
}

//my helper function
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
  //necessary pointer
  AVLNode<Key, Value>* parent = node->getParent();
  AVLNode<Key, Value>* right = node->getRight();

  //node is not a root
	if(parent  != NULL)
	{
    right->setParent(parent);
    if(node == parent->getRight())
    {
      parent ->setRight(right);
    }
    else
    {
      parent->setLeft(right);
    }
    
  }
  //node is the root
  else
  {
    this->rootAVL = right;
    this->root_ = right;
    right->setParent(NULL);
  }

  //child switch case
  if(right->getLeft() != NULL)
  {
    right->getLeft()->setParent(node);
    node->setRight(right->getLeft());
  }
  else
  {
    node->setRight(NULL);
  }

  right->setLeft(node);
  node->setParent(right);
  return;

}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
  //tree is empty
  if(rootAVL == NULL)
  {
    return;
  }

  //target node and temp root
  AVLNode<Key, Value>* curr = internalFind(key);
  AVLNode<Key, Value>* temp = rootAVL;
  bool didSwitch = false;
  bool isRoot = false;
  int diff = 0;
  
  //target not in tree
  if(curr == NULL)
  {
    return;
  }

  //2 child case
  if((curr->getLeft() != NULL) && (curr->getRight() != NULL))
  {
    temp = predecessor(curr);
    nodeSwap(temp, curr);
    didSwitch = true;
  }

  if(didSwitch == false)
  {
    //logic to determine if we are deleting a root
    if(rootAVL == curr)
    {
      isRoot = true;
    }

    //pointers to assist with removal
    AVLNode<Key, Value>* left = curr->getLeft();
    AVLNode<Key, Value>* right = curr->getRight();
    AVLNode<Key, Value>* parent = curr->getParent();
    

    //logic for getting difference
    if(parent != NULL)
    {
      //right subtree will increase in height
      if(curr == parent->getLeft())
      {
        diff = 1;
      }

      //left subtree will increase in height
      else
      {
        diff = -1;
      }
    }

    //0 child and deleting a root
    if((isRoot) && (right == NULL) && (left == NULL))
    {
      curr->setBalance(0);
      delete curr;
      rootAVL = NULL;
      this->root_ = NULL;
    }

    //1 child case
    else if(((left != NULL) && (right == NULL)) || ((left == NULL) && (right != NULL)))
    {
      //target node is the root
      if(isRoot == true)
      {
        //rootAVL->setBalance(0);

        //target node has left child
        if(left != NULL)
        {
          this->root_ = left;
          rootAVL = left;
          left->setParent(NULL);
          left->setBalance(curr->getBalance());
        }
        //target node has right child
        else
        {
          this->root_ = right;
          rootAVL = right;
          right->setParent(NULL);
          right->setBalance(curr->getBalance());
        }
      }

      //target node is not the root
      else
      {
        //target has a left child
        if(left != NULL)
        {
          left->setParent(parent);
          
          //target is a left child
          if(parent->getLeft() == curr)
          {
            parent->updateBalance(-1);
            parent->setLeft(left);
          }
          //target is a right child
          else
          {
            parent->updateBalance(1);
            parent->setRight(left);
          }   
        } 

        //target has a right child
        else
        {
          right->setParent(parent);

          //target is a left child
          if(parent->getLeft() == curr)
          {
            parent->updateBalance(-1);
            parent->setLeft(right);  
          }

          //target is a right child
          else
          {
            parent->updateBalance(1);
            parent->setRight(right);
          }
        }
      }

      delete curr;
    }

    //0 child case
    else
    {
      //target node is a left child
      if(parent->getLeft() == curr)
      {
        parent->setLeft(NULL);
      }

      //target node is a right child
      else
      {
        parent->setRight(NULL);
      }

      delete curr;
    }

    //patch up tree
    removeFix(parent, diff);
  }

  //did do switch
  else
  {
    if(rootAVL == temp)
    {
      isRoot = true;
    }
    //logic to determine if we are deleting a root
    if(rootAVL == curr)
    {
      rootAVL = temp;
      this->root_ = temp;
    }

    //pointers to assist with removal
    AVLNode<Key, Value>* left = curr->getLeft();
    AVLNode<Key, Value>* right = curr->getRight();
    AVLNode<Key, Value>* parent = curr->getParent();
    

    //logic for getting difference
    if(parent != NULL)
    {
      //right subtree will increase in height
      if(curr == parent->getLeft())
      {
        diff = 1;
      }

      //left subtree will increase in height
      else
      {
        diff = -1;
      }
    }

    //0 child and deleting a root
    if((isRoot) && (right == NULL) && (left == NULL))
    {
      curr->setBalance(0);
      delete curr;
      rootAVL = NULL;
      this->root_ = NULL;
    }

    //1 child case
    else if(((left != NULL) && (right == NULL)) || ((left == NULL) && (right != NULL)))
    {
      //target node is the root
      if(isRoot == true)
      {
        //target node has left child
        if(left != NULL)
        {
          this->root_ = left;
          rootAVL = left;
          left->setParent(NULL);
          left->setBalance(curr->getBalance());
        }
        //target node has right child
        else
        {
          this->root_ = right;
          rootAVL = right;
          right->setParent(NULL);
          right->setBalance(curr->getBalance());
        }
      }

      //target node is not the root
      else
      {
        //target has a left child
        if(left != NULL)
        {
          left->setParent(parent);
          
          //target is a left child
          if(curr->getLeft() == left)
          {
            parent->updateBalance(-1);
            parent->setLeft(left);
          }
          //target is a right child
          else
          {
            parent->updateBalance(1);
            parent->setRight(left);
          }   
        } 

        //target has a right child
        else
        {
          right->setParent(parent);

          //target is a left child
          if(curr->getLeft() == right)
          {
            curr->updateBalance(-1);
            curr->setLeft(right);  
          }

          //target is a right child
          else
          {
            curr->updateBalance(1);
            curr->setRight(right);
          }
        }
      }

      delete curr;
    }

    //0 child case
    else
    {
      //target node is a left child
      if(parent->getLeft() == curr)
      {
        parent->setLeft(NULL);
      }

      //target node is a right child
      else
      {
        parent->setRight(NULL);
      }

      delete curr;
    }

    //patch up tree
    removeFix(parent, diff);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int diff)
{
  if(node == NULL)
  {
    return;
  }

  AVLNode<Key, Value>* parent = node->getParent();
  int ndiff = -1;


  if(parent != NULL)
  {
    if(node == parent->getLeft())
    {
      ndiff = 1;
    }
  }

  if(diff == -1)
  {
    if(node->getBalance() + diff == -2)
    {
      //initialize tallChild to be left child, and change if it isnt true
      AVLNode<Key, Value>* tallChild = node->getLeft();
      if(getNodeHeight(node->getLeft()) < getNodeHeight(node->getRight()))
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
      else if(tallChild->getBalance() == 0)
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
        
        if(grandParent == NULL){}

        else if(grandParent->getBalance() == 1)
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
      if(getNodeHeight(node->getLeft()) < getNodeHeight(node->getRight()))
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
      else if(tallChild->getBalance() == 0)
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

        if(grandParent == NULL){}

        else if(grandParent->getBalance() == -1)
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
template<typename Key, typename Value>
int AVLTree<Key, Value>::getNodeHeight(const AVLNode<Key, Value>* current) const
{
    // Base case
    if(current == NULL)
    {
        return -1;
    }
       
    // Initialize distance as -1
    int dist = -1;
  
    if((dist = getNodeHeight(current->getLeft() ) >= 0) || (dist = getNodeHeight(current->getRight() ) >= 0))
    {
      // Return depth of the node
      return dist + 1;
    }
  
    return dist;

}

//my helper function
template<class Key, class Value>
bool AVLTree<Key, Value>::has2Children(AVLNode<Key, Value>* node)
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
