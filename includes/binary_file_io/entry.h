#ifndef ENTRY_H
#define ENTRY_H

#include <iostream>
#include <string>
using namespace std;


enum Operations
{
    DELETE,
    INSERT,
};

struct Entry
{
    // entry format:
    // _operation _key_size _data_size _key _data
public:
    string _data;
    string _key;
    int _key_size;
    int _data_size;
    Operations _operation;
    Entry();
    Entry(string key, string data, Operations operation);
    ~Entry();
    void set_key(string key);
    void set_data(string data);
    friend std::ostream& operator << (std::ostream& outs, const Entry& print_me);
};

Entry::Entry()
{
    this->_data = "";
    this->_key = "";
    this->_key_size = 0;
    this->_data_size = 0;
    this->_operation = INSERT;
}

Entry::Entry(string key, string data, Operations operation)
{
    this->_operation = operation;
    this->set_key(key);
    this->set_data(data);
}

Entry::~Entry()
{
}

void Entry::set_key(string key)
{
    this->_key = key;
    this->_key_size = key.length();
}

void Entry::set_data(string data)
{
    this->_data = data;
    this->_data_size = data.length();
}

std::ostream& operator << (std::ostream& outs, const Entry& print_me)
{
    outs << "key:" << print_me._key << ",size:" << print_me._key_size << endl;
    outs << "data:" << print_me._data << ",size:" << print_me._data_size << endl;
    outs << "operation:";
    if(print_me._operation == INSERT) outs << "INSERT" << endl;
    if(print_me._operation == DELETE) outs << "DELETE" << endl;
    return outs;
}

#endif // ENTRY_H