#include "src/Set.hpp"              // vengono inclusi gli header che contengono il codice da testare.

#include <gtest/gtest.h>   // non viene definito un main_test, ma si sfrutta il main della libreria gtest che chiama automaticamente i nostri test.

TEST(SetTest, Add) {
    Set<int> s;
    s.add(5);
    EXPECT_TRUE(s.contains(5));  
}

TEST(SetTest, Remove) {
    Set<int> s;
    s.add(5);
    ASSERT_EQ(s.contains(5), true);
    s.remove(5);
    EXPECT_FALSE(s.contains(5));
}

