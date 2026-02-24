#pragma once
#include "tstack.h"
#include "tqueue.h"
#include <functional>
#include <stdexcept>
#include <format>
#include <iostream>

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

template <typename T>
class IHandler;

// Parent Class for arith. Only for making dependencies between arith and handlers

template <typename T>
class TMaker {
protected:
  T result;
  string inf_str;
  string postfix_str;
  TDynamicQueue<Lexeme<T>> lexems_stream_int;
  TDynamicQueue<Lexeme<T>> lexems_postfix_int;
  friend class IHandler<T>;
public:
  TMaker(string _inf_str, size_t size = 0) : lexems_stream_int(size) {
    inf_str = _inf_str;
    postfix_str = "";
  }
  string InfStr() { return inf_str; }
  string PostFixStr() { return postfix_str; }
  TDynamicQueue<Lexeme<T>> LexemsStreamInt() { return lexems_stream_int; }
  TDynamicQueue<Lexeme<T>> LexemsPostfixInt() { return lexems_postfix_int; }
  T& GetResult() { return result; }
};

// Just class interface for handlers

template <typename T>
class IHandler {
protected:
  TMaker<T> *tarith;
  string &inf_str() { return tarith->inf_str; }
  string &postfix_str() { return tarith->postfix_str; }
  TDynamicQueue<Lexeme<T>> &lexems_stream_int() { return tarith->lexems_stream_int;}
  TDynamicQueue<Lexeme<T>> &lexems_postfix_int() { return tarith->lexems_postfix_int; }
  T& result() { return tarith->result; }

public:
  virtual void Do() = 0;
};

// Class arith that realises work of arithmetic expressions

template <typename T>
class TArith : public TMaker<T> {
  IHandler<T> **handlers;
  size_t handlers_size;
public:
  TArith(string _inf_str) : TMaker<T>(_inf_str, _inf_str.size()) {
    this->inf_str = _inf_str;
    handlers_size = 0;
  }
  void AddHandler(IHandler<T>& ihandler) {
    IHandler<T> **tmphandlers;
    tmphandlers = new IHandler<T> *[handlers_size + 1];
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
    this->inf_str = str;
    return *this;
  }
};

// Lexeme Translator that translate symbols to lexems. But this class don't define binary and singular operations

template <typename T>
class ILexemeTranslator : public IHandler<T> {
  TuringMachine<SCFunc> turm;
  TDynamicQueue<Lexeme<T>> *qe;
  bool isPushed = true;
  Lexeme<T> lxt;
  size_t Decode(char ch) {
    if (ch >= '0' && ch <= '9')
      return 0;
    else if (ch >= '*' && ch <= '/')
      return 1;
    else if (ch == '(' || ch == ')')
      return 2;
    else
      return 3;
  }
  void NumberOne(char c) {
    lxt.Text = c;
    lxt.Type = number;
    lxt.Value = c - 48;
    isPushed = false;
  }
  void NumberTwo(char c) {
    lxt.Value = lxt.Value * 10 + c - 48;
    lxt.Text += c;
    isPushed = false;
  }
  void OperationOne(char c) {
    Lexeme<int> lx;
    if (!isPushed)
      qe->Push(lxt);
    isPushed = true;
    lx.Text = c;
    lx.Value = c;
    lx.Type = binary_operation;
    if (c == '*' || c == '/')
      lx.Priority = 1;
    else
      lx.Priority = 2;
    qe->Push(lx);
  }
  void SkobeOne(char c) {
    Lexeme<int> lx;
    if (!isPushed)
      qe->Push(lxt);
    isPushed = true;
    lx.Text = c;
    lx.Value = c;
    lx.Type = skobe;
    lx.Priority = 0;
    qe->Push(lx);
  }
public:

