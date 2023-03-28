#include "gtest/gtest.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
//------------------------------------------------------------------------------
//@TODO: include all files we are testing:
#include "../../includes/serializer/serializer.h"

//------------------------------------------------------------------------------

using namespace std;

class TestObj
{
private:
    int _x;
    std::string _y;
    std::string _z;

public:
    TestObj(int x = 0, string y = "test", string z = "obj") : _x(x), _y(y), _z(z) {}
    friend std::ostream& operator<<(std::ostream& os, const TestObj& obj)
    {
        os << obj._x << " " << obj._y << " " << obj._z;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, TestObj& obj)
    {
        is >> obj._x >> obj._y >> obj._z;
        return is;
    }

    // for testing, not necessary for other use cases
    friend bool operator==(const TestObj& left, const TestObj& right)
    {
        bool x_same = left._x == right._x;
        bool y_same = left._y == right._y;
        bool z_same = left._z == right._z;
        return x_same && y_same && z_same;
    }
    friend bool operator!=(const TestObj& left, const TestObj& right)
    {
        return !(left == right);
    }
};

// test custom type
bool test_serializer1(bool debug = false)
{
    Serializer<TestObj> serializer;
    vector<TestObj> objects;
    vector<string> strings;
    int size = 200;
    for (int i = 0; i < size; ++i)
    {
        TestObj obj = TestObj(i);
        string serialized = serializer.serialize(obj);
        objects.push_back(obj);
        strings.push_back(serialized);
    }
    bool same_size = (objects.size() == strings.size()) && (objects.size() == size);
    EXPECT_TRUE(same_size);
    for (int i = 0; i < size; ++i)
    {
        TestObj obj = serializer.deserialize(strings[i]);
        if (obj != objects[i])
        {
            if (debug)
            {
                cout << "Not the same object:" << endl;
                cout << obj << endl;
                cout << "Not equal to:" << endl;
                cout << objects[i] << endl;
            }
            return false;
        }
    }
    return true;
}

// test primative types double
bool test_serializer2(bool debug = false)
{
    Serializer<double> serializer;
    vector<double> objects;
    vector<string> strings;

    int size = 200;
    for (double i = 0; i < size; i += 0.5)
    {
        double obj = i;
        string serialized = serializer.serialize(obj);
        objects.push_back(obj);
        strings.push_back(serialized);
    }
    bool same_size = (objects.size() == strings.size()) && (objects.size() == size * 2);
    EXPECT_TRUE(same_size);
    for (int i = 0; i < size; ++i)
    {
        double obj = serializer.deserialize(strings[i]);
        if (obj != objects[i])
        {
            if (debug)
            {
                cout << "Not the same object:" << endl;
                cout << obj << endl;
                cout << "Not equal to:" << endl;
                cout << objects[i] << endl;
            }
            return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------

// Lord help me!
const bool debug = false;

TEST(TEST_SERIALIZER, TestSerializer1)
{
    bool success = test_serializer1(debug);
    EXPECT_EQ(success, true);
}

TEST(TEST_SERIALIZER, TestSerializer2)
{
    bool success = test_serializer2(debug);
    EXPECT_EQ(success, true);
}
//------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running test_serializer.cpp---------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}