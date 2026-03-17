#pragma once
#include <iostream>
#include <stdexcept>
#include <vector>
#include "tstack.h"

using namespace std;

// Base List

template <typename T>
class BaseList {
protected:
  size_t size = 0;
  static void default_func(T& a, T& b) {}
public:
  class Iterator {
  public:
    virtual T& operator*() = 0;
    virtual Iterator& operator++() = 0;
    virtual bool operator!=(const Iterator& other) const = 0;
    virtual ~Iterator() = default;
  };
  virtual Iterator* Begin() = 0;
  virtual Iterator* End() = 0;
  virtual const Iterator* Begin() const = 0;
  virtual const Iterator* End() const = 0;
  size_t Size() { return size; }
  virtual void addFirst(T val) = 0;
  virtual void addLast(T val) = 0;
  virtual void addSorted(T val, int (*comp)(T a, T b), void (*op)(T& a, T& b) = default_func) = 0;
  virtual void addAt(T val, size_t ind) = 0;
  virtual T& getFirst() = 0;
  virtual T& getLast() = 0;
  virtual T& getAt(size_t ind) = 0;
  virtual T delFirst() = 0;
  virtual T delLast() = 0;
  virtual T delAt(size_t ind) = 0;
  virtual void Sort(int (*comp)(T a, T b), void (*op)(T& a, T& b) = default_func) = 0;
  virtual void Merge(BaseList<T>* other) = 0;
  virtual void mergeSorted(BaseList<T>* other, int (*comp)(T a, T b), void (*op)(T& a, T& b) = default_func, bool reverse = false) = 0;
  virtual void Print() = 0;
  virtual ~BaseList() = default;
};

// Standard List

