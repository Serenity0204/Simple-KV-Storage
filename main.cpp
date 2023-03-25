#include <iostream>
#include "includes/simple_kv.h"
using namespace std;

int main(int argc, char* argv[]) 
{
    cout << "success";
    return 0;
}

// #include <iostream>
// #include <fstream>
// #include <vector>

// using namespace std;

// struct Entry {
//     string _data;
//     string _key;
//     int _key_size;
//     int _data_size;

//     Entry() : _key_size(0), _data_size(0) {}
//     Entry(string key, string data) : _key(key), _data(data), _key_size(key.size()), _data_size(data.size()) {}
// };

// long long write_file(const Entry& entry) {
//     // Open the file for binary output, and move the put pointer to the end of the file.
//     fstream file("data.bin", ios::out | ios::binary | ios::app);
//     file.seekp(0, ios::end);
//     // Get the index at which the entry was written.
//     long long index = file.tellg();
//     // Write the key size and data size to the file.
//     int key_size = entry._key_size + 1;
//     int data_size = entry._data_size + 1;
//     file.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
//     file.write(reinterpret_cast<const char*>(&data_size), sizeof(data_size));

//     // Write the key and data strings to the file.
//     file.write(entry._key.c_str(), key_size);
//     file.write(entry._data.c_str(), data_size);

    
//     file.close();

//     return index;
// }


// Entry read_file(long long index) {
//     fstream file("data.bin", ios::in | ios::out | ios::binary);
//     Entry entry;

//     // move file pointer to the specified index
//     file.seekg(index);

//     // read entry from file
//     file.read(reinterpret_cast<char*>(&entry._key_size), sizeof(int));
//     file.read(reinterpret_cast<char*>(&entry._data_size), sizeof(int));

//     char* key = new char[entry._key_size];
//     char* data = new char[entry._data_size];

//     file.read(key, entry._key_size);
//     file.read(data, entry._data_size);

//     entry._key = key;
//     entry._data = data;

//     delete[] key;
//     delete[] data;

//     file.close();
//     return entry;
// }


// int main() {
//     remove("data.bin");
    
//     vector<Entry> entries = {
//         {"hello", "world"},
//         {"apple", "banana"},
//         {"red", "green"},
//         {"one", "two"},
//         {"cat", "dog"},
//         {"short", "data"},
//         {"a", "b"},
//         {"123456789", "0123456789abcdef"},
//         {"very long key", "very long data that takes up more than 16 bytes"}
//     };

//     // write entries to file
//     vector<long long> indexes;
//     for (const auto& entry : entries) {
//         auto index = write_file(entry);
//         indexes.push_back(index);
//         cout << "index:" << index << endl;
//     }

//     // read entries from file
//     for (const auto& index : indexes) {
//         Entry entry = read_file(index);
//         cout << "Key: " << entry._key << ", Data: " << entry._data << endl;
//     }
//     cout << "done" << endl;


//     // in.open("data.bin");
//     // if(in.good()) 
//     // {
//     //     cout << "removed done" << endl;
//     //     remove("data.bin");
//     // }
//     // in.close();
//     return 0;
// }