#include "includes/SimpleKV.h"
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