# SimpleKV Storage

A Fast and Persistent Header Files Only Key-Value Storage that's built on top of bitcask, it supports template keys and template values.
The code only consists of header files hence it's easy to integrate into other projects.

## APIs
- CONNECT() // connect to the database
- CLOSE() // close the database
- PUT(KEY, VALUE) // insert key: value
- REMOVE(KEY) // remove by key
- EXISTS(KEY) // check if key exists
- GET(KEY) // get the value from key, error if key does not exist
- GET_ALL_KEYS() // get all of the keys inside the database, will return a vector of key
- GET_ALL_VALUES() // get all of the values inside the database, will return a vector of value
- EMPTY() // check if database is empty
- SIZE() // get the number of entries inside the database

## Design

Can be found in https://github.com/Serenity0204/Simple_KV_Storage/blob/master/design.txt

## Features

- persistent storage
- fast file io
- quick search in disk
- O(n) loading time with loading every entry's key vs index into memory and excluding the invalid ones
- O(log(n)) time removal in memory, and O(1) time removal in disk(appending new entry at the end and marked as removed)
- O(log(n)) time insertion in memory, and O(1) time insertion in disk with the same reason as removal
- O(log(n)) time retrieval in memory, and O(1) time retrieval in disk


## Installation using CMake
Include the following inside your CmakeLists.txt

```
## Fetch the content
include(FetchContent)
FetchContent_Declare(
  SimpleKVStorage
  GIT_REPOSITORY https://github.com/Serenity0204/Simple-KV-Storage.git
)
FetchContent_MakeAvailable(SimpleKVStorage)

## Linking
ADD_EXECUTABLE(main main.cpp)
TARGET_LINK_LIBRARIES(main PRIVATE SimpleKVStorage)
```


## Usage
Make sure the object you want to store overloaded the following operators
* operator<<
* operator>>
* operator<  

## Limitations
* The serialization/deserialization are highly dependent on operator<< and operator>>, so you have to sacrifice the usage of these 2 operators.
* If you have a really complex object as key/value, the definition of operator<< and operator>> will become too complex and you have to make sure the I/O process of the chosen object will produce the correct values. 


## Simple Usecase
```
#include <SimpleKV.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char* argv[])
{
    cout << "example use case:" << endl;
    string db_path = "db.data";
    string merge_path = "merge.merge";
    SimpleKV<int, int> kv(db_path, merge_path);
    bool success = kv.CONNECT();
    if (!success) return 1;
    kv.PUT(1, 2);
    kv.PUT(1, 3);
    kv.PUT(2, 4);
    cout << "1:" << kv.GET(1) << endl;
    cout << "2:" << kv.GET(2) << endl;
    kv.REMOVE(1);
    if (kv.EXISTS(1)) cout << "???" << endl;
    if (!kv.EXISTS(1)) cout << "removed 1 success" << endl;
    cout << "put (1,999) into db" << endl;
    kv.PUT(1, 999);
    cout << "1:" << kv.GET(1) << endl;
    kv.DISPLAY();
    cout << "number of records:" << kv.SIZE() << endl;

    vector<int> v = kv.GET_ALL();
    for (int i = 0; i < v.size(); ++i) cout << v[i] << " ";
    cout << endl;
    cout << "success";
    success = kv.CLOSE();
    if (!success) return 1;
    return 0;
}
```


## Complex Usecase
```
#include <SimpleKV.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class MyClass
{
private:
    std::vector<int> intVector;
    std::string myString;
    int myInt;

public:
    // Default Constructor
    MyClass() : myString(""), myInt(0) {}

    // Overloaded Constructor
    MyClass(const std::vector<int>& vec, const std::string& str, int num)
        : intVector(vec), myString(str), myInt(num) {}

    // Destructor
    ~MyClass()
    {
    }

    // Overloaded << operator
    friend std::ostream& operator<<(std::ostream& out, const MyClass& obj)
    {
        for (int value : obj.intVector)
        {
            out << value << " ";
        }
        out << endl;
        out << obj.myString;
        out << endl;
        out << obj.myInt;
        return out;
    }

    // Overloaded >> operator
    friend std::istream& operator>>(std::istream& in, MyClass& obj)
    {
        int value;
        obj.intVector.clear();
        while (in >> value)
        {
            obj.intVector.push_back(value);
        }
        in.clear();
        getline(in, obj.myString);
        in >> obj.myInt;

        return in;
    }

    // Overloaded < operator
    friend bool operator<(const MyClass& self, const MyClass& other)
    {
        return (self.myInt < other.myInt);
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec)
{
    out << "[";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        out << vec[i];
        if (i < vec.size() - 1)
            out << ", ";
    }
    out << "]";
    return out;
}

int main(int argc, char* argv[])
{
    MyClass obj1({1, 2, 3, 4, 5, 6}, "Helloddddd", 422);

    MyClass obj2({1, 2, 3}, "Hello", 42);
    MyClass obj3({7, 8, 9}, "World qwefqwefqwef qwefqwefwfqweffffffffffffffffffffffff", 100);
    MyClass obj4({10, 20, 30, 40, 50}, "Goodbye", 555);

    cout << "example use case:" << endl;
    string db_path = "db.data";
    string merge_path = "merge.merge";
    SimpleKV<MyClass, MyClass> kv(db_path, merge_path);
    bool success = kv.CONNECT();
    if (!success) return 1;
    kv.PUT(obj1, obj2);
    kv.PUT(obj2, obj2);
    kv.PUT(obj2, obj3);
    kv.PUT(obj3, obj3);
    cout << "1:" << kv.GET(obj1) << endl; // should be obj2
    cout << "2:" << kv.GET(obj2) << endl; // should be obj3
    kv.REMOVE(obj1);

    if (kv.EXISTS(obj1)) cout << "???" << endl;
    if (!kv.EXISTS(obj1)) cout << "removed 1 success" << endl;
    cout << "put (obj1, obj4) into db" << endl;
    kv.PUT(obj1, obj4);
    cout << "1:" << kv.GET(obj1) << endl; // should be obj4
    kv.DISPLAY();                         // print out (obj1, obj4), (obj2, obj3) (obj3, obj3)
    cout << "number of records:" << kv.SIZE() << endl;
    cout << "All keys:" << endl;
    cout << kv.GET_ALL_KEYS() << endl; // obj1, obj2, obj3
    cout << "All values:" << endl;
    cout << kv.GET_ALL_VALUES() << endl; // obj4, obj3, obj3

    cout << "success";
    success = kv.CLOSE();
    if (!success) return 1;
    return 0;
}
```

## References

bitcask: https://riak.com/assets/bitcask-intro.pdf
