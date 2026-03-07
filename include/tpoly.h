#pragma once
#include "tlist.h"
#include "tcomp.h"
#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T, template<typename> class List>
class Polynom;

// Monom

template <typename T, template<typename> class List = TStdList>
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
      x *= x;
      deg >>= 1;
    }
    return res;
  }
  T toDegree(T x, char deg) {
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
  Monom<T, List> operator*(const T& other) {
    Monom<T, List> tmp(coeff, N.a);
    tmp.coeff = coeff * other;
    return tmp;
  }
  Monom<T, List> operator-() {
    Monom<T, List> tmp(-coeff, N.a);
    return tmp;
  }
  Monom<T, List> operator*(const Monom<T, List>& other) {
    Monom<T, List> tmp(coeff, N.a);
    tmp.coeff *= other.coeff;
    for (size_t i = 0; i < 4; i++) {
      if (tmp.N.b[i] > 0 && other.N.b[i] > 0 && (127 - tmp.N.b[i]) < other.N.b[i])
        throw out_of_range("Big degree");
      if (tmp.N.b[i] < 0 && other.N.b[i] < 0 && (-128 + tmp.N.b[i]) > other.N.b[i])
        throw out_of_range("Little degree");
      tmp.N.b[i] += other.N.b[i];
    }
    return tmp;
  }
  Polynom<T, List> operator+(const T& other) {
    Monom<T, List> tmp(other, 0);
    return tmp + *this;
  }
  Polynom<T, List> operator+(const Monom<T, List>& other) {
    Polynom<T, List> tmp(coeff, N.a);
    return tmp + other;
  }
  Polynom<T, List> operator+(const Polynom<T, List> other) {
    other = other + *this;
    return other;
  }
  Polynom<T, List> operator-(const T& other) {
    Monom<T, List> tmp(other, 0);
    return *this - tmp;
  }
  Polynom<T, List> operator-(const Monom<T, List>& other) {
    Polynom<T, List> tmp(coeff, N.a);
    return tmp - other;
  }
  Polynom<T, List> operator-(const Polynom<T, List>& other) {
    Polynom<T, List> tmp(coeff, N.a);
    return tmp - other;
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
  friend Polynom<T, List> operator+(T left, Monom<T, List> right) {
    return right + left;
  }
  friend Polynom<T, List> operator-(T left, Monom<T, List> right) {
    Monom<T, List> tmp(left, 0);
    return tmp - right;
  }
  friend Monom<T, List> operator*(T left, Monom<T, List> right) {
    return right * left;
  }
};

// Comparator for Monoms

template <typename T, template<typename> class List = TStdList>
int CompareM(Monom<T, List> a, Monom<T, List> b) {
  for (size_t i = 0; i < 4; i++) {
    if(a.getAt(i) < b.getAt(i))
      return 0;
    if(a.getAt(i) > b.getAt(i))
      return 1;
  }
  return -1;
}

// Operation Plus

template <typename T, template<typename> class List = TStdList>
void OpPlus(Monom<T, List>& a, Monom<T, List>& b) {
  a.addOne(b);
}

// Operation Minus

template <typename T, template<typename> class List = TStdList>
void OpMinus(Monom<T, List>& a, Monom<T, List>& b) {
  a.subOne(b);
}

// Operation Multiply

template <typename T, template<typename> class List = TStdList>
void OpMultiply(Monom<T, List>& a, Monom<T, List>& b) {
  a = a * b;
}

// Polynom

template <typename T, template<typename> class List = TStdList>
class Polynom {
  List<Monom<T, List>> list;
public:
  Polynom(T startcoeff = 0, char x = 0, char y = 0, char z = 0, char w = 0) {
    Monom<T, List> tmp(startcoeff, 0);
    tmp.setAt(x, 0);
    tmp.setAt(y, 1);
    tmp.setAt(z, 2);
    tmp.setAt(w, 3);
    if (startcoeff != 0)
      list.addFirst(tmp);
  }
  Polynom<T, List> operator+(const T& other) {
    Monom<T, List> tmp(other, 0);
    return *this + tmp;
  }
  Polynom<T, List> operator-(const T& other) {
    Monom<T, List> tmp(other, 0);
    return *this - tmp;
  }
  Polynom<T, List> operator*(const T& other) {
    Monom<T, List> tmp(other, 0);
    return *this * tmp;
  }
  Polynom<T, List> operator+(const Monom<T, List> other) {
    Polynom<T, List> tmp(*this);
    tmp.list.addSorted(other, CompareM<T, List>, OpPlus<T, List>);
    return tmp;
  }
  Polynom<T, List> operator-(Monom<T, List> other) {
    Polynom<T, List> tmp(*this);
    other = -other;
    tmp.list.addSorted(other, CompareM<T, List>, OpMinus<T, List>);
    return tmp;
  }
  Polynom<T, List> operator*(const Monom<T, List> other) {
    Polynom<T, List> tmp(*this);
    auto* begin = tmp.list.Begin();
    auto* end = tmp.list.End();
    for (auto* it = begin; *it != *end; ++(*it)) {
      **it = **it * other;
    }
    delete begin;
    delete end;
    return tmp;
  }
  Polynom<T, List> operator+(Polynom<T, List> other) {
    Polynom<T, List> tmp(*this);
    tmp.list.mergeSorted(&other.list, CompareM<T, List>, OpPlus<T, List>);
    return tmp;
  }
  Polynom<T, List> operator-(Polynom<T, List> other) {
    Polynom<T, List> tmp(*this);
    tmp.list.mergeSorted(&other.list, CompareM<T, List>, OpMinus<T, List>, true);
    return tmp;
  }
  Polynom<T, List> operator*(Polynom<T, List> other) {
    Polynom<T, List> tmp;
    auto* begin = list.Begin();
    auto* end = list.End();
    auto* oend = other.list.End();
    for (auto* it1 = begin; *it1 != *end; ++(*it1)) {
      auto* obegin = other.list.Begin();
      for (auto* it2 = obegin; *it2 != *oend; ++(*it2)) {
        tmp.list.addLast((**it1) * (**it2));
      }
      delete obegin;
    }
    delete begin;
    delete end;
    delete oend;
    tmp.list.Sort(CompareM<T, List>, OpPlus<T, List>);
    return tmp;
  }
  T Count(T x = 1, T y = 1, T z = 1, T w = 1) {
    T tmp = 0;
    auto* begin = list.Begin();
    auto* end = list.End();
    for (auto* it = begin; *it != *end; ++(*it)) {
      tmp = tmp + (**it).Count(x, y, z, w);
    }
    return tmp;
  }
  friend Polynom<T, List> operator+(T left, Polynom<T, List> right) {
    return right + left;
  }
  friend Polynom<T, List> operator-(T left, Polynom<T, List> right) {
    Monom<T, List> tmp(left, 0);
    return tmp - right;
  }
  friend Polynom<T, List> operator*(T left, Polynom<T, List> right) {
    return right * left;
  }
  friend Polynom<T, List> operator*(Monom<T, List> left, const Polynom<T, List> other) {
    other = other * left;
    return other;
  }
};

