#include <gtest/gtest.h>
#include "tarith.h"
#include "tqueue.h"
#include "tpoly.h"

TEST(TuringMachine, turing_machine_can_be_created) {
  ASSERT_NO_FATAL_FAILURE(TuringMachine<TFunc> turm(2, 2));
}

TEST(TuringMachine, turing_machine_cannot_be_created_with_negative_size) {
  ASSERT_ANY_THROW(TuringMachine<TFunc> turm(-2, 2));
  ASSERT_ANY_THROW(TuringMachine<TFunc> turm(2, -2));
}

TEST(TuringMachine, turing_machine_cannot_be_created_with_greater_size) {
  ASSERT_ANY_THROW(TuringMachine<TFunc> turm(MAXTURING + 1, 1));
  ASSERT_ANY_THROW(TuringMachine<TFunc> turm(1, MAXTURING + 1));
}

TEST(TuringMachine, turing_machine_can_return_number_of_states) {
  TuringMachine<TFunc> turm(2, 3);
  EXPECT_EQ(turm.GetNumStates(), 2);
}

TEST(TuringMachine, turing_machine_can_return_number_of_variants) {
  TuringMachine<TFunc> turm(2, 3);
  EXPECT_EQ(turm.GetNumVars(), 3);
}

TEST(TuringMachine, turing_machine_can_return_tpair) {
  TuringMachine<TFunc> turm(2, 2);
  EXPECT_EQ(turm[0][0].NextState, 0);
  EXPECT_EQ(turm[0][0].Function, nullptr);
}

TEST(TuringMachine, turing_machine_can_be_managed) {
  TuringMachine<TFunc> turm(2, 2);
  for (size_t i = 0; i < 4; i++)
    turm[i >> 1][i & 1].NextState = i;
  for (size_t i = 0; i < 4; i++)
    EXPECT_EQ(turm[i >> 1][i & 1].NextState, i);
}

TEST(TuringMachine, turing_machine_can_be_loaded) {
  TuringMachine<TFunc> turm(2, 2);
  TPair<TFunc> tpairs[4];
  for (size_t i = 0; i < 4; i++)
    tpairs[i].NextState = i;
  turm.Load(tpairs, 4);
  for (size_t i = 0; i < 4; i++)
    EXPECT_EQ(turm[i >> 1][i & 1].NextState, i);
}

TEST(TuringMachine, turing_machine_can_work_with_cursor) {
  TuringMachine<TFunc> turm(2, 2);
  EXPECT_EQ(turm.Cursor(), 0);
  turm.Cursor() = 2;
  EXPECT_EQ(turm.Cursor(), 2);
}

TEST(TuringMachine, turing_machine_can_be_copied) {
  TuringMachine<TFunc> turm(2, 2);
  for (size_t i = 0; i < 4; i++)
    turm[i >> 1][i & 1].NextState = i;
  TuringMachine<TFunc> turm2(turm);
  for (size_t i = 0; i < 4; i++)
    EXPECT_EQ(turm2[i >> 1][i & 1].NextState, i);
  EXPECT_EQ(turm2.GetNumStates(), 2);
  EXPECT_EQ(turm2.GetNumVars(), 2);
}

TEST(TuringMachine, turing_machine_can_assign) {
  TuringMachine<TFunc> turm(2, 2);
  for (size_t i = 0; i < 4; i++)
    turm[i >> 1][i & 1].NextState = i;
  TuringMachine<TFunc> turm2(turm);
  for (size_t i = 0; i < 4; i++)
    EXPECT_EQ(turm2[i >> 1][i & 1].NextState, i);
  EXPECT_EQ(turm2.GetNumStates(), 2);
  EXPECT_EQ(turm2.GetNumVars(), 2);
}

