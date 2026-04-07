#include <gtest/gtest.h>
#include "ttree.h"

TEST(AVLTree, ttree_can_add_value) {
  AVLTree<int> tr;
  tr.Insert(6);
  tr.printTLR();
  tr.printTLR(true);
  tr.Insert(3);
  tr.printTLR();
  tr.printTLR(true);
  tr.Insert(1);
  tr.printTLR();
  tr.printTLR(true);
  tr.Insert(8);
  tr.printTLR();
  tr.printTLR(true);
  tr.Insert(4);
  tr.printTLR();
  tr.printTLR(true);
  tr.Insert(5);
  tr.printTLR();
  tr.printTLR(true);
}
