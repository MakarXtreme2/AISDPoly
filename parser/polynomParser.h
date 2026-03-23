
// Generated from polynom.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  polynomParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, NEWLINE = 3, WS = 4, MUL = 5, DIV = 6, ADD = 7, 
    SUB = 8, X = 9, Y = 10, Z = 11, W = 12, ARROWUP = 13, INT = 14
  };

  enum {
    RulePoly = 0, RuleMon = 1
  };

  explicit polynomParser(antlr4::TokenStream *input);

  polynomParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~polynomParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class PolyContext;
  class MonContext; 

  class  PolyContext : public antlr4::ParserRuleContext {
  public:
    PolyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    PolyContext() = default;
    void copyFrom(PolyContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  MulDivContext : public PolyContext {
  public:
    MulDivContext(PolyContext *ctx);

    antlr4::Token *op = nullptr;
    std::vector<PolyContext *> poly();
    PolyContext* poly(size_t i);
    antlr4::tree::TerminalNode *MUL();
    antlr4::tree::TerminalNode *DIV();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AddSubContext : public PolyContext {
  public:
    AddSubContext(PolyContext *ctx);

    antlr4::Token *op = nullptr;
    std::vector<PolyContext *> poly();
    PolyContext* poly(size_t i);
    antlr4::tree::TerminalNode *ADD();
    antlr4::tree::TerminalNode *SUB();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ParensContext : public PolyContext {
  public:
    ParensContext(PolyContext *ctx);

    PolyContext *poly();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  MonomContext : public PolyContext {
  public:
    MonomContext(PolyContext *ctx);

    MonContext *mon();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  PolyContext* poly();
  PolyContext* poly(int precedence);
  class  MonContext : public antlr4::ParserRuleContext {
  public:
    MonContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INT();
    std::vector<antlr4::tree::TerminalNode *> ARROWUP();
    antlr4::tree::TerminalNode* ARROWUP(size_t i);
    std::vector<antlr4::tree::TerminalNode *> X();
    antlr4::tree::TerminalNode* X(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Y();
    antlr4::tree::TerminalNode* Y(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Z();
    antlr4::tree::TerminalNode* Z(size_t i);
    std::vector<antlr4::tree::TerminalNode *> W();
    antlr4::tree::TerminalNode* W(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MonContext* mon();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool polySempred(PolyContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

