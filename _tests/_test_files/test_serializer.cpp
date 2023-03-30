#include "gtest/gtest.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
//------------------------------------------------------------------------------
//@TODO: include all files we are testing:
#include "../../includes/serializer/serializer.h"
#include "../_test_utls/testing_objects.h"
//------------------------------------------------------------------------------

using namespace std;

// test custom type
bool test_serializer1(bool debug = false)
{
    vector<TestObjA> objects;
    vector<string> strings;
    int size = 200;
    for (int i = 0; i < size; ++i)
    {
        TestObjA obj = TestObjA(i);
        string serialized = Serializer<TestObjA>::serialize(obj);
        objects.push_back(obj);
        strings.push_back(serialized);
    }
    bool same_size = (objects.size() == strings.size()) && (objects.size() == size);
    EXPECT_TRUE(same_size);
    for (int i = 0; i < size; ++i)
    {
        TestObjA obj = Serializer<TestObjA>::deserialize(strings[i]);
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
    vector<double> objects;
    vector<string> strings;

    int size = 200;
    for (double i = 0; i < size; i += 0.5)
    {
        double obj = i;
        string serialized = Serializer<double>::serialize(obj);
        objects.push_back(obj);
        strings.push_back(serialized);
    }
    bool same_size = (objects.size() == strings.size()) && (objects.size() == size * 2);
    EXPECT_TRUE(same_size);
    for (int i = 0; i < size; ++i)
    {
        double obj = Serializer<double>::deserialize(strings[i]);
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