#pragma once

#include "tarith.h"
#include "tstack.h"
#include "tqueue.h"
#include "ttree.h"
#include <stdexcept>

template <typename T>
class TSolveTree {

  struct Node {
    Lexeme<T> lexeme = Lexeme<T>();
    size_t num_of_nodes = 0;
    Node** nodes = nullptr;
    T result = 0;

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
        break;
      case single_operation:
        break;
      case binary_operation:
        break;
      case variable:
        break;
      case assign:
        break;
      case semicolon:
        break;
      case special_word:
        break;
      case condition:
        break;
      default:
        break;
      }
    }

  };

  Node* root;

public:

  TSolveTree() : root(nullptr) {}

};
