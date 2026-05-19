#include <gtest/gtest.h>
#include <iostream>
#include "thash.h"

using namespace std;

TEST (HashTable, hash_table_work_correct) {
  HashTable<string, int> ht(100);
  cout << "1\n";
  ht.Insert("x", 10);
  ht.Insert("yxgs", 50);
  ht.Insert("w", 80);
  EXPECT_EQ(ht.Find("d"), nullptr);
  EXPECT_NE(ht.Find("w"), nullptr);
  EXPECT_EQ(*ht.Find("w"), 80);
  ht.Delete("w");
  EXPECT_EQ(ht.Find("w"), nullptr);
}
