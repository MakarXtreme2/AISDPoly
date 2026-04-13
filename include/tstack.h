#pragma once
#include <stdexcept>

#define MAXSTACK 1000
using namespace std;

template <typename T>
class TDynamicStack {
  T* arr;
  size_t sz;
  long long curr;

  void changeSize() {
    TDynamicStack<T> tmp(sz + 1);
    for (size_t i = 0; i < sz; i++)
      tmp.arr[i] = arr[i];
    swap(arr, tmp.arr);
    sz++;
  }

public:

  TDynamicStack(size_t _sz = 1) {
    if (_sz < 0 || _sz > MAXSTACK)
      throw out_of_range("Invalid size");
    sz = _sz;
    arr = new T[sz];
    curr = -1;
  }

  ~TDynamicStack() { delete[] arr; }

  size_t Size() { return curr + 1; }

  T Top() {
    if (isEmpty())
      throw out_of_range("Stack is Empty");
    return arr[curr];
  }

  TDynamicStack(const TDynamicStack<T>& other) {
    sz = other.sz;
    arr = new T[sz];
    curr = other.curr;
    for (size_t i = 0; i < sz; i++)
      arr[i] = other.arr[i];
  }

  TDynamicStack<T>& operator=(const TDynamicStack<T>& other) {
    if (this == &other)
      throw out_of_range("SelfAssign");
    if (sz != other.sz) {
      delete[] arr;
      sz = other.sz;
      arr = new T[sz];
    }
    curr = other.curr;
    for (size_t i = 0; i < sz; i++)
      arr[i] = other.arr[i];
    return *this;
  }

  void Clear() {
    sz = 0;
    curr = -1;
  }

  void Pop() {
    if (isEmpty())
      throw out_of_range("Stack is Empty");
    curr--;
  }

  void Push(T elem) {
    if (curr == MAXSTACK)
      throw out_of_range("Stack can't be extended");
    if (isFull())
      changeSize();
    arr[++curr] = elem;
  }

  bool isEmpty() { return curr == -1; }

  bool isFull() {
    return curr == sz - 1;
  }

  friend ostream& operator<<(ostream& out, TDynamicStack<T>& st) {
    TDynamicStack<T> tmp = st;
    while (!tmp.isEmpty()) {
      if (tmp.Size() == 1)
        out << tmp.Top();
      else
        out << tmp.Top() << " ";
      tmp.Pop();
    }
    return out;
  }

};
