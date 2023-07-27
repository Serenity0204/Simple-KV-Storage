# SimpleKV Storage

A Persistent Single File key value storage that's built on top of bitcask, it supports template keys and template values.
The code only consists of header files hence it's easy to integrate into other projects.

## APIs

- CONNECT()
- CLOSE()
- PUT(KEY, VALUE)
- REMOVE(KEY)
- EXISTS(KEY)
- GET(KEY)
- EMPTY()
- SIZE()

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


## Installation
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


## Example Usage

```
#include "<SimpleKV.h>"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
    cout << "example use case:" << endl;
    string db_path = "db.data";
    string merge_path = "merge.merge";
    SimpleKV<int, int> kv(db_path, merge_path);
    bool success = kv.CONNECT();
    if (!success) return -1;
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
    cout << "success";
    success = kv.CLOSE();
    if (!success) return -1;
    return 0;
}
```

## References

bitcask: https://riak.com/assets/bitcask-intro.pdf