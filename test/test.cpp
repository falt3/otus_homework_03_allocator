#include <gtest/gtest.h>

#include "mylist.h"
#include "myallocator.h"


//-----------------------------------------------------

TEST(mylist, empty) {
    MyList<int> lst;
    ASSERT_EQ(lst.size(), 0);
}

TEST(mylist, notEmpty) {
    MyList<int> lst;
    for (int i = 0; i < 10; ++i)
        lst.push_back(i*10 + i + 1);
    ASSERT_TRUE(lst.size() > 0);
}

TEST(mylist, size) {
    MyList<int> lst;
    for (int i = 0; i < 10; ++i)
        lst.push_back(i*10 + i + 1);
    ASSERT_TRUE(lst.size() == 10);
}

TEST(mylist, erase) {
    MyList<int> lst;
    for (int i = 0; i < 10; ++i)
        lst.push_back(i*10);
    auto it = lst.begin();
    it = lst.erase(it);
    it = lst.erase(it);
    ASSERT_TRUE(lst.size() == 8);
}

//-----------------------------------------------------

TEST(myAllocator, freeMemory) {
    MyAllocator<int, 3> al;
    ASSERT_EQ(al.countFreeMemory(), 3);
}

TEST(myAllocator, oneMemory) {
    MyAllocator<int, 3> al;
    al.allocate(1);
    ASSERT_EQ(al.countFreeMemory(), 2);
}

TEST(myAllocator, emptyMemory) {
    MyAllocator<int, 3> al;
    al.allocate(1);
    al.allocate(1);
    al.allocate(1);
    ASSERT_EQ(al.countFreeMemory(), 0);
}

TEST(myAllocator, checkMemory) {
    MyAllocator<int, 3> al;
    int* ptr = al.allocate(1);
    ASSERT_TRUE(al.checkMemory(ptr));
}

//-----------------------------------------------------

TEST(blockAllocator, empy) {
    BlockAllocator<int, 3> aloc;
    ASSERT_TRUE(aloc.countPages() == 0);
}

TEST(blockAllocator, notEmpy) {
    BlockAllocator<int, 3> alloc;
    alloc.allocate(1);
    ASSERT_TRUE(alloc.countPages() == 1);
}

TEST(blockAllocator, addPages) {
    BlockAllocator<int, 3> alloc;
    for (int i = 0; i < 5; i++)
        alloc.allocate(1);
    ASSERT_TRUE(alloc.countPages() == 2);
}

TEST(blockAllocator, delPages) {
    BlockAllocator<int, 3> alloc;
    for (int i = 0; i < 3; i++)
        alloc.allocate(1);
    int* ptr = alloc.allocate(1);
    ASSERT_TRUE(alloc.countPages() == 2);
    alloc.deallocate(ptr, 1);
    ASSERT_TRUE(alloc.countPages() == 1);
}