#ifndef BINARY_FILE_IO_H
#define BINARY_FILE_IO_H

#include "entry.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class BinaryFileIO
{
private:
    static const string DB_FILE_PATH;
    static const string MERGE_FILE_PATH;

public:
    BinaryFileIO();
    ~BinaryFileIO();
    long long write_file(const Entry& entry);
    Entry read_file(long long index);
};

const string BinaryFileIO::DB_FILE_PATH = "simple_kv_db.data";
const string BinaryFileIO::MERGE_FILE_PATH = "simple_kv_db.merge";

BinaryFileIO::BinaryFileIO()
{
    ifstream in(BinaryFileIO::DB_FILE_PATH);
    if (!in.good())
    {
        ofstream out(BinaryFileIO::DB_FILE_PATH, ios::binary);
        out.close();
    }
    in.close();
    in.open(BinaryFileIO::MERGE_FILE_PATH);
    if (!in.good())
    {
        ofstream out(BinaryFileIO::MERGE_FILE_PATH, ios::binary);
        out.close();
    }
    in.close();
}

BinaryFileIO::~BinaryFileIO()
{
}

long long BinaryFileIO::write_file(const Entry& entry)
{
    // open the file for binary output, and move the put pointer to the end of the file.
    fstream file(BinaryFileIO::DB_FILE_PATH, ios::out | ios::binary | ios::app);
    file.seekp(0, ios::end);

    // get the index at which the entry was written.
    long long index = file.tellg();

    // write the key size and data size to the file. + 1 for '\0'
    int key_size = entry._key_size + 1;
    int data_size = entry._data_size + 1;

    // operation,key size,data size
    file.write(reinterpret_cast<const char*>(&entry._operation), sizeof(entry._operation));
    file.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
    file.write(reinterpret_cast<const char*>(&data_size), sizeof(data_size));

    // write the key and data strings to the file.
    file.write(entry._key.c_str(), key_size);
    file.write(entry._data.c_str(), data_size);
    file.close();
    return index;
}

Entry BinaryFileIO::read_file(long long index)
{
    fstream file(BinaryFileIO::DB_FILE_PATH, ios::in | ios::out | ios::binary);
    Entry entry;

    // move file pointer to the specified index
    file.seekg(index);

    // read entry from file
    file.read(reinterpret_cast<char*>(&entry._operation), sizeof(Operations));
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

#endif // BINARY_FILE_IO_H