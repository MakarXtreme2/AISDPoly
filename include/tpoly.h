#pragma once
#include "tlist.h"
#include "tcomp.h"
#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T, class List>
class Polynom;

// Monom

template <typename T, class List = TStdList<T>>
class Monom {
  union Degree {
    unsigned int a;
    char b[4];
  };
  Degree N;
  T coeff;
  T quickDegree(T x, char deg) {
    T res = 1;
    while (deg > 0) {
      if (deg & 1) {
        res *= x;
      }
      res *= res;
      deg >>= 1;
    }
    return res;
  }
  T toDegree(T x, char deg) {
    T res = 1;
    if (deg > 0)
      return quickDegree(x, deg);
    else if (deg < 0)
      return 1 / quickDegree(x, deg);
    return 1;
  }
public:
  Monom(T _coeff = 0, unsigned int _a = 0) :
    coeff(_coeff) {
    N.a = _a;
  }
  Monom<T, List>& addOne(const Monom<T, List>& other) {
    if (N.a != other.N.a)
      throw out_of_range("Different monoms");
    coeff += other.coeff;
    return *this;
  }
  Monom<T, List>& subOne(const Monom<T, List>& other) {
    if (N.a != other.N.a)
      throw out_of_range("Different monoms");
    coeff -= other.coeff;
    return *this;
  }
  Monom<T, List>& operator*(const Monom<T, List>& other) {
    coeff *= other.coeff;
    for (size_t i = 0; i < 4; i++) {
      if (N.b[i] > 0 && other.N.b[i] > 0 && (127 - N.b[i]) < other.N.b[i])
        throw out_of_range("Big degree");
      if (N.b[i] < 0 && other.N.b[i] < 0 && (-128 + N.b[i]) > other.N.b[i])
        throw out_of_range("Little degree");
      N.b[i] += other.N.b[i];
    }
    return *this;
  }
  T Count(T x, T y, T z, T w) {
    T tmp = coeff;
    tmp *= toDegree(x, N.b[0]);
    tmp *= toDegree(y, N.b[1]);
    tmp *= toDegree(z, N.b[2]);
    tmp *= toDegree(w, N.b[3]);
    return tmp;
  }
  void setAt(char val, size_t ind) {
    if (ind < 0 || ind > 3)
      throw out_of_range("Wrong Index");
    N.b[ind] = val;
  }
  void setN(unsigned int val) {
    N.a = val;
  }
  char getAt(size_t ind) {
    if (ind < 0 || ind > 3)
      throw out_of_range("Wrong Index");
    return N.b[ind];
  }
  unsigned int getN() {
    return N.a;
  }
  void setCoeff(T val) {
    coeff = val;
  }
  T getCoeff() {
    return coeff;
  }
};

// Comparator for Monoms

template <typename T, class List = TStdList<T>>
int CompareM(Monom<T, List> a, Monom<T, List> b) {
  if(a.getN() < b.getN())
    return 0;
  if(a.getN() > b.getN())
    return 1;
  return -1;
}

// Operation Plus

template <typename T, class List = TStdList<T>>
void OpPlus(Monom<T, List>& a, Monom<T, List>& b) {
  a.addOne(b);
}

// Operation Minus

template <typename T, class List = TStdList<T>>
void OpMinus(Monom<T, List>& a, Monom<T, List>& b) {
  a.subOne(b);
}

// Operation Multiply

template <typename T, class List = TStdList<T>>
void OpMultiply(Monom<T, List>& a, Monom<T, List>& b) {
  a = a * b;
}

// Polynom

template <typename T, class List = TStdList<T>>
class Polynom {
  TStdList<Monom<T, List>> list;
public:
  Polynom(T startcoeff = 0, unsigned int startN = 0) {
    Monom<T, List> tmp(startcoeff, startN);
    if (startcoeff != 0)
      list.addFirst(tmp);
  }
  Polynom<T, List>& operator+(const Monom<T, List> other) {
    list.addSorted(other, CompareM<T, List>, OpPlus<T, List>);
    return *this;
  }
  Polynom<T, List>& operator-(const Monom<T, List> other) {
    list.addSorted(other, CompareM<T, List>, OpMinus<T, List>);
    return *this;
  }
  Polynom<T, List>& operator*(const Monom<T, List> other) {
    auto* begin = list.Begin();
    auto* end = list.End();
    for (auto* it = begin; *it != *end; ++(*it)) {
      **it = **it * other;
    }
    delete begin;
    delete end;
    return *this;
  }
  Polynom<T, List>& operator+(const Polynom<T, List>& other) {

  }
  Polynom<T, List>& operator-(const Polynom<T, List>& other) {

  }
  Polynom<T, List>& operator*(const Polynom<T, List>& other) {

  }
  T Count() {
  }
};
