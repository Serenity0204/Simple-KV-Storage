#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
//------------------------------------------------------------------------------
//@TODO: include all files we are testing:
#include "../../includes/hash_table/hash_table.h"



//------------------------------------------------------------------------------

using namespace std;



bool test_hash1(bool debug1 = false, bool debug2 = false)
{
  HashTable<int> hash;
  int size = 25;
  for(int i = 0; i < size; ++i)
  {
    string key = string("key__") + to_string(i);
    hash.insert(key, i);
    bool found = hash.count(key);
    if(debug2)
    {
      cout << "Hash is:" << endl;
      cout << hash << endl;
    }
    if(!found) 
    {
      cout << "not found " << key << " when it's present" << endl;
      return false;
    }
  }
  if(debug1)
  {
    cout << "Hash is:" << endl;
    cout << hash << endl;
  }
  if(hash.size() != size) 
  {
    cout << "size error" << endl;
    return false;
  }
  return true;
}

bool test_hash2(bool debug1 = false, bool debug2 = false)
{
  HashTable<string> hash;
  int size = 30;
  for(int i = 0; i < size; ++i)
  {
    int val = (i % 2 == 0) ? i : i * i;
    string key = to_string(val);
    hash.insert(key, string("item__") + to_string(val));
    bool found = hash.count(key);
    if(debug2)
    {
      cout << "Hash is:" << endl;
      cout << hash << endl;
    }
    if(!found) 
    {
      cout << "not found " << key << " when it's present" << endl;
      return false;
    }
  }
  if(debug1)
  {
    cout << "Hash is:" << endl;
    cout << hash << endl;
  }
  if(hash.size() != size) 
  {
    cout << "size error" << endl;
    return false;
  }
  return true;
}


// test load
bool test_hash3(bool debug1 = false, bool debug2 = false)
{
  HashTable<string> hash;
  int size = 10000;
  for(int i = 0; i < size; ++i)
  {
    int key = i;
    string str_key = to_string(key);
    hash.insert(to_string(i), std::string("item__") + to_string(key));
    bool found = hash.count(str_key);
    if(debug2)
    {
      cout << "Hash is:" << endl;
      cout << hash << endl;
    }
    if(!found) return false;
  }
  if(debug1)
  {
    cout << "Hash is:" << endl;
    cout << hash << endl;
  }
  if(hash.size() != size) return false;
  for(int i = 0; i < size; ++i)
  {
    int key = i;
    string str_key = to_string(key);
    hash.insert(to_string(i), std::string("item__") + to_string(key * 2));
    bool found = hash.count(str_key);
    if(debug2)
    {
      cout << "Hash is:" << endl;
      cout << hash << endl;
    }
  }
  if(debug1)
  {
    cout << "Hash is:" << endl;
    cout << hash << endl;
  }
  if(hash.size() != size) return false;
  return true;
}


bool test_hash4(bool debug1=false, bool debug2=false)
{
  HashTable<int> hash;
  int size = 2500;
  vector<int> v;
  for(int i = 0; i < size; ++i)
  {
    string key = to_string(i);
    int val = i;
    hash[key] = val;
    if(!hash.count(key)) return false;
    if(hash[key] != i) return false;
  }
  if(hash.size() != size) return false;
  hash.clear();
  if(hash.size() != 0) return false;
  return true;
}


//------------------------------------------------------------------------------


// Lord help me! 
const bool debug1 = false;
const bool debug2 = false; // dangerous



TEST(TEST_HASH, TestHash1)
{
  bool success = test_hash1(debug1, debug2);
  EXPECT_EQ(success, true);
}

TEST(TEST_HASH, TestHash2)
{
  bool success = test_hash2(debug1, debug2);
  EXPECT_EQ(success, true);
}

TEST(TEST_HASH, TestHash3)
{
  bool success = test_hash3(debug1, debug2);
  EXPECT_EQ(success, true);
}

TEST(TEST_HASH, TestHash4)
{
  bool success = test_hash4(debug1, debug2);
  EXPECT_EQ(success, true);
}





//------------------------------------------------------------------------------


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running test_hash.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}