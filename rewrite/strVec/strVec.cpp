#include "strVec.h"
#include <utility>  // std::move

strVector::strVector() { elements = first_free = cap = nullptr; }

strVector::strVector(strVector& rhs) noexcept {
  auto data = alloc_n_copy(rhs.begin(), rhs.end());
  elements = data.first;
  first_free = cap = data.second;
}

strVector::strVector(strVector&& s) noexcept : elements(s.elements),
                                               first_free(s.first_free),
                                               cap(s.cap) {
  s.elements = s.first_free = s.cap = nullptr;
}

strVector::~strVector() { free(); }

strVector& strVector::operator=(const strVector& rhs) {
  if (this != &rhs) {
    auto data = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements = data.first;
    first_free = cap = data.second;
  }
  return *this;
}

strVector& strVector::operator=(strVector&& rhs) noexcept {
  if (this != &rhs) {
    free();
    elements = rhs.elements;
    first_free = rhs.first_free;
    cap = rhs.cap;
    rhs.elements = rhs.first_free = rhs.cap = nullptr;
  }
  return *this;
}

void strVector::push_back(const string& s) {
  check_alloc();
  alloc.construct(first_free++, s);
}

void strVector::pop_back() {
  if (size() == 0) {
    throw out_of_range("pop on empty strVec");
  } else {
    alloc.destroy(first_free--);
  } 
}

size_t strVector::size() const { return size_t(first_free - elements); }

void strVector::resize(size_t n, string val) {
  if (size() < n) {
    if (capacity() < n) reserve(n);
    auto t = n - size();
    while (t--) alloc.construct(first_free++, val);
  } else {

  }
}

size_t strVector::capacity() const { return size_t(cap - elements); }

void strVector::reserve(size_t n) {
  if (capacity() < n) {
    auto newcapacity = n;
    alloc_n_move(newcapacity);
  } else {
    throw length_error("bad reserve");
  }
}

string* strVector::begin() const { return elements; }
string* strVector::end() const { return first_free; }

void strVector::check_alloc() {
  if (this->size() == this->capacity()) reallocate();
}

void strVector::reallocate() {
  auto newcapacity = size() ? 2 * size() : 1;
  alloc_n_move(newcapacity);
}

void strVector::alloc_n_move(size_t new_cap) {
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

pair<string*, string*> strVector::alloc_n_copy(const string* b,
                                               const string* e) {
  auto data = alloc.allocate(e - b);
  return {data, uninitialized_copy(b, e, data)};
}

void strVector::free() {
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