TEST(ILexemeTranslator, ilexemetranslator_work_correct) {
  string str = "254 + 23/9 + (12 - 76)";
  TArith<int> arth(str);
  ILexemeTranslator<int> hand1(arth);
  ICorrectChecker<int> hand2(arth);
  arth.AddHandler(hand1);
  arth.LaunchHandler(0);
  TDynamicQueue<Lexeme<int>> qe = arth.LexemsStreamInt();
  EXPECT_EQ(qe.Top().Value, 254);
  qe.Pop();
  EXPECT_EQ(qe.Top().Type, binary_operation);
  qe.Pop();
  EXPECT_EQ(qe.Top().Text, "23");
  qe.Pop();
  EXPECT_EQ(qe.Top().Value, 47);
  qe.Pop();
  EXPECT_EQ(qe.Top().Type, number);
  qe.Pop();
  EXPECT_EQ(qe.Top().Text, "+");
  qe.Pop();
  EXPECT_EQ(qe.Top().Value, 40);
  qe.Pop();
  EXPECT_EQ(qe.Top().Type, number);
  qe.Pop();
  EXPECT_EQ(qe.Top().Text, "-");
  qe.Pop();
  EXPECT_EQ(qe.Top().Value, 76);
  qe.Pop();
  EXPECT_EQ(qe.Top().Type, skobe);
}

TEST(ICorrectChecker, icorrectchecker_work_correct) {
  TArith<int> arth("28");
  ILexemeTranslator<int> hand1(arth);
  ICorrectChecker<int> hand2(arth);
  arth.AddHandler(hand1);
  arth.AddHandler(hand2);
  ASSERT_NO_THROW(arth.LaunchAllHandlers());
  arth = ")";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "125-25(80)";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "28++30";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "+";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "35-70*";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "*2-5";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "94 + 5 * (*3 - 5)";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "()";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "(356+9-)";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "(25 + 3)78";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "(61 - 2)(28 + 5)";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "25+ 3 * (96 - 10) / 23";
  ASSERT_NO_THROW(arth.LaunchAllHandlers());
  arth = "2548(89-)/0+5-000009";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
}

TEST(IPostfixMaker, ipostfixmaker_work_correct) {
  TArith<int> arth("28 + 5 * 9 + (20 / 3 + 5 * 1)");
  ILexemeTranslator<int> hand1(arth);
  ICorrectChecker<int> hand2(arth);
  IPostfixMaker<int> hand3(arth);
  arth.AddHandler(hand1);
  arth.AddHandler(hand2);
  arth.AddHandler(hand3);
  arth.LaunchAllHandlers();
  EXPECT_EQ(arth.PostFixStr(), "28 5 9 * + 20 3 / 5 1 * + +");
  arth = "(51 - 5 * 3 / 2) - (11 * 8 + 6) - 3";
  arth.LaunchAllHandlers();
  EXPECT_EQ(arth.PostFixStr(), "51 5 3 * 2 / - 11 8 * 6 + - 3 -");
}

TEST(ICounter, icounter_work_correct) {
  TArith<int> arth("10 / 2 + 6 * (20 - 3)");
  ILexemeTranslator<int> hand1(arth);
  ICorrectChecker<int> hand2(arth);
  IPostfixMaker<int> hand3(arth);
  ICounter<int> hand4(arth);
  arth.AddHandler(hand1);
  arth.AddHandler(hand2);
  arth.AddHandler(hand3);
  arth.AddHandler(hand4);
  arth.LaunchAllHandlers();
  EXPECT_EQ(arth.GetResult(), 107);
  arth = "-18 / 9 + (80 - 6) * 5";
  arth.LaunchAllHandlers();
  EXPECT_EQ(arth.GetResult(), 368);
  arth = "((2)+3)";
  EXPECT_NO_THROW(arth.LaunchAllHandlers());
  EXPECT_EQ(5, arth.GetResult());
}

