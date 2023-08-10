#ifndef SIMPLEKV_H
#define SIMPLEKV_H
#include "binary_file_io/binary_file_io.h"
#include "map/map.h"
#include "serializer/serializer.h"

template <class K, class V>
class SimpleKV
{
private:
    // disk io
    BinaryFileIO _io;
    std::string _db_file_path;
    std::string _merge_file_path;
    // indexing
    Map<K, long long> _table;
    void _load_index();

public:
    SimpleKV(std::string db_file_path = "simple_kv_db.data", std::string merge_file_path = "simple_kv_db.merge");
    ~SimpleKV() {}
    bool CONNECT();
    bool CLOSE();
    void PUT(K key, V value);
    V GET(K key);
    bool EXISTS(K key);
    void REMOVE(K key);
    void DISPLAY();
    bool EMPTY() { return this->_table.empty(); }
    size_t SIZE() { return this->_table.size(); }
    // only be called when file path duplicates
    // bool RELOCATE_DB(std::string db_file_path = "simple_kv_db.data", std::string merge_file_path = "simple_kv_db.merge");
};

template <class K, class V>
SimpleKV<K, V>::SimpleKV(std::string db_file_path, std::string merge_file_path)
    : _db_file_path(db_file_path), _merge_file_path(merge_file_path)
{
}

// // only be called when file path duplicates
// template <class K, class V>
// bool SimpleKV<K, V>::RELOCATE_DB(std::string db_file_path, std::string merge_file_path)
// {
//     if (db_file_path == merge_file_path) return false;
//     this->_db_file_path = db_file_path;
//     this->_merge_file_path = merge_file_path;
//     return true;
// }

template <class K, class V>
bool SimpleKV<K, V>::CONNECT()
{
    if (this->_db_file_path == this->_merge_file_path) return false;
    this->_io = BinaryFileIO(this->_db_file_path, this->_merge_file_path);
    this->_table = Map<K, long long>();
    this->_load_index();
    return true;
}

// call io.dump_to_merge_file when destructed
template <class K, class V>
bool SimpleKV<K, V>::CLOSE()
{
    this->_io.dump_to_merge_file(this->_table);
    int guard = remove(this->_db_file_path.c_str());
    if (guard == -1) return false;
    guard = rename(this->_merge_file_path.c_str(), this->_db_file_path.c_str());
    if (guard == -1) return false;
    this->_table.clear();
    return true;
}

template <class K, class V>
void SimpleKV<K, V>::PUT(K key, V value)
{
    std::string entry_key = Serializer<K>::serialize(key);
    std::string entry_data = Serializer<V>::serialize(value);
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
    return this->_table.contains(key);
}

template <class K, class V>
void SimpleKV<K, V>::REMOVE(K key)
{
    if (!this->EXISTS(key)) return;
    V value = this->GET(key);
    std::string entry_key = Serializer<K>::serialize(key);
    std::string entry_data = Serializer<V>::serialize(value);
    Entry entry(entry_key, entry_data, DELETE);
    this->_io.write_file(entry);
    this->_table.erase(key);
}

template <class K, class V>
void SimpleKV<K, V>::DISPLAY()
{
    typename Map<K, long long>::Iterator it;
    if (this->_table.empty())
    {
        std::cout << "Empty" << std::endl;
        return;
    }

    for (it = this->_table.begin(); it != this->_table.end(); ++it)
    {
        assert(!it.is_null());
        Pair<K, long long> p = (*it);
        long long index = p.value;
        // loop through disks and read the entry at specified index
        Entry entry = this->_io.read_file(index);
        std::cout << "{key:" << entry._key << ", value:" << entry._data << "}" << std::endl;
    }
}

template <class K, class V>
void SimpleKV<K, V>::_load_index()
{
    this->_io.load_index(this->_table);
}

#endif // SIMPLEKV_H