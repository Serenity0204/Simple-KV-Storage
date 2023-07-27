// #include <SimpleKV.h>
// #include <iostream>
// #include <string>
// using namespace std;

// int main(int argc, char* argv[])
// {
//     cout << "example use case:" << endl;
//     string db_path = "db.data";
//     string merge_path = "merge.merge";
//     SimpleKV<int, int> kv(db_path, merge_path);
//     bool success = kv.CONNECT();
//     if (!success) return -1;
//     kv.PUT(1, 2);
//     kv.PUT(1, 3);
//     kv.PUT(2, 4);
//     cout << "1:" << kv.GET(1) << endl;
//     cout << "2:" << kv.GET(2) << endl;
//     kv.REMOVE(1);
//     if (kv.EXISTS(1)) cout << "???" << endl;
//     if (!kv.EXISTS(1)) cout << "removed 1 success" << endl;
//     cout << "put (1,999) into db" << endl;
//     kv.PUT(1, 999);
//     cout << "1:" << kv.GET(1) << endl;
//     kv.DISPLAY();
//     cout << "number of records:" << kv.SIZE() << endl;
//     cout << "success";
//     success = kv.CLOSE();
//     if (!success) return 1;
//     return 0;
// }
