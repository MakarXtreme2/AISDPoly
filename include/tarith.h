#pragma once
#include "tlist.h"
#include "tstack.h"
#include "tqueue.h"
#include "tpoly.h"
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
  polynom,
  word
};

template <typename T>
class EPoly;

template <typename T>
class BiOp;

template <typename T>
class SiOp;

template <typename T>
class Visitor {
public:
  virtual T visitEPoly(EPoly<T>* poly) = 0;
  virtual T visitBiOp(BiOp<T>* op) = 0;
  virtual T visitSiOp(SiOp<T>* op) = 0;
};

template <typename T>
class Expr {
public:
  virtual T accept(Visitor<T>* v) = 0;
  virtual ~Expr() = default;
};

template <typename T>
class EPoly : public Expr<T> {
  T val;
public:
  EPoly(T _val) : val(_val) {}
  T getVal() { return val; }
  T accept(Visitor<T>* v) {
    return v->visitEPoly(this);
  }
};

template <typename T>
class BiOp : public Expr<T> {
  char op;
  Expr<T>* left;
  Expr<T>* right;
public:
  BiOp(char _op, Expr<T>* _left, Expr<T>* _right) :
    op(_op), left(_left), right(_right) {}
  T accept(Visitor<T>* v) {
    return v->visitBiOp(this);
  }
  Expr<T>* Left() { return left; }
  Expr<T>* Right() { return right; }
  char Op() { return op; }
};

template <typename T>
class SiOp : public Expr<T> {
  char op;
  Expr<T>* part;
public:
  SiOp(char _op, Expr<T>* _part) :
    op(_op), part(_part) {}
  T accept(Visitor<T>* v) {
    return v->visitSiOp(this);
  }
  Expr<T>* Part() { return part; }
  char Op() { return op; }
};

template <typename T>
class CalcVisitor : public Visitor<T> {
public:
  T visitEPoly(EPoly<T>* poly) {
    return poly->getVal();
  }
  T visitBiOp(BiOp<T>* op) {
    T left = op->Left()->accept(this);
    T right = op->Right()->accept(this);
    switch (op->Op()) {
    case '+':
      return left + right;
      break;
    case '-':
      return left - right;
      break;
    case '*':
      return left * right;
      break;
    case '/':
      return left / right;
      break;
    default:
      throw out_of_range("Unknown op");
      break;
    }
  }
  T visitSiOp(SiOp<T>* op) {
    T part = op->Part()->accept(this);
    if (op->Op() == '-')
      return -part;
    return part;
  }
};

