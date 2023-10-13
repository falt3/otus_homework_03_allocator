#include <gtest/gtest.h>

#include "mylist.h"

TEST(mylist, empty) {
    MyList<int> lst;
    ASSERT_EQ(lst.size(), 0);
}

TEST(mylist, notEmpty) {
    MyList<int> lst;
    for (int i = 0; i < 10; ++i)
        lst.push_back(i*10 + i + 1);
    ASSERT_EQ(lst.size(), 10);
}

TEST(mylist, asd) {
    MyList<int> lst;
    for (int i = 0; i < 10; ++i)
        lst.push_back(i*10);
    int a = lst.pop_back();
    int b = lst.pop_front();
    ASSERT_EQ(a, 99);    
    ASSERT_EQ(b, 0);
}
