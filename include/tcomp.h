#pragma once

// Comparator where first value is greater than second

template <typename T>
int CompareG(T a, T b) {
  return a > b ? 1 : 0;
}

// Comparator where first value is less than second

template <typename T>
int CompareL(T a, T b) {
  return a < b ? 1 : 0;
}

// Comparator where first value is equal to second

template <typename T>
int CompareE(T a, T b) {
  return a == b ? 1 : 0;
}

// Comparator where first value is greater than or equal to second

template <typename T>
int CompareGE(T a, T b) {
  return a >= b ? 1 : 0;
}

// Comparator where first value is less than or equal to second

template <typename T>
int CompareLE(T a, T b) {
  return a <= b ? 1 : 0;
}
