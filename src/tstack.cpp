#include "tstack.h"
#include <stdexcept>
#include "tarith.h"

using namespace std;

template <typename T>
void TDynamicStack<T>::changeSize() {
  TDynamicStack<T> tmp(sz + 1);
  for (size_t i = 0; i < sz; i++)
    tmp.arr[i] = arr[i];
  swap(arr, tmp.arr);
  sz++;
}

template <typename T>
T TDynamicStack<T>::Top() {
  if (isEmpty())
    throw out_of_range("Stack is Empty");
  return arr[curr];
}

template <typename T>
void TDynamicStack<T>::Pop() {
  if (isEmpty())
    throw out_of_range("Stack is Empty");
  curr--;
}

template <typename T>
void TDynamicStack<T>::Push(T elem) {
  if (curr == MAXSTACK)
    throw out_of_range("Stack can't be extended");
  if (isFull())
    changeSize();
  arr[++curr] = elem;
}

template <typename T>
bool TDynamicStack<T>::isEmpty() {
  return curr == -1;
}

template <typename T>
bool TDynamicStack<T>::isFull() {
  return curr == sz - 1;
}

template class TDynamicStack<int>;
template class TDynamicStack<double>;
template class TDynamicStack<Lexeme<int>>;
template class TDynamicStack<Lexeme<double>>;
template class TDynamicStack<Exception>;
template class TDynamicStack<size_t>;
