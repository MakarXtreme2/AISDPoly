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
  EXPECT_ANY_THROW(mn.addOne(mn2));
  mn2.setN(5);
  mn = mn.addOne(mn2);
  EXPECT_EQ(mn.getCoeff(), 5);
}

TEST(Monom, monom_can_subtract) {
  Monom<int> mn(2, 5);
  Monom<int> mn2(3, 6);
  EXPECT_ANY_THROW(mn.subOne(mn2));
  mn2.setN(5);
  mn = mn.subOne(mn2);
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
  Monom<int> mn(2, 2, 2);
  EXPECT_EQ(mn.Count(3, 3, 1, 1), 162);
}

TEST(Polynom, polynom_can_be_created) {
  ASSERT_NO_THROW(Polynom<int> p1);
}

TEST(Polynom, polynom_can_contain_monom) {
  Polynom<int> p1(2, 2);
  EXPECT_EQ(p1.Count(2), 8);
}

TEST(Polynom, polynom_can_add_another_monom) {
  Polynom<int> p1(2, 2);
  Monom<int> mn(3, 3);
  p1 = p1 + mn;
  EXPECT_EQ(p1.Count(2), 32);
}

TEST(Polynom, polynom_can_subtract_monom) {
  Polynom<int> p1(2, 2);
  Monom<int> mn(3, 3);
  p1 = p1 - mn;
  EXPECT_EQ(p1.Count(2), -16);
}

TEST(Polynom, polynom_can_multiply_monom) {
  Polynom<int> p1(2, 2);
  Monom<int> mn(3, 3);
  p1 = p1 * mn;
  EXPECT_EQ(p1.Count(2), 192);
}

TEST(Polynom, polynom_can_add_number) {
  Polynom<int> p1(2, 2);
  int num = 10;
  p1 = p1 + num;
  EXPECT_EQ(p1.Count(2), 18);
}

TEST(Polynom, polynom_can_subtract_number) {
  Polynom<int> p1(2, 2);
  int num = 10;
  p1 = p1 - num;
  EXPECT_EQ(p1.Count(2), -2);
}

TEST(Polynom, polynom_can_multiply_number) {
  Polynom<int> p1(2, 2);
  int num = 10;
  p1 = p1 * num;
  EXPECT_EQ(p1.Count(2), 80);
}

TEST(Polynom, polynom_can_work_backward) {
  Polynom<double> p1(2, 2);
  Monom<double> mn1(1, 3), mn2(1, -3), mn3(1, 4);
  p1 = mn1 * mn3 - mn2 + p1;
  EXPECT_LE(p1.Count(2) - 135.875, 3e-10);
}

TEST(Polynom, polynom_can_do_hard_actions) {
  Polynom<int> p1(2, 2);
  Monom<int> mn1(2, 2), mn2(3, 3);
  int num1 = 20, num2 = -10;
  p1 = num1 + p1 - num2 * (p1 + mn1) * mn2;
  EXPECT_EQ(p1.Count(2), 3868);
}

TEST(Polynom, polynom_can_work_with_other_polynoms) {
  Polynom<int> p1(2, 2);
  Polynom<int> p2(1, 3);
  p1 = p1 + 10 - 2 * p2;
  EXPECT_EQ(p1.Count(2), 2);
  p1 = p1 * p1;
  EXPECT_EQ(p1.Count(2), 4);
}

TEST(Polynom, polynom_super_mega_test) {
  Polynom<int> p1(10, 3);
  Polynom<int> p2(20, 5, 2);
  Polynom<int> p3(30, 0, 0, 4);
  p2 = p2 + p3;
  cout << p2 << endl;
  p1 = p3 - p1;
  cout << p1 << endl;
  p1 = p1 * p2;
  cout << p1 << endl;
  EXPECT_EQ(p1.Count(1, 2, 3), 6074200);
}

TEST(Polynom, polynom_can_count_same_value) {
  Polynom<int> p1(1);
  Monom<int> mn1(1, 1);
  Monom<int> mn2(1, 2);
  Monom<int> mn3(1, 3);
  p1 = p1 + mn1 + mn2 + mn3;
  p1 = p1 * p1;
  EXPECT_EQ(p1.Count(2), 225);
}
