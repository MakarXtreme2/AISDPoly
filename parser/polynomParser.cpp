
// Generated from polynom.g4 by ANTLR 4.13.2


#include "polynomVisitor.h"

#include "polynomParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct PolynomParserStaticData final {
  PolynomParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  PolynomParserStaticData(const PolynomParserStaticData&) = delete;
  PolynomParserStaticData(PolynomParserStaticData&&) = delete;
  PolynomParserStaticData& operator=(const PolynomParserStaticData&) = delete;
  PolynomParserStaticData& operator=(PolynomParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag polynomParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<PolynomParserStaticData> polynomParserStaticData = nullptr;

void polynomParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (polynomParserStaticData != nullptr) {
    return;
  }
#else
  assert(polynomParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<PolynomParserStaticData>(
    std::vector<std::string>{
      "poly", "mon"
    },
    std::vector<std::string>{
      "", "'('", "')'", "", "", "'*'", "'/'", "'+'", "'-'", "'x'", "'y'", 
      "'z'", "'w'"
    },
    std::vector<std::string>{
      "", "", "", "NEWLINE", "WS", "MUL", "DIV", "ADD", "SUB", "X", "Y", 
      "Z", "W", "ARROWUP", "INT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,14,42,2,0,7,0,2,1,7,1,1,0,1,0,1,0,1,0,1,0,1,0,3,0,11,8,0,1,0,1,0,
  	1,0,1,0,1,0,1,0,5,0,19,8,0,10,0,12,0,22,9,0,1,1,3,1,25,8,1,1,1,4,1,28,
  	8,1,11,1,12,1,29,1,1,3,1,33,8,1,4,1,35,8,1,11,1,12,1,36,1,1,3,1,40,8,
  	1,1,1,0,1,0,2,0,2,0,3,1,0,5,6,1,0,7,8,1,0,9,12,47,0,10,1,0,0,0,2,39,1,
  	0,0,0,4,5,6,0,-1,0,5,11,3,2,1,0,6,7,5,1,0,0,7,8,3,0,0,0,8,9,5,2,0,0,9,
  	11,1,0,0,0,10,4,1,0,0,0,10,6,1,0,0,0,11,20,1,0,0,0,12,13,10,4,0,0,13,
  	14,7,0,0,0,14,19,3,0,0,5,15,16,10,3,0,0,16,17,7,1,0,0,17,19,3,0,0,4,18,
  	12,1,0,0,0,18,15,1,0,0,0,19,22,1,0,0,0,20,18,1,0,0,0,20,21,1,0,0,0,21,
  	1,1,0,0,0,22,20,1,0,0,0,23,25,5,14,0,0,24,23,1,0,0,0,24,25,1,0,0,0,25,
  	34,1,0,0,0,26,28,7,2,0,0,27,26,1,0,0,0,28,29,1,0,0,0,29,27,1,0,0,0,29,
  	30,1,0,0,0,30,32,1,0,0,0,31,33,5,13,0,0,32,31,1,0,0,0,32,33,1,0,0,0,33,
  	35,1,0,0,0,34,27,1,0,0,0,35,36,1,0,0,0,36,34,1,0,0,0,36,37,1,0,0,0,37,
  	40,1,0,0,0,38,40,5,14,0,0,39,24,1,0,0,0,39,38,1,0,0,0,40,3,1,0,0,0,8,
  	10,18,20,24,29,32,36,39
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  polynomParserStaticData = std::move(staticData);
}

}

polynomParser::polynomParser(TokenStream *input) : polynomParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

polynomParser::polynomParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  polynomParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *polynomParserStaticData->atn, polynomParserStaticData->decisionToDFA, polynomParserStaticData->sharedContextCache, options);
}

polynomParser::~polynomParser() {
  delete _interpreter;
}

const atn::ATN& polynomParser::getATN() const {
  return *polynomParserStaticData->atn;
}

std::string polynomParser::getGrammarFileName() const {
  return "polynom.g4";
}

const std::vector<std::string>& polynomParser::getRuleNames() const {
  return polynomParserStaticData->ruleNames;
}