template <typename T>
class TStdList : public BaseList<T> {
  using Iterator = typename BaseList<T>::Iterator;
  struct Node {
    T val;
    Node* next;
  };
  Node* first;
  Node* last;
  class StdIterator : public Iterator {
    Node* curr;
    Node* head;
  public:
    StdIterator(Node* _curr, Node* _head) : curr(_curr), head(_head) {}
    T& operator*() {
      return curr->val;
    }
    Iterator& operator++() {
      curr = curr->next;
      return *this;
    }
    bool operator!=(const Iterator& other) const {
      const StdIterator& otherIt = static_cast<const StdIterator&>(other);
      return curr != otherIt.curr;
    }
  };
public:
  TStdList() {
    first = new Node();
    first->next = first;
    last = first;
  }
  TStdList(const TStdList<T>& other) {
    first = new Node();
    Node* tmp = first;
    tmp->next = first;
    Node* tmp2 = other.first->next;
    while (tmp2 != other.first) {
      tmp->next = new Node();
      tmp = tmp->next;
      tmp->val = tmp2->val;
      tmp->next = first;
      tmp2 = tmp2->next;
      this->size++;
    }
    last = tmp;
  }
  TStdList<T>& operator=(const TStdList<T> other) {
    if (this == &other)
      throw out_of_range("SelfAssign");
    while (this->size != 0) {
      delFirst();
    }
    Node* tmp = first;
    tmp->next = first;
    Node* tmp2 = other.first->next;
    while (tmp2 != other.first) {
      tmp->next = new Node();
      tmp = tmp->next;
      tmp->val = tmp2->val;
      tmp->next = first;
      tmp2 = tmp2->next;
      this->size++;
    }
    last = tmp;
    return *this;
  }
  Iterator* Begin() {
    return new StdIterator(this->first->next, this->last);
  }
  Iterator* End() {
    return new StdIterator(this->first, this->last);
  }
  const Iterator* Begin() const override {
    return new StdIterator(this->first->next, this->first);
  }
  const Iterator* End() const override {
    return new StdIterator(this->first, this->first);
  }
  void addFirst(T val) {
    Node* tmp = new Node();
    tmp->val = val;
    tmp->next = first->next;
    first->next = tmp;
    this->size++;
  }
  void addLast(T val) {
    Node* tmp = new Node();
    last->next = tmp;
    tmp->next = first;
    tmp->val = val;
    last = tmp;
    this->size++;
  }
  void addSorted(T val, int (*comp)(T a, T b), void (*op)(T& a, T& b) = BaseList<T>::default_func) {
    Node* tmp = first->next;
    Node* tmp2 = first;
    int f;
    while (tmp != first) {
      f = comp(tmp->val, val);
      if (f == 0 || f == -1)
        break;
      tmp = tmp->next;
      tmp2 = tmp2->next;
    }
    switch (f) {
    case 1: case 0:
      tmp2->next = new Node();
      tmp2->next->val = val;
      tmp2->next->next = tmp;
      if (tmp2 == last)
        last = tmp2->next;
      this->size++;
      break;
    default:
      op(tmp->val, val);
      break;
    }
  }
  void addAt(T val, size_t ind) {
    if (ind < 0 || ind >= this->size)
      throw out_of_range("Wrong Index");
    Node* tmp = first;
    for (size_t i = 0; i < ind; i++)
      tmp = tmp->next;
    Node* tmp2 = tmp->next;
    tmp->next = new Node();
    tmp->next->val = val;
    tmp->next->next = tmp2;
    this->size++;
  }
  void Print() {}
  T& getFirst() {
    if (this->size == 0)
      throw out_of_range("List is empty");
    return first->next->val;
  }
  T& getLast() {
    if (this->size == 0)
      throw out_of_range("List is empty");
    return this->last->val;
  }
  T& getAt(size_t ind) {
    if (ind < 0 || ind >= this->size)
      throw out_of_range("Wrong Index");
    Node* tmp = first->next;
    for (size_t i = 0; i < ind; i++)
      tmp = tmp->next;
    return tmp->val;
  }
  T delFirst() {
    if (this->size == 0)
      throw out_of_range("List is already empty");
    Node* tmp = first->next;
    T tmpval = tmp->val;
    first->next = tmp->next;
    delete tmp;
    if (this->size == 1)
      last = first;
    this->size--;
    return tmpval;
  }
  T delLast() {
    if (this->size == 0)
      throw out_of_range("List is already empty");
    Node* tmp = first;
    while (tmp->next != last)
      tmp = tmp->next;
    T tmpval = last->val;
    delete last;
    last = tmp;
    tmp->next = first;
    this->size--;
    return tmpval;
  }
  T delAt(size_t ind) {
    if (ind < 0 || ind >= this->size)
      throw out_of_range("Wrong Index");
    Node* tmp = this->first;
    for (size_t i = 0; i < ind; i++)
      tmp = tmp->next;
    Node* tmp2 = tmp->next;
    T tmpval = tmp2->val;
    tmp->next = tmp2->next;
    if (last == tmp2)
      last = tmp;
    delete tmp2;
    this->size--;
    return tmpval;
  }
  void Sort(int (*comp)(T a, T b), void (*op)(T& a, T& b) = BaseList<T>::default_func) {
    TStdList<T> list;
    Node* tmp = first->next;
    while (tmp != first) {
      list.addSorted(tmp->val, comp, op);
      tmp = tmp->next;
    }
    swap(first, list.first);
    swap(last, list.last);
    this->size = list.size;
  }
  void Merge(BaseList<T>* other) {
    if (this == other)
      throw out_of_range("SelfAssign");
    auto* begin = other->Begin();
    auto* end = other->End();
    for (auto* it = begin; *it != *end; ++(*it))
      addLast(**it);
    delete begin;
    delete end;
  }
  void mergeSorted(BaseList<T>* other, int (*comp)(T a, T b), void (*op)(T& a, T& b) = BaseList<T>::default_func, bool reverse = false) {
    if (this == other)
      throw out_of_range("SelfAssign");
    TStdList<T> list;
    auto* begin1 = Begin();
    auto* end1 = End();
    auto* begin2 = other->Begin();
    auto* end2 = other->End();
    auto* it1 = begin1;
    auto* it2 = begin2;
    int f;
    while (*it1 != *end1 && *it2 != *end2) {
      switch (comp(**it1, **it2)) {
      case 1:
        list.addLast(**it1);
        ++(*it1);
        break;
      case 0:
        if (reverse)
          list.addLast(-(**it2));
        else
          list.addLast(**it2);
        ++(*it2);
        break;
      default:
        T tmp = **it1;
        op(tmp, **it2);
        list.addLast(tmp);
        ++(*it1);
        ++(*it2);
        break;
      }
    }
    while (*it1 != *end1) {
      list.addLast(**it1);
      ++(*it1);
    }
    while (*it2 != *end2) {
      if (reverse)
        list.addLast(-(**it2));
      else
        list.addLast(**it2);
      ++(*it2);
    }
    swap(first, list.first);
    swap(last, list.last);
    swap(this->size, list.size);
    delete begin1;
    delete begin2;
    delete end1;
    delete end2;
  }
  ~TStdList() {
    while (this->size != 0)
      delFirst();
    delete first;
  }
};

