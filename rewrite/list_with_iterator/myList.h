#include <iostream>
using namespace std;

template <class T>
class list {
 public:
  struct listelem {
    T data;
    listelem *next, *prev;
    // suggest u to use the constructor
    listelem(const T &s, listelem *n = NULL, listelem *p = NULL)
        : data(s), next(n), prev(p) {}
  };
  class iterator {
   public:
    // suggest you to use friend class
    friend class list;
    explicit iterator(listelem *p = 0) { this->ptr = p; }
    iterator(const iterator &other) { this->ptr = other.ptr; }
    // ++it
    iterator &operator++() {
      ptr = ptr->next;
      return (*this);
    }
    iterator &operator--() {
      ptr = ptr->prev;
      return (*this);
    }
    // it++
    iterator operator++(int x) {
      iterator temp;
      temp.ptr = ptr;
      ptr = ptr->next;
      return temp;
    }
    iterator operator--(int x) {
      iterator temp;
      temp.ptr = ptr;
      ptr = ptr->prev;
      return temp;
    }
    // what the hell?
    listelem *operator->() { return ptr; }
    // what the hell?
    T &operator*() { return ptr->data; }
    // To use iterator as `listelem*` sometimes
    operator listelem *() { return ptr; }

   private:
    // current listelem or 0;
    listelem *ptr;
  };

  friend class iterator;
  list() { h.ptr = t.ptr = NULL; }
  list(size_t n_elements, const T &c) {
    h.ptr = t.ptr = NULL;
    while (n_elements--) this->push_back(c);
  }
  list(const list &x) {
    h.ptr = t.ptr = NULL;
    iterator it = x.h;
    while (it != x.end()) {
      push_back(*it++);
    }
  }
  list(iterator b, iterator e) {
    h.ptr = t.ptr = NULL;
    iterator it = b;
    while (it != e) {
      push_back(*it++);
    }
  }
  ~list() { this->clear(); }
  // head
  iterator begin() const { return h; }
  // tail's next position(if tail is not nullptr)
  iterator end() const {
    iterator temp = t;
    if (temp.ptr != NULL) temp++;
    return temp;
  }
  size_t size() {
    if (h.ptr == NULL) return 0;
    iterator temp = h;
    size_t size = 1;
    while (temp != t) {
      temp++;
      size++;
    }
    return size;
  }
  void push_front(const T &c) {
    if (h.ptr == NULL) {
      h.ptr = new listelem(c);
      t = h;
    } else {
      iterator temp;
      temp.ptr = new listelem(c, h, NULL);
      h.ptr->prev = temp.ptr;
      h = temp;
    }
  }
  void push_back(const T &c) {
    if (t.ptr == NULL) {
      t.ptr = new listelem(c);
      h = t;
    } else {
      iterator temp;
      temp.ptr = new listelem(c, NULL, t);
      t.ptr->next = temp.ptr;
      t = temp;
    }
  }
  void pop_front() {
    if (h.ptr != NULL) {
      iterator temp;
      temp.ptr = h.ptr->next;
      delete h.ptr;
      h.ptr = NULL;
      h = temp;
      if (h.ptr == NULL)
        t.ptr = h.ptr;
      else
        h.ptr->prev = NULL;
    }
  }
  void pop_back() {
    iterator temp;
    temp.ptr = t.ptr->prev;
    delete t.ptr;
    t.ptr = NULL;
    t = temp;
    if (t.ptr == NULL)
      h.ptr = t.ptr;
    else
      t.ptr->next = NULL;
  }
  iterator insert(iterator &position, const T &val) {
    if (position == end()) {
      push_back(val);
      position.ptr = t.ptr->next;
      return t;
    } else if (position == begin()) {
      iterator it = h;
      push_front(val);
      return h;
    } else {
      iterator temp;
      iterator p;
      p.ptr = position.ptr->prev;
      temp.ptr = new listelem(val);
      temp.ptr->next = position.ptr;
      temp.ptr->prev = position.ptr->prev;
      p.ptr->next = temp;
      position.ptr->prev = temp;
      return (position);
    }
  }
  iterator erase(iterator &position) {
    if (position == h) {
      iterator temp;
      temp.ptr = h.ptr;
      h++;
      delete temp.ptr;
      temp.ptr = NULL;
      return h;
    } else if (position == t) {
      iterator temp;
      temp.ptr = t.ptr;
      t--;
      delete temp.ptr;
      temp.ptr = NULL;
      return t;
    } else {
      iterator temp;
      iterator it;
      it.ptr = position.ptr->next;
      temp.ptr = position.ptr->prev;
      temp.ptr->next = position.ptr->next;
      position.ptr->next->prev = temp.ptr;
      delete position;
      position.ptr = NULL;
      return it;
    }
  }
  bool empty() const { return (h.ptr == NULL); }
  T &front() { return h->data; }
  T &back() { return t->data; }
  void clear() {
    while (!empty()) {
      pop_back();
    }
    h.ptr = t.ptr = NULL;
  }
  friend ostream &operator<<(ostream &out, const list &x) {
    iterator it = x.begin();
    out << "[ ";
    while (it != x.end()) {
      out << it->data << " ";
      it++;
    }
    out << "]";
    return out;
  }

