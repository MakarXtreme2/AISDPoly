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

  void changeSize() {
    TDynamicQueue<T> tmp(sz + 1);
    int j = 0;
    for (size_t i = st; i != Next(fin); i = Next(i))
      tmp.arr[j++] = arr[i];
    swap(arr, tmp.arr);
    st = 0;
      fin = sz - 2;
    sz++;
  }

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

  T Top() {
    if (isEmpty())
      throw out_of_range("Queue is Empty");
    return arr[st];
  }

  void Pop() {
    if (isEmpty())
      throw out_of_range("Queue is Empty");
    st = Next(st);
  }

  void Push(T elem) {
    if (sz == MAXQUEUE)
      throw out_of_range("Max size");
    if (isFull())
      changeSize();
    fin = Next(fin);
    arr[fin] = elem;
  }

  bool isEmpty() { return st == Next(fin); }

  bool isFull() {
    return st == Next(Next(fin));
  }

};
