#pragma once

#include <iostream>

using namespace std;

enum LexemeType {
  number,
  single_operation,
  binary_operation,
  skobe,
  polynom,
  variable,
  assign,
  semicolon,
  special_word,
  condition,
  word
};

template <typename T>
struct Lexeme {
  string Text;
  T Value;
  LexemeType Type;
  int Priority;
};