 private:
  // head and tail
  iterator h, t;
};

/**************standatd answer****************/
/*#ifndef _MY_LIST_H_
#define _MY_LIST_H_

#include <iostream>
#include <cassert>
#include <string>
using namespace std;


template<class T>
class list {
 public:
  struct listelem;  // forward declarations
  class iterator;

  list() {
    h.ptr = t.ptr = 0;
  }  // construct the empty list
  list(size_t n_elements, const T& c) {
    h.ptr = t.ptr = 0;
    for ( size_t i = 0; i < n_elements; ++i )
      push_front(c);
  }

  list(const list& x) {
    list::iterator r = x.begin();
    h.ptr = t.ptr = 0;
    while (r != 0) {
      push_back(*r++);
    }
  }

  list(iterator b, iterator e)  {
    h.ptr = t.ptr = 0;
    while (b != e) {
      push_back(*b++);
    }
  }

  ~list() { clear(); }

  iterator begin() const { return h; }

  iterator end() const {
    iterator temp = t;
    return (temp.ptr == 0 ? iterator(0) : ++temp);
  }

  size_t size() {
    size_t count = 0;
    iterator temp = h;
    while (temp != end()) {
      temp++;
      count++;
    }
    return count;
  }

  void push_front(const T& c) {
    listelem* temp = new listelem(c, h, 0);
    if (h.ptr != 0) {  // was a nonempty list
      h->prev = temp;
      h.ptr = temp;
    } else {  // was an empty list
      h.ptr = t.ptr = temp;
    }
  }

  void push_back(const T& c) {
    listelem* temp = new listelem(c, 0, t);
    if (t != 0) {  // was a nonempty list
      t->next = temp;
      t.ptr = temp;
    } else {  // was an empty list
      h.ptr = t.ptr = temp;
    }
  }

  void pop_front() {
    listelem* temp = h.ptr;
    if (h.ptr != 0) {
      if (h.ptr == t.ptr) t.ptr = 0;
      h.ptr = h->next;
      if (h.ptr != 0) h->prev = 0;
      delete temp;
    }
  }

  void pop_back() {
    listelem* temp = t.ptr;
    if (t.ptr != 0) {
      if (h.ptr == t.ptr) h.ptr = 0;
      t.ptr = t->prev;
      if (t.ptr != 0) t->next = 0;
      delete temp;
    }
  }

  iterator insert(iterator position, const T& val) {
    listelem* p = position.ptr;
    if (p == 0) {
      // p == end()
      push_back(val);
      return t;
    } else if (p->prev == 0) {
      // p == begin()
      push_front(val);
      return begin();
    } else {
      listelem* temp = new listelem(val, p, p->prev);
      p->prev = temp;
      temp->prev->next = temp;
      return iterator(temp);
    }
  }

  iterator erase(iterator position) {
    listelem* temp = position.ptr;
    if (temp == h) {
      // temp == begin();
      pop_front();
      return h;
    }
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    listelem* re = temp->next;
    delete temp;
    return iterator(re);
  }

  bool empty() const { return h.ptr == 0; }

  T& front() { return *h; }
  T& back() { return *t; }

  void clear() {
    while (h.ptr != 0) {
      pop_front();
    }
    h.ptr = t.ptr = 0;
  }

  friend ostream& operator << (ostream& out, const list& x)  {
    list<T>::iterator p = x.begin();
    out << "[ ";
    while (p != 0) {
      out << *p << " ";
      ++p;  // advances iterator using next
    }
    out << "]";
    return out;
  }

  struct listelem {      // list cell
    T data;
    listelem *next, *prev;
    listelem(const T& s, listelem* n, listelem* p) \
      : data(s), next(n), prev(p) {}
  };
  // scopeed within class list



 private:
  iterator h, t;  // head and tail
};

#endif  // _MY_LIST_H_
*********************************************************
*/