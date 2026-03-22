#include <gtest/gtest.h>
#include "tlist.h"
#include "tcomp.h"

TEST(TStdList, list_can_be_created) {
  ASSERT_NO_FATAL_FAILURE(BaseList<int>* tmp = new TStdList<int>());
  ASSERT_NO_FATAL_FAILURE(TStdList<int> tmp);
}

TEST(TStdList, list_can_change_first) {
  BaseList<int>* tmp = new TStdList<int>();
  EXPECT_EQ(tmp->Size(), 0);
  tmp->addFirst(1);
  tmp->addFirst(2);
  EXPECT_EQ(tmp->Size(), 2);
  EXPECT_EQ(tmp->getFirst(), 2);
  tmp->delFirst();
  EXPECT_EQ(tmp->getFirst(), 1);
  EXPECT_EQ(tmp->Size(), 1);
  tmp->delFirst();
  EXPECT_ANY_THROW(tmp->getFirst());
  EXPECT_ANY_THROW(tmp->delFirst());
  EXPECT_EQ(tmp->Size(), 0);
  delete tmp;
}

TEST(TStdList, list_can_change_last) {
  BaseList<int>* tmp = new TStdList<int>();
  tmp->addLast(1);
  tmp->addLast(2);
  EXPECT_EQ(tmp->Size(), 2);
  EXPECT_EQ(tmp->getFirst(), 1);
  EXPECT_EQ(tmp->getLast(), 2);
  tmp->delLast();
  EXPECT_EQ(tmp->getLast(), 1);
  EXPECT_EQ(tmp->Size(), 1);
  tmp->delLast();
  EXPECT_ANY_THROW(tmp->getFirst());
  EXPECT_ANY_THROW(tmp->delFirst());
  EXPECT_ANY_THROW(tmp->getLast());
  EXPECT_ANY_THROW(tmp->delLast());
  EXPECT_EQ(tmp->Size(), 0);
  delete tmp;
}

TEST(TStdList, list_can_change_at) {
  BaseList<int>* tmp = new TStdList<int>();
  for (int i = 0; i < 10; i++) {
    tmp->addLast(i);
  }
  tmp->addAt(99, 1);
  EXPECT_EQ(tmp->Size(), 11);
  EXPECT_EQ(tmp->getAt(1), 99);
  tmp->delAt(10);
  EXPECT_EQ(tmp->Size(), 10);
  EXPECT_EQ(tmp->getLast(), 8);
  tmp->delAt(0);
  EXPECT_EQ(tmp->getFirst(), 99);
  for (int i = 0; i < 9; i++)
    tmp->delAt(0);
  EXPECT_ANY_THROW(tmp->delAt(0));
  delete tmp;
}

TEST(TStdList, list_can_add_sorted) {
  BaseList<int>* tmp = new TStdList<int>();
  for (int i = 0; i < 10; i++) {
    tmp->addLast(i);
  }
  tmp->addSorted(10, CompareL<int>);
  EXPECT_EQ(tmp->getLast(), 10);
}

TEST(TStdList, list_can_sort) {
  BaseList<int>* tmp = new TStdList<int>();
  for (int i = 0; i < 10; i++) {
    tmp->addFirst(i);
  }
  tmp->Sort(CompareL<int>);
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(tmp->getFirst(), i);
    tmp->delFirst();
  }
  delete tmp;
}

TEST(TStdList, list_can_be_ran_through) {
  BaseList<int>* tmp = new TStdList<int>();
  for (int i = 0; i < 10; i++) {
    tmp->addLast(i);
  }
  auto* begin = tmp->Begin();
  auto* end = tmp->End();
  int i = 0;
  for (auto* it = begin; *it != *end; ++(*it)) {
    EXPECT_EQ(**it, i);
    i++;
  }
  delete tmp;
  delete begin;
  delete end;
}

TEST(TStdList, list_can_merge) {
  BaseList<int>* tmp = new TStdList<int>();
  BaseList<int>* tmp2 = new TStdList<int>();
  for (int i = 0; i < 10; i++) {
    tmp->addLast(i);
    tmp2->addLast(i + 10);
  }
  tmp->Merge(tmp2);
  auto* begin = tmp->Begin();
  auto* end = tmp->End();
  int i = 0;
  for (auto* it = begin; *it != *end; ++(*it)) {
    EXPECT_EQ(**it, i);
    i++;
  }
  EXPECT_EQ(tmp->Size(), 20);
  delete tmp;
  delete tmp2;
  delete begin;
  delete end;
}

TEST(TStdList, list_can_merge_sorted) {
  BaseList<int>* tmp = new TStdList<int>();
  BaseList<int>* tmp2 = new TStdList<int>();
  for (int i = 0; i < 20; i += 2) {
    tmp->addLast(i);
    tmp2->addLast(i + 1);
  }
  tmp->mergeSorted(tmp2, CompareL<int>);
  auto* begin = tmp->Begin();
  auto* end = tmp->End();
  int i = 0;
  for (auto* it = begin; *it != *end; ++(*it)) {
    EXPECT_EQ(**it, i);
    i++;
  }
  EXPECT_EQ(tmp->Size(), 20);
  delete tmp;
  delete tmp2;
  delete begin;
  delete end;
}

TEST(SkipList, slist_can_be_created) {
  ASSERT_NO_FATAL_FAILURE(BaseList<int>* tmp = new SkipList<int>());
  ASSERT_NO_FATAL_FAILURE(SkipList<int> tmp);
}

