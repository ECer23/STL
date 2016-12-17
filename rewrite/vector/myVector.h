#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <memory>
#include <exception>
#include <initializer_list>
using namespace std;

template <class T, class Alloc = allocator<T>>
class myVector {
 public:
  typedef T value_type;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef value_type *iterator;
  typedef value_type &reference;
  typedef const value_type &const_reference;

  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

 public:
  // (constructor)
  myVector() { elements = first_free = cap = nullptr; }  // default construct
  myVector(const myVector &rhs) noexcept {               // copy construct
    auto data = alloc_n_copy(rhs.begin(), rhs.end());
    elements = data.first;
    first_free = cap = data.second;
  }
  myVector(myVector &&s) noexcept : elements(s.elements),
                                    first_free(s.first_free),
                                    cap(s.cap) {  // move construct
    s.elements = s.first_free = s.cap = nullptr;
  }
  myVector(initializer_list<value_type> il) {  // initializer_list construct
    elements = alloc.allocate(il.size());
    first_free = elements;
    for (auto e : il) {
      alloc.construct(first_free++, std::move(e));
    }
    cap = first_free;
  }
  myVector(size_type n, const value_type &val = 0) {  // fill construct
    elements = alloc.allocate(n);
    first_free = elements;
    for (auto i = 0; i < n; i++) alloc.construct(first_free++, val);
    cap = first_free;
  }

  // (destructor)
  ~myVector() { free(); }

  // capacity
  size_type size() const { return size_type(first_free - elements); }
  void resize(size_type n, value_type val = value_type()) {
    if (size() < n) {
      if (capacity() < n) reserve(n);
      auto t = n - size();
      while (t--) alloc.construct(first_free++, val);
    } else {
      throw length_error("bad resize");
    }
  }
  size_type capacity() const { return size_t(cap - elements); }
  void reserve(size_type n) {
    if (capacity() < n) {
      auto newcapacity = n;
      alloc_n_move(newcapacity);
    } else {
      throw length_error("bad reserve");
    }
  }

  // iterator
  iterator begin() const { return elements; }
  iterator end() const { return first_free; }

  // Assign content
  myVector &operator=(const myVector &rhs) {
    if (this != &rhs) {
      auto data = alloc_n_copy(rhs.begin(), rhs.end());
      free();
      elements = data.first;
      first_free = cap = data.second;
    }
    return *this;
  }
  myVector &operator=(myVector &&rhs) noexcept {
    if (this != &rhs) {
      free();
      elements = rhs.elements;
      first_free = rhs.first_free;
      cap = rhs.cap;
      rhs.elements = rhs.first_free = rhs.cap = nullptr;
    }
    return *this;
  }

  // Element access
  reference operator[](size_type n) {
    if (n >= size()) throw out_of_range("bad access");
    return elements[n];
  }
  const_reference operator[](size_type n) const {
    if (n >= size()) throw out_of_range("bad access");
    return elements[n];
  }
  reference at(size_type n) { return elements[n]; }
  const_reference at(size_type n) const { return elements[n]; }
  reference front() { return at(0); }
  const_reference front() const { return at(0); }
  reference back() { return at(size() - 1); }
  const_reference back() const { return at(size() - 1); }
  pointer data() noexcept { return elements; }
  const pointer data() const noexcept { return elements; }

  // Modifiers
  void push_back(const value_type &val) {
    check_alloc();
    alloc.construct(first_free++, val);
  }
  void pop_back() {
    if (size() == 0) {
      throw out_of_range("pop on empty strVec");
    } else {
      alloc.destroy(first_free--);
    }
  }

 private:
  // check if myVector::reallocate() is needed
  void check_alloc() {
    if (this->size() == this->capacity()) reallocate();
  }
  // move (*this) to a new bigger space
  void alloc_n_move(size_type new_cap) {
    auto newdata = alloc.allocate(new_cap);
    auto elem = elements;
    auto dest = newdata;
    // use std::move() rather than move()
    for (auto i = 0; i != size(); i++) {
      alloc.construct(dest++, std::move(*elem++));
    }
    free();
    first_free = dest;
    elements = newdata;
    cap = elements + new_cap;
  }
  // tool function, used by copy constructor, assign operator
  pair<value_type *, value_type *> alloc_n_copy(const T *b, const T *e) {
    auto data = alloc.allocate(e - b);
    return {data, uninitialized_copy(b, e, data)};
  }
  // destory() and deallocate()
  void free() {
    if (elements != nullptr) {
      // first_free is pointing to the first unconstructed string block
      // [ 1 ][ 2 ][  ][  ]
      //        ^    ^
      //       --p   p
      for (auto p = first_free; p != elements; /* empty */) {
        alloc.destroy(--p);
      }
      alloc.deallocate(elements, capacity());
    }
  }
  /* [ 0 ][ 1 ][ 2 ][ 3 ][   unconstructed   ]
                          ^                  ^
                        old_cap            new_cap */
  // allocate more memeory when needed
  void reallocate() {
    auto newcapacity = size() ? 2 * size() : 1;
    alloc_n_move(newcapacity);
  }

 public:
  /* [ 0 ][ 1 ][ 2 ][ 3 ][  unconstructed   ]
      ^                   ^                 ^
  elements           first_free            cap */
  iterator elements;
  iterator first_free;
  iterator cap;
  Alloc alloc;
};

#endif  // !MY_VECTOR_H
