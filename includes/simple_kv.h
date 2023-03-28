#ifndef SIMPLE_KV_H
#define SIMPLE_KV_H
#include "binary_file_io/binary_file_io.h"
#include "hash_table/hash_table.h"
#include "serializer/serializer.h"
using namespace std;

template <class K, class V>
class Simple_KV
{
private:
    BinaryFileIO _io;
    HashTable<K, int> _table;
    Serializer<K> _key_serializer;
    Serializer<V> _value_serializer;

public:
    Simple_KV();
    ~Simple_KV();
    void put(K key, V value);
    V get(K key);
    bool exist(K key);
    void remove(K key);
    void display();
};

template <class K, class V>
Simple_KV<K, V>::Simple_KV()
{
    this->_io = BinaryFileIO();
    this->_table = HashTable<K, int>();
    this->_key_serializer = Serializer<K>();
    this->_key_serializer = Serializer<V>();
}

template <class K, class V>
Simple_KV<K, V>::~Simple_KV()
{
}

template <class K, class V>
void Simple_KV<K, V>::put(K key, V value)
{
}

template <class K, class V>
V Simple_KV<K, V>::get(K key)
{
}

template <class K, class V>
bool Simple_KV<K, V>::exist(K key)
{
    if (!this->_table.count(key)) return false;
    return true;
}

template <class K, class V>
void Simple_KV<K, V>::remove(K key)
{
}

template <class K, class V>
void Simple_KV<K, V>::display()
{
}

// load everything to cache;
// exclude the invalid stuff

#endif // SIMPLE_KV_H