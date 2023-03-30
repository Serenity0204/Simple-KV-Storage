#ifndef TESTING_OBJECTS_H
#define TESTING_OBJECTS_H
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const vector<string> OPTIONS = {
    "hello",
    "OK",
    "WTF",
    "???",
    "123",
    "COVID",
};

class TestObjA
{
public:
    int _x;
    std::string _y;
    std::string _z;
    TestObjA(int x = 0, string y = "test", string z = "obj") : _x(x), _y(y), _z(z) {}
    friend std::ostream& operator<<(std::ostream& os, const TestObjA& obj)
    {
        os << obj._x << " " << obj._y << " " << obj._z;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, TestObjA& obj)
    {
        is >> obj._x >> obj._y >> obj._z;
        return is;
    }

    // required for using hash
    friend bool operator==(const TestObjA& left, const TestObjA& right)
    {
        bool x_same = left._x == right._x;
        bool y_same = left._y == right._y;
        bool z_same = left._z == right._z;
        return x_same && y_same && z_same;
    }
    friend bool operator!=(const TestObjA& left, const TestObjA& right)
    {
        return !(left == right);
    }
    friend bool operator<(const TestObjA& left, const TestObjA& right)
    {
        return left._x < right._x;
    }
    friend bool operator>(const TestObjA& left, const TestObjA& right)
    {
        return left._x > right._x;
    }
};

// required for hash
namespace std
{
    template <>
    struct hash<TestObjA>
    {
        std::size_t operator()(const TestObjA& obj) const
        {
            return std::hash<int>()(obj._x) ^ std::hash<string>()(obj._y) ^ std::hash<string>()(obj._z);
        }
    };
}
#endif // TESTING_OBJECTS_H