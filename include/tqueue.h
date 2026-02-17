#pragma once
#include <stdexcept>

#define MAXQUEUE 1000
using namespace std;

template <typename T>
class TDynamicQueue {
  T *arr;
  size_t sz;
  size_t st, fin;

  size_t Next(size_t i) { return (i + 1) % sz; }

  void changeSize();

public:

  TDynamicQueue(size_t _sz = 1) {
    if (_sz < 0 || _sz > MAXQUEUE)
      throw out_of_range("Invalid size");
    st = 0;
    sz = _sz + 1;
    arr = new T[sz];
    fin = sz - 1;
  }

  TDynamicQueue(const TDynamicQueue &qe) {
    sz = qe.sz;
    st = qe.st;
    fin = qe.fin;
    arr = new T[sz];
    for (size_t i = 0; i < sz; i++)
      arr[i] = qe.arr[i];
  }

  TDynamicQueue &operator=(const TDynamicQueue &qe) {
    if (this == &qe)
      return *this;
    delete[] arr;
    sz = qe.sz;
    st = qe.st;
    fin = qe.fin;
    arr = new T[sz];
    for (size_t i = 0; i < sz; i++)
      arr[i] = qe.arr[i];
    return *this;
  }

  ~TDynamicQueue() { delete[] arr; }

  size_t Size() {
    if (isEmpty())
      return 0;
    else {
      if (fin >= st)
        return fin - st + 1;
      else
        return sz - st + fin + 1;
    }
  }

  T Top();
  void Pop();
  void Push(T elem);
  bool isEmpty();
  bool isFull();

};
