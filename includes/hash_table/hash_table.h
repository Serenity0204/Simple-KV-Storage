#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "../avl/avl_tree.h"
#include "hash_record.h"
#include <cassert> // Provides assert
#include <cstdlib> // Provides size_t
#include <functional>
#include <iomanip>  // Provides std::setw
#include <iostream> // Provides std::cout
#include <vector>

using namespace std;

template <class K, class V>
class HashTable
{
public:
    // MEMBER CONSTANT
    static const std::size_t CAPACITY = 10007;
    // CONSTRUCTORS AND DESTRUCTOR
    HashTable();
    HashTable(const HashTable<K, V>& source);
    ~HashTable();
    HashTable<K, V>& operator=(const HashTable<K, V>& source);
    // MODIFICATION MEMBER FUNCTIONS
    void insert(K key, V value);
    void remove(K key);
    // CONSTANT MEMBER FUNCTIONS
    bool count(K key) const;
    void clear();
    // subscript operator
    V& operator[](K key);
    const V& operator[](K key) const;
    bool empty() const { return this->_size == 0; }
    std::size_t size() const { return _size; }
    // OVERLOAD OPERATOR FUNCTIONS
    template <class X, class Y>
    friend std::ostream& operator<<(std::ostream& outs, const HashTable<X, Y>& hash);
    vector<HashRecord<K, V>> to_vector() const;

private:
    // MEMBER VARIABLES
    AVL<HashRecord<K, V>> _data[CAPACITY];
    std::size_t _size;
    // HELPER MEMBER FUNCTION
    std::size_t _hash(K key) const
    {
        return std::hash<K>()(key) % HashTable<K, V>::CAPACITY;
    }
    void _find(K key, bool& found, HashRecord<K, V>& result) const;
    void _print_hash(std::ostream& outs = std::cout) const;
};

// Implementation MEMBER FUNCTIONS

// TODO
template <class K, class V>
HashTable<K, V>::HashTable()
{
    for (int i = 0; i < HashTable<K, V>::CAPACITY; ++i) this->_data[i] = AVL<HashRecord<K, V>>();
    this->_size = 0;
}

template <class K, class V>
HashTable<K, V>::HashTable(const HashTable<K, V>& source)
{
    this->_size = source._size;
    for (int i = 0; i < HashTable<K, V>::CAPACITY; ++i) this->_data[i] = source._data[i];
}

template <class K, class V>
HashTable<K, V>::~HashTable() {}

template <class K, class V>
HashTable<K, V>& HashTable<K, V>::operator=(const HashTable<K, V>& source)
{
    if (this == &source) return *this;
    this->_size = source._size;
    for (int i = 0; i < HashTable<K, V>::CAPACITY; ++i) this->_data[i] = source._data[i];
    return *this;
}

// MODIFICATION MEMBER FUNCTIONS
template <class K, class V>
void HashTable<K, V>::insert(K key, V value)
{
    std::size_t index = this->_hash(key);
    bool found = false;
    HashRecord<K, V> entry(key);
    this->_find(key, found, entry);

    // set the insert entry to have the key and value we want
    entry._key = key;
    entry._value = value;
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

template <class K, class V>
void HashTable<K, V>::remove(K key)
{
    bool found = false;
    std::size_t index = this->_hash(key);
    HashRecord<K, V> temp(key);
    this->_find(key, found, temp);
    if (!found) return;
    // else found
    this->_data[index].erase(temp);
    this->_size--;
}
// CONSTANT MEMBER FUNCTIONS

template <class K, class V>
bool HashTable<K, V>::count(K key) const
{
    bool found = false;
    HashRecord<K, V> temp(key);
    this->_find(key, found, temp);
    return found;
}

template <class K, class V>
V& HashTable<K, V>::operator[](K key)
{
    std::size_t index = this->_hash(key);
    binary_tree_node<HashRecord<K, V>>* find = this->_data[index].search(HashRecord<K, V>(key));
    if (find == nullptr) this->insert(key, V());
    assert(this->count(key));
    return this->_data[index].search(HashRecord<K, V>(key))->data()._value;
}

template <class K, class V>
const V& HashTable<K, V>::operator[](K key) const
{
    std::size_t index = this->_hash(key);
    binary_tree_node<HashRecord<K, V>>* find = this->_data[index].search(HashRecord<K, V>(key));
    assert(find != nullptr);
    return this->_data[index].search(HashRecord<K, V>(key))->data()._value;
}

template <class K, class V>
void HashTable<K, V>::clear()
{
    this->_size = 0;
    for (int i = 0; i < HashTable<K, V>::CAPACITY; ++i) this->_data[i] = AVL<HashRecord<K, V>>();
}

template <class K, class V>
void HashTable<K, V>::_find(K key, bool& found, HashRecord<K, V>& result) const
{
    std::size_t index = this->_hash(key);
    found = false;
    binary_tree_node<HashRecord<K, V>>* find = this->_data[index].search(HashRecord<K, V>(key));
    if (find == nullptr) return;
    result = find->data();
    found = true;
}

template <class X, class Y>
std::ostream& operator<<(std::ostream& outs, const HashTable<X, Y>& hash)
{
    hash._print_hash(outs);
    return outs;
}

template <class K, class V>
void HashTable<K, V>::_print_hash(std::ostream& outs) const
{
    for (int i = 0; i < HashTable<K, V>::CAPACITY; i++)
    {
        outs << '[' << std::setfill('0') << std::setw(3) << i << "] ";
        if (this->_data[i].root() == nullptr) outs << "empty";
        print_inorder(this->_data[i].root());
        outs << std::endl;
    }
    outs << "hashmap size:" << this->size() << endl;
}

template <class K, class V>
vector<HashRecord<K, V>> HashTable<K, V>::to_vector() const
{
    vector<HashRecord<K, V>> items;
    items.clear();
    for (int i = 0; i < HashTable<K, V>::CAPACITY; ++i)
    {
        if (this->_data[i].root() == nullptr) continue;
        vector_inorder(this->_data[i].root(), items);
    }
    return items;
}

#endif // HASH_TABLE_H