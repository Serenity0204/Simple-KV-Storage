#include "includes/simple_kv.h"
#include <iostream>
#include <string>
using namespace std;


int main(int argc, char* argv[])
{
    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");
    SimpleKV<int, int> kv;

    kv.PUT(1, 2);
    kv.PUT(1, 3);
    kv.PUT(2, 4);
    cout << kv.GET(1) << endl;
    cout << kv.GET(2) << endl;
    kv.REMOVE(1);
    if (kv.EXISTS(1)) cout << "???" << endl;
    if(!kv.EXISTS(1)) cout << "removed 1 success" << endl;
    kv.PUT(1, 999);
    cout << kv.GET(1) << endl;
    kv.DISPLAY();
    cout << "success";
    remove("simple_kv_db.merge");
    remove("simple_kv_db.data");   
    return 0;
}


