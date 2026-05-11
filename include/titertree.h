#pragma once

#include <stdexcept>
#include <iostream>

#include "tarith.h"
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
class ITreeMaker;

template <typename T>
class TSolveTree {

  TableAVL<string, T> table;

  struct Node {

    Lexeme<T> lexeme = Lexeme<T>();
    size_t num_of_nodes = 0;
    Node** nodes = nullptr;
    NodeType nodetype = numberT;
    T result = T();

    Node(Lexeme<T> _lexeme = Lexeme<T>(),
         NodeType _nodetype = numberT,
         size_t _num_of_nodes = 0) :
           lexeme(_lexeme),
           num_of_nodes(_num_of_nodes),
           nodetype(_nodetype)
    {
      if (num_of_nodes != 0)
        nodes = new Node*[num_of_nodes];
    }

    Node(const Node& other) {
      lexeme = other.lexeme;
      nodetype = other.nodetype;
      num_of_nodes = other.num_of_nodes;
      if (num_of_nodes != 0)
        nodes = new Node*[num_of_nodes];
      for (size_t i = 0; i < num_of_nodes; ++i)
        nodes[i] = nullptr;
    }

    Node& operator=(const Node& other) {
      if (this == &other)
        throw out_of_range("SelfAssign");
      if (num_of_nodes != other.num_of_nodes) {
        delete[] nodes;
        num_of_nodes = other.num_of_nodes;
        nodes = new Node*[num_of_nodes];
      }
      nodetype = other.nodetype;
      lexeme = other.lexeme;
      for (size_t i = 0; i < num_of_nodes; ++i)
        nodes[i] = nullptr;
    }

    ~Node() {
      if (num_of_nodes != 0)
        delete[] nodes;
    }

    void setNodes(size_t num_nodes) {
      if (nodes)
        delete[] nodes;
      num_of_nodes = num_nodes;
      nodes = new Node*[num_of_nodes];
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

  void Copy(const TSolveTree<T>& other) {
    TDynamicStack<Node*> other_stack;
    TDynamicStack<Node*> tail_stack;
    TDynamicStack<int> op_stack;
    Node* other_tmp = other.root;
    Node* tmp = nullptr;
    int op;
    if (other.root != nullptr) {
      root = new Node(*other_tmp);
    } else {
      root = nullptr;
      return;
    }
    tmp = root;
    for (int i = 0; i < other_tmp->num_of_nodes; ++i) {
      if (other_tmp->nodes[i]) {
        other_stack.Push(other_tmp->nodes[i]);
        op_stack.Push(i);
      }
    }
    while (!other_stack.isEmpty()) {
      other_tmp = other_stack.Top();
      other_stack.Pop();
      op = op_stack.Top();
      op_stack.Pop();
      while (op == -1 && !op_stack.isEmpty()) {
        tmp = tail_stack.Top();
        tail_stack.Pop();
        op = op_stack.Top();
        op_stack.Pop();
      }
      if (op != -1) {
        tmp->nodes[op] = new Node(*other_tmp);
        tail_stack.Push(tmp);
        tmp = tmp->nodes[op];
        op_stack.Push(-1);
      }
      for (int i = 0; i < other_tmp->num_of_nodes; ++i) {
        if (other_tmp->nodes[i]) {
          other_stack.Push(other_tmp->nodes[i]);
          op_stack.Push(i);
        }
      }
    }
  }

  Node* root;

public:

  friend TMaker<T>;
  friend IHandler<T>;
  friend ITreeMaker<T>;

  TSolveTree() : root(nullptr) {}
  TSolveTree(TSolveTree<T>& other) {
    Copy(other);
  }

  TSolveTree<T>& operator=(TSolveTree<T>& other) {
    if (this == &other)
      throw out_of_range("SelfAssign");
    Clear();
    Copy(other);
  }

  void printTLR() {
    if (root == nullptr)
      return;
    TDynamicStack<Node*> stk;
    TDynamicStack<long long> tst;
    TDynamicStack<int> sst;
    stk.Push(root);
    tst.Push(0);
    sst.Push(0);
    cout << "----- Tree -----\n\n";
    while (!stk.isEmpty()) {
      Node* tmp = stk.Top();
      stk.Pop();
      long long h = tst.Top();
      int num = sst.Top();
      sst.Pop();
      tst.Pop();
      for (long long i = 0; i < h; i++)
        cout << '\t';
      cout << num << ": " << nodetypeToStr(tmp->nodetype) << endl;
      for (int i = tmp->num_of_nodes - 1; i >= 0; --i) {
        if (tmp->nodes[i]) {
          stk.Push(tmp->nodes[i]);
          tst.Push(h + 1);
          sst.Push(i);
        }
      }
    }
    cout << endl;
  }

  void Clear() {
    TDynamicStack<Node*> st;
    if (root == nullptr)
      return;
    Node* tmp = root;
    st.Push(tmp);
    while (!st.isEmpty()) {
      tmp = st.Top();
      st.Pop();
      for (int i = 0; i < tmp->num_of_nodes; ++i) {
        if (tmp->nodes[i])
          st.Push(tmp->nodes[i]);
      }
      delete tmp;
    }
    root = nullptr;
  }

  ~TSolveTree() {
    Clear();
  }


};
