#include "gtest/gtest.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
//------------------------------------------------------------------------------
// Files we are testing:
#include "../../includes/simple_kv.h"

//------------------------------------------------------------------------------

using namespace std;
const vector<string> OPTIONS = {
    "hello",
    "OK",
    "WTF",
    "???",
    "123",
    "COVID",
};

bool test_simpleKV1(bool debug)
{
    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");   
    SimpleKV<string, string> kv;
    
    EXPECT_FALSE(kv.EXISTS(OPTIONS[0]));
    kv.PUT(OPTIONS[0], OPTIONS[1]);
    bool check = kv.EXISTS(OPTIONS[0]);
    if(!check) return false;
    string val = kv.GET(OPTIONS[0]);
    if(val != OPTIONS[1]) return false;
    // putting same key but different value
    kv.PUT(OPTIONS[0], OPTIONS[2]);
    check = kv.EXISTS(OPTIONS[0]);
    if(!check) return false;
    val = kv.GET(OPTIONS[0]);
    if(val != OPTIONS[2]) return false;
    
    // removal
    kv.REMOVE(OPTIONS[0]);
    check = kv.EXISTS(OPTIONS[0]);
    EXPECT_FALSE(check);
    kv.REMOVE(OPTIONS[3]);
    if(debug) kv.DISPLAY();
    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");   
    return true;
}

// test load
bool test_simpleKV_stress1(bool debug)
{
    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");   
    SimpleKV<int, int> kv;
    int size = 10000;
    for(int i = 0; i < size; ++i)
    {
        kv.PUT(i, i * 2);
    }
    for(int i = 0; i < size; ++i)
    {
        int val = kv.GET(i);
        if(val != i * 2) return false;
    }
    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");   
    return true;
}



const bool debug = false;
TEST(TEST_SIMPLEKV, TestSimpleKV1)
{
    bool success = test_simpleKV1(debug);
    EXPECT_EQ(true, success);
}


TEST(TEST_SIMPLEKV_STRESS, TestSimpleKVStress1)
{
    bool success = test_simpleKV_stress1(debug);
    EXPECT_EQ(true, success);
}
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running test_simpleKV.cpp---------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}
