#include "tstack.h"
#include "tqueue.h"
#include <functional>
#include <stdexcept>
#include <format>

#define MAXTURING 1000

using namespace std;

typedef void(*TFunc)();
typedef void (*CFunc)(char);
typedef function<void(char)> SCFunc;

// Class Exception for calling exceptions from arithmetic expressions

class Exception {
  string message;
  size_t pos;
public:
  Exception(string _message = "", size_t _pos = 0) : message(_message), pos(_pos) {}
  string &GetMessage() { return message; }
  size_t &GetPos() { return pos; }
};

// Type of lexem

enum LexemeType {
  number,
  single_operation,
  binary_operation,
  skobe,
  word
};

// Structure TPair is used for making state machine 

template <typename T>
struct TPair {
  size_t NextState = 0;
  T Function = nullptr;
};

// Class that contains table of machine

template <typename T>
class TuringMachine {
  TPair<T> *table;
  TPair<T> **states;
  size_t numStates;
  size_t numVars;
  size_t cursor;
public:
  TuringMachine(size_t _numStates = 1, size_t _numVars = 1) {
    if (_numStates <= 0 || _numStates > MAXTURING || _numVars <= 0 || _numVars > MAXTURING)
      throw out_of_range("Table can't be with given size");
    numStates = _numStates;
    numVars = _numVars;
    table = new TPair<T>[numStates * numVars];
    states = new TPair<T> *[numStates];
    cursor = 0;
    for (size_t i = 0; i < numStates; i++)
      states[i] = &(table[numVars*i]);
  }

  ~TuringMachine() {
    delete[] states;
    delete[] table;
  }

  TuringMachine(const TuringMachine &turm) {
    numStates = turm.numStates;
    numVars = turm.numVars;
    cursor = 0;
    size_t tmp = numStates * numVars;
    table = new TPair<T>[tmp];
    states = new TPair<T> *[numStates];
    for (size_t i = 0; i < tmp; i++)
      table[i] = turm.table[i];
    for (size_t i = 0; i < numStates; i++)
      states[i] = &(table[numVars*i]);
  }

  TuringMachine &operator=(const TuringMachine &turm) {
    if (this == &turm)
      return *this;
    size_t tmp = numStates * numVars;
    if (numStates != turm.numStates || numVars != turm.numVars) {
      delete[] states;
      delete[] table;
      numStates = turm.numStates;
      numVars = turm.numVars;
      cursor = 0;
      tmp = numStates * numVars;
      table = new TPair<T>[tmp];
      states = new TPair<T> *[numStates];
      for (size_t i = 0; i < numStates; i++)
        states[i] = &(table[numVars * i]);
    }
    for (size_t i = 0; i < tmp; i++)
      table[i] = turm.table[i];
    return *this;
  }

  size_t &GetNumStates() { return numStates; }
  size_t &GetNumVars() { return numVars; }
  size_t &Cursor() { return cursor; }

  TPair<T> *&operator[](size_t num) { return states[num]; }

  void Load(TPair<T> *tpairs, size_t num = 0) {
    if (num <= 0 || num > numStates * numVars)
      throw out_of_range("Invalid Number");
    for (size_t i = 0; i < num; i++)
      table[i] = tpairs[i];
  }

};

// Structure for lexem description

template <typename T>
struct Lexeme {
  string Text;
  T Value;
  LexemeType Type;
  int Priority;
};

typedef function<void(Lexeme<int>)> STFunc;

// Parent Class for arith. Only for making dependencies between arith and handlers

class TMaker {
protected:
  int result;
  string inf_str;
  string postfix_str;
  TDynamicQueue<Lexeme<int>> lexems_stream_int;
  TDynamicQueue<Lexeme<int>> lexems_postfix_int;
  friend class IHandler;
public:
  TMaker(string _inf_str, size_t size = 0) : lexems_stream_int(size) {
    inf_str = _inf_str;
    postfix_str = "";
  }
  string InfStr() { return inf_str; }
  string PostFixStr() { return postfix_str; }
  TDynamicQueue<Lexeme<int>> LexemsStreamInt() { return lexems_stream_int; }
  TDynamicQueue<Lexeme<int>> LexemsPostfixInt() { return lexems_postfix_int; }
  int& GetResult() { return result; }
};

// Just class interface for handlers

class IHandler {
protected:
  TMaker *tarith;
  string &inf_str() { return tarith->inf_str; }
  string &postfix_str() { return tarith->postfix_str; }
  TDynamicQueue<Lexeme<int>> &lexems_stream_int() { return tarith->lexems_stream_int;}
  TDynamicQueue<Lexeme<int>> &lexems_postfix_int() { return tarith->lexems_postfix_int; }
  int& result() { return tarith->result; }

public:
  virtual void Do() = 0;
};

// Class arith that realises work of arithmetic expressions

class TArith : public TMaker {
  IHandler **handlers;
  size_t handlers_size;
public:
  TArith(string _inf_str) : TMaker(_inf_str, _inf_str.size()) {
    inf_str = _inf_str;
    handlers_size = 0;
  }
  void AddHandler(IHandler& ihandler) {
    IHandler **tmphandlers;
    tmphandlers = new IHandler *[handlers_size + 1];
    for (size_t i = 0; i < handlers_size; i++)
      tmphandlers[i] = handlers[i];
    tmphandlers[handlers_size] = &ihandler;
    handlers_size++;
    swap(handlers, tmphandlers);
  }
  void LaunchHandler(size_t i) { handlers[i]->Do(); }
  void LaunchAllHandlers() {
    for (size_t i = 0; i < handlers_size; i++)
      handlers[i]->Do();
  }
  TArith &operator=(string str) {
    inf_str = str;
    return *this;
  }
};

