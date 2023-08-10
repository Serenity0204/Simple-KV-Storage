#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include "bplustree_array_functions.h" // Include the implementation.
#include <cassert>                     // Provides assert
#include <cstdlib>                     // Provides size_t
#include <iomanip>                     // Provides std::setw
#include <iostream>                    // Provides std::cout
#include <string>                      // Provides std::to_string

template <class Item>
class BPlusTree
{
public:
    // TYPEDEFS and MEMBER CONSTANTS
    typedef Item value_type;
    // NESTED CLASS DEFINITION
    class Iterator
    {
    public:
        friend class BPlusTree;
        // CONSTRUCTORS
        Iterator(BPlusTree* _it = nullptr, std::size_t _key_ptr = 0) : it(_it), key_ptr(_key_ptr) {}
        // MODIFICATION MEMBER FUNCTIONS
        inline Item operator*()
        {
            assert(this->it != nullptr);
            return this->it->data[this->key_ptr];
        }
        inline const Item operator*() const
        {
            assert(this->it != nullptr);
            return this->it->data[this->key_ptr];
        }
        inline Iterator operator++(int un_used) // Postfix ++ (it++)
        {
            Iterator hold = *this;
            if (this->key_ptr == this->it->data_count - 1)
            {
                this->it = this->it->next;
                this->key_ptr = 0;
                return hold;
            }
            this->key_ptr++;
            return hold;
        }
        inline Iterator operator++() // Prefix ++ (++it)
        {
            if (this->key_ptr == this->it->data_count - 1)
            {
                this->it = this->it->next;
                this->key_ptr = 0;
                return *this;
            }
            this->key_ptr++;
            return *this;
        }
        // OVERLOADED OPERATORS
        friend inline bool operator==(const Iterator& lhs, const Iterator& rhs)
        {
            if (lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr)
                return true;
            return false;
        }
        friend inline bool operator!=(const Iterator& lhs, const Iterator& rhs)
        {
            return !(lhs == rhs);
        }
        // HELPER FUNCTIONS
        bool is_null()
        {
            return !this->it;
        }
        void print_Iterator()
        {
            if (!this->is_null())
            {
                print_array(this->it->data, this->it->data_count);
                return;
            }
            std::cout << "nullptr iterator" << std::endl;
        }

    private:
        BPlusTree<Item>* it;
        std::size_t key_ptr;
    };

    // CONSTRUCTORS and DESTRUCTOR
    BPlusTree();
    BPlusTree(const BPlusTree& source);
    BPlusTree(Item* a, std::size_t size);
    ~BPlusTree() { clear_tree(); }
    // MODIFICATION MEMBER FUNCTIONS
    BPlusTree& operator=(const BPlusTree& source);
    void clear_tree();
    void copy_tree(const BPlusTree& source);
    void copy_tree(const BPlusTree& source, BPlusTree*& last_node);
    bool insert(const Item& entry);
    bool erase(const Item& target);
    // NON-CONSTANT MEMBER FUNCTIONS
    Iterator find(const Item& entry);
    Item& get(const Item& entry);

    // ITERATORS
    Iterator lower_bound(const Item& key);
    Iterator upper_bound(const Item& key);
    Iterator begin();
    Iterator end() { return Iterator(); }
    // CONSTANT MEMBER FUNCTIONS
    bool contains(const Item& entry) const;
    const Item& get(const Item& entry) const;

    // SIZES
    std::size_t size();
    std::size_t size_list();
    std::size_t count() const;

