#include "lexeme.h"

string nodetypeToStr(NodeType node_type) {
  switch (node_type) {
  case numberT:
    return "number";
  case varT:
    return "var";
  case numvarT:
    return "numvar";
  case endT:
    return "end";
  case beginT:
    return "begin";
  case whileT:
    return "while";
  case singleminusT:
    return "singleminus";
  case plusminusT:
    return "plusminus";
  case muldivT:
    return "muldiv";
  case condT:
    return "cond";
  case skobeopenT:
    return "skobeopen";
  case skobecloseT:
    return "skobeclose";
  case assignT:
    return "assign";
  case scT:
    return "sc";
  case exprT:
    return "expr";
  case conditionT:
    return "condition";
  case assignopT:
    return "assignop";
  case lineT:
    return "line";
  case codeblockT:
    return "codeblock";
  case cycleT:
    return "cycle";
  case progT:
    return "prog";
  case ifT:
    return "if";
  case ifopT:
    return "ifop";
  default:
    return "NULL";
  }
}
