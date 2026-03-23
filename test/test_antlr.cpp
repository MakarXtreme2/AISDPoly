#include <ANTLRInputStream.h>
#include <any>
#include <gtest/gtest.h>
#include <sstream>
#include "CommonTokenStream.h"
#include "countPolynomVisitor.h"
#include "polynomLexer.h"
#include "polynomParser.h"
#include "tpoly.h"


TEST(Antlr, antlr_work_correct) {
  std::string expr = "xy + 2 - 8 * (20 - z)";
  std::stringstream stream(expr);
  antlr4::ANTLRInputStream input(stream);
  polynomLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  polynomParser parser(&tokens);
  countPolynomVisitor visitor;
  Polynom<int> result = std::any_cast<Polynom<int>>(visitor.visit(parser.poly()));
  EXPECT_EQ(result.Count(1, 2, 0), -156);
}
