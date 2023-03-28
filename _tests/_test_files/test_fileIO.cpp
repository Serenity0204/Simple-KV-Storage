#include "gtest/gtest.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
//------------------------------------------------------------------------------
// Files we are testing:
#include "../../includes/binary_file_io/binary_file_io.h"

//------------------------------------------------------------------------------

using namespace std;

bool test_fileIO1(bool debug = false)
{
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

    // read entries from file
    for(int i = 0; i < indices.size(); ++i)
    {
        Entry entry = IO.read_file(indices[i]);
        if(entry != entries[i]) return false;
        if (debug) cout << entry << endl;
    }

    ifstream in;

    in.open("simple_kv_db.data");
    if (in.good())
    {
        if (debug) cout << "removed done" << endl;
        remove("simple_kv_db.data");
    }
    in.close();
    return true;
}

bool test_fileIO2(bool debug = false)
{
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

    // read entries from file
    for(int i = 0; i < indices.size(); ++i)
    {
        Entry entry = IO.read_file(indices[i]);
        if(entry != entries[i]) return false;
        if (debug) cout << entry << endl;
    }

    ifstream in;

    in.open("simple_kv_db.data");
    if (in.good())
    {
        if (debug) cout << "removed done" << endl;
        remove("simple_kv_db.data");
    }
    in.close();
    return true;
}



const bool debug = false;

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
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    std::cout << "\n\n----------running test_fileIO.cpp---------\n\n"
              << std::endl;
    return RUN_ALL_TESTS();
}
