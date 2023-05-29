#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream>
#include <sstream>
#include <string>
using namespace std;


template <class T>
struct Serializer
        {
    // the object you want to serialize and deserialize has to overload the << and >> operators as friends
public:
    Serializer() {}
    ~Serializer() {}
    static string serialize(const T& data);
    static T deserialize(const string& data);
};

template <class T>
string Serializer<T>::serialize(const T& data)
{
    std::ostringstream oss;
    oss << data;
    return oss.str();
}

template <class T>
T Serializer<T>::deserialize(const string& data)
{
    std::istringstream iss(data);
    T obj;
    iss >> obj;
    return obj;
}
#endif // SERIALIZER_H