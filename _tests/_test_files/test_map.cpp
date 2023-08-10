#include "gtest/gtest.h"
#include <iomanip>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>  /* printf, NULL */
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <unordered_map>
//------------------------------------------------------------------------------
//@TODO: include all files we are testing:
#include "../../includes/Map/map.h"

//------------------------------------------------------------------------------

using namespace std;
int randNum()
{
    return rand() % 10001;
}

template <class T>
bool is_same_vector(const vector<T>& v1, const vector<T>& v2)
{
    if (v1.size() != v2.size()) return false;
    for (int i = 0; i < v1.size(); ++i)
        if (v1[i] != v2[i]) return false;
    return true;
}
//------------------------------------------------------------------------------
//@TODO: add more test functions here:

bool test_map1(bool debug = false)
{
    unordered_map<int, string> standard_map;
    Map<int, string> map;
    // Add elements to the map
    int n = 100000;
    for (size_t i = 0; i < n; i++)
    {
        int rand_num = randNum();
        map[rand_num] = to_string(rand_num);
        standard_map[rand_num] = to_string(rand_num);
        if (map[rand_num] != standard_map[rand_num])
        {
            if (debug) cout << map << endl;
            return false;
        }
    }
    int i = 0;
    while (!map.empty())
    {
        map.erase(i);
        i++;
    }
    if (!map.empty()) return false;
    return true;
}


//--
//------------------------------------------------------------------------------

// Lord help me!
const bool debug = false;

TEST(TEST_MAP, TestMap1)
{
    bool success = test_map1(debug);
    EXPECT_EQ(true, success);
}

//------------------------------------------------------------------------------

int main(int argc, char** argv)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    srand(time(NULL));
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running testB.cpp---------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}
