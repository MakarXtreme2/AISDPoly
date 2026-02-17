#include "tqueue.h"
#include "tarith.h"
#include <stdexcept>

template <typename T>
void TDynamicQueue<T>::changeSize() {
  TDynamicQueue<T> tmp(sz + 1);
  int j = 0;
  for (size_t i = st; i != Next(fin); i = Next(i))
    tmp.arr[j++] = arr[i];
  swap(arr, tmp.arr);
  st = 0;
  fin = sz - 2;
  sz++;
}

template <typename T>
T TDynamicQueue<T>::Top() {
  if (isEmpty())
    throw out_of_range("Queue is Empty");
  return arr[st];
}

template <typename T>
void TDynamicQueue<T>::Pop() {
  if (isEmpty())
    throw out_of_range("Queue is Empty");
  st = Next(st);
}

template <typename T>
void TDynamicQueue<T>::Push(T elem) {
  if (sz == MAXQUEUE)
    throw out_of_range("Max size");
  if (isFull())
    changeSize();
  fin = Next(fin);
  arr[fin] = elem;
}

template <typename T>
bool TDynamicQueue<T>::isEmpty() {
  return st == Next(fin);
}

template <typename T>
bool TDynamicQueue<T>::isFull() {
  return st == Next(Next(fin));
}

template class TDynamicQueue<int>;
template class TDynamicQueue<double>;
template class TDynamicQueue<Lexeme<int>>;
template class TDynamicQueue<Lexeme<double>>;
template class TDynamicQueue<Exception>;
template class TDynamicQueue<size_t>;
