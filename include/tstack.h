#pragma once
#include <stdexcept>

#define MAXSTACK 1000
using namespace std;

template <typename T>
class TDynamicStack {
  T* arr;
  size_t sz;
  size_t curr;

  void changeSize();

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

  T Top();
  void Pop();
  void Push(T elem);
  bool isEmpty();
  bool isFull();

};
