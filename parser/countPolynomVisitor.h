#pragma once


#include "antlr4-runtime.h"
#include "polynomParser.h"
#include "polynomBaseVisitor.h"
#include "tpoly.h"
#include <any>
#include <variant>



/**
 * This class defines an abstract visitor for a parse tree
 * produced by polynomParser.
 */
class  countPolynomVisitor : public polynomBaseVisitor {
public:

  /**
   * Visit parse trees produced by polynomParser.
   */
  virtual std::any visitMulDiv(polynomParser::MulDivContext *context) {
    Polynom<int> left = std::any_cast<Polynom<int>>(visit(context->poly(0)));
    Polynom<int> right = std::any_cast<Polynom<int>>(visit(context->poly(1)));
    if (context->op->getType() == polynomParser::MUL)
      return left * right;
    else
      return left / right;
  }

  virtual std::any visitAddSub(polynomParser::AddSubContext *context) {
    Polynom<int> left = std::any_cast<Polynom<int>>(visit(context->poly(0)));
    Polynom<int> right = std::any_cast<Polynom<int>>(visit(context->poly(1)));
    if (context->op->getType() == polynomParser::ADD)
      return left + right;
    else
      return left - right;
  }

  virtual std::any visitParens(polynomParser::ParensContext *context) {
    return visit(context->poly());
  }

  virtual std::any visitMonom(polynomParser::MonomContext *context) {
    size_t ind = 0;
    int coeff = 0;
    size_t now = 0;
    bool isNeg = false;
    bool isLetter = false;
    char deg[4]{};
    std::string lx = context->getText();
    if (!(lx[0] >= '0' && lx[0] <= '9'))
      coeff = 1;
    while (lx[ind] >= '0' && lx[ind] <= '9') {
      coeff = coeff * 10 + lx[ind] - '0';
      ind++;
    }
    char tmp = 0;
    for (; ind < lx.size(); ind++) {
      if (monSym.contains(lx[ind])) {
        if (isLetter)
          deg[now] += 1;
        else {
          if (isNeg)
            tmp = -tmp;
          deg[now] += tmp;
        }
        tmp = 0;
        now = monSym.find(lx[ind]);
        isNeg = false;
        isLetter = true;
      }
      else if (lx[ind] == '-') {
        isNeg = true;
        isLetter = false;
      }
      else if (lx[ind] >= '0' && lx[ind] <= '9') {
        tmp = tmp * 10 + lx[ind] - '0';
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
    Polynom<int> p(coeff, deg[0], deg[1], deg[2], deg[3]);
    return p;
  }

  virtual std::any visitMon(polynomParser::MonContext *context) {
    size_t ind = 0;
    int coeff = 0;
    size_t now = 0;
    bool isNeg = false;
    bool isLetter = false;
    char deg[4]{};
    std::string lx = context->getText();
    if (!(lx[0] >= '0' && lx[0] <= '9'))
      coeff = 1;
    while (lx[ind] >= '0' && lx[ind] <= '9') {
      coeff = coeff * 10 + lx[ind] - '0';
      ind++;
    }
    char tmp = 0;
    for (; ind < lx.size(); ind++) {
      if (monSym.contains(lx[ind])) {
        if (isLetter)
          deg[now] += 1;
        else {
          if (isNeg)
            tmp = -tmp;
          deg[now] += tmp;
        }
        tmp = 0;
        now = monSym.find(lx[ind]);
        isNeg = false;
        isLetter = true;
      }
      else if (lx[ind] == '-') {
        isNeg = true;
        isLetter = false;
      }
      else if (lx[ind] >= '0' && lx[ind] <= '9') {
        tmp = tmp * 10 + lx[ind] - '0';
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
    Polynom<int> p(coeff, deg[0], deg[1], deg[2], deg[3]);
    return p;
  }


};
