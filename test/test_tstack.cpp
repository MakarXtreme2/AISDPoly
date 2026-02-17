#include <gtest/gtest.h>
#include "tstack.h"

TEST(TDynamicStack, stack_can_be_created) {
  ASSERT_NO_FATAL_FAILURE(TDynamicStack<int> st(2));
}

TEST(TDynamicStack, stack_cannot_be_created_with_negative_size) {
  ASSERT_ANY_THROW(TDynamicStack<int> st(-5));
}

TEST(TDynamicStack, stack_cannot_be_created_with_bigger_size) {
  ASSERT_ANY_THROW(TDynamicStack<int> st(MAXSTACK + 1));
}

TEST(TDynamicStack, stack_push_and_pop_work_correct) {
  TDynamicStack<int> st(3);
  st.Push(15);
  st.Push(20);
  st.Push(25);
  EXPECT_EQ(25, st.Top());
  st.Pop();
  EXPECT_EQ(20, st.Top());
  st.Push(30);
  EXPECT_EQ(30, st.Top());
  st.Pop();
  EXPECT_EQ(20, st.Top());
  st.Pop();
  EXPECT_EQ(15, st.Top());
}


TEST(TDynamicStack, stack_can_return_size) {
  TDynamicStack<int> st(3);
  st.Push(1);
  st.Push(2);
  EXPECT_EQ(2, st.Size());
}

TEST(TDynamicStack, stack_can_be_empty) {
  TDynamicStack<int> st(3);
  EXPECT_TRUE(st.isEmpty());
  st.Push(2);
  EXPECT_FALSE(st.isEmpty());
  st.Pop();
  EXPECT_TRUE(st.isEmpty());
}

TEST(TDynamicStack, stack_can_be_full) {
  TDynamicStack<int> st(3);
  EXPECT_FALSE(st.isFull());
  st.Push(1);
  st.Push(2);
  st.Push(3);
  EXPECT_TRUE(st.isFull());
  st.Pop();
  EXPECT_FALSE(st.isFull());
  st.Push(4);
  EXPECT_TRUE(st.isFull());
}

TEST(TDynamicStack, stack_cannot_be_poped_if_it_is_empty) {
  TDynamicStack<int> st(2);
  ASSERT_ANY_THROW(st.Pop());
}

TEST(TDynamicStack, stack_cannot_be_pushed_if_size_is_over_max) {
  TDynamicStack<int> st(MAXSTACK);
  for (size_t i = 0; i < MAXSTACK; i++)
    st.Push(i);
  ASSERT_ANY_THROW(st.Push(2));
}

TEST(TDynamicStack, stack_can_be_extended) {
  TDynamicStack<int> st(3);
  for (size_t i = 0; i < 5; i++)
    st.Push(i);
  EXPECT_EQ(5, st.Size());
  EXPECT_EQ(4, st.Top());
  st.Pop();
  EXPECT_EQ(3, st.Top());
  st.Push(10);
  st.Push(11);
  EXPECT_EQ(6, st.Size());
  EXPECT_EQ(11, st.Top());
}
