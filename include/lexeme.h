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

enum NodeType {
  numberT,
  varT,
  numvarT,
  endT,
  beginT,
  whileT,
  singleminusT,
  plusminusT,
  muldivT,
  condT,
  skobeopenT,
  skobecloseT,
  assignT,
  scT,
  exprT,
  conditionT,
  assignopT,
  lineT,
  codeblockT,
  cycleT,
  progT
};


string nodetypeToStr(NodeType node_type);
