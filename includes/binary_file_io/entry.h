#ifndef ENTRY_H
#define ENTRY_H

#include <iostream>
#include <string>

enum Operations
{
    DELETE,
    INSERT,
};

struct Entry
{
    // entry format:
    // _operation, _index, _key_size, _data_size, _key, _data
public:
    std::string _data;
    std::string _key;
    long long _index;
    int _key_size;
    int _data_size;
    Operations _operation;
    Entry(std::string key = "", std::string data = "", Operations operation = INSERT, long long index = 0);
    ~Entry();
    void set_key(std::string key);
    void set_data(std::string data);

    // friends
    friend bool operator==(const Entry& left, const Entry& right);
    friend bool operator!=(const Entry& left, const Entry& right);
    friend std::ostream& operator<<(std::ostream& outs, const Entry& print_me);
};

Entry::Entry(std::string key, std::string data, Operations operation, long long index)
{
    this->_operation = operation;
    this->_index = index;
    this->set_key(key);
    this->set_data(data);
}

Entry::~Entry()
{
}

void Entry::set_key(std::string key)
{
    this->_key = key;
    this->_key_size = key.length();
}

void Entry::set_data(std::string data)
{
    this->_data = data;
    this->_data_size = data.length();
}

std::ostream& operator<<(std::ostream& outs, const Entry& print_me)
{
    outs << "key:" << print_me._key << std::endl;
    outs << "key size:" << print_me._key_size << std::endl;
    outs << "data:" << print_me._data << std::endl;
    outs << "data size:" << print_me._data_size << std::endl;
    outs << "operation:";
    if (print_me._operation == INSERT) outs << "INSERT" << std::endl;
    if (print_me._operation == DELETE) outs << "DELETE" << std::endl;
    outs << "file index:" << print_me._index << std::endl;
    return outs;
}

bool operator==(const Entry& left, const Entry& right)
{
    bool data_check = left._data == right._data;
    bool key_check = left._key == right._key;
    return data_check && key_check;
}
bool operator!=(const Entry& left, const Entry& right)
{
    return !(left == right);
}
#endif // ENTRY_H