#pragma once
#include "tlist.h"
#include "tcomp.h"
#include <iostream>
#include <stdexcept>

using namespace std;


// Monom

template <typename T>
class Monom {
  union Degree {
    unsigned int a;
    char b[4];
  };
  Degree N;
  T coeff;
  T toDegree(T x, char deg) {
    T res = 1;
    if (deg > 0) {
      while (deg > 0) {
        if (deg & 1) {
          res *= x;
        }
        res *= res;
        deg >>= 1;
      }
      return res;
    }
    else if (deg < 0) {
      while (deg > 0) {
        if (deg & 1) {
          res *= x;
        }
        res *= res;
        deg >>= 1;
      }
      return 1 / res;
    }
    return 1;
  }
public:
  Monom(T _coeff = 0, unsigned int _a = 0) :
    coeff(_coeff) {
    N.a = _a;
  }
  Monom<T>& operator+(const Monom<T>& other) {
    if (N.a != other.N.a)
      throw out_of_range("Different monoms");
    coeff += other.coeff;
    return *this;
  }
  Monom<T>& operator-(const Monom<T>& other) {
    if (N.a != other.N.a)
      throw out_of_range("Different monoms");
    coeff -= other.coeff;
    return *this;
  }
  Monom<T>& operator*(const Monom<T>& other) {
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

template <typename T>
bool CompareM(Monom<T> a, Monom<T> b) {
  return a.getN() < b.getN();
}

// Operation Plus

template <typename T>
void OpPlus(Monom<T>& a, Monom<T>& b) {
  a = a + b;
}

// Operation Minus

template <typename T>
void OpMinus(Monom<T>& a, Monom<T>& b) {
  a = a - b;
}

// Operation Multiply

template <typename T>
void OpMultiply(Monom<T>& a, Monom<T>& b) {
  a = a * b;
}

// Polynom

template <typename T, typename List = TStdList<T>>
class Polynom {
  TStdList<Monom<T>> list;
public:
  Polynom(T startcoeff = 0, unsigned int startN = 0) {
    Monom<T> tmp(startcoeff, startN);
    if (startcoeff != 0)
      list.addFirst(tmp);
  }
  Polynom& operator+(const Monom<T> other) {
    list.addSorted(other, CompareM<T>, OpPlus<T>);
    return *this;
  }
  Polynom& operator-(const Monom<T> other) {
    list.addSorted(other, CompareM<T>, OpMinus<T>);
    return *this;
  }
  Polynom& operator*(const Monom<T> other) {
    auto* begin = list.Begin();
    auto* end = list.End();
    for (auto* it = begin; *it != *end; ++(*it)) {
      **it = **it * other;
    }
    return *this;
  }
  friend Polynom& operator+(Monom<T> first, Polynom<T, List> second);
  friend Polynom& operator-(Monom<T> first, Polynom<T, List> second);
  friend Polynom& operator*(Monom<T> first, Polynom<T, List> second);
  Polynom& operator+(const Polynom& other) {

  }
  Polynom& operator-(const Polynom& other) {

  }
  Polynom& operator*(const Polynom& other) {

  }
  T Count() {
  }
};