const dfa::Vocabulary& polynomParser::getVocabulary() const {
  return polynomParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView polynomParser::getSerializedATN() const {
  return polynomParserStaticData->serializedATN;
}


//----------------- PolyContext ------------------------------------------------------------------

polynomParser::PolyContext::PolyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t polynomParser::PolyContext::getRuleIndex() const {
  return polynomParser::RulePoly;
}

void polynomParser::PolyContext::copyFrom(PolyContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- MulDivContext ------------------------------------------------------------------

std::vector<polynomParser::PolyContext *> polynomParser::MulDivContext::poly() {
  return getRuleContexts<polynomParser::PolyContext>();
}

polynomParser::PolyContext* polynomParser::MulDivContext::poly(size_t i) {
  return getRuleContext<polynomParser::PolyContext>(i);
}

tree::TerminalNode* polynomParser::MulDivContext::MUL() {
  return getToken(polynomParser::MUL, 0);
}

tree::TerminalNode* polynomParser::MulDivContext::DIV() {
  return getToken(polynomParser::DIV, 0);
}

polynomParser::MulDivContext::MulDivContext(PolyContext *ctx) { copyFrom(ctx); }


std::any polynomParser::MulDivContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<polynomVisitor*>(visitor))
    return parserVisitor->visitMulDiv(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AddSubContext ------------------------------------------------------------------

std::vector<polynomParser::PolyContext *> polynomParser::AddSubContext::poly() {
  return getRuleContexts<polynomParser::PolyContext>();
}

polynomParser::PolyContext* polynomParser::AddSubContext::poly(size_t i) {
  return getRuleContext<polynomParser::PolyContext>(i);
}

tree::TerminalNode* polynomParser::AddSubContext::ADD() {
  return getToken(polynomParser::ADD, 0);
}

tree::TerminalNode* polynomParser::AddSubContext::SUB() {
  return getToken(polynomParser::SUB, 0);
}

polynomParser::AddSubContext::AddSubContext(PolyContext *ctx) { copyFrom(ctx); }


std::any polynomParser::AddSubContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<polynomVisitor*>(visitor))
    return parserVisitor->visitAddSub(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParensContext ------------------------------------------------------------------

polynomParser::PolyContext* polynomParser::ParensContext::poly() {
  return getRuleContext<polynomParser::PolyContext>(0);
}

polynomParser::ParensContext::ParensContext(PolyContext *ctx) { copyFrom(ctx); }


std::any polynomParser::ParensContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<polynomVisitor*>(visitor))
    return parserVisitor->visitParens(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MonomContext ------------------------------------------------------------------

polynomParser::MonContext* polynomParser::MonomContext::mon() {
  return getRuleContext<polynomParser::MonContext>(0);
}

polynomParser::MonomContext::MonomContext(PolyContext *ctx) { copyFrom(ctx); }


std::any polynomParser::MonomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<polynomVisitor*>(visitor))
    return parserVisitor->visitMonom(this);
  else
    return visitor->visitChildren(this);
}

polynomParser::PolyContext* polynomParser::poly() {
   return poly(0);
}

polynomParser::PolyContext* polynomParser::poly(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  polynomParser::PolyContext *_localctx = _tracker.createInstance<PolyContext>(_ctx, parentState);
  polynomParser::PolyContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 0;
  enterRecursionRule(_localctx, 0, polynomParser::RulePoly, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(10);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case polynomParser::X:
      case polynomParser::Y:
      case polynomParser::Z:
      case polynomParser::W:
      case polynomParser::INT: {
        _localctx = _tracker.createInstance<MonomContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(5);
        mon();
        break;
      }

      case polynomParser::T__0: {
        _localctx = _tracker.createInstance<ParensContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(6);
        match(polynomParser::T__0);
        setState(7);
        poly(0);
        setState(8);
        match(polynomParser::T__1);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(20);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(18);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<MulDivContext>(_tracker.createInstance<PolyContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RulePoly);
          setState(12);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(13);
          antlrcpp::downCast<MulDivContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == polynomParser::MUL

          || _la == polynomParser::DIV)) {
            antlrcpp::downCast<MulDivContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(14);
          poly(5);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<AddSubContext>(_tracker.createInstance<PolyContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RulePoly);
          setState(15);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(16);
          antlrcpp::downCast<AddSubContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == polynomParser::ADD

          || _la == polynomParser::SUB)) {
            antlrcpp::downCast<AddSubContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(17);
          poly(4);
          break;
        }

        default:
          break;
        } 
      }
      setState(22);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- MonContext ------------------------------------------------------------------