TEST(ILexemeTranslatorP, ilexemetranslatorp_work_correct) {
  string str = "254 + x^2y^-3 + (zxy^-2 - 76)";
  TArith<Polynom<int>> arth(str);
  ILexemeTranslatorP<int> hand1(arth);
  arth.AddHandler(hand1);
  arth.LaunchHandler(0);
  TDynamicQueue<Lexeme<Polynom<int>>> qe = arth.LexemsStreamInt();
  EXPECT_EQ(qe.Top().Text, "254");
  qe.Pop();
  EXPECT_EQ(qe.Top().Type, binary_operation);
  qe.Pop();
  EXPECT_EQ(qe.Top().Text, "x^2y^-3");
  qe.Pop();
  EXPECT_EQ(qe.Top().Type, binary_operation);
  qe.Pop();
  EXPECT_EQ(qe.Top().Type, skobe);
  qe.Pop();
  EXPECT_EQ(qe.Top().Text, "zxy^-2");
  qe.Pop();
  EXPECT_EQ(qe.Top().Text, "-");
  qe.Pop();
  EXPECT_EQ(qe.Top().Type, polynom);
  qe.Pop();
  EXPECT_EQ(qe.Top().Type, skobe);
}

TEST(ICorrectCheckerP, icorrectcheckerp_work_correct) {
  TArith<Polynom<double>> arth("28");
  ILexemeTranslatorP<double> hand1(arth);
  ICorrectCheckerP<double> hand2(arth);
  arth.AddHandler(hand1);
  arth.AddHandler(hand2);
  ASSERT_NO_THROW(arth.LaunchAllHandlers());
  arth = ")";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "125-25(80)";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "28++30";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "+";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "35-70*";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "*2-5";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "94 + 5 * (*3 - 5)";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "()";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "(356+9-)";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "(25 + 3)78";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "(61 - 2)(28 + 5)";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "25+ 3 * (96 - 10) / 23";
  ASSERT_NO_THROW(arth.LaunchAllHandlers());
  arth = "2548(89-)/0+5-000009";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "25 + x^ + 2";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "25+ ^ - 2";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "25+ ^x -3";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "25+ x^^y -2";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "25+ x2y2 - 2";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "25+ 0001x - 2";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "25+ x^002 - 2";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "25+ abc - 2";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "25+ ^ - 2";
  ASSERT_ANY_THROW(arth.LaunchAllHandlers());
  arth = "25+ x^2y^-2 - 2";
  ASSERT_NO_THROW(arth.LaunchAllHandlers());
  TDynamicQueue<Lexeme<Polynom<double>>> qe = arth.LexemsStreamInt();
  EXPECT_EQ(qe.Top().Value.Count(0), 25);
  qe.Pop();
  EXPECT_EQ(qe.Top().Text, "+");
  qe.Pop();
  EXPECT_LT(qe.Top().Value.Count(3, 2) - 2.25, 3e-10);
}

TEST(ICounter, icounter_work_correct_with_polynom) {
  TArith<Polynom<int>> arth("10 / 2 + 6 * (20 - 3)");
  ILexemeTranslatorP<int> hand1(arth);
  ICorrectCheckerP<int> hand2(arth);
  IPostfixMaker<Polynom<int>> hand3(arth);
  ICounter<Polynom<int>> hand4(arth);
  arth.AddHandler(hand1);
  arth.AddHandler(hand2);
  arth.AddHandler(hand3);
  arth.AddHandler(hand4);
  arth.LaunchAllHandlers();
  EXPECT_EQ(arth.GetResult().Count(), 107);
  arth = "-18 / 9 + (80 - 6) * 5";
  arth.LaunchAllHandlers();
  EXPECT_EQ(arth.GetResult().Count(), 368);
  arth = "((2)+3)";
  EXPECT_NO_THROW(arth.LaunchAllHandlers());
  EXPECT_EQ(5, arth.GetResult().Count());
  arth = "x^2 + xy^2 + 10";
  arth.LaunchAllHandlers();
  EXPECT_EQ(arth.GetResult().Count(2, 3), 32);
}
