#ifndef SIMPLE_KV_H
#define SIMPLE_KV_H
#include "binary_file_io/binary_file_io.h"
#include "hash_table/hash_table.h"

template <class T>
class Simple_KV
{
private:
    BinaryFileIO _io;
    HashTable<T> _table;
public:
    Simple_KV();
    ~Simple_KV();
};

template <class T>
Simple_KV<T>::Simple_KV(/* args */)
{
}

template <class T>
Simple_KV<T>::~Simple_KV()
{
}

// load everything to cache;
// exclude the invalid stuff

#endif // SIMPLE_KV_H