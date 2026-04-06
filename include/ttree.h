#pragma once
#include "tstack.h"

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
    if (right->balance < 0)
      smallRightRotate(right);
    Node* l = tmp->left;
    Node* m = right->left;
    Node* r = right->right;
    right->left = tmp;
    tmp->right = m;
    tmp->balance = 0;
    right->balance = 0;
    tmp = right;
  }
  void smallRightRotate(Node*& tmp) {
    Node* left = tmp->left;
    if (left->balance > 0)
      smallLeftRotate(left);
    Node* l = left->left;
    Node* m = left->right;
    Node* r = tmp->right;
    left->right = tmp;
    tmp->left = m;
    tmp->balance = 0;
    left->balance = 0;
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
          smallLeftRotate(tmp);
          return;
        }
        else if (parent->balance == -2) {
          smallRightRotate(tmp);
          return;
        }
        tmp = parent;
      }
    } else {

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
};
