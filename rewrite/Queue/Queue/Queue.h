#include <iostream>

template <typename T> class Queue {
public:
  Queue();
  Queue(const Queue &another);
  ~Queue();
  bool empty();
  int size();
  T front() const;
  T back() const;
  void push(T element);
  void pop();
  void swap(Queue &queue);

private:
  struct Node {
    T element;
    Node *next;
    Node(T ele, Node *n = NULL) {
      element = ele;
      next = n;
    }
  };
  Node *front_node;
  Node *back_node;
  int node_num;
};

template <typename T> Queue<T>::Queue() {
  front_node = NULL;
  back_node = NULL;
  node_num = 0;
}

template <typename T> Queue<T>::Queue(const Queue &another) {
  back_node = NULL;
  front_node = NULL;
  node_num = 0;
  Node *p = another.front_node;
  while (node_num != another.node_num) {
    push(p->element);
    p = p->next;
  }
}

template <typename T> Queue<T>::~Queue() {
  if (front_node != NULL) {
    Node* p = front_node;
    while (p) {
      Node* tmp = p;
      p = p->next;
      delete tmp;
    }
  }
}

template <typename T> bool Queue<T>::empty() { return (node_num <= 0); }

template <typename T> int Queue<T>::size() { return (node_num); }

template <typename T> T Queue<T>::front() const {
  return (front_node->element);
}

template <typename T> T Queue<T>::back() const { return (back_node->element); }

template <typename T> void Queue<T>::push(T element) {
  Node *p = new Node(element, NULL);
  if (back_node == NULL) {
    front_node = p;
    back_node = p;
  } else {
    back_node->next = p;
    back_node = p;
  }
  node_num++;
}

template <typename T> void Queue<T>::pop() {
  if (empty())
    return;
  Node *p = front_node;
  front_node = front_node->next;
  delete p;
  node_num--;
  if (empty()) {
    front_node = NULL;
    back_node = NULL;
  }
}

template <typename T> void Queue<T>::swap(Queue &queue) {
  int t1;
  Node *p1, *p2;
  t1 = this->node_num;
  this->node_num = queue.node_num;
  queue.node_num = t1;

  p1 = this->front_node;
  this->front_node = queue.front_node;
  queue.front_node = p1;

  p2 = this->back_node;
  this->back_node = queue.back_node;
  queue.back_node = p2;
}
