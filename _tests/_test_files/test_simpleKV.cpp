#include "gtest/gtest.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
//------------------------------------------------------------------------------
// Files we are testing:
#include "../../includes/SimpleKV.h"
#include "../_test_utls/testing_objects.h"
//------------------------------------------------------------------------------

using namespace std;

bool test_simpleKV1(bool debug)
{
    remove("simple_kv_db1.merge");
    remove("simple_kv_db1.data");
    SimpleKV<string, string> kv;
    kv.CONNECT();

    EXPECT_FALSE(kv.EXISTS(OPTIONS[0]));
    kv.PUT(OPTIONS[0], OPTIONS[1]);
    bool check = kv.EXISTS(OPTIONS[0]);
    if (!check)
    {
        cout << "Exists 1 failed" << endl;
        return false;
    }
    string val = kv.GET(OPTIONS[0]);
    if (val != OPTIONS[1])
    {
        cout << "Value 1 error" << endl;
        return false;
    }
    // putting same key but different value
    kv.PUT(OPTIONS[0], OPTIONS[2]);
    check = kv.EXISTS(OPTIONS[0]);
    if (!check)
    {
        cout << "Exists 2 failed" << endl;
        return false;
    }
    val = kv.GET(OPTIONS[0]);
    if (val != OPTIONS[2])
    {
        cout << "Value 2 error" << endl;
        return false;
    }

    // // removal
    kv.REMOVE(OPTIONS[0]);
    check = kv.EXISTS(OPTIONS[0]);
    EXPECT_FALSE(check);
    kv.REMOVE(OPTIONS[3]);
    if (debug) kv.DISPLAY();

    kv.CLOSE();

    remove("simple_kv_db1.merge");
    remove("simple_kv_db1.data");
    return true;
}

// test load
bool test_simpleKV_stress1(bool debug)
{
    remove("simple_kv_db2.merge");
    remove("simple_kv_db2.data");
    SimpleKV<int, int> kv;
    kv.CONNECT();
    int size = 10000;
    for (int i = 0; i < size; ++i) kv.PUT(i, i * 2);

    for (int i = 0; i < size; ++i)
    {
        int val = kv.GET(i);
        if (val != i * 2) return false;
    }
    kv.CLOSE();
    remove("simple_kv_db2.merge");
    remove("simple_kv_db2.data");
    return true;
}

// test merge
bool test_simpleKV_merge1(bool debug)
{
    string db_file_path = "test_db.data3";
    string merge_file_path = "test_db.merge3";
    SimpleKV<int, int> kv(db_file_path, merge_file_path);
    if (!kv.CONNECT())
    {
        if (debug) cout << "connection issue!" << endl;
        return false;
    }
    int size = 1000;
    for (int i = 0; i < size; ++i) kv.PUT(i, i * 2);
    if (!kv.CLOSE())
    {
        if (debug) cout << "merge issue!" << endl;
        return false;
    }
    // cannot be called when not connect
    if (kv.EXISTS(10)) return false;

    if (!kv.CONNECT())
    {
        if (debug) cout << "connection issue!" << endl;
        return false;
    }
    // test get
    for (int i = 0; i < size; ++i)
    {
        int val = kv.GET(i);
        if (val != i * 2) return false;
    }

    if (debug)
    {
        cout << "merged db:" << endl;
        kv.DISPLAY();
        cout << endl;
    }
    // dump again
    if (!kv.CLOSE())
    {
        if (debug) cout << "merge issue!" << endl;
        return false;
    }
    return true;
}

// test objects key value
bool test_simpleKV_objects1(bool debug)
{
    string db_file_path = "test_db_obj1.data";
    string merge_file_path = "test_db_obj1.merge";
    SimpleKV<TestObjA, int> kv(db_file_path, merge_file_path);
    vector<TestObjA> objs;
    int size = 50;

    // test put
    if (!kv.CONNECT())
    {
        if (debug) cout << "connection issue!" << endl;
        return false;
    }
    for (int i = 0; i < size; ++i)
    {
        TestObjA obj(i);
        objs.push_back(obj);
        kv.PUT(obj, i);
    }
    if (!kv.CLOSE())
    {
        if (debug) cout << "merge issue!" << endl;
        return false;
    }

    // test get
    if (!kv.CONNECT())
    {
        if (debug) cout << "connection issue!" << endl;
        return false;
    }
    for (int i = 0; i < size; ++i)
    {
        int val = kv.GET(objs[i]);
        if (val != i) return false;
    }
    if (!kv.CLOSE())
    {
        if (debug) cout << "merge issue!" << endl;
        return false;
    }

    // test remove
    if (!kv.CONNECT())
    {
        if (debug) cout << "connection issue!" << endl;
        return false;
    }
    for (int i = 0; i < size; ++i) kv.REMOVE(objs[i]);
    if (!kv.EMPTY()) return false;
    if (!kv.CLOSE())
    {
        if (debug) cout << "merge issue!" << endl;
        return false;
    }

    // test empty
    if (!kv.CONNECT())
    {
        if (debug) cout << "connection issue!" << endl;
        return false;
    }
    if (!kv.EMPTY()) return false;
    if (!kv.CLOSE())
    {
        if (debug) cout << "merge issue!" << endl;
        return false;
    }
    return true;
}

// test objects key value
bool test_simpleKV_objects2(bool debug)
{
    string db_file_path = "test_db_obj2.data";
    string merge_file_path = "test_db_obj2.merge";
    SimpleKV<TestObjA, TestObjA> kv(db_file_path, merge_file_path);
    vector<TestObjA> objs;
    int size = 50;

    // test put
    if (!kv.CONNECT())
    {
        if (debug) cout << "connection issue!" << endl;
        return false;
    }
    for (int i = 0; i < size; ++i)
    {
        TestObjA obj(i);
        objs.push_back(obj);
        kv.PUT(obj, obj);
    }
    if (!kv.CLOSE())
    {
        if (debug) cout << "merge issue!" << endl;
        return false;
    }

    // test get
    if (!kv.CONNECT())
    {
        if (debug) cout << "connection issue!" << endl;
        return false;
    }
    for (int i = 0; i < size; ++i)
    {
        TestObjA obj = kv.GET(objs[i]);
        if (obj != objs[i]) return false;
    }
    if (!kv.CLOSE())
    {
        if (debug) cout << "merge issue!" << endl;
        return false;
    }

    // test remove
    if (!kv.CONNECT())
    {
        if (debug) cout << "connection issue!" << endl;
        return false;
    }
    for (int i = 0; i < size; ++i) kv.REMOVE(objs[i]);
    if (!kv.EMPTY()) return false;
    if (!kv.CLOSE())
    {
        if (debug) cout << "merge issue!" << endl;
        return false;
    }

    // test empty
    if (!kv.CONNECT())
    {
        if (debug) cout << "connection issue!" << endl;
        return false;
    }
    if (!kv.EMPTY()) return false;
    if (!kv.CLOSE())
    {
        if (debug) cout << "merge issue!" << endl;
        return false;
    }
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

TEST(TEST_SIMPLEKV_MERGE, TestSimpleKVMerge1)
{
    bool success = test_simpleKV_merge1(debug);
    EXPECT_EQ(true, success);
}

TEST(TEST_SIMPLEKV_OBJECTS, TestSimpleKVObjects1)
{
    bool success = test_simpleKV_objects1(debug);
    EXPECT_EQ(true, success);
}

TEST(TEST_SIMPLEKV_OBJECTS, TestSimpleKVObjects2)
{
    bool success = test_simpleKV_objects2(debug);
    EXPECT_EQ(true, success);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running test_simpleKV.cpp---------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}