    // VALIDATORS
    bool empty() const { return (data_count == 0 && is_leaf()); }
    void print(int indent = 0, std::ostream& outs = std::cout) const;
    bool is_valid() const;
    // OVERLOAD OPERATOR FUNCTIONS
    friend std::ostream& operator<<(std::ostream& outs, const BPlusTree<Item>& btree)
    {
        btree.print(0, outs);
        return outs;
    }

private:
    // MEMBER CONSTANTS
    static const std::size_t MINIMUM = 1;
    static const std::size_t MAXIMUM = 2 * MINIMUM;
    // MEMBER VARIABLES
    std::size_t data_count;
    Item data[MAXIMUM + 1];
    std::size_t child_count;
    BPlusTree* subset[MAXIMUM + 2];
    BPlusTree* next;
    // HELPER MEMBER FUNCTIONS
    bool is_leaf() const { return (child_count == 0); }
    // for iterators
    BPlusTree<Item>* get_smallest_node();
    // insert element functions
    bool loose_insert(const Item& entry);
    void fix_excess(std::size_t i);
    // remove element functions
    bool loose_erase(const Item& target);
    BPlusTree<Item>* fix_shortage(std::size_t i);
    void get_smallest_item(Item& entry);
    void transfer_from_right(std::size_t i);
    void transfer_from_left(std::size_t i);
    BPlusTree<Item>* merge_with_next_subset(std::size_t i);
    // helper for get
    Item* get_helper(const Item& entry)
    {
        size_t index = BPlusTreeUtils::first_ge(this->data, this->data_count, entry);
        bool found = (index < this->data_count) && !(entry < this->data[index]);

        // leaf and found
        if (this->is_leaf() && found)
            return &this->data[index];
        // interal node and found
        if (!this->is_leaf() && found)
            return this->subset[index + 1]->get_helper(entry);
        // internal node and not found
        if (!this->is_leaf() && !found)
            return this->subset[index]->get_helper(entry);

        // if leaf and not found
        return nullptr;
    }
};

// Implementation ITERATOR MEMBER FUNCTIONS

// TODO

// Implementation BPlusTree MEMBER FUNCTIONS
template <class Item>
BPlusTree<Item>::BPlusTree()
    : data_count(0), child_count(0), next(nullptr)
{
}

template <class Item>
BPlusTree<Item>::BPlusTree(const BPlusTree<Item>& source)
    : BPlusTree()
{
    this->copy_tree(source);
}

template <class Item>
BPlusTree<Item>::BPlusTree(Item* a, std::size_t size)
    : BPlusTree()
{
    for (size_t i = 0; i < size; ++i)
        this->insert(a[i]);
}

template <class Item>
BPlusTree<Item>& BPlusTree<Item>::operator=(const BPlusTree<Item>& source)
{
    if (this == &source) return *this;
    this->clear_tree();
    this->copy_tree(source);
    return *this;
}

template <class Item>
std::size_t BPlusTree<Item>::size()
{
    if (this->empty()) return 0;
    typename BPlusTree<Item>::Iterator it = this->begin();
    size_t count = 0;
    for (it = this->begin(); it != this->end(); ++it) count++;
    return count;
}
template <class Item>
std::size_t BPlusTree<Item>::count() const
{
    size_t count_items = this->data_count;
    // leaf wont enter this loop
    for (size_t i = 0; i < this->child_count; ++i)
    {
        count_items += this->subset[i]->count();
    }
    return count_items;
}

template <class Item>
void BPlusTree<Item>::copy_tree(const BPlusTree<Item>& source)
{
    BPlusTree<Item>* last = nullptr;
    this->copy_tree(source, last);
}

template <class Item>
void BPlusTree<Item>::copy_tree(const BPlusTree<Item>& source, BPlusTree<Item>*& last_node)
{
    this->child_count = source.child_count;
    BPlusTreeUtils::copy_array(this->data, source.data, this->data_count, source.data_count);
    this->next = nullptr;
    for (int i = 0; i < source.child_count; ++i)
    {
        this->subset[i] = new BPlusTree<Item>();
        this->subset[i]->copy_tree(*source.subset[i], last_node);
    }

    if (source.is_leaf())
    {
        if (last_node != nullptr)
            last_node->next = this;

        last_node = this;
    }
}

template <class Item>
void BPlusTree<Item>::clear_tree()
{
    for (size_t i = 0; i < this->child_count; ++i)
    {
        this->subset[i]->clear_tree();
        delete subset[i];
    }
    this->child_count = 0;
    this->data_count = 0;
    this->next = nullptr;
}

