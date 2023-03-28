#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "../avl/avl_tree.h"
#include "hash_record.h"
#include <cassert> // Provides assert
#include <cstdlib> // Provides size_t
#include <functional>
#include <iomanip>  // Provides std::setw
#include <iostream> // Provides std::cout

using namespace std;

template <class T>
class HashTable
{
public:
    // MEMBER CONSTANT
    // static const std::size_t CAPACITY = 10007;
    static const std::size_t CAPACITY = 811;
    // CONSTRUCTORS AND DESTRUCTOR
    HashTable();
    HashTable(const HashTable& source);
    ~HashTable();
    HashTable<T>& operator=(const HashTable<T>& source);
    // MODIFICATION MEMBER FUNCTIONS
    void insert(string key, T value);
    void remove(string key);
    // CONSTANT MEMBER FUNCTIONS
    bool count(string key) const;
    void clear();
    // subscript operator
    T& operator[](string key);
    const T& operator[](string key) const;

    std::size_t size() const { return _size; }
    std::size_t capacity() const { return HashTable<T>::CAPACITY; }
    // OVERLOAD OPERATOR FUNCTIONS
    template <class U>
    friend std::ostream& operator<<(std::ostream& outs, const HashTable<U>& hash);

private:
    // MEMBER VARIABLES
    AVL<HashRecord<T>> _data[CAPACITY];
    std::size_t _size;
    // HELPER MEMBER FUNCTION
    std::size_t _hash(string key) const
    {
        std::hash<std::string> h;
        return h(key) % HashTable<T>::CAPACITY;
    }
    void _find(string key, bool& found, HashRecord<T>& result) const;
    void _print_hash(std::ostream& outs = std::cout) const;
};

// Implementation MEMBER FUNCTIONS

// TODO
template <class T>
HashTable<T>::HashTable()
{
    for (int i = 0; i < HashTable<T>::CAPACITY; ++i) this->_data[i] = AVL<HashRecord<T>>();
    this->_size = 0;
}

template <class T>
HashTable<T>::HashTable(const HashTable<T>& source)
{
    this->_size = source._size;
    for (int i = 0; i < HashTable<T>::CAPACITY; ++i) this->_data[i] = source._data[i];
}

template <class T>
HashTable<T>::~HashTable() {}

template <class T>
HashTable<T>& HashTable<T>::operator=(const HashTable<T>& source)
{
    if (this == &source) return *this;
    this->_size = source._size;
    for (int i = 0; i < HashTable<T>::CAPACITY; ++i) this->_data[i] = source._data[i];
    return *this;
}

// MODIFICATION MEMBER FUNCTIONS
template <class T>
void HashTable<T>::insert(string key, T value)
{
    std::size_t index = this->_hash(key);
    bool found = false;
    HashRecord<T> entry(key);
    this->_find(key, found, entry);
    // if not found, then just insert
    if (!found)
    {
        this->_data[index].insert(entry);
        this->_size++;
        return;
    }
    // if found, then delete it first, then insert
    this->_data[index].erase(entry);
    this->_data[index].insert(entry);
}

template <class T>
void HashTable<T>::remove(string key)
{
    bool found = false;
    std::size_t index = this->_hash(key);
    HashRecord<T> temp(key);
    this->_find(key, found, temp);
    if (!found) return;
    // else found
    this->_data[index].erase(temp);
    this->_size--;
}
// CONSTANT MEMBER FUNCTIONS

template <class T>
bool HashTable<T>::count(string key) const
{
    bool found = false;
    HashRecord<T> temp(key);
    this->_find(key, found, temp);
    return found;
}

template <class T>
T& HashTable<T>::operator[](string key)
{
    std::size_t index = this->_hash(key);
    binary_tree_node<HashRecord<T>>* find = this->_data[index].search(HashRecord<T>(key));
    if (find == nullptr) this->insert(key, T());
    assert(this->count(key));
    return this->_data[index].search(HashRecord<T>(key))->data().item;
}

template <class T>
const T& HashTable<T>::operator[](string key) const
{
    std::size_t index = this->_hash(key);
    binary_tree_node<HashRecord<T>>* find = this->_data[index].search(HashRecord<T>(key));
    assert(find != nullptr);
    return this->_data[index].search(HashRecord<T>(key))->data().item;
}

template <class T>
void HashTable<T>::clear()
{
    this->_size = 0;
    for (int i = 0; i < HashTable<T>::CAPACITY; ++i) this->_data[i] = AVL<HashRecord<T>>();
}

template <class T>
void HashTable<T>::_find(string key, bool& found, HashRecord<T>& result) const
{
    std::size_t index = this->_hash(key);
    found = false;
    binary_tree_node<HashRecord<T>>* find = this->_data[index].search(HashRecord<T>(key));
    if (find == nullptr) return;
    result = find->data();
    found = true;
}

template <class U>
std::ostream& operator<<(std::ostream& outs, const HashTable<U>& hash)
{
    hash._print_hash(outs);
    return outs;
}

template <class T>
void HashTable<T>::_print_hash(std::ostream& outs) const
{
    for (int i = 0; i < HashTable<T>::CAPACITY; i++)
    {
        outs << '[' << std::setfill('0') << std::setw(3) << i << "] ";
        if (this->_data[i].root() == nullptr) outs << "empty";
        print_inorder(this->_data[i].root());
        outs << std::endl;
    }
    outs << "hashmap size:" << this->size() << endl;
}

#endif // HASH_TABLE_H