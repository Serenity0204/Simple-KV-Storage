#include "gtest/gtest.h"
#include <iomanip>
#include <iostream>
#include <set>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#include <cstdlib>
#include <ctime>
//------------------------------------------------------------------------------
// Files we are testing:
#include "../../includes/BPlusTree/bplustree.h"

//------------------------------------------------------------------------------

using namespace std;

int randNumBig()
{
    int random_num = rand() % 1000001;
    return random_num;
}

bool test_load1(bool debug = false)
{
    vector<int> inserts;
    BPlusTree<int> bt;
    set<int> s;
    int size = 1000000;
    for (int i = size; i >= 0; --i)
    {
        int num = randNumBig();
        bt.insert(num);
        inserts.push_back(num);
        // s.insert(num);
    }
    // if (s.size() != bt.size()) return false;
    if (!bt.is_valid()) return false;
    for (int i = 0; i < inserts.size(); ++i)
    {
        int num = inserts[i];
        bt.erase(num);
    }
    if (!bt.is_valid()) return false;
    if (!bt.empty()) return false;
    if (bt.size() != 0) return false;
    return true;
}

// Lord help me!
const bool debug = false;

TEST(BASIC_TEST, TestBPlusTreeLoad)
{
    EXPECT_TRUE(test_load1(debug));
}

int main(int argc, char** argv)
{
    srand(time(NULL));
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running basic_test.cpp---------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}