template <class Item>
BPlusTree<Item>* BPlusTree<Item>::get_smallest_node()
{
    if (this->is_leaf())
        return this;
    return this->subset[0]->get_smallest_node();
}

template <class Item>
void BPlusTree<Item>::print(int indent, std::ostream& outs) const
{
    static const std::string down_bracket = "^"; // ﹇
    static const std::string up_bracket = "v";   // ﹈
    for (int i = this->data_count; i >= 0; --i)
    {
        if (i < this->data_count)
        {
            if (i == this->data_count - 1 && this->next != nullptr)
                outs << std::string(4 * indent, ' ') << "^" << std::endl;
            if (i == this->data_count - 1)
                outs << std::string(4 * indent, ' ') << down_bracket << std::endl; // ⎴
            outs << std::string(4 * indent, ' ') << this->data[i] << std::endl;
            if (i == 0)
                outs << std::string(4 * indent, ' ') << up_bracket << std::endl; // ⎵
        }

        if (!this->is_leaf())
            this->subset[i]->print(indent + 1, outs);
    }
}

template <class Item>
bool BPlusTree<Item>::insert(const Item& entry)
{
    if (!this->loose_insert(entry))
        return false;
    if (this->data_count > MAXIMUM)
    {
        BPlusTree<Item>* new_root = new BPlusTree<Item>();
        BPlusTreeUtils::copy_array(new_root->data, this->data, new_root->data_count, this->data_count);
        BPlusTreeUtils::copy_array(new_root->subset, this->subset, new_root->child_count, this->child_count);

        this->data_count = 0;
        this->subset[0] = new_root;
        this->child_count = 1;

        this->fix_excess(0);
    }
    return true;
}

template <class Item>
bool BPlusTree<Item>::loose_insert(const Item& entry)
{
    size_t index = BPlusTreeUtils::first_ge(this->data, this->data_count, entry);
    bool found = (index < this->data_count) && !(entry < this->data[index]);
    bool flag = false;
    if (this->is_leaf() && found)
        return false;
    if (this->is_leaf() && !found)
    {
        BPlusTreeUtils::insert_item(this->data, index, this->data_count, entry);
        return true;
    }
    if (!this->is_leaf() && found)
    {
        flag = this->subset[index + 1]->loose_insert(entry);
        if (this->subset[index + 1]->data_count > MAXIMUM)
            this->fix_excess(index + 1);
    }
    if (!this->is_leaf() && !found)
    {
        flag = this->subset[index]->loose_insert(entry);
        if (this->subset[index]->data_count > MAXIMUM)
            this->fix_excess(index);
    }
    return flag;
}

template <class Item>
void BPlusTree<Item>::fix_excess(std::size_t i)
{
    BPlusTree<Item>* subtree = this->subset[i];
    BPlusTree<Item>* split_tree = new BPlusTree<Item>();
    BPlusTreeUtils::split(subtree->data, subtree->data_count, split_tree->data, split_tree->data_count);
    BPlusTreeUtils::split(subtree->subset, subtree->child_count, split_tree->subset, split_tree->child_count);
    Item item;
    BPlusTreeUtils::detach_item(subtree->data, subtree->data_count, item);
    BPlusTreeUtils::ordered_insert(this->data, this->data_count, item);

    if (subtree->is_leaf())
    {
        BPlusTreeUtils::ordered_insert(split_tree->data, split_tree->data_count, item);
        split_tree->next = subtree->next;
        subtree->next = split_tree;
    }
    BPlusTreeUtils::insert_item(this->subset, i + 1, this->child_count, split_tree);
}

template <class Item>
typename BPlusTree<Item>::Iterator BPlusTree<Item>::find(const Item& entry)
{
    size_t index = first_ge(this->data, this->data_count, entry);
    bool found = (index < this->data_count) && !(entry < this->data[index]);
    // leaf and found
    if (this->is_leaf() && found)
        return Iterator(this, index);
    // interal node and found
    if (!this->is_leaf() && found)
        return this->subset[index + 1]->find(entry);
    // internal node and not found
    if (!this->is_leaf() && !found)
        return this->subset[index]->find(entry);
    // leaf and not found
    return Iterator();
}

