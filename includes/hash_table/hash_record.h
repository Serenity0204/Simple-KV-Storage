#ifndef HASH_RECORD_H
#define HASH_RECORD_H

#include <iostream>
using namespace std;

template <class T>
struct HashRecord
{
public:
    std::string key;
    T item;

    HashRecord(std::string key = "", T item = T()) : key(key), item(item) {}

    friend std::ostream& operator<<(std::ostream& outs, const HashRecord& h)
    {
        std::cout << "[" << h.key << ":" << h.item << "]";
        return outs;
    }

    friend bool operator<(const HashRecord& left, const HashRecord& right)
    {
        return left.key < right.key;
    }

    friend bool operator>(const HashRecord& left, const HashRecord& right)
    {
        return left.key > right.key;
    }

    friend bool operator==(const HashRecord& left, const HashRecord& right)
    {
        return left.key == right.key;
    }
};

#endif // HASH_RECORD_H