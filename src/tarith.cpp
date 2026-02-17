#include "tarith.h"
#include "tstack.h"
#include "tqueue.h"
#include <format>
#include <stdexcept>
#include <string>
#include <iostream>


size_t ILexemeTranslator::Decode(char ch) {
  if (ch >= '0' && ch <= '9')
    return 0;
  else if (ch >= '*' && ch <= '/')
    return 1;
  else if (ch == '(' || ch == ')')
    return 2;
  else
    return 3;
}

void ILexemeTranslator::NumberOne(char c) {
  lxt.Text = c;
  lxt.Type = number;
  lxt.Value = c - 48;
  isPushed = false;
}

void ILexemeTranslator::NumberTwo(char c) {
  lxt.Value = lxt.Value * 10 + c - 48;
  lxt.Text += c;
  isPushed = false;
}

void ILexemeTranslator::OperationOne(char c) {
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

void ILexemeTranslator::SkobeOne(char c) {
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

void ILexemeTranslator::Do() {
  if (qe == nullptr)
    qe = new TDynamicQueue<Lexeme<int>>(inf_str().size());
  for (size_t i = 0; i < inf_str().size(); i++) {
    size_t tmp = Decode(inf_str()[i]);
    if (tmp != 3) {
      turm[turm.Cursor()][tmp].Function(inf_str()[i]);
      turm.Cursor() = turm[turm.Cursor()][tmp].NextState;
    }
  }
  if (!isPushed)
    qe->Push(lxt);
  isPushed = true;
  lxt.Text = "";
  lxt.Value = 0;
  turm.Cursor() = 0;
  lexems_stream_int() = *qe;
  delete qe;
  qe = nullptr;
}

void ICorrectChecker::NumberOne(Lexeme<int> lx) {
  if (lx.Value != 0 && lx.Text[0] == '0')
    throw Exception(format("Number {} can't start with zero on pos: {}", lx.Text, pos), pos);
  if (lx.Value == 0 && lx.Text != "0")
    throw Exception(format("Duplicating zeroes on pos: {}", pos), pos);
  qe->Push(lx);
}

void ICorrectChecker::NumberTwo(Lexeme<int> lx) {
  if (lx.Value != 0 && lx.Text[0] == '0')
    throw Exception(format("Number {} can't start with zero on pos: {}", lx.Text, pos), pos);
  if (lx.Value == 0 && lx.Text != "0")
    throw Exception(format("Duplicating zeroes on pos: {}", pos), pos);
  if (divide_by && lx.Value == 0)
    throw Exception(format("Dividing by zero on pos: {}", pos), pos);
  divide_by = false;
  qe->Push(lx);
}

void ICorrectChecker::OperationOne(Lexeme<int> lx) {
  if (lx.Text != "+" && lx.Text != "-")
    throw Exception(format("Wrong operation {} on pos: {}", lx.Text, pos),pos);
  lx.Type = single_operation;
  qe->Push(lx);
}

void ICorrectChecker::OperationTwo(Lexeme<int> lx) {
  qe->Push(lx);
  if (lx.Text == "/")
    divide_by = true;
}

void ICorrectChecker::SkobeOpenOne(Lexeme<int> lx) {
  qe->Push(lx);
  st->Push(pos);
}

void ICorrectChecker::SkobeCloseOne(Lexeme<int> lx) {
  qe->Push(lx);
  if (st->isEmpty())
    throw Exception(format("Wrong skobe {} on pos: {}", lx.Text, pos), pos);
  st->Pop();
}

size_t ICorrectChecker::Decode(Lexeme<int> lx) {
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

void ICorrectChecker::printExceptions() {
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

void ICorrectChecker::Do() {
  if (qe == nullptr) {
    qe = new TDynamicQueue<Lexeme<int>>(lexems_stream_int().Size());
    st = new TDynamicStack<size_t>(lexems_stream_int().Size());
    exqe = new TDynamicQueue<Exception>(lexems_stream_int().Size());
  }
  TDynamicQueue<Lexeme<int>> qe2 = tarith->LexemsStreamInt();
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
    while (inf_str()[pos] == ' ')
      pos++;
    qe2.Pop();
  }
  if (tmplx.Type == binary_operation || tmplx.Type == single_operation) {
    Exception exfin(format("Expression can't be ended by operation {} on pos: {}", tmplx.Text, pos - 1), pos);
    exqe->Push(exfin);
   }
  lexems_stream_int() = *qe;
  printExceptions();
}

void IPostfixMaker::Do() {
  TDynamicQueue<Lexeme<int>> qe = tarith->LexemsStreamInt();
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
  lexems_postfix_int() = qe2;
  postfix_str() = posttmp;
}

void ICounter::Do() {
  TDynamicQueue<Lexeme<int>> qe = tarith->LexemsPostfixInt();
  TDynamicStack<int> st(qe.Size());
  result() = 0;
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
  result() = st.Top();
}
