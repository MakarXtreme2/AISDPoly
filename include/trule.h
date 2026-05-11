#pragma once

#include <vector>
#include <iostream>

#include "lexeme.h"
#include "tstack.h"

class Rule {
  std::vector<NodeType> tokens;
  std::vector<int> positions;
  bool valid = true;
public:
  Rule() {}
  Rule(std::vector<NodeType> _tokens, std::vector<int> _positions) :
    tokens(_tokens), positions(_positions) {}

  bool isSet(std::vector<NodeType> other_tokens);
  bool isFullSet(std::vector<NodeType> other_tokens);
  bool isDirectSet(std::vector<NodeType> other_tokens);
  bool isFullDirectSet(std::vector<NodeType> other_tokens);

  void setRule(std::vector<NodeType> _tokens, std::vector<int> _positions) {
    tokens = _tokens;
    positions = _positions;
  }

  void setRule(std::vector<NodeType> _tokens) {
    tokens = _tokens;
  }

  int countOfTokens() { return tokens.size(); }

  void setInvalid() { valid = false; }
  bool isValid() { return valid; }
};

struct ReduceRule {
  NodeType term;
  std::vector<Rule> rules;

  ReduceRule() {}
  ReduceRule(NodeType _term, std::vector<Rule> _rules) :
    term(_term), rules(_rules) {}

  Rule isSet(std::vector<NodeType> other_tokens);
  Rule isFullSet(std::vector<NodeType> other_tokens);
  Rule isDirectSet(std::vector<NodeType> other_tokens);
  Rule isFullDirectSet(std::vector<NodeType> other_tokens);
};
