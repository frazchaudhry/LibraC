#include <gtest/gtest.h>

extern "C" {
#include "../src/libraCore.h"
}

// =====================================Strings and String Operations================================================

TEST(Strings, LC_InitializeString) {
    // Arrange
    LC_String string;
    char message[] = "Hello, World!";
    
    // Act
    LC_String_Initialize(&string, message);

    // Assert
    ASSERT_STREQ(string.data, "Hello, World!");
    ASSERT_EQ(string.length, 13);
}

TEST(Strings, LC_IsEqualStringCString) {
    // Arrange
    LC_String string;
    char message[] = "Hello, World!";
    char wrongMessage[] = "Hello!";
    char wrongAgain[] = "Hello, World@";
    LC_String_Initialize(&string, message);

    // Act
    bool shouldBeTrue = LC_String_IsEqualCString(&string, message);
    bool shouldBeFalse = LC_String_IsEqualCString(&string, wrongMessage);
    bool shouldBeAlsoFalse = LC_String_IsEqualCString(&string, wrongAgain);

    // Assert
    ASSERT_TRUE(shouldBeTrue);
    ASSERT_FALSE(shouldBeFalse);
}

TEST(Strings, LC_IsEqualString) {
    // Arrange
    LC_String string1;
    char message1[] = "Hello, World!";
    LC_String_Initialize(&string1, message1);
    LC_String string2;
    char message2[] = "Hello, World!";
    LC_String_Initialize(&string2, message2);
    LC_String wrongString;
    char wrongMessage[] = "Hello!";
    LC_String_Initialize(&wrongString, wrongMessage);

    // Act
    bool shouldBeTrue = LC_String_IsEqual(&string1, &string2);
    bool shouldBeFalse = LC_String_IsEqual(&string1, &wrongString);
    bool shouldBeAlsoFalse = LC_String_IsEqual(&string2, &wrongString);

    // Assert
    ASSERT_TRUE(shouldBeTrue);
    ASSERT_FALSE(shouldBeFalse);
    ASSERT_FALSE(shouldBeAlsoFalse);
}

// =====================================Utility Operations===========================================================
TEST(Utility, LC_SwapValues) {
    // Arrange
    int a = 10;
    int b = 5;
    bool success = false;

    // Act
    LC_SwapValues(&a, &b, sizeof(int), &success);

    // Assert
    ASSERT_EQ(a, 5);
    ASSERT_EQ(b, 10);
    ASSERT_TRUE(success);
}

