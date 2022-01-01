// Copyright Davydov Nikolay 2021

#include <gtest/gtest.h>

#include "shared_ptr.hpp"

TEST(SharedPtr, INIT) {
    int* a = new int(5);

    SharedPtr<int> ptr1;
    SharedPtr<int> ptr2(nullptr);
    SharedPtr<int> ptr3(new int(1));
    SharedPtr<int> ptr4(a);

    SharedPtr<int> copy(ptr3);

    EXPECT_EQ(ptr1, nullptr);
    EXPECT_EQ(ptr2, nullptr);
    EXPECT_EQ(copy, ptr3);

    SharedPtr<int> moved(std::move(copy));
    ptr1 = ptr4;

    EXPECT_EQ(moved, ptr3);
    EXPECT_EQ(copy, nullptr);
    EXPECT_EQ(ptr1, ptr4);

    ptr1 = std::move(moved);
    EXPECT_EQ(ptr1, ptr3);
    EXPECT_EQ(moved, nullptr);
}

TEST(SharedPtr, BOOL) {
    int* a = new int(5);

    SharedPtr<int> ptr1;
    SharedPtr<int> ptr2(nullptr);
    SharedPtr<int> ptr3(new int(1));
    SharedPtr<int> ptr4(a);

    EXPECT_FALSE(ptr1.operator bool());
    EXPECT_FALSE(ptr2.operator bool());
    EXPECT_TRUE(ptr3.operator bool());
    EXPECT_TRUE(ptr4.operator bool());
}

TEST(SharedPtr, DEREFERENCES) {
    int* a = new int(5);
    SharedPtr<int> ptr(a);
    SharedPtr<int> ptr2(ptr);

    EXPECT_EQ(*ptr, 5);
    EXPECT_EQ(*ptr2, 5);
    EXPECT_EQ(ptr.get(), a);
    EXPECT_EQ(ptr.get(), ptr2.get());
}

TEST(SharedPtr, USE_COUNT) {
    int* a = new int(5);

    SharedPtr<int> ptr1;
    SharedPtr<int> ptr2(nullptr);
    SharedPtr<int> ptr3(a);
    SharedPtr<int> ptr4(ptr3);

    EXPECT_EQ(ptr1.use_count(), 0);
    EXPECT_EQ(ptr2.use_count(), 0);
    EXPECT_EQ(ptr3.use_count(), 2);
    EXPECT_EQ(ptr4.use_count(), 2);

    ptr1 = ptr3;

    EXPECT_EQ(ptr1.use_count(), 3);
    EXPECT_EQ(ptr3.use_count(), 3);
    EXPECT_EQ(ptr4.use_count(), 3);

    ptr2 = std::move(ptr4);
    EXPECT_EQ(ptr1.use_count(), 3);
    EXPECT_EQ(ptr2.use_count(), 3);
    EXPECT_EQ(ptr3.use_count(), 3);
    EXPECT_EQ(ptr4.use_count(), 0);

    {
        SharedPtr<int> ptr5(ptr2);
        EXPECT_EQ(ptr1.use_count(), 4);
        EXPECT_EQ(ptr5.use_count(), 4);
    }
    EXPECT_EQ(ptr1.use_count(), 3);
}

TEST(SharedPtr, RESET) {
    int* a = new int(5);
    int* b = new int(5);

    SharedPtr<int> ptr1(a);
    SharedPtr<int> ptr2;
    ptr1.reset();
    ptr2.reset(b);

    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(ptr1.use_count(), 0);
    EXPECT_EQ(ptr2.get(), b);
    EXPECT_EQ(ptr2.use_count(), 1);

    ptr1 = ptr2;
    EXPECT_EQ(ptr1.get(), b);
    EXPECT_EQ(ptr1.use_count(), 2);

    ptr2.reset();
    EXPECT_EQ(ptr2.get(), nullptr);
    EXPECT_EQ(ptr2.use_count(), 0);
    EXPECT_EQ(ptr1.get(), b);
    EXPECT_EQ(ptr1.use_count(), 1);
}

TEST(SharedPtr, SWAP) {
    SharedPtr<int> ptr1(new int(10));
    SharedPtr<int> ptr2(new int(5));

    SharedPtr<int> copy1(ptr1);
    SharedPtr<int> copy2(ptr2);

    ptr1.swap(ptr2);
    EXPECT_EQ(ptr1, copy2);
    EXPECT_EQ(ptr2, copy1);
}