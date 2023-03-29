#include "gtest/gtest.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
//------------------------------------------------------------------------------
// Files we are testing:
#include "../../includes/binary_file_io/binary_file_io.h"
#include "../../includes/hash_table/hash_table.h"
//------------------------------------------------------------------------------

using namespace std;

bool test_fileIO1(bool debug = false)
{
    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");

    BinaryFileIO IO;
    vector<Entry> entries = {
        {"hello", "world", INSERT},
        {"apple", "banana", DELETE},
        {"red", "green", INSERT},
        {"one", "two", INSERT},
        {"cat", "dog", DELETE},
        {"short", "data", INSERT},
        {"a", "b", DELETE},
        {"123456789", "0123456789abcdef", INSERT},
        {"very long key", "very long data that takes up more than 16 bytes", DELETE},
        {"empty data", "", INSERT},
        {"", "empty key", DELETE},
    };

    // write entries to file
    vector<long long> indices;
    for (const auto& entry : entries)
    {
        long long index = IO.write_file(entry);
        indices.push_back(index);
        if (debug) cout << "index:" << index << endl;
    }
    if (debug) cout << endl;

    // read entries from file
    for (int i = 0; i < indices.size(); ++i)
    {
        Entry entry = IO.read_file(indices[i]);
        if (entry != entries[i]) return false;
        if (debug) cout << entry << endl;
    }

    // real all
    if (debug) cout << "read all:" << endl;
    vector<Entry> all_entries = IO.read_all();
    for (int i = 0; i < all_entries.size(); ++i)
    {
        if (all_entries[i] != entries[i]) return false;
        if (debug) cout << all_entries[i] << endl;
    }

    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");
    return true;
}

bool test_fileIO2(bool debug = false)
{
    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");

    BinaryFileIO IO;
    vector<Entry> entries = {
        {"1", "1", INSERT},
        {"2", "2", DELETE},
        {"3", "3", INSERT},
        {"4", "4", DELETE},
    };

    // write entries to file
    vector<long long> indices;
    for (const auto& entry : entries)
    {
        long long index = IO.write_file(entry);
        indices.push_back(index);
        if (debug) cout << "index:" << index << endl;
    }
    if (debug) cout << endl;

    // read entries from file
    for (int i = 0; i < indices.size(); ++i)
    {
        Entry entry = IO.read_file(indices[i]);
        if (entry != entries[i]) return false;
        if (debug) cout << entry << endl;
    }

    // real all
    if (debug) cout << "read all:" << endl;
    vector<Entry> all_entries = IO.read_all();
    for (int i = 0; i < all_entries.size(); ++i)
    {
        if (all_entries[i] != entries[i]) return false;
        if (debug) cout << all_entries[i] << endl;
    }

    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");
    return true;
}

bool test_fileIO3(bool debug = false)
{
    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");

    BinaryFileIO IO;
    vector<Entry> entries;
    int size = 10000;
    for (int i = 0; i < size; ++i) entries.push_back(Entry(to_string(i), to_string(i), INSERT));

    // write entries to file
    vector<long long> indices;
    for (const auto& entry : entries)
    {
        long long index = IO.write_file(entry);
        indices.push_back(index);
        if (debug) cout << "index:" << index << endl;
    }

    // read entries from file
    for (int i = 0; i < indices.size(); ++i)
    {
        Entry entry = IO.read_file(indices[i]);
        if (entry != entries[i]) return false;
        if (debug) cout << entry << endl;
    }

    // real all
    vector<Entry> all_entries = IO.read_all();
    for (int i = 0; i < all_entries.size(); ++i)
    {
        if (all_entries[i] != entries[i]) return false;
        if (debug) cout << all_entries[i] << endl;
    }

    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");
    return true;
}

// test dump
bool test_fileIO4(bool debug = false)
{
    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");
    HashTable<string, long long> map;
    BinaryFileIO IO;
    int size = 3;

    // binary file data
    vector<long long> indices;
    long long idx;
    idx = IO.write_file(Entry("ok1", to_string(0), INSERT));
    indices.push_back(idx);
    idx = IO.write_file(Entry("ok1", to_string(1), INSERT));
    indices.push_back(idx);
    idx = IO.write_file(Entry("ok1", to_string(2), INSERT));
    indices.push_back(idx);

    idx = IO.write_file(Entry("ok2", to_string(0), INSERT));
    indices.push_back(idx);
    idx = IO.write_file(Entry("ok2", to_string(1), INSERT));
    indices.push_back(idx);
    idx = IO.write_file(Entry("ok2", to_string(2), INSERT));
    indices.push_back(idx);

    idx = IO.write_file(Entry("ok3", to_string(0), INSERT));
    indices.push_back(idx);
    idx = IO.write_file(Entry("ok3", to_string(1), INSERT));
    indices.push_back(idx);
    idx = IO.write_file(Entry("ok3", to_string(2), INSERT));
    indices.push_back(idx);

    for (int i = 0; i < 9; ++i)
    {
        if (0 <= i && i <= 2) map.insert("ok1", indices[i]);
        if (3 <= i && i <= 5) map.insert("ok2", indices[i]);
        if (6 <= i && i <= 8) map.insert("ok3", indices[i]);
    }
    vector<HashRecord<string, long long>> records = map.to_vector();
    if (debug)
    {
        cout << "vector records:" << endl;
        for (int i = 0; i < records.size(); ++i) cout << records[i] << endl;
        cout << endl;
    }

    IO.dump_to_merge_file(map);

    if (debug) cout << "read all:" << endl;
    vector<Entry> entries = IO.read_all(MERGE_FILE);
    if (debug)
    {
        cout << "entries:" << endl;
        for (int i = 0; i < entries.size(); ++i) cout << entries[i] << endl;
        cout << endl;
    }
    if (entries.size() != size) return false;

    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");
    return true;
}

bool test_fileIO5(bool debug = false)
{
    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");

    vector<Entry> entries = {
        {"1", "1", INSERT},
        {"2", "2", DELETE},
        {"3", "3", INSERT},
        {"4", "4", DELETE},
        {"4", "5", INSERT},
    };

    BinaryFileIO IO;
    for (const auto& entry : entries)
    {
        long long index = IO.write_file(entry);
        if (debug) cout << "index:" << index << endl;
    }
    if (debug) cout << endl;

    vector<HashRecord<int, long long>> cache;
    cache.clear();
    vector<Operations> ops;
    ops.clear();
    IO.load_index(cache, ops);

    for (int i = 0; i < cache.size(); ++i)
        if (debug) cout << cache[i] << endl;

    if (debug) cout << endl;

    int size = 5;
    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");
    if (cache.size() != size) return false;
    return true;
}

const bool debug = false;
const bool debug_dangerous = false; // dangerous!

TEST(TEST_FILE_IO, TestFileIO1)
{
    bool success = test_fileIO1(debug);
    EXPECT_EQ(true, success);
}
TEST(TEST_FILE_IO, TestFileIO2)
{
    bool success = test_fileIO2(debug);
    EXPECT_EQ(true, success);
}

TEST(TEST_FILE_IO, TestFileIO3)
{
    bool success = test_fileIO3(debug_dangerous);
    EXPECT_EQ(true, success);
}

TEST(TEST_FILE_IO, TestFileIO4)
{
    bool success = test_fileIO4(debug);
    EXPECT_EQ(true, success);
}

TEST(TEST_FILE_IO, TestFileIO5)
{
    bool success = test_fileIO5(debug);
    EXPECT_EQ(true, success);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running test_fileIO.cpp---------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}
