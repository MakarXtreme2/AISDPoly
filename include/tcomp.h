#pragma once

// Comparator where first value is greater than second

template <typename T>
bool CompareG(T a, T b) {
  return a > b;
}

// Comparator where first value is less than second

template <typename T>
bool CompareL(T a, T b) {
  return a < b;
}

// Comparator where first value is equal to second

template <typename T>
bool CompareE(T a, T b) {
  return a == b;
}

// Comparator where first value is greater than or equal to second

template <typename T>
bool CompareGE(T a, T b) {
  return a >= b;
}

// Comparator where first value is less than or equal to second

template <typename T>
bool CompareLE(T a, T b) {
  return a <= b;
}
