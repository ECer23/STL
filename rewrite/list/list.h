#ifndef LIST_H_
#define LIST_H_

#include <iostream>
#include <memory>
using namespace std;

template <class T, class Alloc = allocator<T>>
class list {
 public:
  /****************************** Definitions *******************************/
  struct listnode {
    listnode *next;
    listnode *prev;
    T data;
  };

  typedef listnode list_node;
  typedef listnode *link_type;
  typedef allocator<list_node> list_node_allocator;

  typedef T value_type;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef value_type &reference;
  typedef const value_type &const_reference;

  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  link_type node;

  struct iterator {
    link_type ptr;
    explicit iterator(listnode *p) : ptr(p) {}
    iterator() : ptr(0) {}
    iterator &operator++() {
      ptr = ptr->next;
      return *this;
    }
    iterator &operator--() {
      ptr = ptr->prev;
      return *this;
    }
    iterator operator++(int) {
      iterator temp = *this;
      ptr = ptr->next;
      return temp;
    }
    iterator operator--(int) {
      iterator temp = *this;
      ptr = ptr->prev;
      return temp;
    }
    listnode *operator->() { return ptr; }
    T &operator*() { return ptr->data; }
    operator listnode *() { return ptr; }  // conversion
  };

 public:
  /**************************** Member functions *****************************/

  list() { empty_initialize(); }
  list(iterator first, iterator last) {
    empty_initialize();
    while (first != last) {
      this->push_back(*(first++));
    }
  }
  list(list<T> &l) {
    empty_initialize();
    *this = l;
  }
  list(int n, value_type value) {
    empty_initialize();
    while (n--) {
      this->push_back(value);
    }
  }

  ~list() {
    this->clear();
    destroy_node(node);
    node = nullptr;
  }

  list &operator=(list<T> &l) {
    iterator first = l.begin();
    iterator last = l.end();
    while (first != last) {
      this->push_back(*(first++));
    }
    return *this;
  }

  friend ostream &operator<<(ostream &out, list<T> &l) {
    iterator first = l.begin();
    iterator last = l.end();
    cout << "[ ";
    while (first != last) {
      out << *first << " ";
      first++;
    }
    cout << "]";
    return out;
  }

  /******************************* Iterators ********************************/

  iterator begin() { return (iterator)node->next; }
  iterator end() { return (iterator)node; }

  /******************************** Capacity ********************************/

  bool empty() { return node->next == node; }
  size_type size() { return (size_type)distance(node->next, node); }

  /****************************** Element access *****************************/

  reference front() { return begin()->data; }
  reference back() { return end()->prev->data; }

  /******************************** Modifiers ********************************/

  /* Insert element at beginning */
  void push_front(const T &x) { insert(begin(), x); }
  /* Add element at the end */
  void push_back(const T &x) { insert(end(), x); }
  /* Delete first element */
  void pop_front() { erase(begin()); }
  /* Delete last element */
  void pop_back() { erase(end()->prev); }
  /* Insert elements through pointer */
  iterator insert(iterator position, const T &x) {
    link_type p = creat_node(x);
    p->next = position.ptr;
    p->prev = position->prev;
    position->prev = p;
    p->prev->next = p;
    return (iterator)p;
  }
  /* Erase elements */
  iterator erase(iterator position) {
    link_type next_node = position->next;
    link_type prev_node = position->prev;
    prev_node->next = position->next;
    next_node->prev = position->prev;
    destroy_node(position);
    return (iterator)next_node;
  }
  iterator erase(link_type position) { return erase((iterator)position); }
  /* Clear content */
  void clear() {
    link_type first = begin();
    link_type last = end();
    while (first != last) {
      link_type next = first->next;
      destroy_node(first);
      first = next;
    }
    node->next = node;
    node->prev = node;
  }

  /******************************* Operations ********************************/

  /* Remove elements with specific value  */
  void remove(const T &value) {
    link_type first = begin();
    link_type last = end();
    while (first != last) {
      if (first->data == value) {
        first = erase(first);
      } else {
        first = first->next;
      }
    }
  }
  /* Remove duplicate values  */
  void unique() {
    link_type first = begin();
    link_type last = end();
    value_type value = first->data;
    while (first->next != last) {
      link_type next = first->next;
      if (value == next->data) {
        first = erase(next);
      } else {
        value = next->data;
        first = next;
      }
    }
  }

  /* Sort elements in container */
  void sort() {
    for (iterator i = begin(); i != end(); i++) {
    }
  }

  /* Transfer elements from list to list  */
  /* Transfer a list */
  void splice(iterator position, list<T> &l) {
    if (!l.empty()) transfer(position, l.begin(), l.end());
  }
  /* Transfer an element on i */
  void splice(iterator position, list<T> &, iterator i) {
    iterator j = i;
    j++;
    if (position == i || position == j)
      return;
    else
      transfer(position, i, j);
  }
  /* Transfer some elements in a list from i to j */
  void splice(iterator position, list<T> &, iterator first, iterator last) {
    if (first != last) {
      transfer(position, first, last);
    }
  }

 protected:
  /****************************** Memory access *****************************/

  /* Allocate one node and return */
  link_type get_node() { return alloc.allocate(1); }
  /* Creat(allocate and construct) a node with value and return */
  link_type creat_node(const T &x) {
    link_type p = get_node();
    alloc.construct(&p->data, x);
    return p;
  }
  /* Deallocate a node */
  void put_node(link_type p) { alloc.deallocate(p, 1); }
  /* Delete(destroy and deallocate) a node */
  void destroy_node(link_type p) {
    alloc.destroy(p);
    put_node(p);
  }

  /****************************** Tools *****************************/

  /* Return the distance between two list node pointer */
  difference_type distance(link_type begin, link_type end) {
    difference_type n = 0;
    while (begin != end) {
      begin = begin->next;
      n++;
    }
    return n;
  }
  /* Transfer elements in [first,last) to position */
  void transfer(iterator position, iterator first, iterator last) {
    if (position != last) {
      link_type tmp = last->prev;
      first->prev->next = last;
      last->prev = first->prev;

      first->prev = position->prev;
      tmp->next = position;
      first->prev->next = first;
      position->prev = tmp;
    }
  }
  /* Creat an empty list with an empty node */
  void empty_initialize() {
    node = get_node();
    node->next = node;
    node->prev = node;
  }
  list_node_allocator alloc;
};

#endif