// Skip List

template <typename T>
class SkipList : public BaseList<T> {
  using BaseList<T>::default_func;
  using Iterator = typename BaseList<T>::Iterator;
  const size_t maxlevel = 4;
  size_t steps[4] = {1, 2, 4, 8};
  struct Node {
    T val;
    size_t level;
    Node** next;
  };
  struct MPair {
    size_t ind;
    Node* next;
  };
  Node* first;
  Node* last;
  vector<MPair> update;
  size_t randomLevel() {
    size_t level = 1;
    double p = 1;
    for (size_t i = 1; i < maxlevel; i++) {
      p = p / 2;
      double tmp = double(rand() % 1000) / 1000;
      if (tmp >= p)
        break;
      level++;
    }
    return level;
  }
  void setLevels(Node* tmp) {
    size_t lvl = 0;
    Node* tmp2 = tmp->next[0];
    for (size_t step = 1; step <= steps[tmp->level - 1]; step++) {
      if (tmp2 == nullptr)
        break;
      if (step == steps[lvl])
        tmp->next[lvl++] = tmp2;
      tmp2 = tmp2->next[0];
    }
  }
  void getUpdate(size_t ind2) {
    if (ind2 > this->size)
      throw out_of_range("Wrong index");
    size_t ind = 0;
    Node* tmp = first;
    update.clear();
    while (ind < ind2) {
      if (tmp == nullptr)
        break;
      for (int i = tmp->level - 1; i >= 0; i--) {
        if (ind + steps[i] >= ind2) {
          MPair mp(static_cast<size_t>(i), tmp);
          update.push_back(mp);
        }
      }
      ind++;
      tmp = tmp->next[0];
    }
  }
  void setUpdate() {
    for (auto& it : update) {
      Node* tmpn = it.next;
      for (size_t i = 0; i < steps[it.ind]; i++) {
        tmpn = tmpn->next[0];
        if (tmpn == nullptr)
          break;
      }
      it.next->next[it.ind] = tmpn;
    }
    update.clear();
  }
public:
  class SkipIterator : public Iterator {
    Node* curr;
    Node* head;
  public:
    SkipIterator(Node* _curr, Node* _head) : curr(_curr), head(_head) {}
    T& operator*() {
      return curr->val;
    }
    Iterator& operator++() {
      curr = curr->next[0];
      return *this;
    }
    bool operator!=(const Iterator& other) const {
      const SkipIterator& otherIt = static_cast<const SkipIterator&>(other);
      return curr != otherIt.curr;
    }
  };
  SkipList() {
    first = nullptr;
    last = nullptr;
  }
  SkipList(const SkipList<T>& other) {
    first = nullptr;
    last = nullptr;
    Node* tmp = first;
    Node* tmp2 = other.first;
    while (tmp2 != nullptr) {
      if (first == nullptr) {
        first = new Node();
        first->val = tmp2->val;
        last = first;
        tmp = first;
        tmp->level = tmp2->level;
        tmp->next = new Node*[tmp->level]{};
        for (size_t i = 0; i < tmp->level; i++)
          tmp->next[i] = nullptr;
      } else {
        tmp->next[0] = new Node();
        tmp = tmp->next[0];
        last = tmp;
        tmp->val = tmp2->val;
        tmp->level = tmp2->level;
        tmp->next = new Node*[tmp->level]{};
        for (size_t i = 0; i < tmp->level; i++)
          tmp->next[i] = nullptr;
      }
      tmp2 = tmp2->next[0];
    }
    tmp = first;
    while (tmp != nullptr) {
      setLevels(tmp);
      tmp = tmp->next[0];
    }
    this->size = other.size;
  }
  SkipList<T>& operator=(const SkipList<T>& other) {
    if (this == &other)
      return out_of_range("SelfAssign");
    while (this->size != 0)
      delFirst();
    last = nullptr;
    Node* tmp = first;
    Node* tmp2 = other.first;
    while (tmp2 != nullptr) {
      if (first == nullptr) {
        first = new Node();
        first->val = tmp2->val;
        last = first;
        tmp = first;
        tmp->level = tmp2->level;
        tmp->next = new Node*[tmp->level];
        for (size_t i = 0; i < tmp->level; i++)
          tmp->next[i] = nullptr;
      } else {
        tmp->next[0] = new Node();
        tmp = tmp->next[0];
        last = tmp;
        tmp->val = tmp2->val;
        tmp->level = tmp2->level;
        tmp->next = new Node*[tmp->level];
        for (size_t i = 0; i < tmp->level; i++)
          tmp->next[i] = nullptr;
      }
      tmp2 = tmp2->next[0];
    }
    tmp = first;
    while (tmp != nullptr) {
      setLevels(tmp);
      tmp = tmp->next[0];
    }
    this->size = other.size;
    return *this;
  }
  Iterator* Begin() {
    return new SkipIterator(first, last->next[0]);
  }
  Iterator* End() {
    return new SkipIterator(last->next[0], last->next[0]);
  }
  const Iterator* Begin() const {
    return new SkipIterator(first, last->next[0]);
  }
  const Iterator* End() const {
    return new SkipIterator(last->next[0], last->next[0]);
  }
  void Print() {
    Node* tmp = first;
    while (tmp != nullptr) {
      cout << "{" << tmp->val << ", (";
      for (int i = 0; i < tmp->level - 1; i++) {
        if (tmp->next[i] == nullptr)
          cout << "nullptr, ";
        else
          cout << tmp->next[i]->val << ", ";
      }
      if (tmp->next[tmp->level - 1] == nullptr)
        cout << "nullptr)} ";
      else
        cout << tmp->next[tmp->level - 1]->val << ")} ";
      tmp = tmp->next[0];
    }
    cout << endl;
  }
  void addFirst(T val) {
    Node* tmp = new Node();
    tmp->val = val;
    tmp->level = randomLevel();
    tmp->next = new Node*[tmp->level];
    tmp->next[0] = first;
    first = tmp;
    if (this->size == 0)
      last = tmp;
    setLevels(tmp);
    this->size++;
  }
  void addLast(T val) {
    addAt(val, this->size);
  }
  void addSorted(T val, int (*comp)(T a, T b), void (*op)(T& a, T& b) = default_func) {
    size_t ind2 = 0;
    Node* tmp = first;
    while (tmp != nullptr && comp(tmp->val, val) == 1) {
      bool f = false;
      for (int i = tmp->level - 1; i >= 0; i--) {
        if (tmp->next[i] != nullptr && comp( tmp->next[i]->val, val) == 1) {
          ind2 += steps[i];
          tmp = tmp->next[i];
          f = true;
          break;
        }
      }
      if (!f) {
        tmp = tmp->next[0];
        ind2++;
      }
    }
    if (tmp != nullptr) {
      switch (comp(tmp->val, val)) {
      case 0: case 1:
        addAt(val, ind2);
        break;
      default:
        op(tmp->val, val);
        break;
      }
    } else
      addLast(val);
  }
  void addAt(T val, size_t ind) {
    if (ind < 0 || ind > this->size)
      throw out_of_range("Index out of range");
    Node* tmp = new Node();
    tmp->level = randomLevel();
    tmp->val = val;
    tmp->next = new Node*[tmp->level];
    if (ind != 0) {
      getUpdate(ind);
      if (update.back().next->next[0] != nullptr)
        tmp->next[0] = update.back().next->next[0];
      else
        tmp->next[0] = nullptr;
      update.back().next->next[0] = tmp;
      update.pop_back();
      setUpdate();
    }
    else {
      tmp->next[0] = first;
      first = tmp;
    }
    if (ind >= this->size - 1) {
      for (size_t i = 1; i < tmp->level; i++)
        tmp->next[i] = nullptr;
    } else
      setLevels(tmp);
    if (ind == this->size)
      last = tmp;
    if (this->size == 0)
      first = tmp;
    this->size++;
    update.clear();
  }
  T& getFirst() {
    if (first == nullptr)
      throw out_of_range("List is empty");
    return first->val;
  }
  T& getLast() {
    if (last == nullptr)
      throw out_of_range("List is empty");
    return last->val;
  }
  T& getAt(size_t ind) {
    if (ind >= this->size)
      throw out_of_range("Index out of range");
    size_t ind2 = 0;
    Node* tmp = first;
    while (ind2 != ind) {
      for (int i = tmp->level - 1; i >= 0; i--) {
        if (ind2 + steps[i] <= ind) {
          ind2 += steps[i];
          tmp = tmp->next[i];
          break;
        }
      }
    }
    return tmp->val;
  }
  T delFirst() {
    if (this->size == 0)
      throw out_of_range("List is already empty");
    T tmp = first->val;
    Node* tmp2 = first->next[0];
    delete[] first->next;
    delete first;
    first = tmp2;
    if (this->size == 1)
      last = nullptr;
    this->size--;
    return tmp;
  }
  T delLast() {
    return delAt(this->size - 1);
  }
  T delAt(size_t ind) {
    if (ind < 0 || ind >= this->size)
      throw out_of_range("Index out of range");
    Node* tmp;
    T val;
    if (ind != 0) {
      getUpdate(ind);
      tmp = update.back().next->next[0];
      update.back().next->next[0] = tmp->next[0];
      val = tmp->val;
      delete[] tmp->next;
      delete tmp;
      if (ind == this->size - 1) {
        last = update.back().next;
      }
      update.pop_back();
      setUpdate();
    } else {
      tmp = first;
      first = first->next[0];
      val = tmp->val;
      delete[] tmp->next;
      delete tmp;
      if (this->size == 1) {
        first = nullptr;
        last = nullptr;
      }
    }
    this->size--;
    return val;
  }
  void Sort(int (*comp)(T a, T b), void (*op)(T& a, T& b) = default_func) {
    SkipList<T> list;
    Node* tmp = first;
    while (tmp != nullptr) {
      list.addSorted(tmp->val, comp, op);
      tmp = tmp->next[0];
    }
    swap(first, list.first);
    swap(last, list.last);
    this->size = list.size;
  }
  void Merge(BaseList<T>* other) {
    if (this == other)
      throw out_of_range("SelfAssign");
    auto* begin = other->Begin();
    auto* end = other->End();
    SkipList<T> list;
    TDynamicStack<T> st;
    Node* tmp = first;
    while (tmp != nullptr) {
      st.Push(tmp->val);
    }
    for (auto* it = begin; *it != *end; ++(*it))
      st.Push(**it);
    delete begin;
    delete end;
    while (!st.isEmpty()) {
      list.addFirst(st.Top());
      st.Pop();
    }
    swap(first, list.first);
    swap(last, list.last);
    this->size = list.size;
  }
  void mergeSorted(BaseList<T>* other, int (*comp)(T a, T b), void (*op)(T& a, T& b) = default_func, bool reverse = false) {
    if (this == other)
      throw out_of_range("SelfAssign");
    SkipList<T> list;
    TDynamicStack<T> st;
    auto* begin1 = Begin();
    auto* end1 = End();
    auto* begin2 = other->Begin();
    auto* end2 = other->End();
    auto* it1 = begin1;
    auto* it2 = begin2;
    int f;
    while (*it1 != *end1 && *it2 != *end2) {
      switch (comp(**it1, **it2)) {
      case 1:
        list.addLast(**it1);
        ++(*it1);
        break;
      case 0:
        if (reverse)
          st.Push(-(**it2));
        else
          st.Push(**it2);
        ++(*it2);
        break;
      default:
        T tmp = **it1;
        op(tmp, **it2);
        st.Push(tmp);
        ++(*it1);
        ++(*it2);
        break;
      }
    }
    while (*it1 != *end1) {
      st.Push(**it1);
      ++(*it1);
    }
    while (*it2 != *end2) {
      if (reverse)
        st.Push(-(**it2));
      else
        st.Push(**it2);
      ++(*it2);
    }
    while (!st.isEmpty()) {
      list.addFirst(st.Top());
      st.Pop();
    }
    swap(first, list.first);
    swap(last, list.last);
    swap(this->size, list.size);
    delete begin1;
    delete begin2;
    delete end1;
    delete end2;
  }
  ~SkipList() {
    while (this->size != 0)
      delFirst();
  }
};
