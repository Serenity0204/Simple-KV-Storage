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

## References

bitcask: https://riak.com/assets/bitcask-intro.pdf
