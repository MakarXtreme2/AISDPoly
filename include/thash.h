#pragma once
#include <vector>
#include <functional>
#include <utility>
#include "tset.h"

using namespace std;

template <typename Key, typename Value>
class HashTable {
  struct HashPair {
    Key key;
    Value val;
    HashPair() = default;
    HashPair(Key _key, Value _val) : key(_key), val(_val) {}
  };

  vector<HashPair> data;
  TSet used;
  int size;
  size_t A = 1140671485;
  size_t B = 764789433;
  int maxTry = 5;

  vector<pair<int, int>> hash_num_pairs = {
    {15485863, 32452843},
    {1009, 1013},
    {987, 1597},
    {1140671485, 764789433}
  };
  int pair_index = 0;
  bool resize_no_work = false;

  size_t hashFunction(int p) {
    return (A + B * p) % size;
  }

  int findIndex(Key key) {
    hash<Key> hasher;
    int p = hasher(key);
    for (int i = 0; i < maxTry; i++) {
      p = hashFunction(p);
      if (used.IsMember(p)) {
        if (data[p].key == key)
          return p;
      }
    }
    return -1;
  }

  void changeCoeff() {
    pair_index++;
    if (pair_index >= hash_num_pairs.size()) {
      pair_index = 0;
    }

    A = hash_num_pairs[pair_index].first;
    B = hash_num_pairs[pair_index].second;

    vector<HashPair> oldData = data;
    TSet oldUsed = used;

    used = TSet(size);
    data.clear();
    data.resize(size);

    for (int i = 0; i < size; i++) {
      if (oldUsed.IsMember(i)) {
        Insert(oldData[i].key, oldData[i].val);
      }
    }
  }


public:
  HashTable(int _size) : used(_size), data(_size), size(_size) {}


  Value* Find(Key key) {
    int idx = findIndex(key);
    if (idx != -1)
      return &data[idx].val;
    return nullptr;
  }

  void Resize() {
    HashTable new_table(size * 2);
    for (int i = 0; i < size; i++) {
      if (used.IsMember(i)) {
        new_table.Insert(data[i].key, data[i].val);
      }
    }
    data = new_table.data;
    size = size * 2;
    used = new_table.used;
  }

  void Insert(Key key, Value val) {
    int pos = findIndex(key);
    if (pos != -1) {
      data[pos].val = val;
      return;
    }
    hash<Key> hasher;
    int p = hasher(key);
    for (int i = 0; i < maxTry; i++) {
      p = hashFunction(p);
      if (!used.IsMember(p)) {
        data[p] = {key, val};
        used.InsElem(p);
        resize_no_work = false;
        return;
      }
    }
    if (resize_no_work) {
      changeCoeff();
      resize_no_work = true;
      Insert(key, val);
    } else {
      Resize();
      resize_no_work = true;
      Insert(key, val);
    }
    resize_no_work = false;
  }

  void Delete(Key key) {
    int pos = findIndex(key);
    if (pos != -1)
      used.DelElem(pos);
  }
};
