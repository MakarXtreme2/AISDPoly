#pragma once
#include "tstack.h"
#include <iostream>

using namespace std;

template <typename T>
class AVLTree {
  struct Node {
    T val;
    int balance = 0;
    Node* left = nullptr;
    Node* right = nullptr;
    Node(T _val = 0) : val(_val) {}
  };
  Node* root = nullptr;
  TDynamicStack<Node*> st;
  void partOfDelete(Node*& tmp) {
    Node* parent = st.isEmpty() ? nullptr : st.Top();
    if (!tmp->left && !tmp->right) {
      if (parent) {
        if (parent->left == tmp)
          parent->balance++;
        else if (parent->right == tmp)
          parent->balance--;
      }
      delete tmp;
      tmp = parent;
    }
    else if (!tmp->left) {
      if (parent) {
        if (parent->left == tmp) {
          parent->balance++;
          parent->left = tmp->right;
        }
        else if (parent->right == tmp) {
          parent->balance--;
          parent->right = tmp->right;
        }
      }
      Node* tmp2 = tmp->right;
      delete tmp;
      tmp = tmp2;
    }
    else if (!tmp->right) {
      if (parent) {
        if (parent->left == tmp) {
          parent->balance++;
          parent->left = tmp->left;
        }
        else if (parent->right == tmp) {
          parent->balance--;
          parent->right = tmp->left;
        }
      }
      Node* tmp2 = tmp->left;
      delete tmp;
      tmp = tmp2;
    }
    else {
      st.Push(tmp);
      Node* tmp2 = tmp->left;
      while (tmp2->right) {
        st.Push(tmp2);
        tmp2 = tmp2->right;
      }
      parent = st.Top();
      tmp->val = tmp2->val;
      if (parent != tmp)
        parent->right = tmp2->left;
      else
        parent->left = tmp2->left;
      delete tmp2;
      if (parent != tmp)
        parent->balance--;
      else
        parent->balance++;
      tmp = parent;
    }
  }
  void smallLeftRotate(Node*& tmp) {
    Node* right = tmp->right;
    bool f = false;
    bool ff = false;
    if (tmp->balance == 1)
      f = true;
    if (right->balance < 0) {
      smallRightRotate(right);
      ff = true;
    }
    if (!f) {
      tmp->balance = 0;
      right->balance = 0;
    } else {
      if (right->balance == -1) {
        tmp->balance = 0;
        right->balance = -2;
      } else if (right->balance == 1) {
        tmp->balance = -1;
        right->balance = -1;
      }
      else {
        tmp->balance = 0;
        right->balance = 0;
      }
    }
    if (ff) {
      if (right->balance == 1) {
        right->balance = 0;
        tmp->balance = 0;
      } else if (right->balance == 2) {
        right->balance = 0;
        tmp->balance = -1;
      }
    }
    Node* l = tmp->left;
    Node* m = right->left;
    Node* r = right->right;
    right->left = tmp;
    tmp->right = m;
    tmp = right;
  }
  void smallRightRotate(Node*& tmp) {
    Node* left = tmp->left;
    bool f = false;
    bool ff = false;
    if (tmp->balance == -1)
      f = true;
    if (left->balance > 0) {
      smallLeftRotate(left);
      ff = true;
    }
    if (!f) {
      tmp->balance = 0;
      left->balance = 0;
    } else {
      if (left->balance == 1) {
        tmp->balance = 0;
        left->balance = 2;
      } else if (left->balance == -1) {
        tmp->balance = 1;
        left->balance = 1;
      } else {
        tmp->balance = 0;
        left->balance = 0;
      }
    }
    if (ff) {
      if (left->balance == -1) {
        left->balance = 0;
        tmp->balance = 0;
      } else if (left->balance == -2) {
        left->balance = 0;
        tmp->balance = -1;
      }
    }
    Node* l = left->left;
    Node* m = left->right;
    Node* r = tmp->right;
    left->right = tmp;
    tmp->left = m;
    tmp = left;
  }
  void doBalance(bool ins = true) {
    Node* tmp, parent;
    if (ins) {
      tmp = st.Top();
      st.Pop();
      if (tmp->balance == 2) {
        smallLeftRotate(tmp);
        return;
      }
      else if (tmp->balance == -2) {
        smallRightRotate(tmp);
        return;
      }
      while (!st.isEmpty()) {
        st.Pop();
        parent = st.Top();
        if (tmp == parent->left)
          parent->balance--;
        else if (tmp == parent->right)
          parent->balance++;
        if (parent->balance == 2) {
          smallLeftRotate(parent);
          return;
        }
        else if (parent->balance == -2) {
          smallRightRotate(parent);
          return;
        }
        tmp = parent;
      }
    } else {
      tmp = st.Top();
      st.Pop();
      if (tmp->balance == 2)
        smallLeftRotate(tmp);
      else if (tmp->balance == -2)
        smallRightRotate(tmp);
      while (!st.isEmpty()) {
        st.Pop();
        parent = st.Top();
        if (tmp == parent->left)
          parent->balance--;
        else if (tmp == parent->right)
          parent->balance++;
        if (parent->balance == 2)
          smallLeftRotate(parent);
        else if (parent->balance == -2)
          smallRightRotate(parent);
        tmp = parent;
      }
    }
  }
public:
  AVLTree() = default;
  void Insert(T val) {
    st.Clear();
    if (root == nullptr) {
      root = new Node(val);
      return;
    }
    Node* tmp = root;
    while (true) {
      st.Push(tmp);
      if (val < tmp->val && tmp->left != nullptr)
        tmp = tmp->left;
      else if (val > tmp->val && tmp->right != nullptr)
        tmp = tmp->right;
      else
        break;
    }
    if (val < tmp->val) {
      tmp->left = new Node(val);
      tmp->balance--;
    }
    else if (val > tmp->val) {
      tmp->right = new Node(val);
      tmp->balance++;
    }
    else
      return;
    st.Push(tmp);
    doBalance(tmp);
  }
  void Delete(T val) {
    st.Clear();
    if (root == nullptr)
      return;
    Node* tmp = root;
    while (true) {
      st.Push(tmp);
      if (val < tmp->val && tmp->left != nullptr)
        tmp = tmp->left;
      else if (val > tmp->val && tmp->right != nullptr)
        tmp = tmp->right;
      else
        break;
    }
    if (val != tmp->val)
      return;
    else
      partOfDelete(tmp);
    doBalance(tmp, false);
  }
  T* Find(T val) {
    if (root == nullptr)
      return nullptr;
    Node* tmp = root;
    while (true) {
      if (val < tmp->val && tmp->left != nullptr)
        tmp = tmp->left;
      else if (val > tmp->val && tmp->right != nullptr)
        tmp = tmp->right;
      else
        break;
    }
    if (tmp->val == val)
      return &tmp->val;
    else
      return nullptr;
  }
  void printTLR() {
    if (root == nullptr)
      return;
    st.Clear();
    TDynamicStack<long long> tst;
    st.Push(root);
    tst.Push(0);
    while (!st.isEmpty()) {
      Node* tmp = st.Top();
      st.Pop();
      long long h = tst.Top();
      tst.Pop();
      for (long long i = 0; i < h; i++)
        cout << '\t';
      cout << tmp->val << endl;
      if (tmp->right != nullptr) {
        st.Push(tmp->right);
        tst.Push(h + 1);
      }
      if (tmp->left != nullptr) {
        st.Push(tmp->left);
        tst.Push(h + 1);
      }
    }
  }
};