// Lexeme Translator that translate symbols to lexems. But this class don't define binary and singular operations

class ILexemeTranslator : public IHandler {
  TuringMachine<SCFunc> turm;
  TDynamicQueue<Lexeme<int>> *qe;
  bool isPushed = true;
  Lexeme<int> lxt;
  size_t Decode(char ch);
  void NumberOne(char c);
  void NumberTwo(char c);
  void OperationOne(char c);
  void SkobeOne(char c);
public:

  ILexemeTranslator(TArith &_tarith) : turm(2, 3) {
    tarith = &_tarith;
    qe = new TDynamicQueue<Lexeme<int>>(inf_str().size());
    TPair<SCFunc> tpairs[6] = {{1, [this](char c) { this->NumberOne(c); }},
                               {0, [this](char c) { this->OperationOne(c); }},
                               {0, [this](char c) { this->SkobeOne(c); }},
                               {1, [this](char c) { this->NumberTwo(c); }},
                               {0, [this](char c) { this->OperationOne(c); }},
                               {0, [this](char c) { this->SkobeOne(c); }}};
    turm.Load(tpairs, 6);
  }
  ~ILexemeTranslator() {
    if (qe != nullptr)
      delete qe;
  }
  void Do();

};

// Class that checks correctness of arithmetic expressions

class ICorrectChecker : public IHandler {
  size_t pos;
  TDynamicStack<size_t>* st;
  TDynamicQueue<Exception>* exqe;
  TuringMachine<STFunc> turm;
  TDynamicQueue<Lexeme<int>> *qe;
  Lexeme<int> tmplx;
  size_t Decode(Lexeme<int> lx);
  void NumberOne(Lexeme<int> lx);
  void NumberTwo(Lexeme<int> lx);
  void OperationOne(Lexeme<int> lx);
  void OperationTwo(Lexeme<int> lx);
  void SkobeOpenOne(Lexeme<int> lx);
  void SkobeCloseOne(Lexeme<int> lx);
  void printExceptions();
  bool divide_by = false;
public:
  ICorrectChecker(TArith &_tarith) : turm(5, 4) {
    tarith = &_tarith;
    qe = new TDynamicQueue<Lexeme<int>>(inf_str().size());
    st = new TDynamicStack<size_t>(inf_str().size());
    exqe = new TDynamicQueue<Exception>(inf_str().size());
    //mod 4: 0 - number, 1 - operation, 2 - skobe "(", 3 - skobe ")"
    //states:
    //state 0 - start
    //state 1 - got number
    //state 2 - got operation
    //state 3 - got skobe "("
    //state 4 - got skobe ")"
    TPair<STFunc> tpairs[20] = {
      {1, [this](Lexeme<int> lx) { this->NumberOne(lx); }},
      {2, [this](Lexeme<int> lx) { this->OperationOne(lx); }},
      {3, [this](Lexeme<int> lx) { this->SkobeOpenOne(lx); }},
      {0, [this](Lexeme<int> lx) { throw Exception(format("Wrong skobe {} on pos: {}", lx.Text, pos), pos); }},
      {1, [this](Lexeme<int> lx) { throw Exception(format("Duplicate numbers {} on pos: {}", lx.Text, pos), pos); }},
      {2, [this](Lexeme<int> lx) { this->OperationTwo(lx); }},
      {3, [this](Lexeme<int> lx) { this->st->Push(pos); throw Exception(format("Skobe {} can't be after number on pos: {}", lx.Text, pos), pos); }},
      {4, [this](Lexeme<int> lx) { this->SkobeCloseOne(lx); }},
      {1, [this](Lexeme<int> lx) { this->NumberTwo(lx); }},
      {2, [this](Lexeme<int> lx) { throw Exception(format("Wrong operation {} on pos: {}", lx.Text, pos), pos); }},
      {2, [this](Lexeme<int> lx) { this->SkobeOpenOne(lx); }},
      {4, [this](Lexeme<int> lx) { if (!this->st->isEmpty()) this->st->Pop(); throw Exception(format("Wrong skobe {} after operation on pos: {}", lx.Text, pos), pos); }},
      {1, [this](Lexeme<int> lx) { this->NumberOne(lx); }},
      {2, [this](Lexeme<int> lx) { this->OperationOne(lx); }},
      {3, [this](Lexeme<int> lx) { this->SkobeOpenOne(lx); }},
      {3, [this](Lexeme<int> lx) { if (!this->st->isEmpty()) this->st->Pop(); throw Exception(format("Skobe {} close empty space on pos: {}", lx.Text, pos), pos); }},
      {4, [this](Lexeme<int> lx) { throw Exception(format("Number {} can't be after close skobe on pos: {}", lx.Text, pos), pos); }},
      {2, [this](Lexeme<int> lx) { this->OperationTwo(lx); }},
      {3, [this](Lexeme<int> lx) { this->st->Push(pos); throw Exception(format("Skobe {} can't be after close skobe on pos: {}", lx.Text, pos), pos);}},
      {4, [this](Lexeme<int> lx) { this->SkobeCloseOne(lx); }}
    };
    turm.Load(tpairs, 20);
  }
  ~ICorrectChecker() {
    if (qe != nullptr) {
      delete qe;
      delete st;
      delete exqe;
    }
  }
  void Do();
};

// Class that makes postfix view of arithmetic expressions

class IPostfixMaker : public IHandler {
public:
  IPostfixMaker(TArith &_tarith) {
    tarith = &_tarith;
  }
  void Do();
};

// Class that counts arithmetic expression

class ICounter : public IHandler {
public:
  ICounter(TArith &_tarith) {
    tarith = &_tarith;
  }
  void Do();
};
