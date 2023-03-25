#ifndef AVL_ROTATIONS_H
#define AVL_ROTATIONS_H

#include "binary_tree.h"
using namespace std;


// ROTATION FUNCTIONS for the AVL<Item>
// ---------------- ROTATIONS ----------------
template <class Item>
binary_tree_node<Item>* rotate_right(binary_tree_node<Item>* root);
template <class Item>
binary_tree_node<Item>* rotate_left(binary_tree_node<Item>* root);
template <class Item>
binary_tree_node<Item>* rotate_right_left(binary_tree_node<Item>* root);
template <class Item>
binary_tree_node<Item>* rotate_left_right(binary_tree_node<Item>* root);


// rotation functions
template <class Item>
binary_tree_node<Item>* rotate_right(binary_tree_node<Item>* root)
{
  binary_tree_node<Item>* root_left_child = root->left();
  root->set_left(root_left_child->right());
  root_left_child->set_right(root);
  root->update_height();
  root_left_child->update_height();
  return root_left_child;
}


template <class Item>
binary_tree_node<Item>* rotate_left(binary_tree_node<Item>* root)
{
  binary_tree_node<Item>* root_right_child = root->right();
  root->set_right(root_right_child->left());
  root_right_child->set_left(root);
  root->update_height();
  root_right_child->update_height();
  return root_right_child;
}

template <class Item>
binary_tree_node<Item>* rotate_right_left(binary_tree_node<Item>* root)
{
  root->set_right(rotate_right(root->right()));
  return rotate_left(root);
}

template <class Item>
binary_tree_node<Item>* rotate_left_right(binary_tree_node<Item>* root)
{
  root->set_left(rotate_left(root->left()));
  return rotate_right(root);
}


#endif // AVL_ROTATIONS_H