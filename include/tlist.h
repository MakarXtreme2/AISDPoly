#pragma once
#include <iostream>
#include <stdexcept>

using namespace std;

// Base List

template <typename T>
class BaseList {
protected:
  struct Node {
    T val;
    Node* next;
  };
  Node* first;
  Node* last;
  size_t size = 0;
public:
  class Iterator {
  public:
    virtual T& operator*() = 0;
    virtual Iterator& operator++() = 0;
    virtual bool operator!=(const Iterator& other) const = 0;
    virtual Iterator* Clone() const = 0;
    virtual ~Iterator() = default;
  };
  virtual Iterator* Begin() = 0;
  virtual Iterator* End() = 0;
  virtual const Iterator* Begin() const = 0;
  virtual const Iterator* End() const = 0;
  size_t Size() { return size; }
  virtual void addFirst(T val) = 0;
  virtual void addLast(T val) = 0;
  virtual void addSorted(T val, bool (*comp)(T a, T b)) = 0;
  virtual void addAt(T val, size_t ind) = 0;
  virtual T& getFirst() = 0;
  virtual T& getLast() = 0;
  virtual T& getAt(size_t ind) = 0;
  virtual T delFirst() = 0;
  virtual T delLast() = 0;
  virtual T delAt(size_t ind) = 0;
  virtual void Sort(bool (*comp)(T a, T b)) = 0;
  virtual void Merge(BaseList<T>*& other) = 0;
  virtual void mergeSorted(BaseList<T>*& other, bool (*comp)(T a, T b)) = 0;
  virtual ~BaseList() = default;
};

// Standard List

template <typename T>
class TStdList : public BaseList<T> {
  using Node = typename BaseList<T>::Node;
  using Iterator = typename BaseList<T>::Iterator;
  class StdIterator : public Iterator {
    Node* curr;
    Node* head;
  public:
    StdIterator(Node* _curr, Node* _head) : curr(_curr), head(_head) {}
    Iterator* Clone() const {
      return new StdIterator(curr, head);
    }
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
    this->first = new Node();
    this->first->next = this->first;
    this->last = this->first;
  }
  TStdList(const TStdList<T>& other) {
    this->first = new Node();
    Node* tmp = this->first;
    tmp->next = this->first;
    Node* tmp2 = other->first->next;
    while (tmp2 != other->first) {
      tmp->next = new Node();
      tmp = tmp->next;
      tmp->val = tmp2->val;
      tmp->next = this->first;
      tmp2 = tmp2->next;
      this->size++;
    }
    this->last = tmp;
  }
  TStdList<T>& operator=(const TStdList<T> other) {
    if (this == &other)
      throw out_of_range("SelfAssign");
    while (this->size != 0) {
      delFirst();
    }
    Node* tmp = this->first;
    tmp->next = this->first;
    Node* tmp2 = other->first->next;
    while (tmp2 != other->first) {
      tmp->next = new Node();
      tmp = tmp->next;
      tmp->val = tmp2->val;
      tmp->next = this->first;
      tmp2 = tmp2->next;
      this->size++;
    }
    this->last = tmp;
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
    tmp->next = this->first->next;
    this->first->next = tmp;
    this->size++;
  }
  void addLast(T val) {
    Node* tmp = new Node();
    this->last->next = tmp;
    tmp->next = this->first;
    tmp->val = val;
    this->last = tmp;
    this->size++;
  }
  void addSorted(T val, bool (*comp)(T a, T b)) {
    Node* tmp = this->first->next;
    Node* tmp2 = this->first;
    while (tmp != this->first) {
      if (comp(val, tmp->val))
        break;
      tmp = tmp->next;
      tmp2 = tmp2->next;
    }
    tmp2->next = new Node();
    tmp2->next->val = val;
    tmp2->next->next = tmp;
    if (tmp2 == this->last)
      this->last = tmp2->next;
    this->size++;
  }
  void addAt(T val, size_t ind) {
    if (ind < 0 || ind >= this->size)
      throw out_of_range("Wrong Index");
    Node* tmp = this->first;
    for (size_t i = 0; i < ind; i++)
      tmp = tmp->next;
    Node* tmp2 = tmp->next;
    tmp->next = new Node();
    tmp->next->val = val;
    tmp->next->next = tmp2;
    this->size++;
  }
  T& getFirst() {
    if (this->size == 0)
      throw out_of_range("List is empty");
    return this->first->next->val;
  }
  T& getLast() {
    if (this->size == 0)
      throw out_of_range("List is empty");
    return this->last->val;
  }
  T& getAt(size_t ind) {
    if (ind < 0 || ind >= this->size)
      throw out_of_range("Wrong Index");
    Node* tmp = this->first->next;
    for (size_t i = 0; i < ind; i++)
      tmp = tmp->next;
    return tmp->val;
  }
  T delFirst() {
    if (this->size == 0)
      throw out_of_range("List is already empty");
    Node* tmp = this->first->next;
    T tmpval = tmp->val;
    this->first->next = tmp->next;
    delete tmp;
    if (this->size == 1)
      this->last = this->first;
    this->size--;
    return tmpval;
  }
  T delLast() {
    if (this->size == 0)
      throw out_of_range("List is already empty");
    Node* tmp = this->first;
    while (tmp->next != this->last)
      tmp = tmp->next;
    T tmpval = this->last->val;
    delete this->last;
    this->last = tmp;
    tmp->next = this->first;
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
    if (this->last == tmp2)
      this->last = tmp;
    delete tmp2;
    this->size--;
    return tmpval;
  }
  void Sort(bool (*comp)(T a, T b)) {
    TStdList<T> list;
    Node* tmp = this->first->next;
    while (tmp != this->first) {
      list.addSorted(tmp->val, comp);
      tmp = tmp->next;
    }
    swap(this->first, list.first);
    swap(this->last, list.last);
    this->size = list.size;
  }
  void Merge(BaseList<T>*& other) {
    if (this == other)
      throw out_of_range("SelfAssign");
    auto* begin = other->Begin();
    auto* end = other->End();
    for (auto* it = begin; *it != *end; ++(*it))
      addLast(**it);
    delete begin;
    delete end;
  }
  void mergeSorted(BaseList<T>*& other, bool (*comp)(T a, T b)) {
    if (this == other)
      throw out_of_range("SelfAssign");
    auto* begin = other->Begin();
    auto* end = other->End();
    for (auto* it = begin; *it != *end; ++(*it))
      addSorted(**it, comp);
    delete begin;
    delete end;
  }
  ~TStdList() {
    while (this->size != 0)
      delFirst();
    delete this->first;
  }
};
