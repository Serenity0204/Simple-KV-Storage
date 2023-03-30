#ifndef BINARY_FILE_IO_H
#define BINARY_FILE_IO_H

#include "../hash_table/hash_table.h"
#include "../serializer/serializer.h"
#include "entry.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum MODE
{
    MERGE_FILE,
    DB_FILE,
};

class BinaryFileIO
{
private:
    string _db_file_path;
    string _merge_file_path;

public:
    BinaryFileIO() {}
    BinaryFileIO(string db_file_path, string merge_file_path);
    ~BinaryFileIO();
    long long write_file(const Entry& entry, MODE mode = DB_FILE);
    Entry read_file(long long index, MODE mode = DB_FILE);
    vector<Entry> read_all(MODE mode = DB_FILE);

    template <class K>
    void dump_to_merge_file(const HashTable<K, long long>& cache);
    template <class K>
    void load_index(vector<HashRecord<K, long long>>& cache, vector<Operations>& operations, MODE mode = DB_FILE);
};

BinaryFileIO::BinaryFileIO(string db_file_path, string merge_file_path)
{
    this->_db_file_path = (db_file_path.length() == 0) ? "simple_kv_db.data" : db_file_path;
    this->_merge_file_path = (merge_file_path.length() == 0) ? "simple_kv_db.merge" : merge_file_path;

    ifstream in(this->_db_file_path, ios::binary);
    if (!in.good())
    {
        ofstream out(this->_db_file_path, ios::binary);
        out.close();
    }
    in.close();
    in.open(this->_merge_file_path, ios::binary);
    if (!in.good())
    {
        ofstream out(this->_merge_file_path, ios::binary);
        out.close();
    }
    in.close();
}

BinaryFileIO::~BinaryFileIO()
{
}

long long BinaryFileIO::write_file(const Entry& entry, MODE mode)
{
    // open the file for binary output, and move the put pointer to the end of the file.
    string file_path = (mode == DB_FILE) ? this->_db_file_path : this->_merge_file_path;
    fstream file(file_path, ios::out | ios::binary | ios::app);
    file.seekp(0, ios::end);

    // get the index at which the entry was written.
    long long index = file.tellg();

    // write the key size and data size to the file. + 1 for '\0'
    int key_size = entry._key_size + 1;
    int data_size = entry._data_size + 1;

    // operation,index,key size,data size
    file.write(reinterpret_cast<const char*>(&entry._operation), sizeof(Operations));
    file.write(reinterpret_cast<const char*>(&index), sizeof(long long));
    file.write(reinterpret_cast<const char*>(&key_size), sizeof(int));
    file.write(reinterpret_cast<const char*>(&data_size), sizeof(int));

    // write the key and data strings to the file.
    file.write(entry._key.c_str(), key_size);
    file.write(entry._data.c_str(), data_size);
    file.close();
    return index;
}

Entry BinaryFileIO::read_file(long long index, MODE mode)
{
    string file_path = (mode == DB_FILE) ? this->_db_file_path : this->_merge_file_path;
    fstream file(file_path, ios::in | ios::out | ios::binary);
    Entry entry;

    // move file pointer to the specified index
    file.seekg(index);

    // read entry from file
    file.read(reinterpret_cast<char*>(&entry._operation), sizeof(Operations));
    file.read(reinterpret_cast<char*>(&entry._index), sizeof(long long));
    file.read(reinterpret_cast<char*>(&entry._key_size), sizeof(int));
    file.read(reinterpret_cast<char*>(&entry._data_size), sizeof(int));

    // buffer
    char* key = new char[entry._key_size];
    char* data = new char[entry._data_size];

    // read key and data
    file.read(key, entry._key_size);
    file.read(data, entry._data_size);

    // buffer to entry
    entry._key = key;
    entry._data = data;

    // exclude null char
    if (entry._key_size > 0) entry._key_size--;
    if (entry._data_size > 0) entry._data_size--;

    // delete buffer
    delete[] key;
    delete[] data;

    file.close();

    return entry;
}

// merge
template <class K>
void BinaryFileIO::dump_to_merge_file(const HashTable<K, long long>& cache)
{
    // get the current indices
    vector<HashRecord<K, long long>> keys = cache.to_vector();
    // entries
    vector<Entry> entries;
    // loop through disks and read the entry at specified index
    for (int i = 0; i < keys.size(); ++i)
    {
        Entry entry = this->read_file(keys[i]._value);
        entries.push_back(entry);
    }
    // write to merge file
    for (int i = 0; i < entries.size(); ++i) this->write_file(entries[i], MERGE_FILE);
}

vector<Entry> BinaryFileIO::read_all(MODE mode)
{
    string file_path = (mode == DB_FILE) ? this->_db_file_path : this->_merge_file_path;
    fstream file(file_path, ios::in | ios::out | ios::binary);

    file.seekg(0, ios::end);
    int file_size = file.tellg();
    file.seekg(0, ios::beg);

    vector<Entry> entries;
    entries.clear();

    while (file.tellg() < file_size)
    {
        Entry entry;
        file.read(reinterpret_cast<char*>(&entry._operation), sizeof(Operations));
        file.read(reinterpret_cast<char*>(&entry._index), sizeof(long long));
        file.read(reinterpret_cast<char*>(&entry._key_size), sizeof(int));
        file.read(reinterpret_cast<char*>(&entry._data_size), sizeof(int));

        // buffer
        char* key = new char[entry._key_size];
        char* data = new char[entry._data_size];

        // read key and data
        file.read(key, entry._key_size);
        file.read(data, entry._data_size);

        // buffer to entry
        entry._key = key;
        entry._data = data;

        // exclude null char
        if (entry._key_size > 0) entry._key_size--;
        if (entry._data_size > 0) entry._data_size--;

        // delete buffer
        delete[] key;
        delete[] data;
        entries.push_back(entry);
    }

    return entries;
}

template <class K>
void BinaryFileIO::load_index(vector<HashRecord<K, long long>>& cache, vector<Operations>& operations, MODE mode)
{
    operations.clear();
    cache.clear();
    vector<Entry> entries = this->read_all(mode);
    for (int i = 0; i < entries.size(); ++i)
    {
        string pre_key = entries[i]._key;
        Operations op = entries[i]._operation;
        K key = Serializer<K>::deserialize(pre_key);
        long long index = entries[i]._index;
        HashRecord<K, long long> record(key, index);
        cache.push_back(record);
        operations.push_back(op);
    }
}
#endif // BINARY_FILE_IO_H