#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "avl_functions.h"
#include "avl_rotations.h"
#include "binary_tree.h"
#include <iomanip>
#include <iostream>



template <class Item>
class AVL
{
public:
    // TYPEDEFS
    typedef std::size_t size_type;
    typedef Item value_type;
    // CONSTRUCTORS and DESTRUCTOR
    AVL();
    AVL(const Item* nums, int size = -1); // nums is a sorted list
    AVL(const AVL<Item>& copy_me);

    // consturctor for passing the root in
    AVL(binary_tree_node<Item>* root)
    {
        this->_root_ptr = tree_copy<Item>(root);
        if (this->_root_ptr != nullptr) this->_root_ptr->update_entire_tree_height();
    }
    ~AVL();
    AVL<Item>& operator=(const AVL<Item>& rhs);
    // MODIFICATION FUNCTIONS

    binary_tree_node<Item>*& get_root() { return this->_root_ptr; }
    binary_tree_node<Item>* root() { return this->_root_ptr; }

    binary_tree_node<Item>* search(const Item& target) const;
    void insert(const Item& val);
    void erase(const Item& val);
    void clear_all();
    size_t size() { return this->_size; }
    // CONST FUNCTIONS
    const binary_tree_node<Item>* root() const { return this->_root_ptr; }
    bool empty() const;
    // OVERLOAD OPERATOR FUNCTIONS
    template <class U>
    friend std::ostream& operator<<(std::ostream& outs, const AVL<U>& tree);

private:
    int _size;
    binary_tree_node<Item>* _root_ptr; // Root pointer of binary search tree
};

// Implementation MEMBER FUNCTIONS
// TODO
template <class Item>
AVL<Item>::AVL()
{
    this->_size = 0;
    this->_root_ptr = nullptr;
}

template <class Item>
AVL<Item>::AVL(const Item* nums, int size)
{
    this->_root_ptr = sortedArrayToAVL<Item>(nums, 0, size - 1);
    if (this->_root_ptr == nullptr) this->_size = 0;
    if (this->_root_ptr != nullptr)
    {
        this->_root_ptr->update_entire_tree_height();
        this->_size = tree_size<Item>(this->_root_ptr);
    }
} // nums is a sorted list

template <class Item>
AVL<Item>::AVL(const AVL<Item>& copy_me)
{
    this->_root_ptr = tree_copy<Item>(copy_me._root_ptr);
    if (this->_root_ptr == nullptr) this->_size = 0;
    if (this->_root_ptr != nullptr)
    {
        this->_root_ptr->update_entire_tree_height();
        this->_size = tree_size<Item>(this->_root_ptr);
    }
}

template <class Item>
AVL<Item>::~AVL()
{
    tree_clear<Item>(this->get_root());
}

template <class Item>
AVL<Item>& AVL<Item>::operator=(const AVL<Item>& rhs)
{
    if (this == &rhs) return *this;
    tree_clear<Item>(this->_root_ptr);
    this->_root_ptr = tree_copy<Item>(rhs._root_ptr);
    if (this->_root_ptr == nullptr) this->_size = 0;
    if (this->_root_ptr != nullptr)
    {
        this->_root_ptr->update_entire_tree_height();
        this->_size = tree_size<Item>(this->_root_ptr);
    }
    return *this;
}

// MODIFICATION FUNCTIONS
template <class Item>
binary_tree_node<Item>* AVL<Item>::search(const Item& target) const
{
    return searchAVL(this->_root_ptr, target);
}

template <class Item>
void AVL<Item>::insert(const Item& val)
{
    if (searchAVL(this->_root_ptr, val) != nullptr) return;
    this->_root_ptr = insertIntoAVL(this->_root_ptr, val);
    this->_size++;
}

template <class Item>
void AVL<Item>::erase(const Item& val)
{
    if (searchAVL(this->_root_ptr, val) == nullptr) return;
    this->_root_ptr = deleteNodeAVL(this->_root_ptr, val);
    this->_size--;
}

template <class Item>
void AVL<Item>::clear_all()
{
    // only clear if it's not empty
    if (this->empty()) return;
    tree_clear<Item>(this->_root_ptr);
    this->_size = 0;
}

// CONST FUNCTIONS
template <class Item>
bool AVL<Item>::empty() const
{
    return (this->_root_ptr == nullptr) ? true : false;
}

// OVERLOAD OPERATOR FUNCTIONS
template <class U>
std::ostream& operator<<(std::ostream& outs, const AVL<U>& tree)
{
    print(tree._root_ptr);
    return outs;
}

#endif // AVL_TREE_H