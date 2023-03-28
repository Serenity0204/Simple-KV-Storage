#ifndef HASH_RECORD_H
#define HASH_RECORD_H

#include <iostream>
using namespace std;

template <class K, class V>
struct HashRecord
{
public:
    K _key;
    V _value;

    HashRecord(K key = K(), V value = V()) : _key(key), _value(value) {}

    friend std::ostream& operator<<(std::ostream& outs, const HashRecord& h)
    {
        std::cout << "[" << h._key << ":" << h._value << "]";
        return outs;
    }
    template <class X, class Y>
    friend bool operator<(const HashRecord<X, Y>& left, const HashRecord<X, Y>& right);

    template <class X, class Y>
    friend bool operator>(const HashRecord<X, Y>& left, const HashRecord<X, Y>& right);

    template <class X, class Y>
    friend bool operator==(const HashRecord<X, Y>& left, const HashRecord<X, Y>& right);
};

template <class X, class Y>
bool operator<(const HashRecord<X, Y>& left, const HashRecord<X, Y>& right)
{
    return left._key < right._key;
}
template <class X, class Y>
bool operator>(const HashRecord<X, Y>& left, const HashRecord<X, Y>& right)
{
    return left._key > right._key;
}
template <class X, class Y>
bool operator==(const HashRecord<X, Y>& left, const HashRecord<X, Y>& right)
{
    return left._key == right._key;
}

#endif // HASH_RECORD_H