template <typename T>
class RemoveVisitor : public Visitor<T> {
public:
  T visitEPoly(EPoly<T>* poly) {
    delete poly;
    return 0;
  }
  T visitBiOp(BiOp<T>* op) {
    op->Left()->accept(this);
    op->Right()->accept(this);
    delete op;
    return 0;
  }
  T visitSiOp(SiOp<T>* op) {
    op->Part()->accept(this);
    delete op;
  }
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

template <typename T>
using STFunc = function<void(Lexeme<T>)>;

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
  TDynamicStack<Lexeme<T>> lexems_postfix_stack;
  Expr<T>* root;
  friend class IHandler<T>;
public:
  TMaker(string _inf_str, size_t size = 0) : lexems_stream_int(size), root(nullptr) {
    inf_str = _inf_str;
    postfix_str = "";
  }
  string InfStr() { return inf_str; }
  string PostFixStr() { return postfix_str; }
  TDynamicQueue<Lexeme<T>> LexemsStreamInt() { return lexems_stream_int; }
  TDynamicQueue<Lexeme<T>> LexemsPostfixInt() { return lexems_postfix_int; }
  TDynamicStack<Lexeme<T>> LexemsPostfixStack() { return lexems_postfix_stack; }
  Expr<T>* Root() { return root; }
  T& GetResult() { return result; }
  virtual ~TMaker() {
    RemoveVisitor<T> rvis;
    if (root != nullptr)
      root->accept(&rvis);
  }
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
  TDynamicStack<Lexeme<T>> &lexems_postfix_stack() { return tarith->lexems_postfix_stack; }
  Expr<T>*& root() { return tarith->root; }
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
    Lexeme<T> lx;
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
    Lexeme<T> lx;
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
  TuringMachine<STFunc<T>> turm;
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
    TPair<STFunc<T>> tpairs[20] = {
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
      {3, [this](Lexeme<T> lx) { this->st->Push(pos); throw Exception(format("Skobe {} can't be after close skobe on pos: {}", lx.Text, pos), pos);}},
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
      qe = new TDynamicQueue<Lexeme<T>>(this->lexems_stream_int().Size());
      st = new TDynamicStack<size_t>(this->lexems_stream_int().Size());
      exqe = new TDynamicQueue<Exception>(this->lexems_stream_int().Size());
    }
    TDynamicQueue<Lexeme<T>> qe2 = this->tarith->LexemsStreamInt();
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
    TDynamicQueue<Lexeme<T>> qe = this->tarith->LexemsStreamInt();
    TDynamicQueue<Lexeme<T>> qe2(qe.Size());
    TDynamicStack<Lexeme<T>> st(qe.Size());
    size_t len = qe.Size();
    size_t curr = 0;
    string posttmp = "";
    Lexeme<T> tmplx;
    while (!qe.isEmpty()) {
      tmplx = qe.Top();
      switch (tmplx.Type) {
      case number: case polynom:
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
    TDynamicQueue<Lexeme<T>> qe = this->tarith->LexemsPostfixInt();
    TDynamicStack<T> st(qe.Size());
    this->result() = 0;
    T num = 0;
    while (!qe.isEmpty()) {
      switch (qe.Top().Type) {
      case number: case polynom:
        st.Push(qe.Top().Value);
        break;
      case binary_operation:
        num = st.Top();
        st.Pop();
        switch (qe.Top().Text[0]) {
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
        switch (qe.Top().Text[0]) {
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

// LexemeTranslator for Polynom

template <typename T, template <typename> class List = TStdList>
class ILexemeTranslatorP : public IHandler<Polynom<T, List>> {
  TuringMachine<SCFunc> turm;
  TDynamicQueue<Lexeme<Polynom<T, List>>> *qe;
  bool isPushed = true;
  bool isPolyYet = false;
  Lexeme<Polynom<T, List>> lxt;
  size_t Decode(char ch) {
    if (ch >= '0' && ch <= '9' ||
        ch >= 'a' && ch <= 'z' ||
        ch >= 'A' && ch <= 'Z') {
      isPolyYet = false;
      return 0;
    } else if (ch == '^') {
      isPolyYet = true;
      return 0;
    }
    else if (ch >= '*' && ch <= '/')
      return 1;
    else if (ch == '(' || ch == ')')
      return 2;
    else
      return 3;
  }
  void PolynomOne(char c) {
    lxt.Text = c;
    lxt.Type = polynom;
    isPushed = false;
  }
  void PolynomTwo(char c) {
    lxt.Text += c;
    isPushed = false;
  }
  void OperationOne(char c) {
    Lexeme<Polynom<T, List>> lx;
    if (!isPushed)
      qe->Push(lxt);
    isPushed = true;
    lx.Text = c;
    lx.Type = binary_operation;
    if (c == '*' || c == '/')
      lx.Priority = 1;
    else
      lx.Priority = 2;
    qe->Push(lx);
  }
  void SkobeOne(char c) {
    Lexeme<Polynom<T, List>> lx;
    if (!isPushed)
      qe->Push(lxt);
    isPushed = true;
    lx.Text = c;
    lx.Type = skobe;
    lx.Priority = 0;
    qe->Push(lx);
  }
public:

  ILexemeTranslatorP(TArith<Polynom<T, List>> &_tarith) : turm(2, 3) {
    this->tarith = &_tarith;
    qe = new TDynamicQueue<Lexeme<Polynom<T, List>>>(this->inf_str().size());
    TPair<SCFunc> tpairs[6] = {{1, [this](char c) { this->PolynomOne(c); }},
                               {0, [this](char c) { this->OperationOne(c); }},
                               {0, [this](char c) { this->SkobeOne(c); }},
                               {1, [this](char c) { this->PolynomTwo(c); }},
                               {0, [this](char c) { this->OperationOne(c); }},
                               {0, [this](char c) { this->SkobeOne(c); }}};
    turm.Load(tpairs, 6);
  }
  ~ILexemeTranslatorP() {
    if (qe != nullptr)
      delete qe;
  }
  void Do() {
    if (qe == nullptr)
      qe = new TDynamicQueue<Lexeme<Polynom<T, List>>>(this->inf_str().size());
    for (size_t i = 0; i < this->inf_str().size(); i++) {
      size_t tmp = Decode(this->inf_str()[i]);
      if (isPolyYet && this->inf_str()[i] == '-') {
        isPolyYet = false;
        tmp = 0;
      }
      if (tmp != 3) {
        turm[turm.Cursor()][tmp].Function(this->inf_str()[i]);
        turm.Cursor() = turm[turm.Cursor()][tmp].NextState;
      }
    }
    if (!isPushed)
      qe->Push(lxt);
    isPushed = true;
    lxt.Text = "";
    turm.Cursor() = 0;
    this->lexems_stream_int() = *qe;
    delete qe;
    qe = nullptr;
  }

};

// ICorrectChecker for polynom

template <typename T, template <typename> class List = TStdList>
class ICorrectCheckerP : public IHandler<Polynom<T, List>> {
  size_t pos;
  TDynamicStack<size_t>* st;
  TDynamicQueue<Exception>* exqe;
  TuringMachine<STFunc<Polynom<T, List>>> turm;
  TDynamicQueue<Lexeme<Polynom<T, List>>> *qe;
  Lexeme<Polynom<T, List>> tmplx;
  bool divide_by = false;
  size_t Decode(Lexeme<Polynom<T, List>> lx) {
    if (lx.Type == polynom)
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
  void CheckPolynom(Lexeme<Polynom<T, List>>& lx) {
    size_t ind = 0, ind2 = 0;
    bool isNumber = false;
    bool isLetter = false;
    bool isDeg = false;
    for (size_t i = 0; i < lx.Text.size(); i++) {
      if (lx.Text[i] == '^') {
        isDeg = true;
        if (isNumber)
          throw Exception(format("Polynom {} can't have ^ after number on pos: {}", lx.Text, pos + i), pos + i);
        ind2++;
      }
      else {
        if (lx.Text[i] >= 'a' && lx.Text[i] <= 'z' || lx.Text[i] >= 'A' && lx.Text[i] <= 'Z') {
          if (isDeg)
            throw Exception(format("Polynom {} can't have after ^ no number on pos: {}", lx.Text, pos + i), pos + i);
        }
        isDeg = false;
        ind2 = 0;
      }
      if (ind2 > 1)
        throw Exception(format("Polynom {} can't have more than one ^ on pos: {}", lx.Text, pos + i - ind2 + 1), pos + i - ind2 + 1);
      isNumber = true;
      if (lx.Text[i] >= '0' && lx.Text[i] <= '9') {
        if (isLetter)
          throw Exception(format("You should use ^ in polynom {} on pos: {}", lx.Text, pos + i), pos + i);
      }
      if (!(lx.Text[i] >= '0' && lx.Text[i] <= '9')) {
        if ((i - ind) > 1 && lx.Text[ind] == '0')
          throw Exception(format("Number {} can't start with zero on pos: {}", lx.Text.substr(ind, i - ind), pos + ind), pos + ind);
        ind = i + 1;
        isNumber = false;
      }
      if (lx.Text[i] >= 'a' && lx.Text[i] <= 'z' || lx.Text[i] >= 'A' && lx.Text[i] <= 'Z') {
        isLetter = true;
        if (!monSym.contains(lx.Text[i]))
          throw Exception(format("Symbol {} not in {} of polynom {} on pos: {}", lx.Text[i], monSym, lx.Text, pos + i), pos + i);
      } else
        isLetter = false;
    }
    if ((lx.Text.size() - ind) > 1 && lx.Text[ind] == '0')
      throw Exception(format("Number {} can't start with zero on pos: {}", lx.Text.substr(ind, lx.Text.size() - ind), pos + ind), pos + ind);
    if (lx.Text[0] == '^')
      throw Exception(format("Polynom {} can't start with ^ on pos: {}", lx.Text, pos), pos);
    if (lx.Text[lx.Text.size() - 1] == '^')
      throw Exception(format("Polynom {} can't end with ^ on pos: {}", lx.Text, pos + lx.Text.size() - 1), pos + lx.Text.size() - 1);
  }
  void CreatePolynom(Lexeme<Polynom<T, List>>& lx) {
    size_t ind = 0;
    T coeff = 0;
    size_t now = 0;
    bool isNeg = false;
    bool isLetter = false;
    char deg[4]{};
    if (!(lx.Text[0] >= '0' && lx.Text[0] <= '9'))
      coeff = 1;
    while (lx.Text[ind] >= '0' && lx.Text[ind] <= '9') {
      coeff = coeff * 10 + lx.Text[ind] - '0';
      ind++;
    }
    char tmp = 0;
    for (; ind < lx.Text.size(); ind++) {
      if (monSym.contains(lx.Text[ind])) {
        if (isLetter)
          deg[now] += 1;
        else {
          if (isNeg)
            tmp = -tmp;
          deg[now] += tmp;
        }
        tmp = 0;
        now = monSym.find(lx.Text[ind]);
        isNeg = false;
        isLetter = true;
      }
      else if (lx.Text[ind] == '-') {
        isNeg = true;
        isLetter = false;
      }
      else if (lx.Text[ind] >= '0' && lx.Text[ind] <= '9') {
        tmp = tmp * 10 + lx.Text[ind] - '0';
        isLetter = false;
      }
    }
    if (isLetter)
      deg[now] += 1;
    else {
      if (isNeg)
        tmp = -tmp;
      deg[now] += tmp;
    }
    Polynom<T, List> p(coeff, deg[0], deg[1], deg[2], deg[3]);
    lx.Value = p;
  }
  void PolynomOne(Lexeme<Polynom<T, List>> lx) {
    CheckPolynom(lx);
    CreatePolynom(lx);
    qe->Push(lx);
  }
  void PolynomTwo(Lexeme<Polynom<T, List>> lx) {
    CheckPolynom(lx);
    CreatePolynom(lx);
    if (divide_by && lx.Value == 0)
      throw Exception(format("Dividing by zero on pos: {}", pos), pos);
    divide_by = false;
    qe->Push(lx);
  }
  void OperationOne(Lexeme<Polynom<T, List>> lx) {
    if (lx.Text != "+" && lx.Text != "-")
      throw Exception(format("Wrong operation {} on pos: {}", lx.Text, pos),pos);
    lx.Type = single_operation;
    qe->Push(lx);
  }
  void OperationTwo(Lexeme<Polynom<T, List>> lx) {
    qe->Push(lx);
    if (lx.Text == "/")
      divide_by = true;
  }
  void SkobeOpenOne(Lexeme<Polynom<T, List>> lx) {
    qe->Push(lx);
    st->Push(pos);
  }
  void SkobeCloseOne(Lexeme<Polynom<T, List>> lx) {
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
public:
  ICorrectCheckerP(TArith<Polynom<T, List>> &_tarith) : turm(5, 4) {
    this->tarith = &_tarith;
    qe = new TDynamicQueue<Lexeme<Polynom<T, List>>>(this->inf_str().size());
    st = new TDynamicStack<size_t>(this->inf_str().size());
    exqe = new TDynamicQueue<Exception>(this->inf_str().size());
    //mod 4: 0 - number, 1 - operation, 2 - skobe "(", 3 - skobe ")"
    //states:
    //state 0 - start
    //state 1 - got number
    //state 2 - got operation
    //state 3 - got skobe "("
    //state 4 - got skobe ")"
    TPair<STFunc<Polynom<T, List>>> tpairs[20] = {
      {1, [this](Lexeme<Polynom<T, List>> lx) { this->PolynomOne(lx); }},
      {2, [this](Lexeme<Polynom<T, List>> lx) { this->OperationOne(lx); }},
      {3, [this](Lexeme<Polynom<T, List>> lx) { this->SkobeOpenOne(lx); }},
      {0, [this](Lexeme<Polynom<T, List>> lx) { throw Exception(format("Wrong skobe {} on pos: {}", lx.Text, pos), pos); }},
      {1, [this](Lexeme<Polynom<T, List>> lx) { throw Exception(format("Duplicate numbers {} on pos: {}", lx.Text, pos), pos); }},
      {2, [this](Lexeme<Polynom<T, List>> lx) { this->OperationTwo(lx); }},
      {3, [this](Lexeme<Polynom<T, List>> lx) { this->st->Push(pos); throw Exception(format("Skobe {} can't be after number on pos: {}", lx.Text, pos), pos); }},
      {4, [this](Lexeme<Polynom<T, List>> lx) { this->SkobeCloseOne(lx); }},
      {1, [this](Lexeme<Polynom<T, List>> lx) { this->PolynomTwo(lx); }},
      {2, [this](Lexeme<Polynom<T, List>> lx) { throw Exception(format("Wrong operation {} on pos: {}", lx.Text, pos), pos); }},
      {2, [this](Lexeme<Polynom<T, List>> lx) { this->SkobeOpenOne(lx); }},
      {4, [this](Lexeme<Polynom<T, List>> lx) { if (!this->st->isEmpty()) this->st->Pop(); throw Exception(format("Wrong skobe {} after operation on pos: {}", lx.Text, pos), pos); }},
      {1, [this](Lexeme<Polynom<T, List>> lx) { this->PolynomOne(lx); }},
      {2, [this](Lexeme<Polynom<T, List>> lx) { this->OperationOne(lx); }},
      {3, [this](Lexeme<Polynom<T, List>> lx) { this->SkobeOpenOne(lx); }},
      {3, [this](Lexeme<Polynom<T, List>> lx) { if (!this->st->isEmpty()) this->st->Pop(); throw Exception(format("Skobe {} close empty space on pos: {}", lx.Text, pos), pos); }},
      {4, [this](Lexeme<Polynom<T, List>> lx) { throw Exception(format("Number {} can't be after close skobe on pos: {}", lx.Text, pos), pos); }},
      {2, [this](Lexeme<Polynom<T, List>> lx) { this->OperationTwo(lx); }},
      {3, [this](Lexeme<Polynom<T, List>> lx) { this->st->Push(pos); throw Exception(format("Skobe {} can't be after close skobe on pos: {}", lx.Text, pos), pos);}},
      {4, [this](Lexeme<Polynom<T, List>> lx) { this->SkobeCloseOne(lx); }}
    };
    turm.Load(tpairs, 20);
  }
  ~ICorrectCheckerP() {
    if (qe != nullptr) {
      delete qe;
      delete st;
      delete exqe;
    }
  }
  void Do() {
    if (qe == nullptr) {
      qe = new TDynamicQueue<Lexeme<Polynom<T, List>>>(this->lexems_stream_int().Size());
      st = new TDynamicStack<size_t>(this->lexems_stream_int().Size());
      exqe = new TDynamicQueue<Exception>(this->lexems_stream_int().Size());
    }
    TDynamicQueue<Lexeme<Polynom<T, List>>> qe2 = this->tarith->LexemsStreamInt();
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

template <typename T>
Expr<T>* setVal(T val) {
  return new EPoly<T>(val);
}

template <typename T>
Expr<T>* setBiOp(char op, Expr<T>* left, Expr<T>* right) {
  return new BiOp<T>(op, left, right);
}

template <typename T>
Expr<T>* setSiOp(char op, Expr<T>* part) {
  return new SiOp<T>(op, part);
}

template <typename T, template <typename> class List = TStdList>
class ITreeCreator : public IHandler<T> {
  void freeTree() {
    RemoveVisitor<T> rvis;
    this->root()->accept(&rvis);
  }
public:
  ITreeCreator(TArith<T>& _tarith) {
    this->tarith = &_tarith;
  }
  void Do() {
    if (this->root() != nullptr) {
      freeTree();
      this->root() = nullptr;
    }
    TDynamicQueue<Lexeme<T>> qe = this->lexems_postfix_int();
    TDynamicStack<Expr<T>*> tmp(qe.Size());
    while (!qe.isEmpty()) {
      Expr<T>* now;
      Expr<T>* left;
      Expr<T>* right;
      switch (qe.Top().Type) {
      case binary_operation:
        right = tmp.Top();
        tmp.Pop();
        left = tmp.Top();
        tmp.Pop();
        now = setBiOp(qe.Top().Text[0], left, right);
        tmp.Push(now);
        break;
      case single_operation:
        left = tmp.Top();
        tmp.Pop();
        now = setSiOp(qe.Top().Text[0], left);
        tmp.Push(now);
        break;
      case number: case polynom:
        now = setVal(qe.Top().Value);
        tmp.Push(now);
        break;
      default:
        break;
      }
      qe.Pop();
    }
    this->root() = tmp.Top();
  }
};

template <typename T, template <typename> class List = TStdList>
class ITreeCounter : public IHandler<T> {
public:
  ITreeCounter(TArith<T>& _tarith) {
    this->tarith = &_tarith;
  }
  void Do() {
    CalcVisitor<T> visitor;
    T res = this->root()->accept(&visitor);
    this->result() = res;
  }
};
