#include <gtest/gtest.h>
#include "tpoly.h"

TEST(Monom, monom_can_be_created) {
  Monom<int> mn(2, 5);
  EXPECT_EQ(mn.getCoeff(), 2);
  mn.setCoeff(3);
  EXPECT_EQ(mn.getCoeff(), 3);
  EXPECT_EQ(mn.getAt(0), 5);
  EXPECT_EQ(mn.getN(), 5);
  mn.setAt(-10, 0);
  EXPECT_EQ(mn.getAt(0), -10);
}

TEST(Monom, monom_can_sum) {
  Monom<int> mn(2, 5);
  Monom<int> mn2(3, 6);
  EXPECT_ANY_THROW(mn + mn2);
  mn2.setN(5);
  mn = mn + mn2;
  EXPECT_EQ(mn.getCoeff(), 5);
}

TEST(Monom, monom_can_subtract) {
  Monom<int> mn(2, 5);
  Monom<int> mn2(3, 6);
  EXPECT_ANY_THROW(mn - mn2);
  mn2.setN(5);
  mn = mn - mn2;
  EXPECT_EQ(mn.getCoeff(), -1);
}

TEST(Monom, monom_can_multiply) {
  Monom<int> mn(2, 5);
  Monom<int> mn2(3, 6);
  mn = mn * mn2;
  EXPECT_EQ(mn.getCoeff(), 6);
  EXPECT_EQ(mn.getN(), 11);
  mn.setN(127);
  EXPECT_ANY_THROW(mn * mn2);
  mn2.setAt(-10, 0);
  mn = mn * mn2;
  EXPECT_EQ(mn.getAt(0), 117);
}

TEST(Monom, monom_can_count) {
  Monom<int> mn(2, 514);
  EXPECT_EQ(mn.Count(3, 3, 1, 1), 162);
}