template <class Item>
Item& BPlusTree<Item>::get(const Item& entry)
{
    if (!this->contains(entry)) this->insert(entry);
    return *this->get_helper(entry);
}

template <class Item>
bool BPlusTree<Item>::contains(const Item& entry) const
{
    size_t index = BPlusTreeUtils::first_ge(this->data, this->data_count, entry);
    bool found = (index < this->data_count) && !(entry < this->data[index]);

    // leaf and found
    if (this->is_leaf() && found)
        return true;
    // interal node and found
    if (!this->is_leaf() && found)
        return this->subset[index + 1]->contains(entry);
    // internal node and not found
    if (!this->is_leaf() && !found)
        return this->subset[index]->contains(entry);

    // if leaf and not found
    return false;
}

template <class Item>
const Item& BPlusTree<Item>::get(const Item& entry) const
{
    assert(this->contains(entry));
    return *this->get_helper(entry);
}

template <class Item>
typename BPlusTree<Item>::Iterator BPlusTree<Item>::lower_bound(const Item& key)
{
    Iterator it = this->begin();
    for (it = this->begin(); it != this->end(); ++it)
    {
        assert(!it.is_null());
        if (!(*it < key)) return it;
    }
    return this->end();
}

template <class Item>
typename BPlusTree<Item>::Iterator BPlusTree<Item>::upper_bound(const Item& key)
{
    Iterator it = this->begin();
    for (it = this->begin(); it != this->end(); ++it)
    {
        assert(!it.is_null());
        if (!(*it <= key)) return it;
    }
    return this->end();
}

template <class Item>
typename BPlusTree<Item>::Iterator BPlusTree<Item>::begin()
{
    return Iterator(this->get_smallest_node());
}

// ***********************************************************************************************************************************
template <class Item>
bool BPlusTree<Item>::is_valid() const
{
    bool valid = true;
    // check if the node is empty
    if (empty())
        return true;
    // check if the node has too many entries
    if (data_count > MAXIMUM || data_count < MINIMUM)
    {
        std::cout << "node has too many entries" << std::endl;
        return false;
    }
    // check if the node has too many children
    if (child_count > MAXIMUM + 1 || child_count < 0)
    {
        std::cout << "node has too many children" << std::endl;
        return false;
    }
    // check if the data is sorted
    for (size_t i = 0; i < data_count - 1; i++)
    {
        if (data[i] > data[i + 1])
        {
            std::cout << "node has too many children" << std::endl;
            return false;
        }
    }
    if (!is_leaf())
    {
        // check if the child_count is not equal to data_count+1
        if (child_count != data_count + 1)
        {
            std::cout << "child count:" << child_count << std::endl;
            std::cout << "data count:" << data_count << std::endl;
            std::cout << "at this level:" << std::endl;
            BPlusTreeUtils::print_array(this->data, this->data_count);
            std::cout << "child_count is not equal to data_count+1" << std::endl;
            return false;
        }
        // check if data is in range of children
        for (size_t i = 0; i < data_count; i++)
        {
            // check if data[i] is greater than subset[i]
            valid = BPlusTreeUtils::is_gt(subset[i]->data, subset[i]->data_count, data[i]);
            if (!valid)
            {
                std::cout << "data[i] is not greater than subset[i]" << std::endl;
                return false;
            }
            // check if data[i] is less than subset[i+1]
            valid = BPlusTreeUtils::is_le(subset[i + 1]->data, subset[i + 1]->data_count, data[i]);
            if (!valid)
            {
                std::cout << "if data[i] is not less than subset[i+1]]" << std::endl;
                return false;
            }
            // check if subset[i] is valid
            valid = subset[i]->is_valid();
            if (!valid)
            {
                std::cout << "not valid" << std::endl;
                return false;
            }
            // check if data[i] is the smallest node in subset[i+1]
            valid = data[i] == subset[i + 1]->get_smallest_node()->data[0];
            if (!valid)
            {
                std::cout << "smallest wrong" << std::endl;
                return false;
            }
        }
        // check if the last child is valid
        valid = subset[data_count]->is_valid();
        if (!valid)
        {
            std::cout << "child not valid" << std::endl;
            return false;
        }
    }
    return true;
}
// TODO
//****************************************************************************************************************
template <class Item>
bool BPlusTree<Item>::erase(const Item& entry)
{
    if (!this->loose_erase(entry))
        return false;
    // data_size < MINIMUM && subset_size > 0
    if (this->data_count < MINIMUM && this->child_count == MINIMUM)
    {
        BPlusTree<Item>* child = this->subset[0];
        BPlusTreeUtils::copy_array(this->data, child->data, this->data_count, child->data_count);
        BPlusTreeUtils::copy_array(this->subset, child->subset, this->child_count, child->child_count);
        child->data_count = 0;
        child->child_count = 0;
        delete child;
    }
    return true;
}