polynomParser::MonContext::MonContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* polynomParser::MonContext::INT() {
  return getToken(polynomParser::INT, 0);
}

std::vector<tree::TerminalNode *> polynomParser::MonContext::ARROWUP() {
  return getTokens(polynomParser::ARROWUP);
}

tree::TerminalNode* polynomParser::MonContext::ARROWUP(size_t i) {
  return getToken(polynomParser::ARROWUP, i);
}

std::vector<tree::TerminalNode *> polynomParser::MonContext::X() {
  return getTokens(polynomParser::X);
}

tree::TerminalNode* polynomParser::MonContext::X(size_t i) {
  return getToken(polynomParser::X, i);
}

std::vector<tree::TerminalNode *> polynomParser::MonContext::Y() {
  return getTokens(polynomParser::Y);
}

tree::TerminalNode* polynomParser::MonContext::Y(size_t i) {
  return getToken(polynomParser::Y, i);
}

std::vector<tree::TerminalNode *> polynomParser::MonContext::Z() {
  return getTokens(polynomParser::Z);
}

tree::TerminalNode* polynomParser::MonContext::Z(size_t i) {
  return getToken(polynomParser::Z, i);
}

std::vector<tree::TerminalNode *> polynomParser::MonContext::W() {
  return getTokens(polynomParser::W);
}

tree::TerminalNode* polynomParser::MonContext::W(size_t i) {
  return getToken(polynomParser::W, i);
}


size_t polynomParser::MonContext::getRuleIndex() const {
  return polynomParser::RuleMon;
}


std::any polynomParser::MonContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<polynomVisitor*>(visitor))
    return parserVisitor->visitMon(this);
  else
    return visitor->visitChildren(this);
}

polynomParser::MonContext* polynomParser::mon() {
  MonContext *_localctx = _tracker.createInstance<MonContext>(_ctx, getState());
  enterRule(_localctx, 2, polynomParser::RuleMon);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(39);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(24);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == polynomParser::INT) {
        setState(23);
        match(polynomParser::INT);
      }
      setState(34); 
      _errHandler->sync(this);
      alt = 1;
      do {
        switch (alt) {
          case 1: {
                setState(27); 
                _errHandler->sync(this);
                alt = 1;
                do {
                  switch (alt) {
                    case 1: {
                          setState(26);
                          _la = _input->LA(1);
                          if (!((((_la & ~ 0x3fULL) == 0) &&
                            ((1ULL << _la) & 7680) != 0))) {
                          _errHandler->recoverInline(this);
                          }
                          else {
                            _errHandler->reportMatch(this);
                            consume();
                          }
                          break;
                        }

                  default:
                    throw NoViableAltException(this);
                  }
                  setState(29); 
                  _errHandler->sync(this);
                  alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx);
                } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
                setState(32);
                _errHandler->sync(this);

                switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
                case 1: {
                  setState(31);
                  match(polynomParser::ARROWUP);
                  break;
                }

                default:
                  break;
                }
                break;
              }

        default:
          throw NoViableAltException(this);
        }
        setState(36); 
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
      } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(38);
      match(polynomParser::INT);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool polynomParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 0: return polySempred(antlrcpp::downCast<PolyContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool polynomParser::polySempred(PolyContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 4);
    case 1: return precpred(_ctx, 3);

  default:
    break;
  }
  return true;
}

void polynomParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  polynomParserInitialize();
#else
  ::antlr4::internal::call_once(polynomParserOnceFlag, polynomParserInitialize);
#endif
}
