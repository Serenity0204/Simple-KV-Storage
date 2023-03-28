#include "gtest/gtest.h"
#include <iomanip>
#include <iostream>

//------------------------------------------------------------------------------
// Files we are testing:
#include "../../includes/avl/avl_tree.h"

//------------------------------------------------------------------------------

using namespace std;

template <class Item>
bool isAVL(binary_tree_node<Item>* root)
{
    // base case
    if (root == nullptr) return true;
    // balance factor of root be -1, 0 , 1
    if (root->balance_factor() < -1 || root->balance_factor() > 1) return false;
    // traverse entire tree
    return isAVL(root->left()) && isAVL(root->right());
}

bool test_avl_load1(bool debug = false)
{
    AVL<int> avl;
    for (int i = 0; i < 10000; ++i)
    {
        if (i % 2 == 0)
        {
            avl.insert(i);
            binary_tree_node<int>* find = avl.search(i);
            if (find == nullptr) return false;
            if (find->data() != i) return false;
            if (!isAVL(avl.root())) return false;
            continue;
        }
        avl.erase(i);
        if (!isAVL(avl.root())) return false;
    }
    if (debug)
    {
        cout << avl << endl;
    }
    for (int i = 0; i < 10000; ++i)
    {
        if (i % 2 == 1)
        {
            avl.insert(i);
            binary_tree_node<int>* find = avl.search(i);
            if (find == nullptr) return false;
            if (find->data() != i) return false;
            if (!isAVL(avl.root())) return false;
            continue;
        }
        avl.erase(i);
        if (!isAVL(avl.root())) return false;
    }

    for (int i = 0; i < 10000; ++i)
    {
        if (i % 2 == 1)
        {
            avl.erase(i);
            binary_tree_node<int>* find = avl.search(i);
            if (find != nullptr) return false;
            if (!isAVL(avl.root())) return false;
        }
    }
    if (avl.empty()) return true;
    return false;
}

bool test_avl_size1(bool debug = false)
{
    AVL<int> avl;
    for (int i = 0; i < 1000; ++i)
    {
        avl.insert(i);
        binary_tree_node<int>* find = avl.search(i);
        if (find == nullptr) return false;
        if (find->data() != i) return false;
        if (!isAVL(avl.root())) return false;
    }
    if (debug)
    {
        cout << avl << endl;
    }
    for (int i = 0; i < 200; ++i)
    {
        avl.erase(i);
        binary_tree_node<int>* find = avl.search(i);
        if (find != nullptr) return false;
        if (!isAVL(avl.root())) return false;
    }
    EXPECT_EQ(avl.size(), 800);
    avl.clear_all();
    if (avl.empty()) return true;

    return false;
}

bool test_avl_size2(bool debug = false)
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = 10;
    AVL<int> avl(arr, size);
    if (debug)
    {
        cout << avl << endl;
    }
    EXPECT_EQ(avl.size(), size);
    avl.clear_all();
    if (avl.empty()) return true;
    return false;
}




const bool debug = false;
TEST(TEST_AVL_LOAD, TestAVLLoad1)
{
    bool success = test_avl_load1(debug);
    EXPECT_EQ(true, success);
}

TEST(TEST_AVL_SIZE, TestAVLSize1)
{
    bool success = test_avl_size1(debug);
    EXPECT_EQ(true, success);
}

TEST(TEST_AVL_SIZE, TestAVLSize2)
{
    bool success = test_avl_size2(debug);
    EXPECT_EQ(true, success);
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running test_avl.cpp---------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}
