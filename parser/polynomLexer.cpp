
// Generated from polynom.g4 by ANTLR 4.13.2


#include "polynomLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct PolynomLexerStaticData final {
  PolynomLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  PolynomLexerStaticData(const PolynomLexerStaticData&) = delete;
  PolynomLexerStaticData(PolynomLexerStaticData&&) = delete;
  PolynomLexerStaticData& operator=(const PolynomLexerStaticData&) = delete;
  PolynomLexerStaticData& operator=(PolynomLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag polynomlexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<PolynomLexerStaticData> polynomlexerLexerStaticData = nullptr;

void polynomlexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (polynomlexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(polynomlexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<PolynomLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "NEWLINE", "WS", "MUL", "DIV", "ADD", "SUB", "X", 
      "Y", "Z", "W", "ARROWUP", "INT"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
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
  	4,0,14,71,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,1,0,
  	1,0,1,1,1,1,1,2,3,2,35,8,2,1,2,1,2,1,2,1,2,1,3,4,3,42,8,3,11,3,12,3,43,
  	1,3,1,3,1,4,1,4,1,5,1,5,1,6,1,6,1,7,1,7,1,8,1,8,1,9,1,9,1,10,1,10,1,11,
  	1,11,1,12,1,12,1,12,1,13,4,13,68,8,13,11,13,12,13,69,0,0,14,1,1,3,2,5,
  	3,7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,1,0,2,3,0,
  	9,10,13,13,32,32,1,0,48,57,73,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,
  	1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,
  	0,0,19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,1,
  	29,1,0,0,0,3,31,1,0,0,0,5,34,1,0,0,0,7,41,1,0,0,0,9,47,1,0,0,0,11,49,
  	1,0,0,0,13,51,1,0,0,0,15,53,1,0,0,0,17,55,1,0,0,0,19,57,1,0,0,0,21,59,
  	1,0,0,0,23,61,1,0,0,0,25,63,1,0,0,0,27,67,1,0,0,0,29,30,5,40,0,0,30,2,
  	1,0,0,0,31,32,5,41,0,0,32,4,1,0,0,0,33,35,5,13,0,0,34,33,1,0,0,0,34,35,
  	1,0,0,0,35,36,1,0,0,0,36,37,5,10,0,0,37,38,1,0,0,0,38,39,6,2,0,0,39,6,
  	1,0,0,0,40,42,7,0,0,0,41,40,1,0,0,0,42,43,1,0,0,0,43,41,1,0,0,0,43,44,
  	1,0,0,0,44,45,1,0,0,0,45,46,6,3,0,0,46,8,1,0,0,0,47,48,5,42,0,0,48,10,
  	1,0,0,0,49,50,5,47,0,0,50,12,1,0,0,0,51,52,5,43,0,0,52,14,1,0,0,0,53,
  	54,5,45,0,0,54,16,1,0,0,0,55,56,5,120,0,0,56,18,1,0,0,0,57,58,5,121,0,
  	0,58,20,1,0,0,0,59,60,5,122,0,0,60,22,1,0,0,0,61,62,5,119,0,0,62,24,1,
  	0,0,0,63,64,5,94,0,0,64,65,3,27,13,0,65,26,1,0,0,0,66,68,7,1,0,0,67,66,
  	1,0,0,0,68,69,1,0,0,0,69,67,1,0,0,0,69,70,1,0,0,0,70,28,1,0,0,0,4,0,34,
  	43,69,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  polynomlexerLexerStaticData = std::move(staticData);
}

}

polynomLexer::polynomLexer(CharStream *input) : Lexer(input) {
  polynomLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *polynomlexerLexerStaticData->atn, polynomlexerLexerStaticData->decisionToDFA, polynomlexerLexerStaticData->sharedContextCache);
}

polynomLexer::~polynomLexer() {
  delete _interpreter;
}

std::string polynomLexer::getGrammarFileName() const {
  return "polynom.g4";
}

const std::vector<std::string>& polynomLexer::getRuleNames() const {
  return polynomlexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& polynomLexer::getChannelNames() const {
  return polynomlexerLexerStaticData->channelNames;
}

const std::vector<std::string>& polynomLexer::getModeNames() const {
  return polynomlexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& polynomLexer::getVocabulary() const {
  return polynomlexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView polynomLexer::getSerializedATN() const {
  return polynomlexerLexerStaticData->serializedATN;
}

const atn::ATN& polynomLexer::getATN() const {
  return *polynomlexerLexerStaticData->atn;
}




void polynomLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  polynomlexerLexerInitialize();
#else
  ::antlr4::internal::call_once(polynomlexerLexerOnceFlag, polynomlexerLexerInitialize);
#endif
}
