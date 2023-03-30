#ifndef SIMPLE_KV_H
#define SIMPLE_KV_H
#include "binary_file_io/binary_file_io.h"
#include "hash_table/hash_table.h"
#include "serializer/serializer.h"
using namespace std;

template <class K, class V>
class SimpleKV
{
private:
    BinaryFileIO _io;
    HashTable<K, long long> _table;
    void _load_index();

public:
    SimpleKV();
    ~SimpleKV();
    void PUT(K key, V value);
    V GET(K key);
    bool EXISTS(K key);
    void REMOVE(K key);
    void DISPLAY();
};

template <class K, class V>
SimpleKV<K, V>::SimpleKV()
{
    this->_io = BinaryFileIO();
    this->_table = HashTable<K, long long>();
    this->_load_index();
}

// call io.dump_to_merge_file when destructed
template <class K, class V>
SimpleKV<K, V>::~SimpleKV()
{
}

template <class K, class V>
void SimpleKV<K, V>::PUT(K key, V value)
{
    string entry_key = Serializer<K>::serialize(key);
    string entry_data = Serializer<V>::serialize(value);
    Entry entry(entry_key, entry_data, INSERT);
    long long index = this->_io.write_file(entry);
    this->_table.insert(key, index);
}

template <class K, class V>
V SimpleKV<K, V>::GET(K key)
{
    assert(this->EXISTS(key));
    long long index = this->_table[key];
    Entry entry = this->_io.read_file(index);
    V value = Serializer<V>::deserialize(entry._data);
    return value;
}

template <class K, class V>
bool SimpleKV<K, V>::EXISTS(K key)
{
    if (!this->_table.count(key)) return false;
    return true;
}

template <class K, class V>
void SimpleKV<K, V>::REMOVE(K key)
{
    if (!this->EXISTS(key)) return;
    V value = this->GET(key);
    string entry_key = Serializer<K>::serialize(key);
    string entry_data = Serializer<V>::serialize(value);
    Entry entry(entry_key, entry_data, DELETE);
    this->_io.write_file(entry);
    this->_table.remove(key);
}

template <class K, class V>
void SimpleKV<K, V>::DISPLAY()
{
    vector<HashRecord<K, long long>> all_records = this->_table.to_vector();
    if(all_records.size() == 0)
    {
        cout << "Empty" << endl;
        return;
    }
    for(int i = 0; i < all_records.size(); ++i)
    {
        long long index = all_records[i]._value;
        Entry entry = this->_io.read_file(index);
        cout << "{key:" << entry._key << ", value:" <<entry._data << "}" << endl;
    }
}

template <class K, class V>
void SimpleKV<K, V>::_load_index()
{
    vector<HashRecord<K, long long>> cache;
    vector<Operations> operations;
    cache.clear();
    operations.clear();

    this->_io.load_index(cache, operations);
    for (int i = 0; i < cache.size(); ++i)
    {
        K key = cache[i]._key;
        long long index = cache[i]._value;
        Operations op = operations[i];
        if (op == INSERT) this->_table.insert(key, index);
        if (op == DELETE) this->_table.remove(key);
    }
    // cout << this->_table << endl;
}

// load everything to cache;
// exclude the invalid stuff

#endif // SIMPLE_KV_H