#ifndef SIMPLE_KV_H
#define SIMPLE_KV_H
#include "binary_file_io/binary_file_io.h"
#include "hash_table/hash_table.h"
#include "serializer/serializer.h"
using namespace std;

template <class T>
class Simple_KV
{
private:
    BinaryFileIO _io;
    HashTable<int> _table;
    Serializer<T> _serializer;

public:
    Simple_KV();
    ~Simple_KV();
    void put(string key, T value);
    T get(string key);
    bool exist(string key);
    void remove(string key);
    void display();
};

template <class T>
Simple_KV<T>::Simple_KV()
{
    this->_io = BinaryFileIO();
    this->_table = HashTable<int>();
    this->_serializer = Serializer<T>();
}

template <class T>
Simple_KV<T>::~Simple_KV()
{
}

template <class T>
void Simple_KV<T>::put(string key, T value)
{
}

template <class T>
T Simple_KV<T>::get(string key)
{
}

template <class T>
bool Simple_KV<T>::exist(string key)
{
    if (!this->_table.count(key)) return false;
    return true;
}

template <class T>
void Simple_KV<T>::remove(string key)
{
}

template <class T>
void Simple_KV<T>::display()
{
}

// load everything to cache;
// exclude the invalid stuff

#endif // SIMPLE_KV_H