#ifndef PAIR_H
#define PAIR_H
#include <iostream>

template <typename K, typename V>
struct Pair
{
    K key;
    V value;

    Pair(const K& k = K(), const V& v = V())
        : key(k), value(v)
    {
    }
    friend std::ostream& operator<<(std::ostream& outs, const Pair<K, V>& print_me)
    {
        outs << print_me.key << ":" << print_me.value << std::endl;
        return outs;
    }
    friend bool operator==(const Pair<K, V>& lhs, const Pair<K, V>& rhs) { return lhs.key == rhs.key; }
    friend bool operator<(const Pair<K, V>& lhs, const Pair<K, V>& rhs) { return lhs.key < rhs.key; }
    friend bool operator>(const Pair<K, V>& lhs, const Pair<K, V>& rhs) { return lhs.key > rhs.key; }
    friend bool operator<=(const Pair<K, V>& lhs, const Pair<K, V>& rhs) { return lhs.key <= rhs.key; }
};

#endif // PAIR_H