  ILexemeTranslator(TArith<T> &_tarith) : turm(2, 3) {
    this->tarith = &_tarith;
    qe = new TDynamicQueue<Lexeme<T>>(this->inf_str().size());
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
  void Do() {
    if (qe == nullptr)
      qe = new TDynamicQueue<Lexeme<T>>(this->inf_str().size());
    for (size_t i = 0; i < this->inf_str().size(); i++) {
      size_t tmp = Decode(this->inf_str()[i]);
      if (tmp != 3) {
        turm[turm.Cursor()][tmp].Function(this->inf_str()[i]);
        turm.Cursor() = turm[turm.Cursor()][tmp].NextState;
      }
    }
    if (!isPushed)
      qe->Push(lxt);
    isPushed = true;
    lxt.Text = "";
    lxt.Value = 0;
    turm.Cursor() = 0;
    this->lexems_stream_int() = *qe;
    delete qe;
    qe = nullptr;
  }

};

// Class that checks correctness of arithmetic expressions

template <typename T>
class ICorrectChecker : public IHandler<T> {
  size_t pos;
  TDynamicStack<size_t>* st;
  TDynamicQueue<Exception>* exqe;
  TuringMachine<STFunc> turm;
  TDynamicQueue<Lexeme<T>> *qe;
  Lexeme<T> tmplx;
  size_t Decode(Lexeme<T> lx) {
    if (lx.Type == number)
      return 0;
    else if (lx.Type == binary_operation)
      return 1;
    else if (lx.Text == "(")
      return 2;
    else if (lx.Text == ")")
      return 3;
    else
      return 4;
  }
  void NumberOne(Lexeme<T> lx) {
    if (lx.Value != 0 && lx.Text[0] == '0')
      throw Exception(format("Number {} can't start with zero on pos: {}", lx.Text, pos), pos);
    if (lx.Value == 0 && lx.Text != "0")
      throw Exception(format("Duplicating zeroes on pos: {}", pos), pos);
    qe->Push(lx);
  }
  void NumberTwo(Lexeme<T> lx) {
    if (lx.Value != 0 && lx.Text[0] == '0')
      throw Exception(format("Number {} can't start with zero on pos: {}", lx.Text, pos), pos);
    if (lx.Value == 0 && lx.Text != "0")
      throw Exception(format("Duplicating zeroes on pos: {}", pos), pos);
    if (divide_by && lx.Value == 0)
      throw Exception(format("Dividing by zero on pos: {}", pos), pos);
    divide_by = false;
    qe->Push(lx);
  }
  void OperationOne(Lexeme<T> lx) {
    if (lx.Text != "+" && lx.Text != "-")
      throw Exception(format("Wrong operation {} on pos: {}", lx.Text, pos),pos);
    lx.Type = single_operation;
    qe->Push(lx);
  }
  void OperationTwo(Lexeme<T> lx) {
    qe->Push(lx);
    if (lx.Text == "/")
      divide_by = true;
  }
  void SkobeOpenOne(Lexeme<T> lx) {
    qe->Push(lx);
    st->Push(pos);
  }
  void SkobeCloseOne(Lexeme<T> lx) {
    qe->Push(lx);
    if (st->isEmpty())
      throw Exception(format("Wrong skobe {} on pos: {}", lx.Text, pos), pos);
    st->Pop();
  }
  void printExceptions() {
    size_t len = exqe->Size() + st->Size();
    TDynamicQueue<size_t> qst(st->Size());
    while (!st->isEmpty()) {
      qst.Push(st->Top());
      st->Pop();
    }
    for (size_t i = 0; i < len; i++) {
      if (!qst.isEmpty() && qst.Top() < exqe->Top().GetPos()) {
        cout << "Wrong skobe ( on pos: " << qst.Top() << endl;
        qst.Pop();
      } else {
        cout << exqe->Top().GetMessage() << endl;
        exqe->Pop();
      }
    }
    pos = 0;
    delete qe;
    delete st;
    delete exqe;
    qe = nullptr;
    turm.Cursor() = 0;
    if (len != 0)
      throw out_of_range("This expression has problems");

  }
  bool divide_by = false;
public:
  ICorrectChecker(TArith<T> &_tarith) : turm(5, 4) {
    this->tarith = &_tarith;
    qe = new TDynamicQueue<Lexeme<T>>(this->inf_str().size());
    st = new TDynamicStack<size_t>(this->inf_str().size());
    exqe = new TDynamicQueue<Exception>(this->inf_str().size());
    //mod 4: 0 - number, 1 - operation, 2 - skobe "(", 3 - skobe ")"
    //states:
    //state 0 - start
    //state 1 - got number
    //state 2 - got operation
    //state 3 - got skobe "("
    //state 4 - got skobe ")"
    TPair<STFunc> tpairs[20] = {
      {1, [this](Lexeme<T> lx) { this->NumberOne(lx); }},
      {2, [this](Lexeme<T> lx) { this->OperationOne(lx); }},
      {3, [this](Lexeme<T> lx) { this->SkobeOpenOne(lx); }},
      {0, [this](Lexeme<T> lx) { throw Exception(format("Wrong skobe {} on pos: {}", lx.Text, pos), pos); }},
      {1, [this](Lexeme<T> lx) { throw Exception(format("Duplicate numbers {} on pos: {}", lx.Text, pos), pos); }},
      {2, [this](Lexeme<T> lx) { this->OperationTwo(lx); }},
      {3, [this](Lexeme<T> lx) { this->st->Push(pos); throw Exception(format("Skobe {} can't be after number on pos: {}", lx.Text, pos), pos); }},
      {4, [this](Lexeme<T> lx) { this->SkobeCloseOne(lx); }},
      {1, [this](Lexeme<T> lx) { this->NumberTwo(lx); }},
      {2, [this](Lexeme<T> lx) { throw Exception(format("Wrong operation {} on pos: {}", lx.Text, pos), pos); }},
      {2, [this](Lexeme<T> lx) { this->SkobeOpenOne(lx); }},
      {4, [this](Lexeme<T> lx) { if (!this->st->isEmpty()) this->st->Pop(); throw Exception(format("Wrong skobe {} after operation on pos: {}", lx.Text, pos), pos); }},
      {1, [this](Lexeme<T> lx) { this->NumberOne(lx); }},
      {2, [this](Lexeme<T> lx) { this->OperationOne(lx); }},
      {3, [this](Lexeme<T> lx) { this->SkobeOpenOne(lx); }},
      {3, [this](Lexeme<T> lx) { if (!this->st->isEmpty()) this->st->Pop(); throw Exception(format("Skobe {} close empty space on pos: {}", lx.Text, pos), pos); }},
      {4, [this](Lexeme<T> lx) { throw Exception(format("Number {} can't be after close skobe on pos: {}", lx.Text, pos), pos); }},
      {2, [this](Lexeme<T> lx) { this->OperationTwo(lx); }},
      {3, [this](Lexeme<int> lx) { this->st->Push(pos); throw Exception(format("Skobe {} can't be after close skobe on pos: {}", lx.Text, pos), pos);}},
      {4, [this](Lexeme<T> lx) { this->SkobeCloseOne(lx); }}
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
  void Do() {
    if (qe == nullptr) {
      qe = new TDynamicQueue<Lexeme<int>>(this->lexems_stream_int().Size());
      st = new TDynamicStack<size_t>(this->lexems_stream_int().Size());
      exqe = new TDynamicQueue<Exception>(this->lexems_stream_int().Size());
    }
    TDynamicQueue<Lexeme<int>> qe2 = this->tarith->LexemsStreamInt();
    size_t len = qe2.Size();
    pos = 0;
    while (!qe2.isEmpty()) {
      tmplx = qe2.Top();
      size_t tmp = Decode(tmplx);
      try {
        if (tmp != 4) {
          turm[turm.Cursor()][tmp].Function(tmplx);
          turm.Cursor() = turm[turm.Cursor()][tmp].NextState;
        }
      } catch (Exception ex) {
        exqe->Push(ex);
        turm.Cursor() = turm[turm.Cursor()][tmp].NextState;
      }
      pos += tmplx.Text.size();
      while (this->inf_str()[pos] == ' ')
        pos++;
      qe2.Pop();
    }
    if (tmplx.Type == binary_operation || tmplx.Type == single_operation) {
      Exception exfin(format("Expression can't be ended by operation {} on pos: {}", tmplx.Text, pos - 1), pos);
      exqe->Push(exfin);
    }
    this->lexems_stream_int() = *qe;
    printExceptions();
  }
};

// Class that makes postfix view of arithmetic expressions

template <typename T>
class IPostfixMaker : public IHandler<T> {
public:
  IPostfixMaker(TArith<T> &_tarith) {
    this->tarith = &_tarith;
  }
  void Do() {
    TDynamicQueue<Lexeme<int>> qe = this->tarith->LexemsStreamInt();
    TDynamicQueue<Lexeme<int>> qe2(qe.Size());
    TDynamicStack<Lexeme<int>> st(qe.Size());
    size_t len = qe.Size();
    size_t curr = 0;
    string posttmp = "";
    Lexeme<int> tmplx;
    while (!qe.isEmpty()) {
      tmplx = qe.Top();
      switch (tmplx.Type) {
      case number:
        qe2.Push(tmplx);
        posttmp += tmplx.Text;
        if (!(curr == len - 1) || !st.isEmpty())
          posttmp += " ";
        break;
      case binary_operation:case single_operation:
        if (st.isEmpty())
          st.Push(tmplx);
        else {
          if (st.Top().Priority > tmplx.Priority || st.Top().Type == skobe)
            st.Push(tmplx);
          else {
            while (st.Top().Priority <= tmplx.Priority && st.Top().Type != skobe) {
              qe2.Push(st.Top());
              posttmp += st.Top().Text;
              st.Pop();
              if (!(curr == len - 1) || !st.isEmpty())
                posttmp += " ";
              if (st.isEmpty())
                break;
            }
            st.Push(tmplx);
          }
        }
        break;
      case skobe:
        if (tmplx.Text == "(")
          st.Push(tmplx);
        else {
          while (st.Top().Type != skobe) {
            qe2.Push(st.Top());
            posttmp += st.Top().Text;
            st.Pop();
            if (!(curr == len - 1) || !st.isEmpty())
              posttmp += " ";
          }
          st.Pop();
        }
        break;
      default:
        break;
      }
      curr++;
      qe.Pop();
    }
    len = st.Size();
    curr = 0;
    while (!st.isEmpty()) {
      qe2.Push(st.Top());
      posttmp += st.Top().Text;
      st.Pop();
      if (!(curr == len - 1))
        posttmp += " ";
    }
    this->lexems_postfix_int() = qe2;
    this->postfix_str() = posttmp;
  }
};

// Class that counts arithmetic expression

template <typename T>
class ICounter : public IHandler<T> {
public:
  ICounter(TArith<T> &_tarith) {
    this->tarith = &_tarith;
  }
  void Do() {
    TDynamicQueue<Lexeme<int>> qe = this->tarith->LexemsPostfixInt();
    TDynamicStack<int> st(qe.Size());
    this->result() = 0;
    int num = 0;
    while (!qe.isEmpty()) {
      switch (qe.Top().Type) {
      case number:
        st.Push(qe.Top().Value);
        break;
      case binary_operation:
        num = st.Top();
        st.Pop();
        switch (qe.Top().Value) {
        case '+':
          num = st.Top() + num;
          break;
        case '-':
          num = st.Top() - num;
          break;
        case '*':
          num = st.Top() * num;
          break;
        case '/':
          num = st.Top() / num;
          break;
        default:
          break;
        }
        st.Pop();
        st.Push(num);
        break;
      case single_operation:
        num = st.Top();
        st.Pop();
        switch (qe.Top().Value) {
        case '-':
          num = -num;
          break;
        default:
          break;
        }
        st.Push(num);
        break;
      default:
        break;
      }
      qe.Pop();
    }
    this->result() = st.Top();
  }
};
