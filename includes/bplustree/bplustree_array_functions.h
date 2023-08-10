#ifndef BPLUSTREE_ARRAY_FUNCTIONS_H
#define BPLUSTREE_ARRAY_FUNCTIONS_H

#include <cassert> // Provides assert
#include <cstdlib> // Provides size_t
#include <cstring>
#include <iomanip>  // Provides std::setw
#include <iostream> // Provides std::cout

namespace BPlusTreeUtils
{
    // Declaration

    template <class T>
    void swap(T& a, T& b); // swap the two items

    template <class T>
    T maximal(const T& a, const T& b); // return the larger of the two items

    template <class T>
    std::size_t index_of_maximal(T data[], std::size_t n); // return index of the largest item in data with length n

    template <class T>
    std::size_t first_ge(const T data[], std::size_t n, const T& entry); // return the first index such that data[i] is not less than the entry
                                                                         // if there is no such index, then return n indicating all of data are less than the entry.

    template <class T>
    void insert_item(T data[], std::size_t i, std::size_t& n, T entry); // insert entry at index i in data

    template <class T>
    void ordered_insert(T data[], std::size_t& n, T entry); // insert entry into the sorted array data with length n

    template <class T>
    void attach_item(T data[], std::size_t& n, const T& entry); // append entry to the right of data

    template <class T>
    void delete_item(T data[], std::size_t i, std::size_t& n, T& entry); // delete item at index i and place it in entry

    template <class T>
    void detach_item(T data[], std::size_t& n, T& entry); // remove the last element in data and place it in entry

    template <class T>
    void merge(T data1[], std::size_t& n1, T data2[], std::size_t& n2); // append data2 to the right of data1 and set n2 to 0

    template <class T>
    void split(T data1[], std::size_t& n1, T data2[], std::size_t& n2); // move n/2 elements from the right of data1 to beginning of data2

    template <class T>
    void copy_array(T dest[], const T src[], std::size_t& dest_size, std::size_t src_size); // copy all entries from src[] to replace dest[]

    template <class T>
    bool is_le(const T data[], std::size_t n, const T& item); // return true if item <= all data[i], otherwise return false

    template <class T>
    bool is_gt(const T data[], std::size_t n, const T& item); // return true if item > all data[i], otherwise return false

    template <class T>
    void print_array(const T data[], std::size_t n, std::size_t pos = 0); // print array data

    // Definition

    // TODO

    // swap the two items
    template <class T>
    void swap(T& a, T& b)
    {
        T temp = a;
        a = b;
        b = temp;
    }

    // return the larger of the two items
    template <class T>
    T maximal(const T& a, const T& b)
    {
        return (a > b) ? a : b;
    }

    // return index of the largest item in data with length n
    template <class T>
    std::size_t index_of_maximal(T data[], std::size_t n)
    {
        // [0,1,2]
        T max = data[0];
        std::size_t idx = 0;
        for (size_t i = 1; i < n; ++i)
        {
            T val = maximal(data[i], max);
            if (val != max)
            {
                idx = i;
                max = val;
            }
        }
        return idx;
    }

    // return the first index such that data[i] is not less than the entry
    // if there is no such index, then return n indicating all of data are less than the entry.
    template <class T>
    std::size_t first_ge(const T data[], std::size_t n, const T& entry)
    {
        for (int i = 0; i < n; ++i)
            if (!(data[i] < entry))
                return i;
        return n;
    }

    // insert entry at index i in data
    template <class T>
    void insert_item(T data[], std::size_t i, std::size_t& n, T entry)
    {
        for (size_t idx = n; idx > i; --idx)
            data[idx] = data[idx - 1];
        n++;
        data[i] = entry;
    }

    // insert entry into the sorted array data with length n
    template <class T>
    void ordered_insert(T data[], std::size_t& n, T entry)
    {
        // search index;
        size_t i = first_ge(data, n, entry);
        insert_item(data, i, n, entry);
    }

    // append entry to the right of data
    template <class T>
    void attach_item(T data[], std::size_t& n, const T& entry)
    {
        data[n] = entry;
        n++;
    }

    // delete item at index i and place it in entry
    template <class T>
    void delete_item(T data[], std::size_t i, std::size_t& n, T& entry)
    {
        assert(n > 0);
        entry = data[i];
        for (size_t idx = i; idx < n - 1; ++idx)
            data[idx] = data[idx + 1];
        n--;
    }

    // remove the last element in data and place it in entry
    template <class T>
    void detach_item(T data[], std::size_t& n, T& entry)
    {
        entry = data[n - 1];
        n--;
    }

    // append data2 to the right of data1 and set n2 to 0
    template <class T>
    void merge(T data1[], std::size_t& n1, T data2[], std::size_t& n2)
    {
        // [1,2,3] + [4,5,6] = [1,2,3,4,5,6]
        size_t idx = 0;
        for (size_t i = n1; i < n1 + n2; ++i)
        {
            data1[i] = data2[idx];
            idx++;
        }
        n1 += n2;
        n2 = 0;
    }

    // move n/2 elements from the right of data1 to beginning of data2
    template <class T>
    void split(T data1[], std::size_t& n1, T data2[], std::size_t& n2)
    {
        // [1,2,3,4,5,6,7]
        //
        size_t n = n1 / 2;
        size_t idx = 0;
        size_t i = (n1 % 2 == 1) ? (n + 1) : n;
        for (; i < n1; ++i)
        {
            data2[idx] = data1[i];
            idx++;
        }
        n1 -= n;
        n2 = n;
    }

    // copy all entries from src[] to replace dest[]
    template <class T>
    void copy_array(T dest[], const T src[], std::size_t& dest_size, std::size_t src_size)
    {
        for (size_t i = 0; i < src_size; ++i)
            dest[i] = src[i];
        dest_size = src_size;
        // memcpy(dest, src, src_size * sizeof(T));
        // dest_size = src_size;
    }

    template <class T>
    bool is_le(const T data[], std::size_t n, const T& item)
    {
        for (std::size_t i = 0; i < n; i++)
            if (item > data[i]) return false;

        return true;
    }

    template <class T>
    bool is_gt(const T data[], std::size_t n, const T& item)
    {
        for (std::size_t i = 0; i < n; i++)
            if (item <= data[i]) return false;

        return true;
    }

    // print array data
    template <class T>
    void print_array(const T data[], std::size_t n, std::size_t pos)
    {
        if (n == 0)
        {
            std::cout << "empty" << std::endl;
            return;
        }
        for (size_t i = pos; i < n; ++i) std::cout << "[" << data[i] << "] ";
    }

};

#endif // BPLUSTREE_ARRAY_FUNCTIONS_H
