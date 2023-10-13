/**
 * @file main.cpp
 * @author Lipatkin Dmitry
 * @brief 
 * @version 0.1
 * @date 2023-10-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <map>
#include <memory>
#include <list>
#include <vector>
#include "mylist.h"
#include "myallocator.h"



constexpr int factorial(int k) {
    int res = 1;
    for (int i = 2; i <= k; ++i)
        res *=i;
    return res;
}

//-----------------------------------------------------------------

namespace ns_map_stdAlloc
{
    void work () {
        std::cout << "--------------- 1. map & std::allocator ---------------\n";
        
        std::map<int, int> m;
        for (int i = 0; i <= 9; ++i)
            m.insert({i, factorial(i)});
    }
}

//-----------------------------------------------------------------

namespace ns_map_myAlloc
{
    void work () {
        std::cout << "\n--------------- 2. map & myAllocator ---------------\n";
        
        using TA = MyAllocator<std::pair<const int, int>, 10>;
        std::map<int, int, std::less<int>, TA> m1;

        for (int i = 0; i <= 9; ++i)
            m1.insert({i, factorial(i)});
        for (auto el: m1)
            std::cout << el.first << " " << el.second << std::endl;
    }
}

//-----------------------------------------------------------------

namespace ns_myList_stdAlloc 
{   
    void work () {
        std::cout << "\n--------------- 3 myList & std::allocator ---------------\n";        

        MyList<int> mylist;
        for (int i = 0; i <= 9; ++i)
            mylist.push_back(i);
    }
} // namespace ns_mylist_

//-----------------------------------------------------------------

namespace ns_myList_myAlloc
{
    void work () {
        std::cout << "\n--------------- 4 myList & myAllocator ---------------\n";

        MyList<int, MyAllocator<int, 10>> mylist;
        int n = 10;
        for (int i = 0; i < n; ++i) {
            mylist.push_back(i);
        }
        for (auto it : mylist) {
            std::cout << it << std::endl;
        }
    }    
} // namespace ns_mylist_myalloc


//-----------------------------------------------------------------

namespace ns_myList_blockAlloc
{
    void work() {
        std::cout << "\n--------------- 5 myList & blockAllocator ---------------\n";

        MyList<int, BlockAllocator<int, 10>> mylist;
        for (int i = 0; i < 16; ++i) {
            mylist.push_back(i);
        }     
        for (auto it : mylist) {
            std::cout << it << std::endl;
        }
    }
} // namespace ns_myList_blockAlloc

//-----------------------------------------------------------------

int main()
{
    ns_map_stdAlloc::work();

    ns_map_myAlloc::work();

    ns_myList_stdAlloc::work();

    ns_myList_myAlloc::work();
    
    ns_myList_blockAlloc::work();

    return 0;
}
