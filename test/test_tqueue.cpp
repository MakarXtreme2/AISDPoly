#include <gtest/gtest.h>
#include "tqueue.h"

TEST(TDynamicQueue, queue_can_be_created) {
  ASSERT_NO_FATAL_FAILURE(TDynamicQueue<int> qe(2));
}

TEST(TDynamicQueue, queue_cannot_be_created_with_negative_size) {
  ASSERT_ANY_THROW(TDynamicQueue<int> qe(-5));
}

TEST(TDynamicQueue, queue_cannot_be_created_with_bigger_size) {
  ASSERT_ANY_THROW(TDynamicQueue<int> qe(MAXQUEUE + 1));
}

TEST(TDynamicQueue, queue_push_and_pop_work_correct) {
  TDynamicQueue<int> qe(3);
  qe.Push(1);
  qe.Push(2);
  qe.Push(3);
  EXPECT_EQ(1, qe.Top());
  qe.Pop();
  EXPECT_EQ(2, qe.Top());
  qe.Push(4);
  EXPECT_NE(4, qe.Top());
  qe.Pop();
  EXPECT_EQ(3, qe.Top());
}

TEST(TDynamicQueue, queue_can_return_size) {
  TDynamicQueue<int> qe(3);
  EXPECT_EQ(0, qe.Size());
  qe.Push(1);
  EXPECT_EQ(1, qe.Size());
  qe.Push(2);
  qe.Pop();
  EXPECT_EQ(1, qe.Size());
  qe.Push(3);
  EXPECT_EQ(2, qe.Size());
}

TEST(TDynamicQueue, queue_can_be_empty) {
  TDynamicQueue<int> qe(3);
  EXPECT_TRUE(qe.isEmpty());
  qe.Push(1);
  EXPECT_FALSE(qe.isEmpty());
  qe.Pop();
  EXPECT_TRUE(qe.isEmpty());
}

TEST(TDynamicQueue, queue_can_be_full) {
  TDynamicQueue<int> qe(3);
  EXPECT_FALSE(qe.isFull());
  qe.Push(1);
  qe.Push(2);
  qe.Push(3);
  EXPECT_TRUE(qe.isFull());
  qe.Pop();
  EXPECT_FALSE(qe.isFull());
  qe.Push(4);
  EXPECT_TRUE(qe.isFull());
}

TEST(TDynamicQueue, queue_cannot_be_poped_if_it_is_empty) {
  TDynamicQueue<int> qe(3);
  ASSERT_ANY_THROW(qe.Pop());
}

TEST(TDynamicQueue, queue_cannot_be_pushed_if_size_is_over_max) {
  TDynamicQueue<int> qe(MAXQUEUE);
  for (size_t i = 0; i < MAXQUEUE; i++)
    qe.Push(i);
  ASSERT_ANY_THROW(qe.Push(2));
}

TEST(TDynamicQueue, queue_can_be_extended) {
  TDynamicQueue<int> qe(3);
  for (size_t i = 0; i < 4; i++)
    qe.Push(i);
  EXPECT_EQ(4, qe.Size());
  EXPECT_EQ(qe.Top(), 0);
  qe.Pop();
  EXPECT_EQ(1, qe.Top());
  qe.Push(4);
  qe.Push(5);
  EXPECT_EQ(1, qe.Top());
}

TEST(TDynamicQueue, queue_can_be_copied_by) {
  TDynamicQueue<int> qe(3);
  qe.Push(1);
  qe.Push(2);
  qe.Push(3);
  TDynamicQueue<int> qe2(qe);
  EXPECT_EQ(qe2.Size(), qe.Size());
  EXPECT_EQ(qe2.Top(), qe.Top());
}

TEST(TDynamicQueue, queue_can_assign) {
  TDynamicQueue<int> qe(3);
  qe.Push(1);
  qe.Push(2);
  qe.Push(3);
  TDynamicQueue<int> qe2(2);
  qe2 = qe;
  EXPECT_EQ(qe2.Size(), qe.Size());
  EXPECT_EQ(qe2.Top(), qe.Top());
}
