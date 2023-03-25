#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <cassert>    // Provides assert
#include <cstdlib>    // Provides NULL, std::size_t
#include <iomanip>    // Provides std::setw
#include <iostream>   // Provides std::cout
using namespace std;


template <class Item>
class binary_tree_node
{
public:
  // TYPEDEF
  typedef Item value_type;
  // CONSTRUCTOR
  binary_tree_node(const Item& init_data = Item(),
    binary_tree_node<Item>* init_left = nullptr,
    binary_tree_node<Item>* init_right = nullptr
  );
  // MODIFICATION MEMBER FUNCTIONS
  Item& data();
  binary_tree_node<Item>* left();
  binary_tree_node<Item>* right();

  binary_tree_node<Item>*& get_left(){return this->left_field;}
  binary_tree_node<Item>*& get_right(){return this->right_field;}

  void set_data(const Item& new_data);
  void set_left(binary_tree_node<Item>* new_left);
  void set_right(binary_tree_node<Item>* new_right);
  int balance_factor();
  int update_height();
  // CONST MEMBER FUNCTIONS
  const Item& data() const;
  const binary_tree_node<Item>* left() const;
  const binary_tree_node<Item>* right() const;
  bool is_leaf() const;
  const int height();

  void set_height(int h)
  {
    this->tree_height = h;
  }
  int update_entire_tree_height()
  {
    int left = this->left_field != nullptr ? this->left_field->update_entire_tree_height() : -1;
    int right = this->right_field != nullptr ? this->right_field->update_entire_tree_height() : -1;
    this->tree_height = max(left, right) + 1;
    return this->tree_height;
  }
  // OVERLOAD OPERATOR FUNCTIONS
  template <class U>
  friend std::ostream& operator << (std::ostream& outs, const binary_tree_node<U>& tree_node);
private:

  Item data_field;
  binary_tree_node<Item>* left_field;
  binary_tree_node<Item>* right_field;
  int tree_height;
};


// NON-MEMBER FUNCTIONS for the binary_tree_node<Item>:
template <class Process, class BTNode>
void inorder(Process f, BTNode* node_ptr); 

template <class Process, class BTNode>
void preorder(Process f, BTNode* node_ptr);

template <class Process, class BTNode>
void postorder(Process f, BTNode* node_ptr); 

template <class Item>
void print_inorder(const binary_tree_node<Item>* root);

template <class Item>
void print(binary_tree_node<Item>* node_ptr, int depth=0);

template <class Item>
void print_debug(binary_tree_node<Item>* node_ptr, int depth=0);

template <class Item>
void tree_clear(binary_tree_node<Item>*& root_ptr);

template <class Item>
binary_tree_node<Item>* tree_copy(const binary_tree_node<Item>* root_ptr);

template <class U>
std::size_t tree_size(const binary_tree_node<U>* node_ptr);


// Implementation MEMBER FUNCTIONS
// TODO
template <class Item>
binary_tree_node<Item>::binary_tree_node(const Item& init_data, binary_tree_node<Item>* init_left, binary_tree_node<Item>* init_right)
{
    this->data_field = init_data;
    this->left_field = init_left;
    this->right_field = init_right;
    this->tree_height = 0;
}

template <class Item>
Item& binary_tree_node<Item>::data()
{
    return this->data_field;
}

template <class Item>
binary_tree_node<Item>* binary_tree_node<Item>::left()
{
    return this->left_field;
}


template <class Item>
binary_tree_node<Item>* binary_tree_node<Item>::right()
{
    return this->right_field;
}

template <class Item>
void binary_tree_node<Item>::set_data(const Item& new_data)
{
    this->data_field = new_data;
}

template <class Item>
void binary_tree_node<Item>::set_left(binary_tree_node* new_left)
{
    this->left_field = new_left;
}

template <class Item>
void binary_tree_node<Item>::set_right(binary_tree_node* new_right)
{
    this->right_field = new_right;
}



template <class Item>
int binary_tree_node<Item>::balance_factor()
{
    int left = (this->left_field != nullptr) ? this->left_field->height() : -1;
    int right = (this->right_field != nullptr) ? this->right_field->height() : -1;
    int balance = left - right;
    return balance;
}

template <class Item>
int binary_tree_node<Item>::update_height()
{
    int leftH = left_field ? left_field->tree_height : -1;
    int rightH = right_field ? right_field->tree_height : -1;
    tree_height = max(leftH, rightH) + 1;
    return tree_height;
}


