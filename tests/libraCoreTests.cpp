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
    LC_InitializeString(&string, message);

    // Assert
    ASSERT_STREQ(string.data, "Hello, World!");
}

TEST(Strings, LC_IsEqualStringCString) {
    // Arrange
    LC_String string;
    char message[] = "Hello, World!";
    char wrongMessage[] = "Hello!";
    LC_InitializeString(&string, message);

    // Act
    bool shouldBeTrue = LC_IsEqualStringCString(&string, message);
    bool shouldBeFalse = LC_IsEqualStringCString(&string, wrongMessage);

    // Assert
    ASSERT_TRUE(shouldBeTrue);
    ASSERT_FALSE(shouldBeFalse);
}

TEST(Strings, LC_IsEqualString) {
    // Arrange
    LC_String string1;
    char message1[] = "Hello, World!";
    LC_InitializeString(&string1, message1);
    LC_String string2;
    char message2[] = "Hello, World!";
    LC_InitializeString(&string2, message2);
    LC_String wrongString;
    char wrongMessage[] = "Hello!";
    LC_InitializeString(&wrongString, wrongMessage);

    // Act
    bool shouldBeTrue = LC_IsEqualString(&string1, &string2);
    bool shouldBeFalse = LC_IsEqualString(&string1, &wrongString);
    bool shouldBeAlsoFalse = LC_IsEqualString(&string2, &wrongString);

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

