
// Generated from polynom.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "polynomParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by polynomParser.
 */
class  polynomVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by polynomParser.
   */
    virtual std::any visitMulDiv(polynomParser::MulDivContext *context) = 0;

    virtual std::any visitAddSub(polynomParser::AddSubContext *context) = 0;

    virtual std::any visitParens(polynomParser::ParensContext *context) = 0;

    virtual std::any visitMonom(polynomParser::MonomContext *context) = 0;

    virtual std::any visitMon(polynomParser::MonContext *context) = 0;


};

