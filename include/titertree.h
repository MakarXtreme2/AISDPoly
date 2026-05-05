#pragma once

#include <stdexcept>
#include <iostream>

#include "tstack.h"
#include "tqueue.h"
#include "ttree.h"
#include "lexeme.h"

using namespace std;

template <typename T>
class TMaker;

template <typename T>
class IHandler;

template <typename T>
class TSolveTree {

  TableAVL<string, T> table;

  struct Node {

    Lexeme<T> lexeme = Lexeme<T>();
    size_t num_of_nodes = 0;
    Node** nodes = nullptr;
    T result = T();

    Node(Lexeme<T> _lexeme = Lexeme<T>(), size_t _num_of_nodes = 0) :
      lexeme(_lexeme),
      num_of_nodes(_num_of_nodes)
    {
      if (num_of_nodes != 0)
        nodes = new Node*[num_of_nodes];
    }

    Node(const Node& other) {
      lexeme = other.lexeme;
      num_of_nodes = other.num_of_nodes;
      if (num_of_nodes != 0)
        nodes = new Node*[num_of_nodes];
    }

    Node& operator=(const Node& other) {
      if (this == &other)
        throw out_of_range("SelfAssign");
      if (num_of_nodes != other.num_of_nodes) {
        delete[] nodes;
        num_of_nodes = other.num_of_nodes;
        nodes = new Node*[num_of_nodes];
      }
      lexeme = other.lexeme;
      for (size_t i = 0; i < num_of_nodes; ++i)
        nodes[i] = nullptr;
    }

    ~Node() {
      delete[] nodes;
    }

    void Do() {
      switch (lexeme.Type) {
      case number:
        result = lexeme.Value;
        break;
      case single_operation:
        SingleOperation();
        break;
      case binary_operation:
        BinaryOperation();
        break;
      case variable:
        break;
      case assign:
        Assign();
        break;
      case semicolon:
        break;
      case special_word:
        break;
      case condition:
        Condition();
        break;
      default:
        break;
      }
    }

  private:

    void SingleOperation() {
      switch (lexeme.Text) {
      case "-":
        result = -nodes[0]->result;
        break;
      default:
        break;
      }
    }

    void BinaryOperation() {
      switch (lexeme.Text) {
      case "+":
        result = nodes[0]->result + nodes[1]->result;
        break;
      case "-":
        result = nodes[0]->result - nodes[1]->result;
        break;
      case "*":
        result = nodes[0]->result * nodes[1]->result;
        break;
      case "/":
        result = nodes[0]->result / nodes[1]->result;
        break;
      default:
        break;
      }
    }

    void Assign() {
      result = nodes[1]->result;
      //table->Insert(nodes[0]->lexeme.Text, result);
    }

    void Condition() {
      switch (lexeme.Text) {
      case "<":
        result = nodes[0]->result < nodes[1]->result ? 1 : 0;
        break;
      case ">":
        result = nodes[0]->result > nodes[1]->result ? 1 : 0;
        break;
      case "==":
        result = nodes[0]->result == nodes[1]->result ? 1 : 0;
        break;
      case "<=":
        result = nodes[0]->result <= nodes[1]->result ? 1 : 0;
        break;
      case ">=":
        result = nodes[0]->result >= nodes[1]->result ? 1 : 0;
        break;
      default:
        break;
      }
    }

  };

  Node* root;

public:

  friend TMaker<T>;
  friend IHandler<T>;

  TSolveTree() : root(nullptr) {}


};