template <class Item>
void BPlusTree<Item>::get_smallest_item(Item& entry)
{
    if (this->is_leaf())
    {
        entry = this->data[0];
        return;
    }
    this->subset[0]->get_smallest_item(entry);
}

template <class Item>
bool BPlusTree<Item>::loose_erase(const Item& entry)
{
    size_t index = BPlusTreeUtils::first_ge(this->data, this->data_count, entry);
    bool found = (index < this->data_count) && !(entry < this->data[index]);
    bool flag = false;
    // leaf and not found
    if (this->is_leaf() && !found)
        return false;
    // leaf and found
    if (this->is_leaf() && found)
    {
        Item item;
        BPlusTreeUtils::delete_item(this->data, index, this->data_count, item);
        return true;
    }
    // internal and not found
    if (!this->is_leaf() && !found)
    {
        flag = this->subset[index]->loose_erase(entry);
        if (this->subset[index]->data_count < MINIMUM)
            this->fix_shortage(index);
    }

    // internal and found
    if (!this->is_leaf() && found)
    {
        flag = subset[index + 1]->loose_erase(entry);

        if (this->subset[index + 1]->data_count < MINIMUM)
        {
            // fix shortage returns the subset where it's fixed
            BPlusTree<Item>* fixed_subset = fix_shortage(index + 1);
            size_t data_idx = BPlusTreeUtils::first_ge(this->data, this->data_count, entry);
            size_t subset_idx = BPlusTreeUtils::first_ge(fixed_subset->data, fixed_subset->data_count, entry);

            // found in data
            bool found = data_idx < this->data_count && !(entry < this->data[data_idx]);
            if (data_idx < this->child_count - 1 && found)
            {
                // replace data with smallest
                this->subset[data_idx + 1]->get_smallest_item(this->data[data_idx]);
                return flag;
            }
            // found in fixed subset
            found = subset_idx < fixed_subset->data_count && !(entry < fixed_subset->data[subset_idx]);
            if (subset_idx < fixed_subset->child_count - 1 && found)
            {
                // replace data with smallest
                fixed_subset->subset[subset_idx + 1]->get_smallest_item(fixed_subset->data[subset_idx]);
                return flag;
            }
        }
        // no need to fix, just replace
        if (index < this->data_count && this->subset[index + 1]->data_count >= MINIMUM)
        {
            this->subset[index + 1]->get_smallest_item(this->data[index]);
            return flag;
        }
    }
    return flag;
}

template <class Item>
BPlusTree<Item>* BPlusTree<Item>::fix_shortage(std::size_t i)
{

    // transfer from left to right
    if (i > 0 && this->subset[i - 1]->data_count > MINIMUM)
    {
        this->transfer_from_left(i);
        return subset[i];
    }

    // transfer from right to left
    if (i < this->child_count - 1 && this->subset[i + 1]->data_count > MINIMUM)
    {
        this->transfer_from_right(i);
        return subset[i];
    }

    // merge subset[i - 1] with subset[i]
    if (i > 0 && this->subset[i - 1]->data_count <= MINIMUM) return this->merge_with_next_subset(i - 1);
    // merge subset[i] with subset[i + 1]
    if (i < this->child_count - 1 && this->subset[i + 1]->data_count <= MINIMUM) return this->merge_with_next_subset(i);

    // do nothing
    return subset[i];
}

