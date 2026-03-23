
// Generated from polynom.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  polynomLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, NEWLINE = 3, WS = 4, MUL = 5, DIV = 6, ADD = 7, 
    SUB = 8, X = 9, Y = 10, Z = 11, W = 12, ARROWUP = 13, INT = 14
  };

  explicit polynomLexer(antlr4::CharStream *input);

  ~polynomLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

