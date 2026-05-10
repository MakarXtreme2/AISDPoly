#include "trule.h"

bool Rule::isSet(std::vector<NodeType> other_tokens) {
  if (tokens.size() < other_tokens.size())
    return false;
  int size = tokens.size();
  int other_size = other_tokens.size();
  for (int i = 0; i < other_size; ++i) {
    if (tokens[size - i - 1] != other_tokens[i])
      return false;
  }
  return true;
}

bool Rule::isFullSet(std::vector<NodeType> other_tokens) {
  if (tokens.size() != other_tokens.size())
    return false;
  for (size_t i = 0; i < tokens.size(); ++i) {
    if (tokens[tokens.size() - i - 1] != other_tokens[i])
      return false;
  }
  return true;
}

Rule ReduceRule::isSet(std::vector<NodeType> other_tokens) {
  Rule tmp;
  tmp.setInvalid();
  for (size_t i = 0; i < rules.size(); ++i) {
    if (rules[i].isSet(other_tokens))
      return rules[i];
  }
  return tmp;
}

Rule ReduceRule::isFullSet(std::vector<NodeType> other_tokens) {
  Rule tmp;
  tmp.setInvalid();
  for (size_t i = 0; i < rules.size(); ++i) {
    if (rules[i].isFullSet(other_tokens))
      return rules[i];
  }
  return tmp;
}
