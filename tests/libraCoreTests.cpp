#include <gtest/gtest.h>

extern "C" {
#include "../src/libraCore.h"
}

TEST(Utility, LC_SwapValues) {
    int a = 10;
    int b = 5;
    bool success = false;
    LC_SwapValues(&a, &b, sizeof(int), &success);
    ASSERT_EQ(a, 5);
    ASSERT_EQ(b, 10);
}