// CONST MEMBER FUNCTIONS
template <class Item>
const Item& binary_tree_node<Item>::data() const
{
    return this->data_field;
}


template <class Item>
const binary_tree_node<Item>* binary_tree_node<Item>::left() const
{
    return this->left_field;
}

template <class Item>
const binary_tree_node<Item>* binary_tree_node<Item>::right() const
{
    return this->right_field;
}

template <class Item>
bool binary_tree_node<Item>::is_leaf() const
{
    if(this->right_field == nullptr && this->left_field == nullptr) return true;
    return false;
}


// recursively get the height
template <class Item>
const int binary_tree_node<Item>::height()
{
    return update_height();
}


template <class U>
std::ostream& operator << (std::ostream& outs, const binary_tree_node<U>& tree_node)
{
    outs << "[" <<tree_node.data() << "]";
    return outs;
}


// Implementation NON-MEMBER FUNCTIONS
// TODO 
template <class Process, class BTNode>
void inorder(Process f, BTNode* node_ptr)
{
    if(node_ptr == nullptr) return;

    inorder(f, node_ptr->left());
    f(node_ptr->data());
    inorder(f, node_ptr->right());
}

template <class Process, class BTNode>
void preorder(Process f, BTNode* node_ptr)
{
    if(node_ptr == nullptr) return;
    f(node_ptr->data());
    preorder(f, node_ptr->left());
    preorder(f, node_ptr->right());
}

template <class Process, class BTNode>
void postorder(Process f, BTNode* node_ptr)
{
    if(node_ptr == nullptr) return;
    postorder(f, node_ptr->left());
    postorder(f, node_ptr->right());
    f(node_ptr->data());
}

template <class Item>
void print(binary_tree_node<Item>* node_ptr, int depth)
{
    if(node_ptr == nullptr)
    {
        cout << "null" << endl;
        return;
    }
    if(node_ptr->right() != nullptr) print(node_ptr->right(), depth + 1);
    for(int i = 0; i < depth; ++i) cout << "    ";
    cout << *node_ptr << endl;
    if(node_ptr->left() != nullptr) print(node_ptr->left(), depth + 1);
}

template <class Item>
void print_debug(binary_tree_node<Item>* node_ptr, int depth)
{
    if(node_ptr == nullptr)
    {
        cout << "null" << endl;
        return;
    }
    if(node_ptr->right() != nullptr) print_debug(node_ptr->right(), depth + 1);
    for(int i = 0; i < depth; ++i) cout << "        ";
    cout << *node_ptr << "(h:" << node_ptr->height() << ",bf:" << node_ptr->balance_factor() << ")" << endl;
    if(node_ptr->left() != nullptr) print_debug(node_ptr->left(), depth + 1);  
}

template <class Item>
void tree_clear(binary_tree_node<Item>*& root_ptr)
{
    // base case
    if(root_ptr == nullptr) return;

    // post order traversal, if needed
    if(root_ptr->get_left() != nullptr) tree_clear(root_ptr->get_left());
    if(root_ptr->get_right() != nullptr) tree_clear(root_ptr->get_right());

    // delete goes here
    if(root_ptr->get_left() == nullptr && root_ptr->get_right() == nullptr)
    {
        delete root_ptr;
        root_ptr = nullptr;
    }
}

template <class Item>
binary_tree_node<Item>* tree_copy(const binary_tree_node<Item>* root_ptr)
{
    if(root_ptr == nullptr) return nullptr;
    // using pre order to recursively construct the tree
    binary_tree_node<Item>* root = new binary_tree_node<Item>(root_ptr->data());
    root->set_left(tree_copy(root_ptr->left()));
    root->set_right(tree_copy(root_ptr->right()));
    return root;
}

template <class U>
std::size_t tree_size(const binary_tree_node<U>* node_ptr)
{
    if(node_ptr == nullptr) return 0;
    // count the node using postorder
    std::size_t leftNum = tree_size(node_ptr->left());
    std::size_t rightNum = tree_size(node_ptr->right());

    return leftNum + rightNum + 1;
}

template <class Item>
void print_inorder(const binary_tree_node<Item>* root)
{
  if(root == nullptr) return;
  print_inorder(root->left());
  cout << root->data();
  print_inorder(root->right());
}



#endif // BINARY_TREE_H