#include <gtest/gtest.h>
#include "ttree.h"

TEST(AVLTree, ttree_can_add_and_delete_value) {
  AVLTree<int> tr;
  tr.Insert(6);
  tr.printTRL();
  tr.printTRL(true);
  tr.Insert(3);
  tr.printTRL();
  tr.printTRL(true);
  tr.Insert(1);
  tr.printTRL();
  tr.printTRL(true);
  tr.Insert(8);
  tr.printTRL();
  tr.printTRL(true);
  tr.Insert(4);
  tr.printTRL();
  tr.printTRL(true);
  tr.Insert(5);
  tr.printTRL();
  tr.printTRL(true);
  EXPECT_NE(tr.Find(5), nullptr);
  EXPECT_EQ(tr.Find(99), nullptr);
  tr.Delete(8);
  EXPECT_EQ(tr.Find(8), nullptr);
  tr.printTRL();
  tr.printTRL(true);
  tr.Delete(3);
  EXPECT_EQ(tr.Find(8), nullptr);
  tr.printTRL();
  tr.printTRL(true);
  tr.Delete(1);
  EXPECT_EQ(tr.Find(1), nullptr);
  tr.printTRL();
  tr.printTRL(true);
  tr.Delete(5);
  EXPECT_EQ(tr.Find(5), nullptr);
  tr.printTRL();
  tr.printTRL(true);
  tr.Delete(4);
  EXPECT_EQ(tr.Find(4), nullptr);
  tr.printTRL();
  tr.printTRL(true);
  tr.Delete(6);
  EXPECT_EQ(tr.Find(6), nullptr);
  tr.printTRL();
  tr.printTRL(true);
}

TEST(AVLTree, ttree_can_clear) {
  AVLTree<int> tr;
  tr.Insert(6);
  tr.Insert(3);
  tr.Insert(1);
  tr.Insert(8);
  tr.Insert(4);
  tr.Insert(5);
  tr.printLTR();
  tr.printTRL();
  tr.Clear();
  EXPECT_EQ(tr.Size(), 0);
}

TEST(TableAVL, tableavl_work_correct) {
  TableAVL<char, int> table;
  table.Insert('a', 10);
  table.Insert('b', 20);
  table.printLTR();
  table.Insert('a', 30);
  table.printLTR();
  table.Insert('d', 10);
  table.Insert('c', 80);
  table.printLTR();
  table.printTRL();
  table.Delete('d');
  table.printLTR();
  EXPECT_EQ(*table.Find('a'), 30);
}
