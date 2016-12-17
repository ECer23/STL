#ifndef _H_
#define _H_

#include <iostream>
using namespace std;

template <class T>
class AVLtree{
 public:
  struct Node {
    T value;
    int height;
    Node *left, *right;
    void output();
    int getHeight() { return height; }
    Node(T v = 0, int h = 0, Node* l = nullptr, Node* r = nullptr)
        : value(v), height(h), left(l), right(r) {}
  };

 public:
  AVLtree() { root = nullptr; }
  ~AVLtree() { this->clear(); }
  Node* getRoot() { return root; }
  void insert(const T value) const;
  void erase(T value);
  void clear();
  void output();

 private:
  Node* root;
  Node* leftLeftRotation(Node* k2);
  Node* rightRightRotation(Node* k1);
  Node* leftRightRotation(Node* k3);
  Node* rightLeftRotation(Node* k1);
};

#endif  // !_H_