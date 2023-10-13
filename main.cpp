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
// #include "myvector.h"



constexpr int factorial(int k) {
    int res = 1;
    for (int i = 2; i <= k; ++i)
        res *=i;
    return res;
}

//*****************************************************************

namespace ns_map_stdAlloc
{
    void work () {
        std::cout << "--------------- 1. map & std::allocator ---------------\n";
        
        std::map<int, int> m;
        for (int i = 0; i <= 9; ++i)
            m.insert({i, factorial(i)});
    }
}

//*****************************************************************

namespace ns_map_myAlloc
{
    void work () {
        std::cout << "\n--------------- 2. map & myAllocator ---------------\n";
        
        using TA = MyAllocator_11<std::pair<const int, int>>;
        TA alloc1(10);
        std::map<int, int, std::less<int>, TA> m1(alloc1);

        for (int i = 0; i <= 9; ++i)
            m1.insert({i, factorial(i)});
        for (auto el: m1)
            std::cout << el.first << " " << el.second << std::endl;
    }
}

//*****************************************************************

namespace ns_myList_stdAlloc 
{   
    void work () {
        std::cout << "\n--------------- 3 myList & std::allocator ---------------\n";        

        MyList<int> mylist;
        for (int i = 0; i <= 9; ++i)
            mylist.push_back(factorial(i));
    }
} // namespace ns_mylist_

//*****************************************************************

namespace ns_myList_myAlloc
{
    void work () {
        std::cout << "\n--------------- 4 myList & myAllocator ---------------\n";

        MyList<int, MyAllocator_11<int>> mylist(MyAllocator_11<int>(10));
        int n = 10;
        for (int i = 0; i < n; ++i) {
            mylist.push_back(factorial(i));
        }
        for (auto it : mylist) {
            std::cout << it << std::endl;
        }
    }    
} // namespace ns_mylist_myalloc


//*****************************************************************


namespace ns3
{
    void work() {
    //     std::cout << "--------------------\n";
    //     std::cout << "myList & myAllocator: \n";
    //     using TL = MyAllocator_11<int>;
    //     TL alloc1(10);        
    //     MyList<int, TL> mylist(alloc1);
    //     // MyList<int, MyAllocator_11<int>> mylist(MyAllocator_11<int>(10));
    //     int n = 10;
    //     for (int i = 0; i < n; ++i) {
    //         mylist.push_back(i*10 + i*1);
    //     }
    //     for (int i = 0; i < n; ++i) {
    //         std::cout << mylist.pop_back() << std::endl;
    //     }
    }
} // namespace ns3

namespace ns_myList_blockAlloc
{
    void work() {
        std::cout << "\n--------------- 5 myList & blockAllocator ---------------\n";

        MyList<int, BlockAllocator<int>> mylist;
        int n = 20;
        for (int i = 0; i < n; ++i) {
            mylist.push_back(i+1);
        }     
        for (auto it : mylist) {
            std::cout << it << std::endl;
        }

        std::cout << "-----------\n";
        for (int i = 0; i < 11; ++i)
            mylist.pop_back();
        for (auto it : mylist) {
            std::cout << it << std::endl;
        }
           

        // using TA = MyAllocator_11<std::pair<const int, int>>;
        //             //   MyAllocator<std::pair<int, int>, RANGE
        // TA alloc1(10);
        // std::map<int, int, std::less<int>, TA> m1(alloc1);        
    }
} // namespace ns_myList_blockAlloc

//*****************************************************************

int main()
{
    ns_map_stdAlloc::work();

    ns_map_myAlloc::work();

    ns_myList_stdAlloc::work();

    ns_myList_myAlloc::work();
    
    ns_myList_blockAlloc::work();

    // ns3::work();

    return 0;
}