TEST(SkipList, slist_can_change_first) {
  BaseList<int>* tmp = new SkipList<int>();
  EXPECT_EQ(tmp->Size(), 0);
  tmp->addFirst(1);
  tmp->addFirst(2);
  EXPECT_EQ(tmp->Size(), 2);
  EXPECT_EQ(tmp->getFirst(), 2);
  tmp->delFirst();
  EXPECT_EQ(tmp->getFirst(), 1);
  EXPECT_EQ(tmp->Size(), 1);
  tmp->delFirst();
  EXPECT_ANY_THROW(tmp->getFirst());
  EXPECT_ANY_THROW(tmp->delFirst());
  EXPECT_EQ(tmp->Size(), 0);
  delete tmp;
}

TEST(SkipList, slist_can_change_last) {
  BaseList<int>* tmp = new SkipList<int>();
  tmp->addLast(1);
  tmp->Print();
  tmp->addLast(2);
  tmp->Print();
  EXPECT_EQ(tmp->Size(), 2);
  EXPECT_EQ(tmp->getFirst(), 1);
  EXPECT_EQ(tmp->getLast(), 2);
  tmp->delLast();
  tmp->Print();
  EXPECT_EQ(tmp->getLast(), 1);
  EXPECT_EQ(tmp->Size(), 1);
  tmp->delLast();
  tmp->Print();
  EXPECT_ANY_THROW(tmp->getFirst());
  EXPECT_ANY_THROW(tmp->delFirst());
  EXPECT_ANY_THROW(tmp->getLast());
  EXPECT_ANY_THROW(tmp->delLast());
  EXPECT_EQ(tmp->Size(), 0);
  delete tmp;
}

TEST(SkipList, slist_can_change_at) {
  BaseList<int>* tmp = new SkipList<int>();
  for (int i = 9; i >= 0; i--) {
    tmp->addFirst(i);
  }
  tmp->Print();
  tmp->addAt(99, 1);
  tmp->Print();
  EXPECT_EQ(tmp->Size(), 11);
  EXPECT_EQ(tmp->getAt(1), 99);
  tmp->delAt(10);
  EXPECT_EQ(tmp->Size(), 10);
  EXPECT_EQ(tmp->getLast(), 8);
  tmp->delAt(0);
  EXPECT_EQ(tmp->getFirst(), 99);
  for (int i = 0; i < 9; i++) {
    tmp->Print();
    tmp->delAt(0);
  }
  EXPECT_ANY_THROW(tmp->delAt(0));
  delete tmp;
}

TEST(SkipList, slist_can_add_sorted) {
  BaseList<int>* tmp = new SkipList<int>();
  for (int i = 0; i < 10; i++) {
    tmp->addLast(i);
  }
  tmp->addSorted(10, CompareL<int>);
  tmp->Print();
  EXPECT_EQ(tmp->getLast(), 10);
  tmp->addLast(15);
  tmp->addLast(20);
  tmp->addSorted(13, CompareL<int>);
  tmp->addSorted(10, CompareL<int>);
  tmp->Print();
  EXPECT_EQ(tmp->getAt(tmp->Size() - 3), 13);
}

TEST(SkipList, slist_can_sort) {
  BaseList<int>* tmp = new SkipList<int>();
  for (int i = 0; i < 10; i++) {
    tmp->addFirst(i);
  }
  tmp->Print();
  tmp->Sort(CompareL<int>);
  tmp->Print();
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(tmp->getFirst(), i);
    tmp->delFirst();
  }
  delete tmp;
}

TEST(SkipList, slist_can_be_ran_through) {
  BaseList<int>* tmp = new SkipList<int>();
  for (int i = 0; i < 10; i++) {
    tmp->addLast(i);
  }
  auto* begin = tmp->Begin();
  auto* end = tmp->End();
  int i = 0;
  for (auto* it = begin; *it != *end; ++(*it)) {
    EXPECT_EQ(**it, i);
    i++;
  }
  delete tmp;
  delete begin;
  delete end;
}

TEST(SkipList, slist_can_merge) {
  BaseList<int>* tmp = new SkipList<int>();
  BaseList<int>* tmp2 = new SkipList<int>();
  for (int i = 0; i < 10; i++) {
    tmp->addLast(i);
    tmp2->addLast(i + 10);
  }
  tmp->Merge(tmp2);
  auto* begin = tmp->Begin();
  auto* end = tmp->End();
  int i = 0;
  for (auto* it = begin; *it != *end; ++(*it)) {
    EXPECT_EQ(**it, i);
    i++;
  }
  EXPECT_EQ(tmp->Size(), 20);
  delete tmp;
  delete tmp2;
  delete begin;
  delete end;
}

TEST(SkipList, slist_can_merge_sorted) {
  BaseList<int>* tmp = new SkipList<int>();
  BaseList<int>* tmp2 = new SkipList<int>();
  for (int i = 0; i < 20; i += 2) {
    tmp->addLast(i);
    tmp2->addLast(i + 1);
  }
  tmp->mergeSorted(tmp2, CompareL<int>);
  auto* begin = tmp->Begin();
  auto* end = tmp->End();
  int i = 0;
  for (auto* it = begin; *it != *end; ++(*it)) {
    EXPECT_EQ(**it, i);
    i++;
  }
  EXPECT_EQ(tmp->Size(), 20);
  delete tmp;
  delete tmp2;
  delete begin;
  delete end;
}

TEST(SkipList, slist_can_assign) {
  SkipList<int> list, list2;
  for (int i = 0; i < 10; i++) {
    list.addLast(i);
  }
  list2 = list;
  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(list2.getAt(i), i);
  }
  EXPECT_EQ(list.Size(), list2.Size());
  list.Print();
  list2.Print();
  list2.printFE();
}
