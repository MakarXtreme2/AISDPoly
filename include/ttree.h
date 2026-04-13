#pragma once
#include "tstack.h"
#include <iostream>

using namespace std;

template <typename T>
class AVLTree {
  struct Node {
    T val;
    long long height = 0;
    Node* left = nullptr;
    Node* right = nullptr;
    Node(T _val = 0) : val(_val) {}
  };

  Node* root = nullptr;
  TDynamicStack<Node*> st;
  long long size = 0;

  void printStack() {
    TDynamicStack<Node*> tmp = st;
    while (!tmp.isEmpty()) {
      if (tmp.Size() == 1)
        cout << tmp.Top()->val;
      else
        cout << tmp.Top()->val << " ";
      tmp.Pop();
    }
    cout << endl;
  }

  void partOfDelete(Node*& tmp) {
    Node* parent = st.isEmpty() ? nullptr : st.Top();
    if (!tmp->left && !tmp->right) {
      if (parent) {
        setHeight(parent);
        if (parent->left == tmp)
          parent->left = nullptr;
        else
          parent->right = nullptr;
      }
      if (tmp == root)
        root = nullptr;
      delete tmp;
      tmp = parent;
    }
    else if (!tmp->left) {
      if (parent) {
        if (parent->left == tmp) {
          parent->left = tmp->right;
          setHeight(parent);
        }
        else if (parent->right == tmp) {
          parent->right = tmp->right;
          setHeight(parent);
        }
      }
      Node* tmp2 = tmp->right;
      if (tmp == root)
        root = tmp2;
      delete tmp;
      tmp = tmp2;
    }
    else if (!tmp->right) {
      if (parent) {
        if (parent->left == tmp) {
          parent->left = tmp->left;
          setHeight(parent);
        }
        else if (parent->right == tmp) {
          parent->right = tmp->left;
          setHeight(parent);
        }
      }
      Node* tmp2 = tmp->left;
      if (tmp == root)
        root = tmp2;
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
      setHeight(parent);
      tmp = parent;
    }
  }

  void smallLeftRotate(Node*& tmp) {
    Node* right = tmp->right;
    if (getBalance(right) < 0)
      smallRightRotate(right);
    Node* l = tmp->left;
    Node* m = right->left;
    Node* r = right->right;
    right->left = tmp;
    tmp->right = m;
    if (root == tmp)
      root = right;
    setHeight(tmp);
    setHeight(right);
    tmp = right;
  }

  void smallRightRotate(Node*& tmp) {
    Node* left = tmp->left;
    if (getBalance(left) > 0)
      smallLeftRotate(left);
    Node* l = left->left;
    Node* m = left->right;
    Node* r = tmp->right;
    left->right = tmp;
    tmp->left = m;
    if (root == tmp)
      root = left;
    setHeight(tmp);
    setHeight(left);
    tmp = left;
  }

  void setHeight(Node*& node) {
    long long l = 0, r = 0;
    if (node->left)
      l = node->left->height;
    if (node->right)
      r = node->right->height;
    node->height = l < r ? r + 1 : l + 1;
  }

  long long getBalance(Node*& node) {
    long long l = 0, r = 0;
    if (node->left)
      l = node->left->height;
    if (node->right)
      r = node->right->height;
    return r - l;
  }

  void doBalance() {
    Node* tmp;
    while (!st.isEmpty()) {
      tmp = st.Top();
      setHeight(tmp);
      if (getBalance(tmp) == 2)
        smallLeftRotate(tmp);
      else if (getBalance(tmp) == -2)
        smallRightRotate(tmp);
      st.Pop();
    }
  }

public:
  AVLTree() = default;

  void Insert(T val) {
    st.Clear();
    if (root == nullptr) {
      root = new Node(val);
      size++;
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
      tmp = tmp->left;
    }
    else if (val > tmp->val) {
      tmp->right = new Node(val);
      tmp = tmp->right;
    }
    else
      tmp->val = val;
    st.Push(tmp);
    size++;
    doBalance();
  }

  long long Size() { return size; }

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
    st.Pop();
    if (val != tmp->val)
      return;
    else
      partOfDelete(tmp);
    size--;
    doBalance();
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

