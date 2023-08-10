#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream>
#include <sstream>
#include <string>


// the object you want to serialize and deserialize has to overload the << and >> operators as friends
template <class T>
struct Serializer
{
public:
    Serializer() {}
    ~Serializer() {}
    static std::string serialize(const T& data);
    static T deserialize(const std::string& data);
};

template <class T>
std::string Serializer<T>::serialize(const T& data)
{
    std::ostringstream oss;
    oss << data;
    return oss.str();
}

template <class T>
T Serializer<T>::deserialize(const std::string& data)
{
    std::istringstream iss(data);
    T obj;
    iss >> obj;
    return obj;
}
#endif // SERIALIZER_H