template <class Item>
void BPlusTree<Item>::transfer_from_left(std::size_t i)
{
    BPlusTree<Item>* left_subset = this->subset[i - 1];
    BPlusTree<Item>* right_subset = this->subset[i];
    Item item;
    // left not leaf
    if (!left_subset->is_leaf())
    {
        // delete at root
        BPlusTreeUtils::delete_item(this->data, i - 1, this->data_count, item);
        BPlusTreeUtils::ordered_insert(right_subset->data, right_subset->data_count, item);
        //  delete the last element in left subset data
        BPlusTreeUtils::detach_item(left_subset->data, left_subset->data_count, item);
        BPlusTreeUtils::ordered_insert(this->data, this->data_count, item);

        BPlusTree<Item>* node;
        BPlusTreeUtils::detach_item(left_subset->subset, left_subset->child_count, node);
        BPlusTreeUtils::insert_item(right_subset->subset, 0, right_subset->child_count, node);

        return;
    }
    // left is leaf
    BPlusTreeUtils::detach_item(this->subset[i - 1]->data, this->subset[i - 1]->data_count, this->data[i - 1]);
    BPlusTreeUtils::insert_item(this->subset[i]->data, 0, this->subset[i]->data_count, data[i - 1]);
}

template <class Item>
void BPlusTree<Item>::transfer_from_right(std::size_t i)
{
    BPlusTree<Item>* left_subset = this->subset[i];
    BPlusTree<Item>* right_subset = this->subset[i + 1];
    Item item;

    // right not leaf
    if (!right_subset->is_leaf())
    {
        // delete data[i] in root
        BPlusTreeUtils::delete_item(this->data, i, this->data_count, item);
        BPlusTreeUtils::ordered_insert(left_subset->data, left_subset->data_count, item);
        // delete the first entry in right subset data
        BPlusTreeUtils::delete_item(right_subset->data, 0, right_subset->data_count, item);
        BPlusTreeUtils::ordered_insert(this->data, this->data_count, item);

        BPlusTree<Item>* node;
        BPlusTreeUtils::delete_item(right_subset->subset, 0, right_subset->child_count, node);
        BPlusTreeUtils::attach_item(left_subset->subset, left_subset->child_count, node);
        return;
    }
    // right is leaf
    Item entry;
    BPlusTreeUtils::delete_item(this->subset[i + 1]->data, 0, this->subset[i + 1]->data_count, entry);
    BPlusTreeUtils::attach_item(this->subset[i]->data, this->subset[i]->data_count, entry);
    this->data[i] = this->subset[i + 1]->data[0];
}

// Merge subset i with i+1
template <class Item>
BPlusTree<Item>* BPlusTree<Item>::merge_with_next_subset(std::size_t i)
{
    BPlusTree<Item>* left_subset = this->subset[i];
    BPlusTree<Item>* right_subset = this->subset[i + 1];

    Item item;
    BPlusTreeUtils::delete_item(this->data, i, this->data_count, item);
    // only attach when non left is not leaf
    if (!left_subset->is_leaf()) BPlusTreeUtils::attach_item(left_subset->data, left_subset->data_count, item);
    // merge
    BPlusTreeUtils::merge(this->subset[i]->data, this->subset[i]->data_count, this->subset[i + 1]->data, this->subset[i + 1]->data_count);
    BPlusTreeUtils::merge(this->subset[i]->subset, this->subset[i]->child_count, this->subset[i + 1]->subset, this->subset[i + 1]->child_count);

    BPlusTree<Item>* node;
    BPlusTreeUtils::delete_item(this->subset, i + 1, this->child_count, node);
    // relink if left is leaf
    if (left_subset->is_leaf())
        left_subset->next = node->next;
    node->child_count = 0;
    delete node;
    return subset[i];
}

#endif // BPLUSTREE_H
