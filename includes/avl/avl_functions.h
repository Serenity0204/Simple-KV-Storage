#ifndef AVL_TREE_FUNCTIONS_H
#define AVL_TREE_FUNCTIONS_H

#include "binary_tree.h"
using namespace std;


// NON-MEMBER FUNCTIONS for the AVL<Item>:
template <class Item>
binary_tree_node<Item>* sortedArrayToAVL(const Item* nums, int left, int right);
template <class Item>
binary_tree_node<Item>* searchAVL(binary_tree_node<Item>* root, const Item& val);
template <class Item>
binary_tree_node<Item>* insertIntoAVL(binary_tree_node<Item>* root, const Item& key);
template <class Item>
binary_tree_node<Item>* deleteNodeAVL(binary_tree_node<Item>* root, const Item& key);



// Implementation NON-MEMBER FUNCTIONS
// TODO
template <class Item>
binary_tree_node<Item>* sortedArrayToAVL(const Item* nums, int left, int right)
{
  if(left > right) return nullptr;

  int mid = (left + right) / 2;
  binary_tree_node<Item>* root = new binary_tree_node<Item>(nums[mid]);
  // recursively set the child nodes
  root->set_left(sortedArrayToAVL(nums, left, mid - 1));
  root->set_right(sortedArrayToAVL(nums, mid + 1, right));
  return root;
}

template <class Item>
binary_tree_node<Item>* searchAVL(binary_tree_node<Item>* root, const Item& val)
{
  if(root == nullptr) return nullptr;
  if(val == root->data()) return root;

  if(val > root->data()) return searchAVL(root->right(), val);
  if(val < root->data()) return searchAVL(root->left(), val);
  return root;
}



template <class Item>
binary_tree_node<Item>* insertIntoAVL(binary_tree_node<Item>* root, const Item& key)
{
  if(root == nullptr)
  {
    root = new binary_tree_node<Item>(key);
    return root;
  }
  if (key < root->data()) root->set_left(insertIntoAVL(root->left(), key));
  if (key > root->data()) root->set_right(insertIntoAVL(root->right(), key));
  
  root->update_height();
  int balance = root->balance_factor();
  if((balance > 1) && (key < root->left()->data())) return rotate_right(root);
  if((balance > 1) && (key > root->left()->data())) return rotate_left_right(root);
  if((balance < -1) && (key > root->right()->data())) return rotate_left(root);
  if((balance < -1) && (key < root->right()->data())) return rotate_right_left(root);
  return root;
}


template<class Item>
binary_tree_node<Item>* getMinNode(binary_tree_node<Item>* root)
{
  while(root->left() != nullptr) root = root->left();
  return root;
}


template <class Item>
binary_tree_node<Item>* deleteNodeAVL(binary_tree_node<Item>* root, const Item& key)
{
  if(root == nullptr) return nullptr;

  // found, has to delete
  if(root->data() < key) root->set_right(deleteNodeAVL(root->right(), key));
  if(root->data() > key) root->set_left(deleteNodeAVL(root->left(), key));

  if(root->data() == key)
  {
    // leaf node, delete directly
    if(root->left() == nullptr && root->right() == nullptr)
    {
      delete root;
      return nullptr;
    }

    // only has one child, save the child it has, delete the root, then return the child saved
    if(root->left() == nullptr)
    {
      binary_tree_node<Item>* temp = root->right();
      delete root;
      return temp;
    }
    if(root->right() == nullptr) 
    {
      binary_tree_node<Item>* temp = root->left();
      delete root;
      return temp;
    }         
    binary_tree_node<Item>* minNode = getMinNode(root->right());
    root->set_data(minNode->data());
    root->set_right(deleteNodeAVL(root->right(), minNode->data()));
  }
  if(root == nullptr) return root;

  root->update_height();
  int balance = root->balance_factor();
  if((balance > 1) && (root->left()->balance_factor() >= 0)) return rotate_right(root);
  if((balance > 1) && (root->left()->balance_factor() < 0)) return rotate_left_right(root);
  if((balance < -1) && (root->right()->balance_factor() <= 0)) return rotate_left(root);
  if((balance < -1) && (root->right()->balance_factor() > 0)) return rotate_right_left(root);
  return root;
}




#endif // AVL_FUNCTIONS_H