  void printTLR(bool typeI = false) {
    if (root == nullptr)
      return;
    TDynamicStack<Node*> stk;
    TDynamicStack<long long> tst;
    TDynamicStack<string> sst;
    stk.Push(root);
    tst.Push(0);
    sst.Push("root: ");
    if (typeI)
      cout << "----- Balance -----\n\n";
    else
      cout << "----- Tree -----\n\n";
    while (!stk.isEmpty()) {
      Node* tmp = stk.Top();
      stk.Pop();
      long long h = tst.Top();
      string str = sst.Top();
      sst.Pop();
      tst.Pop();
      for (long long i = 0; i < h; i++)
        cout << '\t';
      if (typeI)
        cout << str << getBalance(tmp) << endl;
      else
        cout << str << tmp->val << endl;
      if (tmp->right != nullptr) {
        stk.Push(tmp->right);
        tst.Push(h + 1);
        sst.Push("right: ");
      }
      if (tmp->left != nullptr) {
        stk.Push(tmp->left);
        tst.Push(h + 1);
        sst.Push("left: ");
      }
    }
    cout << endl;
  }

  void printTRL(bool typeI = false) {
    if (root == nullptr)
      return;
    TDynamicStack<Node*> stk;
    TDynamicStack<long long> tst;
    TDynamicStack<string> sst;
    stk.Push(root);
    tst.Push(0);
    sst.Push("root: ");
    if (typeI)
      cout << "----- Balance -----\n\n";
    else
      cout << "----- Tree -----\n\n";
    while (!stk.isEmpty()) {
      Node* tmp = stk.Top();
      stk.Pop();
      long long h = tst.Top();
      string str = sst.Top();
      sst.Pop();
      tst.Pop();
      for (long long i = 0; i < h; i++)
        cout << '\t';
      if (typeI)
        cout << str << getBalance(tmp) << endl;
      else
        cout << str << tmp->val << endl;
      if (tmp->left != nullptr) {
        stk.Push(tmp->left);
        tst.Push(h + 1);
        sst.Push("left: ");
      }
      if (tmp->right != nullptr) {
        stk.Push(tmp->right);
        tst.Push(h + 1);
        sst.Push("right: ");
      }
    }
    cout << endl;
  }

  void printLTR() {
    if (root == nullptr)
      return;
    TDynamicStack<Node*> tst;
    Node* tmp = root;
    tst.Push(tmp);
    while (tmp->left) {
      tmp = tmp->left;
      tst.Push(tmp);
    }
    while (!tst.isEmpty()) {
      tmp = tst.Top();
      tst.Pop();
      cout << tmp->val << " ";
      if (tmp->right) {
        tmp = tmp->right;
        tst.Push(tmp);

        while (tmp->left) {
          tmp = tmp->left;
          tst.Push(tmp);
        }
      }
    }
    cout << endl;
  }

  void Clear() {
    st.Clear();
    if (root == nullptr)
      return;
    Node* tmp = root;
    st.Push(tmp);
    while (!st.isEmpty()) {
      tmp = st.Top();
      st.Pop();
      if (tmp->left)
        st.Push(tmp->left);
      if (tmp->right)
        st.Push(tmp->right);
      delete tmp;
      size--;
    }
    root = nullptr;
  }

  ~AVLTree() { Clear(); }
};

template <typename Key, typename Value>
class TableAVL {

  struct KeyType {
    Key key;
    Value val;

    KeyType(Key _key = 0, Value _val = 0) : key(_key), val(_val) {}

    KeyType(const KeyType& other) {
      key = other.key;
      val = other.val;
    }

    KeyType& operator=(const KeyType& other) {
      val = other.val;
      return *this;
    }

    bool operator==(const KeyType& other) const {
      return key == other.key;
    }

    bool operator!=(const KeyType& other) const {
      return key != other.key;
    }

    bool operator<(const KeyType& other) const {
      return key < other.key;
    }

    bool operator>(const KeyType& other) const {
      return key > other.key;
    }

    bool operator<=(const KeyType& other) const {
      return key <= other.key;
    }

    bool operator>=(const KeyType& other) const {
      return key >= other.key;
    }

    friend ostream& operator<<(ostream& out, KeyType& other) {
      out << "{\"" << other.key << "\", " << other.val << "}";
      return out;
    }
  };

  KeyType use_key;

  AVLTree<KeyType> avltree;

public:

  TableAVL() {}

  Value* Find(Key key) {
    use_key.key = key;
    KeyType* tmp = avltree.Find(use_key);
    use_key = *tmp;
    if (tmp)
      return &tmp->val;
    return nullptr;
  }

  void Insert(Key key, Value val = 0) {
    use_key.key = key;
    use_key.val = val;
    avltree.Insert(use_key);
  }

  void Delete(Key key) {
    use_key.key = key;
    avltree.Delete(use_key);
  }

  long long Size() {
    avltree.Size();
  }

  void printTLR() {
    avltree.printTLR();
  }

  void printTRL() {
    avltree.printTRL();
  }

  void printLTR() {
    avltree.printLTR();
  }
};
