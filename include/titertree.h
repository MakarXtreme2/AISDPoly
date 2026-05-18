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

  struct StateNode {
    int state;
    Node* node;
  };

  void setStackNode(TDynamicStack<StateNode>& st, Node*& node) {
    StateNode state_node;
    state_node.state = 0;
    switch (node->nodetype) {
    case numvarT:
      if (node->nodes[0])
        state_node.node = node->nodes[0];
      st.Push(state_node);
      break;
    case exprT:
    case conditionT:
    case assignopT:
    case lineT:
    case codeblockT:
      for (int i = node->num_of_nodes - 1; i >= 0; --i) {
        if (node->nodes[i]) {
          state_node.node = node->nodes[i];
          st.Push(state_node);
        }
      }
      break;
    case cycleT:
    case ifopT:
      if (node->nodes[1])
        state_node.node = node->nodes[1];
      st.Push(state_node);
      break;
    default:
      break;
    }
  }

  void doCycle(TDynamicStack<StateNode>& st, Node*& node) {
    StateNode state_node;
    state_node.state = 1;
    state_node.node = node;
    if (node->nodes[1]->result == 1) {
      cout << "Sor\n";
      st.Push(state_node);
      state_node.node = node->nodes[1];
      state_node.state = 0;
      st.Push(state_node);
      if (node->num_of_nodes == 3 || node->num_of_nodes == 4)
        state_node.node = node->nodes[2];
      else
        state_node.node = node->nodes[3];
      st.Push(state_node);
    }
    else if (node->nodes[1]->result == 0) {
      state_node.state = 0;
      if (node->num_of_nodes == 4)
        state_node.node = node->nodes[3];
      if (node->num_of_nodes == 6)
        state_node.node = node->nodes[5];
      st.Push(state_node);
    }
  }

  void doIfOp(TDynamicStack<StateNode>& st, Node*& node) {
    StateNode state_node;
    state_node.state = 0;
    if (node->nodes[1]->result == 1) {
      if (node->num_of_nodes == 4)
        state_node.node = node->nodes[3];
      if (node->num_of_nodes == 6)
        state_node.node = node->nodes[5];
      st.Push(state_node);

      if (node->num_of_nodes == 3 || node->num_of_nodes == 4)
        state_node.node = node->nodes[2];
      else
        state_node.node = node->nodes[3];
      st.Push(state_node);
    }
    else if (node->nodes[1]->result == 0) {
      if (node->num_of_nodes == 4)
        state_node.node = node->nodes[3];
      if (node->num_of_nodes == 6)
        state_node.node = node->nodes[5];
      st.Push(state_node);
    }
  }

  void doOperation(Node*& node) {
    switch (node->nodetype) {
    case numberT:
      node->result = node->lexeme.Value;
      break;
    case varT:
      T* tmp;
      tmp = table.Find(node->lexeme.Text);
      if (tmp != nullptr)
        node->result = *tmp;
      break;
    case numvarT:
      node->result = node->nodes[0]->result;
      break;
    case exprT:
      if (node->num_of_nodes == 1)
        node->result = node->nodes[0]->result;
      else if (node->num_of_nodes == 2)
        node->result = -node->nodes[1]->result;
      else {
        if (node->nodes[0]->nodetype == skobeopenT)
          node->result = node->nodes[1]->result;
        else if (node->nodes[1]->lexeme.Text == "+")
          node->result = node->nodes[0]->result + node->nodes[2]->result;
        else if (node->nodes[1]->lexeme.Text == "-")
          node->result = node->nodes[0]->result - node->nodes[2]->result;
        else if (node->nodes[1]->lexeme.Text == "*")
          node->result = node->nodes[0]->result * node->nodes[2]->result;
        else if (node->nodes[1]->lexeme.Text == "/")
          node->result = node->nodes[0]->result / node->nodes[2]->result;
      }
      break;
    case conditionT:
      if (node->nodes[0]->nodetype == skobeopenT)
        node->result = node->nodes[1]->result;
      else if (node->nodes[1]->lexeme.Text == "<")
        node->result = node->nodes[0]->result < node->nodes[2]->result ? 1 : 0;
      else if (node->nodes[1]->lexeme.Text == "<=")
        node->result = node->nodes[0]->result <= node->nodes[2]->result ? 1 : 0;
      else if (node->nodes[1]->lexeme.Text == ">")
        node->result = node->nodes[0]->result > node->nodes[2]->result ? 1 : 0;
      else if (node->nodes[1]->lexeme.Text == ">=")
        node->result = node->nodes[0]->result >= node->nodes[2]->result ? 1 : 0;
      else if (node->nodes[1]->lexeme.Text == "==")
        node->result = node->nodes[0]->result == node->nodes[2]->result ? 1 : 0;
      else if (node->nodes[1]->lexeme.Text == "<>")
        node->result = node->nodes[0]->result != node->nodes[2]->result ? 1 : 0;
      break;
    case assignopT:
      table.Insert(node->nodes[0]->lexeme.Text, node->nodes[2]->result);
      break;
    default:
      break;
    }
  }

public:

  friend TMaker<T>;
  friend IHandler<T>;
  friend ITreeMaker<T>;

  TSolveTree() : root(nullptr) {}
  TSolveTree(TSolveTree<T>& other) {
    Copy(other);
  }

  void Solve(TableAVL<string, T>& _table) {
    TDynamicStack<StateNode> st;
    StateNode state_node;
    if (root) {
      state_node = {0, root};
      st.Push(state_node);
    }
    while (!st.isEmpty()) {
      state_node = st.Top();
      st.Pop();
      if (state_node.state == 0) {
        state_node.state = 1;
        st.Push(state_node);
        setStackNode(st, state_node.node);
      } else {
        if (state_node.node->nodetype == cycleT)
          doCycle(st, state_node.node);
        else if (state_node.node->nodetype == ifopT)
          doIfOp(st, state_node.node);
        else
          doOperation(state_node.node);
      }
    }
    _table = table;
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
    TDynamicStack<long long> tmp_sst;
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
      for (long long i = 0; i < h - 1; i++) {
        bool f = false;
        tmp_sst = tst;
        while (!tmp_sst.isEmpty()) {
          if (tmp_sst.Top() == i + 1) {
            f = true;
            break;
          }
          tmp_sst.Pop();
        }
        if (f)
          cout << "|      ";
        else
          cout << "       ";
      }
      if (h > 0)
        cout << "|----> ";
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
