#include <gtest/gtest.h>
#include <vector>
#include "trule.h"
#include "lexeme.h"

TEST(Rule, rule_work_correct) {
  std::vector<NodeType> tokens;
  std::vector<int> pos;
  Rule rule;
  tokens.push_back(numberT);
  tokens.push_back(plusminusT);
  tokens.push_back(numberT);
  rule.setRule(tokens, pos);
  EXPECT_TRUE(rule.isFullSet(tokens));
  EXPECT_TRUE(rule.isSet(tokens));
  tokens.pop_back();
  EXPECT_FALSE(rule.isFullSet(tokens));
  EXPECT_TRUE(rule.isSet(tokens));
}

TEST(Rule, rule_work_correct_back) {
  std::vector<NodeType> tokens;
  std::vector<int> pos;
  Rule rule;
  tokens.push_back(numberT);
  tokens.push_back(plusminusT);
  tokens.push_back(numberT);
  tokens.push_back(numberT);
  rule.setRule(tokens, pos);
  tokens.clear();
  tokens.push_back(plusminusT);
  tokens.push_back(numberT);
  EXPECT_FALSE(rule.isFullDirectSet(tokens));
  EXPECT_TRUE(rule.isDirectSet(tokens));
  EXPECT_FALSE(rule.isSet(tokens));
  tokens.clear();
  tokens.push_back(numberT);
  tokens.push_back(numberT);
  tokens.push_back(plusminusT);
  tokens.push_back(numberT);
  EXPECT_TRUE(rule.isFullDirectSet(tokens));
  EXPECT_TRUE(rule.isDirectSet(tokens));
}

TEST(ReduceRule, reducerule_work_correct) {
  std::vector<NodeType> tokens;
  std::vector<int> pos;
  Rule rule;
  ReduceRule rcrule;
  tokens.push_back(numberT);
  tokens.push_back(plusminusT);
  tokens.push_back(numberT);
  rule.setRule(tokens, pos);
  rcrule.rules.push_back(rule);
  tokens.clear();
  tokens.push_back(numberT);
  tokens.push_back(muldivT);
  tokens.push_back(numberT);
  rule.setRule(tokens, pos);
  rcrule.rules.push_back(rule);
  tokens.clear();
  tokens.push_back(numberT);
  tokens.push_back(plusminusT);
  EXPECT_TRUE(rcrule.isSet(tokens).isValid());
  EXPECT_FALSE(rcrule.isFullSet(tokens).isValid());
  tokens.clear();
  tokens.push_back(numberT);
  tokens.push_back(muldivT);
  tokens.push_back(numberT);
  EXPECT_TRUE(rcrule.isSet(tokens).isValid());
  EXPECT_TRUE(rcrule.isFullSet(tokens).isValid());
}
