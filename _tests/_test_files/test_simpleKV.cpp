#include "gtest/gtest.h"
#include <iomanip>
#include <iostream>

//------------------------------------------------------------------------------
// Files we are testing:
#include "../../includes/simple_kv.h"

//------------------------------------------------------------------------------

using namespace std;

bool test_simpleKV1(bool debug)
{
    
    return true;
}

const bool debug = false;
TEST(TEST_SIMPLEKV, TestSimpleKV1)
{
    bool success = test_simpleKV1(debug);
    EXPECT_EQ(true, success);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running test_simpleKV.cpp---------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}
