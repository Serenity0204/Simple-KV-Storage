#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
//------------------------------------------------------------------------------
//Files we are testing:
#include "../../includes/binary_file_io/binary_file_io.h"


//------------------------------------------------------------------------------

using namespace std;



bool test_fileIO1(bool debug=false)
{
    remove("simple_kv_db.data");
    BinaryFileIO IO;
    vector<Entry> entries = 
    {
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
    vector<long long> indexes;
    for (const auto& entry : entries) 
    {
        long long index = IO.write_file(entry);
        indexes.push_back(index);
        if(debug) cout << "index:" << index << endl;
    }

    // read entries from file
    for (const auto& index : indexes) 
    {
        Entry entry = IO.read_file(index);
        if(debug) cout << entry << endl;
    }

    ifstream in;

    in.open("simple_kv_db.data");
    if(in.good()) 
    {
        if(debug) cout << "removed done" << endl;
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

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running test_fileIO.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

